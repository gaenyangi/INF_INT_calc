#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include<iostream>

using namespace std;

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당

	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str) {
	if (str[0] == '-') {
		thesign = false;
		str++;
	}
	else thesign = true;
	length = strlen(str);
	digits = new char[length + 1];
	for (int i = 0; i < length; i++) {
		digits[i] = str[length-i-1];
	}
	digits[length] = '\0';
}

inf_int::inf_int(const inf_int& a) {
	this->length = a.length;
	this->thesign = a.thesign;
	this->digits = new char[a.length + 1];
	for (int i = 0; i < a.length; i++) {
		this->digits[i] = a.digits[i];
	}
	this->digits[length] = '\0';
}

inf_int::~inf_int() {
	delete[] digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this != &a) {  // 이전 값 확인. 혹시 있으면 삭제하고 새로 생성.
		delete[] this->digits;  
		this->digits = new char[a.length + 1];
		strcpy(this->digits, a.digits);
		this->length = a.length;
		this->thesign = a.thesign;
	}
	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	return (a.thesign == b.thesign) && (strcmp(a.digits, b.digits) == 0);
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !(a==b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	if (a.thesign && !b.thesign) return true; //a가 양수 b가 음수인 경우
	else if (!a.thesign && b.thesign) return false; // a가 음수, b가 양수인 경우
	else if (a.thesign) { //both positive
		if (a.length > b.length) return true;
		else if (a.length < b.length) return false;
		else return strcmp(a.digits, b.digits) > 0;
	}
	else { //both negative
		if (a.length > b.length) return false;
		else if (a.length < b.length) return true;
		else return strcmp(a.digits, b.digits) < 0; // length & sign same -> 가장 먼저 나오는 다른 문자로 판별 가능. strcmp(a,b)값이 1이면 앞의게 더 큰거, 0이면 동일, -1이면 뒤에가 더큼. 
	}
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int result;
	if (!(a < b)) result.thesign = true; // sign determine

	int borrow = 0;
	unsigned int maxLen = max(a.length, b.length);

	result.length = maxLen;
	result.digits = new char[maxLen + 1];
	result.digits[maxLen] = '\0';

	if (a < b) {
		result = b - a;
		result.thesign = false;
		return result;
	}

	for (unsigned int i = 0; i < maxLen; i++) {
		int digitA = (i < a.length) ? (a.digits[i] - '0') : 0;
		int digitB = (i < b.length) ? (b.digits[i] - '0') : 0;

		int diff = digitA - digitB - borrow;
		if (diff < 0)
		{
			diff += 10;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		result.digits[i] = diff + '0';
	}
	/*
	while (result.length > 1 && result.digits[result.length - 1] == '0'){// length보다 길 경우 남는 길이만큼 자르기.
		result.length--;
	}*/
	return result;
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int result; //result for total
	inf_int temp; // temporary value for looping
	for (unsigned int i = 0; i < a.length; i++)
	{
		int carry = 0;

		for (unsigned int j = 0; j < b.length; j++)
		{
			int digitA = (i < a.length) ? (a.digits[i] - '0') : 0;
			int digitB = (j < b.length) ? (b.digits[j] - '0') : 0;
			int product = digitA*digitB + carry;
			carry = product / 10;
			product %= 10;
			temp.Add(product+'0', i + j + 1);
		}

		if (carry > 0)
		{
			temp.Add(carry+'0', i + b.length + 1);
		}

		result = temp;
	}
	result.thesign = (a.thesign == b.thesign);

	return result;
}

ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int :: Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}