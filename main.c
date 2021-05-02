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

bool isOperatorMorePrioritized(char firstOperator, char secondOperator);
long long performOperation(long long firstOperand, long long secondOperand, char operator);

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