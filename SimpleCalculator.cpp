#include "SimpleCalculator.h"
#include <deque>
#include <string>
vector<SimpleCalculator::s_element> SimpleCalculator::broke(const string& str)
{
	vector<SimpleCalculator::s_element> res;
	int n = str.size();
	for (int i = 0; i < n; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			res.push_back(s_element(string(1, str[i]), e_type::number));
		else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
			res.push_back(s_element(string(1, str[i]), e_type::sign));
		else if (str[i] == '(' || str[i] == ')')
			res.push_back(s_element(string(1, str[i]), e_type::bracket));
	}
	n = res.size();
	auto countOfPairs = [](vector<s_element>& v)
	{
		int n = v.size(), res = 0;
		for (size_t i = 0; i < n - 1; i++)
		{
			if (v[i].type == e_type::number && v[i + 1].type == e_type::number)
			{
				res++;
			}
		}
		return res;
	};
	while (countOfPairs(res))
	{
		int n = res.size();
		for (size_t i = 0; i < n - 1; i++)
		{
			if (res[i].type == e_type::number && res[i + 1].type == e_type::number)
			{
				res[i].str += res[i + 1].str;
				res.erase(res.begin() + i + 1);
				break;
			}
		}
	}
	return res;
}

string SimpleCalculator::vectorToString(const vector<s_element>& v)
{
	string res;
	for (const auto& var : v)
	{
		res += var.str;
	}
	return res;
}

bool SimpleCalculator::checkBrackets(const vector<s_element>& v)
{
	deque<char> d;
	for (const auto& var : v)
	{
		if (var.type == e_type::bracket)
		{
			if (var.str == "(")
			{
				d.push_back('(');
			}
			else if (var.str == ")")
			{
				if (d.size() > 0)
				{
					d.pop_back();
				}
				else return 0;
			}
		}
	}
	return d.size() == 0;
}

int SimpleCalculator::calculate(const vector<s_element>& vec)
{
	vector<s_element> v = vec;
	auto copy = [](const vector<s_element>& original, vector<s_element>& v, int from, int to)
	{
		v.clear();
		for (int i = from; i <= to; ++i)
		{
			v.push_back(original[i]);
		}
	};
	if (!checkBrackets(v))
		throw exception("Brackets error");
	auto countOfBrackets = [](vector<s_element>& v)
	{
		int res = 0;
		for (const auto& var : v)
		{
			if (var.type == e_type::bracket)
			{
				if (var.str == "(")
				{
					res++;
				}
			}
		}
		return res;
	};
	auto lastOpenBracket = [](vector<s_element>& v)
	{
		int last = -1;
		for (int i = 0; i < v.size(); ++i) 
		{
			if (v[i].type == e_type::bracket) 
			{
				if (v[i].str == "(") 
				{
					last = i;
				}
			}
		}
		if (last == -1)
			throw exception("Tried to find brackets in an expression without brackets");
		return last;
	};
	while (countOfBrackets(v) != 0)
	{
		int last = lastOpenBracket(v);
		for (int i = last; i < v.size(); ++i)
		{
			if (v[i].type == e_type::bracket)
			{
				if (v[i].str == ")")
				{
					/*v[last].type = e_type::toRemove;
					v[i].type == e_type::toRemove;*/
					vector<s_element> toCalc;
					copy(v, toCalc, last + 1, i-1);
					while (toCalc.size() != 1)
					{
						calcSome(toCalc);
					}
					for (int j = last ; j <= i; ++j)
					{
						v[j].type = e_type::toRemove;
					}
					v[i].type = e_type::number;
					v[i].str = toCalc[0].str;
					SimpleCalculator::clear(v);
				}
			}
		}
	}
	while (v.size() >= 3)
	{
		calcSome(v);
	}
	for (auto& var : v)
		if (var.type == e_type::number)
			try { return stoi(var.str); }
	catch (...) { throw exception("Expression was counted incorrectly"); }
	throw exception("Expression was counted incorrectly");
}

void SimpleCalculator::calcSome(vector<s_element>& v)
{
	auto countOfMD = [](vector<s_element>& v)
	{
		int res = 0;
		for (const auto& var : v)
		{
			if (var.type == e_type::sign)
				if (var.str == "*" || var.str == "/")
					res++;
		}
		return res;
	};
	auto countOfPM = [](vector<s_element>& v)
	{
		int res = 0;
		for (const auto& var : v)
		{
			if (var.type == e_type::sign)
				if (var.str == "+" || var.str == "-")
					res++;
		}
		return res;
	};
	while (countOfMD(v) != 0)
	{
		if (v.size() >= 3)
		{
			for (int i = 1; i < v.size() - 1; i++)
			{
				if (v[i].type == e_type::sign)
				{
					if (v[i].str == "*")
					{
						if (v[i - 1].type == e_type::number
							&& v[i + 1].type == e_type::number)
						{
							int a, b;
							try {
								a = stoi(v[i - 1].str);
								b = stoi(v[i + 1].str);
							}
							catch (...)
							{
								throw exception("Ќа месте числа в строке не было чисел");
							}
							v[i].type = e_type::number;
							v[i].str = to_string(a * b);
							v[i - 1].type = e_type::toRemove;
							v[i + 1].type = e_type::toRemove;
						}
						else {
							throw exception("—лева или справа от знака умножени€ сто€ло не число");
						}
					}
					else if (v[i].str == "/")
					{
						if (v[i - 1].type == e_type::number
							&& v[i + 1].type == e_type::number)
						{
							int a, b;
							try {
								a = stoi(v[i - 1].str);
								b = stoi(v[i + 1].str);
							}
							catch (...)
							{
								throw exception("Ќа месте числа в строке не было чисел");
							}
							v[i].type = e_type::number;
							if (b == 0)
								throw exception("ƒеление на ноль");
							v[i].str = to_string(a / b);
							v[i - 1].type = e_type::toRemove;
							v[i + 1].type = e_type::toRemove;
						}
						else {
							throw exception("—лева или справа от знака делени€ сто€ло не число");
						}
					}
					SimpleCalculator::clear(v);
				}
			}
		}
	}
	while (countOfPM(v) != 0)
	{
		if (v.size() >= 3)
		{
			for (int i = 1; i < v.size() - 1; i++)
			{
				if (v[i].type == e_type::sign)
				{
					if (v[i].str == "+")
					{
						if (v[i - 1].type == e_type::number
							&& v[i + 1].type == e_type::number)
						{
							int a, b;
							try {
								a = stoi(v[i - 1].str);
								b = stoi(v[i + 1].str);
							}
							catch (...)
							{
								throw exception("Ќа месте числа в строке не было чисел");
							}
							v[i].type = e_type::number;
							v[i].str = to_string(a + b);
							v[i - 1].type = e_type::toRemove;
							v[i + 1].type = e_type::toRemove;
						}
						else {
							throw exception("—лева или справа от знака сложени€ сто€ло не число");
						}
					}
					else if (v[i].str == "-")
					{
						if (v[i - 1].type == e_type::number
							&& v[i + 1].type == e_type::number)
						{
							int a, b;
							try {
								a = stoi(v[i - 1].str);
								b = stoi(v[i + 1].str);
							}
							catch (...)
							{
								throw exception("Ќа месте числа в строке не было чисел");
							}
							v[i].type = e_type::number;
							v[i].str = to_string(a - b);
							v[i - 1].type = e_type::toRemove;
							v[i + 1].type = e_type::toRemove;
						}
						else {
							throw exception("—лева или справа от знака вычитани€ сто€ло не число");
						}
					}
					SimpleCalculator::clear(v);
				}
			}
		}
	}
}

void SimpleCalculator::clear(vector<s_element>& v)
{
	auto count = [](const vector<s_element>& v)
	{
		int res = 0;
		for (const auto& var : v)
		{
			if (var.type == e_type::toRemove)
			{
				res++;
			}
		}
		return res;
	};
	while (count(v) != 0)
	{
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i].type == e_type::toRemove)
			{
				v.erase(v.begin() + i);
				break;
			}
		}
	}
}

SimpleCalculator::s_element::s_element(string str, e_type type)
{
	this->str = str;
	this->type = type;
}
