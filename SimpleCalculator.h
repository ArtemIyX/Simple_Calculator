#pragma once
#include <string>
#include <vector>
using namespace std;
class SimpleCalculator final
{
public:
	enum class e_type
	{
		number,
		sign,
		bracket,
		toRemove
	};
	struct s_element
	{
		s_element(string str, e_type type);
		string str;
		e_type type = e_type::number;
	};

	static vector<s_element> broke(const string& str);
	static string vectorToString(const vector<s_element>& v);
	static int calculate(const vector<s_element>& vec);
	static bool checkBrackets(const vector<s_element>& v);
	static void calcSome(vector<s_element>& v);
	static void clear(vector<s_element>& v);
private:
};

