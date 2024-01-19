#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main()
{
	inf_int a, e;
	inf_int b(100);
	inf_int c("321111111111122222222222233333333333444444444445555555555");
	inf_int d("123451987651234572749499923455022211");
	inf_int f = d;
	inf_int g(f);

	cout << "a : " << a << endl;
	cout << "b : " << b << endl;
	cout << "c : " << c << endl;
	cout << "d : " << d << endl;
	cout << "f : " << f << endl;


	inf_int x("13");
	inf_int y("13");
	cout << x * y << endl;
	b = c - d;
	if (a > b) cout << "a is bigger than b!" << endl;
	if (!(a == b)&&(a!=b)) cout << "a and b are diffrent! " << endl;
	cout << "\n";

	if (f == d) {
		cout << "a : " << a << endl;
		cout << "b : " << b << endl;
		cout << "c : " << c << endl;
		cout << "d : " << d << endl;
		cout << "f : " << f << endl;
	}
	return 0;
}