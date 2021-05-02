#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>

// ��������� ������� Windows-1251 ��� ����������� ����� � ������ ������� ��������
#define RUS_ENCODING 1251
// ������������ ����� ����������� �� ���� ��������������� ���������
#define MAX_EXPRESSION_LENGTH 2600
// ���������, ������������, ��� � ����� ��� ����������
#define NO_OPERATOR CHAR_MAX

// �������������� ���������, ������� ����� ����������� � ���������
const char operators[] = { '(','-', '+', '/', '*', '%', '^' };
// ��������� ����� ��� ��������� (�����)
typedef struct _operandStackNode {
	long long currentNumber;
	struct _operandStackNode* nextNode;
} operandStackNode;
typedef operandStackNode* operandStackNodePtr;
// ��������� ����� ��� ���������� (�������������� ���������, ����� ��� "+", "-", "*" � ��� �����)
typedef struct _operatorStackNode {
	char operator;
	struct _operatorStackNode* nextNode;
} operatorStackNode;
typedef operatorStackNode* operatorStackNodePtr;

bool isOperator(char symbol);
bool isFirstOperatorMorePrioritized(char firstOperator, char secondOperator);
long long performOperation(long long firstOperand, long long secondOperand, char operator);
void pushOperand(operandStackNodePtr* head, long long number);
long long popOperand(operandStackNodePtr* head);
void pushOperator(operatorStackNodePtr* head, char operator);
char getLastOperator(operatorStackNodePtr head);
char popOperator(operatorStackNodePtr* head);
long long charToLongLong(char digit);

int main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
	char expression[MAX_EXPRESSION_LENGTH] = { 0 };
	puts("������� ���������� �������������� ��������� � ����������� (���������) �����:\n");
	gets_s(expression, MAX_EXPRESSION_LENGTH);

	operandStackNodePtr operandStackHead = NULL;
	operatorStackNodePtr operatorStackHead = NULL;

	// ������ ��������� ���������������, ����� �������
	for (size_t i = 0; expression[i]; i++) {
		// ������� ������ ���������
		char currentSymbol = expression[i];

		// ���� ������� ������ - �����, �� ������ ��������� ��� � ���� ���������
		if (isdigit(currentSymbol)) pushOperand(&operandStackHead, charToLongLong(currentSymbol));

		// ���� ������� ������ �������� ����������
		else if (isOperator(currentSymbol)) {
			/* ���� � ����� ��� ��� ����������, �� ��������� ����, ����� �� �� �� ��� (��� ��� �� � ��� ����������);
			*���� ������� ������ - ����������� ������, �� ����� ���������� ����������, ��������� ������ �������� �� � ���� ����������.*/
			if (operatorStackHead == NULL || currentSymbol == '(') {
				pushOperator(&operatorStackHead, currentSymbol);
				continue;
			}
			
		}
	}
}

// ���������, �������� �� ��������� ������ ���������� (��� ����� ����������� ������)
bool isOperator(char symbol) {
	strchr(operators, symbol) == NULL ? false : true;
}

/*� �������� ���������� ������ ������������ ��������� �� ������ ���������� ����������, � ���� ������ ��������� �� ����������.
���� ��������� ������� ��������� ���� (��������, ������ �������� - ���������, � ������ - ��������), ������������ ������; ����� - ����.*/
bool isFirstOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ������ - ������ ������������ ��������, �� ������ ������� �� ����� �� ����������� ������
	if (firstOperator == '(') return true;
	// ���� ������ �������� - �������, � ������ - ���, �� ������ ����� ������������
	if (firstOperator == '^' && secondOperator != '^') return true;
	// ���� ������ �������� - ���������, �������,  ��� ������ �������, �� �� ������������ �������
	if (strchr("/*%^", firstOperator) && strchr("+-", secondOperator)) return true;
	
	// � ����� ���� ������ ��������� ������� ��������� ���� ���� ����� �������
	return false;
}

// ��������� �������������� �������� � ����� ������� � ��������� ����������
long long performOperation(long long firstOperand, long long secondOperand, char operator) {
	switch (operator) {
		case '-':
			return firstOperand - secondOperand;
		case '+':
			return firstOperand + secondOperand;
		case '/':
			return firstOperand / secondOperand;
		case '*': 
			return firstOperand * secondOperand;
		case '%':
			return firstOperand % secondOperand;
		case '^':
			return (long long)pow(firstOperand, secondOperand);
		default:
			puts("�������������� ��������, ������");
			return false;
	}
}

// ��������� ��������� ��c�� � ����
void pushOperand(operandStackNodePtr* head, long long number) {
	operandStackNodePtr newNodePtr = (operandStackNodePtr) calloc(1, sizeof(operandStackNode));

	if (newNodePtr == NULL) {
		printf("������ ��� ���������� �������� �� ��������� %lld � ���� �����: ������������ ������.", number);
		return;
	}

	newNodePtr->currentNumber = number;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

// ������� ��������� ������� ����� ��������� � ���������� ����������� � ��� �����
long long popOperand(operandStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operandStackNodePtr tempNodePtr = *head;
	long long currentNumber = (*head)->currentNumber;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentNumber;
}

// ��������� ��������� �������� � ���� ������� � ���� 
void pushOperator(operatorStackNodePtr* head, char operator) {
	operatorStackNodePtr newNodePtr = (operatorStackNodePtr)calloc(1, sizeof(operatorStackNode));

	if (newNodePtr == NULL) {
		printf("������ ��� ���������� �������� �� ��������� %c � ���� ����������: ������������ ������.", operator);
		return;
	}

	newNodePtr->operator = operator;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

char getLastOperator(operatorStackNodePtr head) {
	if (head == NULL) return NO_OPERATOR;
	
	return head->operator;
}

// ������� ��������� ������� ����� � ���������� ����������� � ��� ������ (��������)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operatorStackNodePtr tempNodePtr = *head;
	char currentOperator = (*head)->operator;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentOperator;
}

// ������������ �����, ���������� � ���� ������� ���� char, � �������� long long int
long long charToLongLong(char digit) {
	return (long long) digit - '0';
}