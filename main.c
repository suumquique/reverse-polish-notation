#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// �������������� ���������, ������� ����� ����������� � ���������
const char operators[] = { '-', '+', '/', '*', '%', '^' };

bool isOperatorMorePrioritized(char firstOperator, char secondOperator);

int main(void) {

}

/*� �������� ���������� ������ ������������ ��������� �� ������ ���������� ����������, � ���� ������ ��������� �� ����������.
���� ��������� ������� ��������� ���� (��������, ������ �������� - ���������, � ������ - ��������), ������������ ������; ����� - ����.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ���� ������ �������� ��������� � ������ ���������� �������, ������, ����� ������� ���������
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
}