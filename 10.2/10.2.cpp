#include <iostream>
#include "iconvlite.h"
using namespace std;
int main()
{
	string utf = cp2utf("some text in cp1251: р");
	cout << utf << endl;
	string cp1251 = utf2cp("some text in utf8: ╤А");
	cout << cp1251 << endl;
	return 0;
}