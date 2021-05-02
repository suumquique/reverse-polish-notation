#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>

// Кодировка консоли Windows-1251 для корректного ввода и вывода русских символов
#define RUS_ENCODING 1251
// Максимальная длина подаваемого на вход математического выражения
#define MAX_EXPRESSION_LENGTH 2600

// Математические операторы, которые могут встретиться в выражении
const char operators[] = { '(', '-', '+', '/', '*', '%', '^' };
// Структура стека для операндов (чисел)
typedef struct _operandStackNode {
	long long currentNumber;
	struct _operandStackNode* nextNode;
} operandStackNode;
typedef operandStackNode* operandStackNodePtr;
// Структура стека для операторов (математические операторы, такие как "+", "-", "*" и так далее)
typedef struct _operatorStackNode {
	char operator;
	struct _operatorStackNode* nextNode;
} operatorStackNode;
typedef operatorStackNode* operatorStackNodePtr;

bool isOperator(char symbol);
bool isOperatorMorePrioritized(char firstOperator, char secondOperator);
long long performOperation(long long firstOperand, long long secondOperand, char operator);
void pushOperand(operandStackNodePtr* head, long long number);
long long popOperand(operandStackNodePtr* head);
void pushOperator(operatorStackNodePtr* head, char operator);
char popOperator(operatorStackNodePtr* head);
long long charToLongLong(char digit);

int main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
	char expression[MAX_EXPRESSION_LENGTH] = { 0 };
	puts("Введите корректное математическое выражение в стандартной (инфиксной) форме:\n");
	gets_s(expression, MAX_EXPRESSION_LENGTH);

	operandStackNodePtr operandStackHead = NULL;
	operatorStackNodePtr operatorStackHead = NULL;

	for (size_t i = 0; expression[i]; i++) {
		// Если мы считываем число, то просто добавляем его в стек операндов
		if (isdigit(expression[i])) pushOperand(&operandStackHead, charToLongLong(expression[i]));
	}
}

// Проверяет, является ли введенный символ оператором (без учета закрывающей скобки)
bool isOperator(char symbol) {
	strchr(operators, symbol) == NULL ? false : true;
}

/*В качестве аргументов должны передаваться операторы из списка допустимых операторов, в ином случае поведение не определено.
Если приоритет первого оператора выше (например, первый оператор - умножение, а второй - сложение), возвращается истина; иначе - ложь.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// Если первой оператор находится в списке операторов позднее, значит, имеет больший приоритет
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
}

// Выполняет арифметическую операцию с двумя числами и указанным оператором
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
			puts("Несуществующий оператор, ошибка");
			return false;
	}
}

// Добавляет указанное чиcло в стек
void pushOperand(operandStackNodePtr* head, long long number) {
	operandStackNodePtr newNodePtr = (operandStackNodePtr) calloc(1, sizeof(operandStackNode));

	if (newNodePtr == NULL) {
		printf("Ошибка при добавлении элемента со значением %lld в стек чисел: недостаточно памяти.", number);
		return;
	}

	newNodePtr->currentNumber = number;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

// Удаляет последний элемент стека операндов и возвращает находящееся в нем число
long long popOperand(operandStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operandStackNodePtr tempNodePtr = *head;
	long long currentNumber = (*head)->currentNumber;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentNumber;
}

// Добавляет указанный оператор в виде символа в стек 
void pushOperator(operatorStackNodePtr* head, char operator) {
	operatorStackNodePtr newNodePtr = (operatorStackNodePtr)calloc(1, sizeof(operatorStackNode));

	if (newNodePtr == NULL) {
		printf("Ошибка при добавлении элемента со значением %c в стек операторов: недостаточно памяти.", operator);
		return;
	}

	newNodePtr->operator = operator;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

// Удаляет последний элемент стека и возвращает находящийся в нем символ (оператор)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operatorStackNodePtr tempNodePtr = *head;
	char currentOperator = (*head)->operator;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentOperator;
}

// Конвертирует цифру, записанную в виде символа типа char, в значение long long int
long long charToLongLong(char digit) {
	return (long long) digit - '0';
}