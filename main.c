#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Математические операторы, которые могут встретиться в выражении
const char operators[] = { '-', '+', '/', '*', '%', '^' };

bool isOperatorMorePrioritized(char firstOperator, char secondOperator);

int main(void) {

}

/*В качестве аргументов должны передаваться операторы из списка допустимых операторов, в ином случае поведение не определено.
Если приоритет первого оператора выше (например, первый оператор - умножение, а второй - сложение), возвращается истина; иначе - ложь.*/
bool isOperatorMorePrioritized(char firstOperator, char secondOperator) {
	// Если первой оператор находится в списке операторов позднее, значит, имеет больший приоритет
	return strchr(operators, firstOperator) > strchr(operators, secondOperator);
}