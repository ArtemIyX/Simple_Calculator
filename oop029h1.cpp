

#include <iostream>
#include "SimpleCalculator.h"

int main()
{
	string str;
	vector<SimpleCalculator::s_element> v;
	getline(cin, str);
	try {
		v = SimpleCalculator::broke(str);
		cout << SimpleCalculator::calculate(v) << endl;
	}
	catch (exception exp)
	{
		cout << exp.what() << endl;
	}
	system("Pause");
}

