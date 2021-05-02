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
// Константа, показывающая, что в стеке нет операторов
#define NO_OPERATOR CHAR_MAX
// Код ошибки, показывающий, что изначальное выражение некорректно
#define ERROR_INCORRECT_EXPRESSION 999

// Математические операторы, которые могут встретиться в выражении
const char operators[] = { '(','-', '+', '/', '*', '%', '^' };
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
bool isFirstOperatorMorePrioritized(char firstOperator, char secondOperator);
long long performOperation(long long firstOperand, long long secondOperand, char operator);
void pushOperand(operandStackNodePtr* head, long long number);
long long popOperand(operandStackNodePtr* head);
void pushOperator(operatorStackNodePtr* head, char operator);
char getLastOperator(operatorStackNodePtr head);
char popOperator(operatorStackNodePtr* head);
long long charToLongLong(char digit);
long long parseAndCalculateExpression(char* expression);

void main(void) {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);
	
	char expression[MAX_EXPRESSION_LENGTH] = { 0 }; // Строка для хранения математического выражения
	puts("Введите корректное математическое выражение в стандартной (инфиксной) форме:");
	gets_s(expression, MAX_EXPRESSION_LENGTH);

	long long arithmeticResult = parseAndCalculateExpression(expression);
	printf("\nArithmetic result: %lld\n", arithmeticResult);
}


// Проверяет, является ли введенный символ оператором (без учета закрывающей скобки)
bool isOperator(char symbol) {
	strchr(operators, symbol) == NULL ? false : true;
}

/*В качестве аргументов должны передаваться операторы из списка допустимых операторов, в ином случае поведение не определено.
Если приоритет первого оператора выше (например, первый оператор - умножение, а второй - сложение), возвращается истина; иначе - ложь.*/
bool isFirstOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// Скобка - всегда низкоприоритетный оператор, ее нельзя убирать из стека до закрывающей скобки
	if (firstOperator == '(') return false;
	if (secondOperator == '(') return true;
	// Если первый оператор - степень, а второй - нет, то первый точно приоритетнее
	if (firstOperator == '^' && secondOperator != '^') return true;
	// Если первый оператор - умножение, деление,  или взятие остатка, то он приоритетнее второго
	if (strchr("/*%", firstOperator) && strchr("+-", secondOperator)) return true;
	
	// В любом ином случае приоритет первого оператора ниже либо равен второму
	return false;
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
			printf("Ошибка: Несуществующий оператор - %c\n", operator);
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
	newNodePtr->nextNode = *head;
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
	newNodePtr->nextNode = *head;
	*head = newNodePtr;
}

// Получает последний оператор, не изменяя стек
char getLastOperator(operatorStackNodePtr head) {
	if (head == NULL) return NO_OPERATOR;
	
	return head->operator;
}

// Удаляет последний элемент стека и возвращает находящийся в нем символ (оператор)
char popOperator(operatorStackNodePtr* head) {
	if (*head == NULL) return NO_OPERATOR;

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

long long parseAndCalculateExpression(char* expression) {
	char currentSymbol, currentOperator, previousOperator; // Символьные переменные для парсинга строки
	long long firstOperand, secondOperand, arithmeticResult; // Переменные для промежуточных вычислений
	operandStackNodePtr operandStackHead = NULL; // Верх стека с числами (операндами)
	operatorStackNodePtr operatorStackHead = NULL; // Верх стека с математическими операторами

	// Парсим выражение последовательно, слева направо
	for (size_t i = 0; expression[i]; i++) {
		// Текущий символ выражения
		currentSymbol = expression[i];

		// Если текущий символ - число, то просто добавляем его в стек операндов
		if (isdigit(currentSymbol)) pushOperand(&operandStackHead, charToLongLong(currentSymbol));

		// Если текущий символ является оператором
		else if (isOperator(currentSymbol)) {
			currentOperator = currentSymbol;

			/* Если в стеке еще нет операторов, то добавляем этот, каким бы он не был (так как не с чем сравнивать);
			* Если текущий символ - открывающая скобка, не нужно сравнивать приоритеты, требуется просто добавить ее в стек операторов. */
			if (operatorStackHead == NULL || currentOperator == '(') {
				pushOperator(&operatorStackHead, currentOperator);
				continue;
			}

			// Если текущий оператор имеет приоритет строго выше, чем последний оператор в стеке
			if (isFirstOperatorMorePrioritized(currentOperator, getLastOperator(operatorStackHead))) {
				pushOperator(&operatorStackHead, currentOperator);
			}
			/* Если приоритет иной, то нужно достать последний оператор из стека и выполнить соответствующую операцию для
			* двух последних чисел из стека операндов */
			else {
				// Вынимаем из стека предыдущий оператор и добавляем текущий
				previousOperator = popOperator(&operatorStackHead);
				pushOperator(&operatorStackHead, currentOperator);

				/* Вынутое первым число - второй оператор, так как оно было добавлено в стек позже.
				Это важно при многих операциях, таких как вычитание, деление, взятие остатка и возведение в степень */
				secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
				// Вычисляем значение выражения с этими двумя чилами и взятым из стека оператором
				arithmeticResult = performOperation(firstOperand, secondOperand, previousOperator);
				// Добавляем получившееся значение в стек чисел (операндов)
				pushOperand(&operandStackHead, arithmeticResult);
			}
		}
		// Случай, когда текущий символ - закрывающая скобка, обрабатываем отдельно, так как действия там кардинально отличаются
		else if (currentSymbol == ')') {
			while ((currentOperator = popOperator(&operatorStackHead)) != '(' && operatorStackHead != NULL) {

				/* Вынутое первым число - второй оператор, так как оно было добавлено в стек позже.
				Это важно при многих операциях, таких как вычитание, деление, взятие остатка и возведение в степень */
				secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
				// Вычисляем значение выражения с этими двумя чилами и взятым из стека оператором
				arithmeticResult = performOperation(firstOperand, secondOperand, currentOperator);
				// Добавляем получившееся значение в стек чисел (операндов)
				pushOperand(&operandStackHead, arithmeticResult);
			}

			if (currentOperator == NO_OPERATOR) {
				puts("Произошла ошибка в скобочных группах. Некорректное уравнение.");
				exit(ERROR_INCORRECT_EXPRESSION);
			}

		}
		else {
			printf("Ошибка в парсинге выражения - некорректный символ: %c\n", currentSymbol);
			exit(ERROR_INCORRECT_EXPRESSION);
		}
	}

	while (operatorStackHead != NULL) {
		// Вынимаем из стека предыдущий оператор и добавляем текущий
		previousOperator = popOperator(&operatorStackHead);

		/* Вынутое первым число - второй оператор, так как оно было добавлено в стек позже.
		Это важно при многих операциях, таких как вычитание, деление, взятие остатка и возведение в степень */
		secondOperand = popOperand(&operandStackHead), firstOperand = popOperand(&operandStackHead);
		// Вычисляем значение выражения с этими двумя чилами и взятым из стека оператором
		arithmeticResult = performOperation(firstOperand, secondOperand, previousOperator);
		// Добавляем получившееся значение в стек чисел (операндов)
		pushOperand(&operandStackHead, arithmeticResult);
	}

	arithmeticResult = popOperand(&operandStackHead);
	return arithmeticResult;
}