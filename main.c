#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>

//  одировка консоли Windows-1251 дл€ корректного ввода и вывода русских символов
#define RUS_ENCODING 1251
// ћаксимальна€ длина подаваемого на вход математического выражени€
#define MAX_EXPRESSION_LENGTH 2600

// ћатематические операторы, которые могут встретитьс€ в выражении
const char operators[] = { '-', '+', '/', '*', '%', '^' };
// —труктура стека дл€ операндов (чисел)
typedef struct _operandStackNode {
	long long currentNumber;
	struct _operandStackNode* nextNode;
} operandStackNode;
typedef operandStackNode* operandStackNodePtr;
// —труктура стека дл€ операторов (математические операторы, такие как "+", "-", "*" и так далее)
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
	puts("¬ведите корректное математическое выражение в стандартной (инфиксной) форме:\n");
	gets_s(expression, MAX_EXPRESSION_LENGTH);


}

/*¬ качестве аргументов должны передаватьс€ операторы из списка допустимых операторов, в ином случае поведение не определено.
≈сли приоритет первого оператора выше (например, первый оператор - умножение, а второй - сложение), возвращаетс€ истина; иначе - ложь.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// ≈сли первой оператор находитс€ в списке операторов позднее, значит, имеет больший приоритет
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
}

// ¬ыполн€ет арифметическую операцию с двум€ числами и указанным оператором
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
			puts("Ќесуществующий оператор, ошибка");
			return false;
	}
}

// ƒобавл€ет указанное чиcло в стек
void pushOperand(operandStackNodePtr* head, long long number) {
	operandStackNodePtr newNodePtr = (operandStackNodePtr) calloc(1, sizeof(operandStackNode));

	if (newNodePtr == NULL) {
		printf("ќшибка при добавлении элемента со значением %lld в стек чисел: недостаточно пам€ти.", number);
		return;
	}

	newNodePtr->currentNumber = number;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

// ”дал€ет последний элемент стека операндов и возвращает наход€щеес€ в нем число
long long popOperand(operandStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operandStackNodePtr tempNodePtr = *head;
	long long currentNumber = (*head)->currentNumber;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentNumber;
}

// ƒобавл€ет указанный оператор в виде символа в стек 
void pushOperator(operatorStackNodePtr* head, char operator) {
	operatorStackNodePtr newNodePtr = (operatorStackNodePtr)calloc(1, sizeof(operatorStackNode));

	if (newNodePtr == NULL) {
		printf("ќшибка при добавлении элемента со значением %c в стек операторов: недостаточно пам€ти.", operator);
		return;
	}

	newNodePtr->operator = operator;
	newNodePtr->nextNode = head;
	*head = newNodePtr;
}

// ”дал€ет последний элемент стека и возвращает наход€щийс€ в нем символ (оператор)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return ERROR_SUCCESS;

	operatorStackNodePtr tempNodePtr = *head;
	char currentOperator = (*head)->operator;
	*head = (*head)->nextNode;

	free(tempNodePtr);

	return currentOperator;
}