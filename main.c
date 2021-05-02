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
// ��� ������, ������������, ��� ����������� ��������� �����������
#define ERROR_INCORRECT_EXPRESSION 999

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

void printOperandStack(operandStackNodePtr start_ptr_list)
{
	if (start_ptr_list == NULL)
	{
		printf("Stack is null\n");
	}
	else
	{
		printf("Current Stack is:\n");
		while (start_ptr_list != NULL)
		{
			printf("%lld --> ", start_ptr_list->currentNumber);
			start_ptr_list = start_ptr_list->nextNode;
		}
	}
	printf("null \n");
}

void printOperatorStack(operatorStackNodePtr start_ptr_list)
{
	if (start_ptr_list == NULL)
	{
		printf("Stack is null\n");
	}
	else
	{
		printf("Current Stack is:\n");
		while (start_ptr_list != NULL)
		{
			printf("%c --> ", start_ptr_list->operator);
			start_ptr_list = start_ptr_list->nextNode;
		}
	}
	printf("null \n");
}

int main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
	char currentSymbol, currentOperator, previousOperator; // ���������� ���������� ��� �������� ������
	long long firstOperand, secondOperand, arithmeticResult; // ���������� ��� ������������� ����������
	char expression[MAX_EXPRESSION_LENGTH] = { 0 }; // ������ ��� �������� ��������������� ���������
	puts("������� ���������� �������������� ��������� � ����������� (���������) �����:");
	gets_s(expression, MAX_EXPRESSION_LENGTH);

	operandStackNodePtr operandStackHead = NULL;
	operatorStackNodePtr operatorStackHead = NULL;

	// ������ ��������� ���������������, ����� �������
	for (size_t i = 0; expression[i]; i++) {
		// ������� ������ ���������
		currentSymbol = expression[i];

		// ���� ������� ������ - �����, �� ������ ��������� ��� � ���� ���������
		if (isdigit(currentSymbol)) pushOperand(&operandStackHead, charToLongLong(currentSymbol));

		// ���� ������� ������ �������� ����������
		else if (isOperator(currentSymbol)) {
			currentOperator = currentSymbol;

			/* ���� � ����� ��� ��� ����������, �� ��������� ����, ����� �� �� �� ��� (��� ��� �� � ��� ����������);
			* ���� ������� ������ - ����������� ������, �� ����� ���������� ����������, ��������� ������ �������� �� � ���� ����������. */
			if (operatorStackHead == NULL || currentOperator == '(') {
				pushOperator(&operatorStackHead, currentOperator);
				continue;
			}
			
			// ���� ������� �������� ����� ��������� ������ ����, ��� ��������� �������� � �����
			if (isFirstOperatorMorePrioritized(currentOperator, getLastOperator(operatorStackHead))) {
				pushOperator(&operatorStackHead, currentOperator);
			}
			/* ���� ��������� ����, �� ����� ������� ��������� �������� �� ����� � ��������� ��������������� �������� ��� 
			* ���� ��������� ����� �� ����� ��������� */
			else {
				// �������� �� ����� ���������� �������� � ��������� �������
				previousOperator = popOperator(&operatorStackHead);
				pushOperator(&operatorStackHead, currentOperator);

				/* ������� ������ ����� - ������ ��������, ��� ��� ��� ���� ��������� � ���� �����.
				��� ����� ��� ������ ���������, ����� ��� ���������, �������, ������ ������� � ���������� � ������� */
				secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
				// ��������� �������� ��������� � ����� ����� ������ � ������ �� ����� ����������
				arithmeticResult = performOperation(firstOperand, secondOperand, previousOperator);
				// ��������� ������������ �������� � ���� ����� (���������)
				pushOperand(&operandStackHead, arithmeticResult);
			}
		}
		// ������, ����� ������� ������ - ����������� ������, ������������ ��������, ��� ��� �������� ��� ����������� ����������
		else if (currentSymbol == ')') {
			while ((currentOperator = popOperator(&operatorStackHead)) != '(' && operatorStackHead != NULL) {

				/* ������� ������ ����� - ������ ��������, ��� ��� ��� ���� ��������� � ���� �����.
				��� ����� ��� ������ ���������, ����� ��� ���������, �������, ������ ������� � ���������� � ������� */
				secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
				// ��������� �������� ��������� � ����� ����� ������ � ������ �� ����� ����������
				arithmeticResult = performOperation(firstOperand, secondOperand, currentOperator);
				// ��������� ������������ �������� � ���� ����� (���������)
				pushOperand(&operandStackHead, arithmeticResult);
			}

			if (currentOperator == NO_OPERATOR) {
				puts("��������� ������ � ��������� �������. ������������ ���������.");
				exit(ERROR_INCORRECT_EXPRESSION);
			}

		}
		else {
			printf("������ � �������� ��������� - ������������ ������: %c\n", currentSymbol);
			exit(ERROR_INCORRECT_EXPRESSION);
		}
	}
	
	while (operatorStackHead != NULL) {
		// �������� �� ����� ���������� �������� � ��������� �������
		previousOperator = popOperator(&operatorStackHead);

		/* ������� ������ ����� - ������ ��������, ��� ��� ��� ���� ��������� � ���� �����.
		��� ����� ��� ������ ���������, ����� ��� ���������, �������, ������ ������� � ���������� � ������� */
		secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
		// ��������� �������� ��������� � ����� ����� ������ � ������ �� ����� ����������
		arithmeticResult = performOperation(firstOperand, secondOperand, previousOperator);
		// ��������� ������������ �������� � ���� ����� (���������)
		pushOperand(&operandStackHead, arithmeticResult);
	}

	arithmeticResult = popOperand(&operandStackHead);
	printf("\nArithmetic result: %lld\n", arithmeticResult);
}


// ���������, �������� �� ��������� ������ ���������� (��� ����� ����������� ������)
bool isOperator(char symbol) {
	strchr(operators, symbol) == NULL ? false : true;
}

/*� �������� ���������� ������ ������������ ��������� �� ������ ���������� ����������, � ���� ������ ��������� �� ����������.
���� ��������� ������� ��������� ���� (��������, ������ �������� - ���������, � ������ - ��������), ������������ ������; ����� - ����.*/
bool isFirstOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ������ - ������ ����������������� ��������, �� ������ ������� �� ����� �� ����������� ������
	if (firstOperator == '(') return false;
	if (secondOperator == '(') return true;
	// ���� ������ �������� - �������, � ������ - ���, �� ������ ����� ������������
	if (firstOperator == '^' && secondOperator != '^') return true;
	// ���� ������ �������� - ���������, �������,  ��� ������ �������, �� �� ������������ �������
	if (strchr("/*%", firstOperator) && strchr("+-", secondOperator)) return true;
	
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
			printf("������: �������������� �������� - %c\n", operator);
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
	newNodePtr->nextNode = *head;
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
	newNodePtr->nextNode = *head;
	*head = newNodePtr;
}

// �������� ��������� ��������, �� ������� ����
char getLastOperator(operatorStackNodePtr head) {
	if (head == NULL) return NO_OPERATOR;
	
	return head->operator;
}

// ������� ��������� ������� ����� � ���������� ����������� � ��� ������ (��������)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return NO_OPERATOR;

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