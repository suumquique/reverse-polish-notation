#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <consoleapi2.h>

// ��������� ������� Windows-1251 ��� ����������� ����� � ������ ������� ��������
#define RUS_ENCODING 1251

// �������������� ���������, ������� ����� ����������� � ���������
const char operators[] = { '-', '+', '/', '*', '%', '^' };

bool isOperatorMorePrioritized(char firstOperator, char secondOperator);

int main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
}

/*� �������� ���������� ������ ������������ ��������� �� ������ ���������� ����������, � ���� ������ ��������� �� ����������.
���� ��������� ������� ��������� ���� (��������, ������ �������� - ���������, � ������ - ��������), ������������ ������; ����� - ����.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ���� ������ �������� ��������� � ������ ���������� �������, ������, ����� ������� ���������
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
}