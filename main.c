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

// �������������� ���������, ������� ����� ����������� � ���������
const char operators[] = { '-', '+', '/', '*', '%', '^' };
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

bool isOperatorMorePrioritized(char firstOperator, char secondOperator);
long long performOperation(long long firstOperand, long long secondOperand, char operator);
void pushOperand(operandStackNodePtr* head, long long number);
long long popOperand(operandStackNodePtr* head);
void pushOperator(operatorStackNodePtr* head, char operator);
char popOperator(operatorStackNodePtr* head);

int main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
	char expression[MAX_EXPRESSION_LENGTH] = { 0 };
	puts("������� ���������� �������������� ��������� � ����������� (���������) �����:\n");
	gets_s(expression, MAX_EXPRESSION_LENGTH);


}

/*� �������� ���������� ������ ������������ ��������� �� ������ ���������� ����������, � ���� ������ ��������� �� ����������.
���� ��������� ������� ��������� ���� (��������, ������ �������� - ���������, � ������ - ��������), ������������ ������; ����� - ����.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ���� ������ �������� ��������� � ������ ���������� �������, ������, ����� ������� ���������
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
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

// ������� ��������� ������� ����� � ���������� ����������� � ��� ������ (��������)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operatorStackNodePtr tempNodePtr = *head;
	char currentOperator = (*head)->operator;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentOperator;
}