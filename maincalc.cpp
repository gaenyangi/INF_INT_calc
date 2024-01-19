#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main() {
	char first_input[100];
	char sec_input[100];
	char op;
	inf_int result;
	int fcount; int scount;
	while (1) {
		for (int i = 0; i < 100; i++) {
			first_input[i] = '\0';
			sec_input[i] = '\0';
		}
		std::cout << "Input : ";
		cin >> first_input; //first positive integer
		if (first_input[0] == '0') return 0; //exit
		cin >> op; //operator
		cin >> sec_input; //second positive integer
		fcount = 0; scount = 0;
		while (first_input[fcount]!= '\0') {
			fcount++;
		}
		while (sec_input[scount] != '\0') {
			scount++;
		}

		first_input[fcount] = '\0'; sec_input[scount] = '\0';
		inf_int first(first_input);
		inf_int sec(sec_input);

		switch (op) {
		case '+':
			result = first + sec;
			break;
		case '-':
			result = first - sec;
			break;
		case '*':
			result = first * sec;
			break;
		default:
			std::cout << "wrong input!" << endl;
			break;
		}
		std::cout << "Output : "<< result << endl;
	}
	return 0;
}