#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include<iostream>

using namespace std;

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�

	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���
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
	delete[] digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this != &a) {  // ���� �� Ȯ��. Ȥ�� ������ �����ϰ� ���� ����.
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
	if (a.thesign && !b.thesign) return true; //a�� ��� b�� ������ ���
	else if (!a.thesign && b.thesign) return false; // a�� ����, b�� ����� ���
	else if (a.thesign) { //both positive
		if (a.length > b.length) return true;
		else if (a.length < b.length) return false;
		else return strcmp(a.digits, b.digits) > 0;
	}
	else { //both negative
		if (a.length > b.length) return false;
		else if (a.length < b.length) return true;
		else return strcmp(a.digits, b.digits) < 0; // length & sign same -> ���� ���� ������ �ٸ� ���ڷ� �Ǻ� ����. strcmp(a,b)���� 1�̸� ���ǰ� �� ū��, 0�̸� ����, -1�̸� �ڿ��� ��ŭ. 
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

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
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
	while (result.length > 1 && result.digits[result.length - 1] == '0'){// length���� �� ��� ���� ���̸�ŭ �ڸ���.
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

void inf_int :: Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}