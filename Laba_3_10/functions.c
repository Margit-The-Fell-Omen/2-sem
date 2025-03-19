#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

//* ======================== STACK ===========================

Stack* createNode(char data)                        // функция создания элемента стека
{
    Stack* new_node = malloc(sizeof(Stack));
    if (new_node) 
	{
        new_node->data = data;
        new_node->next = NULL;
    }
    return new_node;
}

int isEmpty(Stack* stack)                           // функция проверки стека на пустоту
{
    return stack == NULL;
}

void push(Stack** stack, char new_data)             // функция добавления элемента в стек
{
    Stack* new_node = createNode(new_data);
    if (!new_node) 
    {
        printf("\n Ошибка выделения памяти\n");
        return;
    }

    new_node->next = *stack;
    *stack = new_node;
}

void pop(Stack** stack)                             // функция удаления элемента из стека
{
    if (isEmpty(*stack)) 
	{
        printf("\n Стек уже пуст\n");
        return;
    } 
	else 
	{
        Stack* temp = *stack;
        *stack = (*stack)->next;
        free(temp);
    }
}

int peek(Stack* stack)                              // функция возвращения верхнего элемента стека
{
    if (!isEmpty(stack)) 
	{
        return stack->data;
    }
    return 404;
}

void display_stack(Stack* stack)                    // функция вывода элементов стека
{
    printf("Stack:\n");
    while (stack != NULL)
    {
        printf("%c", stack->data);
        stack = stack->next;
    }
    printf("\n");
}

//* ======================== MATH ========================
void check_brackets(Stack* stack, char* str)        // функция проверки синтаксиса скобок
{
	int error_flag = 0;
	for (int i = 0; i < strlen(str) && !error_flag; i++)
	{
		switch (str[i])
		{
		case '(':
			push(&stack, str[i]);
			break;

		case '[':
			push(&stack, str[i]);
			break;

		case '{':
			push(&stack, str[i]);
			break;

		case ')':
			if (peek(stack) == '(')
			{
				pop(&stack);
			}
			else
			{
				printf("Syntax mistake on the %d character: %c!\n", i, peek(stack));
				error_flag = 1;
			}
			break;

		case ']':
			if (peek(stack) == '[')
			{
				pop(&stack);
			}
			else
			{
				printf("Syntax mistake on the %d character: %c!\n", i, peek(stack));
				error_flag = 1;
			}
			break;

		case '}':
			if (peek(stack) == '{')
			{
				pop(&stack);
			}
			else
			{
				printf("Syntax mistake on the %d character: %c!\n", i, peek(stack));
				error_flag = 1;
			}
			break;
		}
	}
	if (isEmpty(stack) && !error_flag)
	{
		printf("All good!");
	}   
    else 
    {
        printf("Syntax mistake: not closed bracket");
        while(stack) pop(&stack);
    }
	
}

void skipSpaces(const char **s) 
{
    while (isspace(**s)) (*s)++;
}

Linear parseLinearFactor(const char **s, const char *unknownVar) // функция вычисления линейного фактора
{
    skipSpaces(s);
    Linear result = {0, 0};

    // Если встретилась открывающая скобка
    if (**s == '(' || **s == '[' || **s == '{') 
    {
        (*s)++;  // пропускаем '('
        result = parseLinearExpression(s, unknownVar);
        skipSpaces(s);
        if (**s == ')' || **s == ']' || **s == '}') 
        {
            (*s)++;  // пропускаем ')'
        } 
        else 
        {
            printf("Ошибка: ожидалась ')'\n");
            exit(EXIT_FAILURE);
        }
        printf("[DEBUG][Линейный] Скобки: %.2f*x + %.2f\n", result.coef, result.constant);
        return result;
    }
    // Если начинается идентификатор
    else if (isalpha(**s) || **s == '_') 
    {
        char varName[64];
        int pos = 0;
        while ((isalpha(**s) || **s == '_' || isdigit(**s)) && pos < 63) 
        {
            varName[pos++] = **s;
            (*s)++;
        }
        varName[pos] = '\0';
        // Если идентификатор совпадает с неизвестной переменной
        if (strcmp(varName, unknownVar) == 0) 
        {
            result.coef = 1;
            result.constant = 0;
        } 
        else 
        {
            // Здесь можно генерировать ошибку, поскольку в данной реализации
            // разрешена только одна неизвестная переменная.
            fprintf(stderr, "Ошибка: неизвестный идентификатор \"%s\"\n", varName);
            exit(EXIT_FAILURE);
        }
        printf("[DEBUG][Линейный] Переменная '%s': %.2f*x + %.2f\n", varName, result.coef, result.constant);
        return result;
    }
    // Иначе должно идти число
    else 
    {
        char *end;
        double val = strtod(*s, &end);
        if (s == &end) 
        {
            fprintf(stderr, "Ошибка разбора числа\n");
            exit(EXIT_FAILURE);
        }
        *s = end;
        result.coef = 0;
        result.constant = val;
        printf("[DEBUG][Линейный] Литерал: %.2f\n", val);
        return result;
    }
}

Linear parseLinearTerm(const char **s, const char *unknownVar) 
{
    Linear result = parseLinearFactor(s, unknownVar);
    skipSpaces(s);
    while (**s == '*' || **s == '/') 
    {
        char op = **s;
        (*s)++; // пропускаем оператор
        Linear factor = parseLinearFactor(s, unknownVar);
        if (op == '*') 
        {
            // При умножении (a*x+b) * (c*x+d) допускается только если хотя бы один
            // из множителей является чистой константой (то есть coef == 0)
            if (result.coef != 0 && factor.coef != 0) 
            {
                fprintf(stderr, "Ошибка: выражение не является линейным (умножение двух зависимых от x выражений)\n");
                exit(EXIT_FAILURE);
            }
            double newCoef = result.coef * factor.constant + result.constant * factor.coef;
            double newConstant = result.constant * factor.constant;
            result.coef = newCoef;
            result.constant = newConstant;
            printf("[DEBUG][Линейный] Умножение: теперь %.2f*x + %.2f\n", result.coef, result.constant);
        } 
        else if (op == '/') 
        {
            if (fabs(factor.coef) > 1e-9) 
            {
                fprintf(stderr, "Ошибка: делитель должен быть константой в линейном выражении\n");
                exit(EXIT_FAILURE);
            }
            result.coef /= factor.constant;
            result.constant /= factor.constant;
            printf("[DEBUG][Линейный] Деление: теперь %.2f*x + %.2f\n", result.coef, result.constant);
        }
        skipSpaces(s);
    }
    printf("[DEBUG][Линейный] Результат терма: %.2f*x + %.2f\n", result.coef, result.constant);
    return result;
}

Linear parseLinearExpression(const char **s, const char *unknownVar) 
{
    Linear result = parseLinearTerm(s, unknownVar);
    skipSpaces(s);
    while (**s == '+' || **s == '-') 
    {
        char op = **s;
        (*s)++;  // пропускаем оператор
        Linear term = parseLinearTerm(s, unknownVar);
        if (op == '+') 
        {
            result.coef += term.coef;
            result.constant += term.constant;
            printf("[DEBUG][Линейный] Сложение: теперь %.2f*x + %.2f\n", result.coef, result.constant);
        } 
        else 
        {
            result.coef -= term.coef;
            result.constant -= term.constant;
            printf("[DEBUG][Линейный] Вычитание: теперь %.2f*x + %.2f\n", result.coef, result.constant);
        }
        skipSpaces(s);
    }
    printf("[DEBUG][Линейный] Результат выражения: %.2f*x + %.2f\n", result.coef, result.constant);
    return result;
}

/* --------------------------------------------------------------------------
   Вспомогательная функция для проверки, является ли строка простым именем
   переменной (содержащей только идентификатор).
-------------------------------------------------------------------------- */
int isSimpleVariable(const char *str) 
{
    skipSpaces(&str);
    if (*str == '\0')
        return 0;
    if (!(isalpha(*str) || *str == '_'))
        return 0;
    str++;
    while (*str && (isalnum(*str) || *str == '_')) 
    {
        str++;
    }
    skipSpaces(&str);
    return (*str == '\0');
}
/* --------------------------------------------------------------------------
   Функция do_math()
   Если в строке присутствует знак '=', то левая и правая части трактуются
   как линейные выражения, после чего уравнение приводится к виду
     (left.coef - right.coef)*x = (right.constant - left.constant)
   и решается.
   Если '=' нет, то выражение вычисляется и выводится либо как число (если
   коэффициент равен 0), либо в виде линейной комбинации.
-------------------------------------------------------------------------- */
void do_math(char *str) 
{
    // 1. Проверяем баланс скобок с использованием стека.
    Stack *checkStack = NULL;
    for (int i = 0; i < (int)strlen(str); i++) 
    {
        char c = str[i];
        if (c == '(') 
        {
            push(&checkStack, c);
        } 
        else if (c == ')') 
        {
            if (isEmpty(checkStack)) 
            {
                printf("Синтаксическая ошибка в позиции %d: лишняя закрывающая скобка '%c'\n", i, c);
                return;
            }
            pop(&checkStack);
        }
    }
    if (!isEmpty(checkStack)) 
    {
        printf("Синтаксическая ошибка: не все скобки закрыты\n");
        return;
    }

    // 2. Если присутствует знак '=', обрабатываем уравнение.
    char *eq = strchr(str, '=');
    if (eq != NULL) 
    {
        // Разбиваем строку на левую и правую части
        int lenPart = eq - str;
        char leftPart[256], rightPart[256];
        strncpy(leftPart, str, lenPart);
        leftPart[lenPart] = '\0';
        strcpy(rightPart, eq + 1);
        
        // Убираем пробелы в начале и конце левой части
        while(isspace(*leftPart)) 
            memmove(leftPart, leftPart + 1, strlen(leftPart));
        char *end = leftPart + strlen(leftPart) - 1;
        while(end >= leftPart && isspace(*end)) 
        { 
            *end = '\0'; 
            end--; 
        }
        // Аналогично для правой части
        while(isspace(*rightPart)) 
            memmove(rightPart, rightPart + 1, strlen(rightPart));
        end = rightPart + strlen(rightPart) - 1;
        while(end >= rightPart && isspace(*end)) 
        { 
            *end = '\0'; 
            end--; 
        }
        
        // Определяем имя неизвестной переменной:
        // Если левая часть является простым идентификатором, то используем его.
        // Иначе, ищем первый встреченный идентификатор.
        char unknownVar[64] = {0};
        if (isSimpleVariable(leftPart)) 
        {
            strcpy(unknownVar, leftPart);
        } 
        else 
        {
            int i = 0;
            while (leftPart[i] && !isalpha(leftPart[i]) && leftPart[i] != '_') i++;
            if (leftPart[i] == '\0') 
            {
                printf("Ошибка: не найден неизвестный идентификатор в левой части уравнения.\n");
                return;
            }
            int pos = 0;
            while (leftPart[i] && (isalnum(leftPart[i]) || leftPart[i]=='_') && pos < 63) 
            {
                unknownVar[pos++] = leftPart[i];
                i++;
            }
            unknownVar[pos] = '\0';
        }
        printf("[DEBUG] Неизвестная переменная: %s\n", unknownVar);
        
        const char *pLeft = leftPart;
        Linear leftLin = parseLinearExpression(&pLeft, unknownVar);
        const char *pRight = rightPart;
        Linear rightLin = parseLinearExpression(&pRight, unknownVar);
        
        // Приводим уравнение к виду: leftLin.coef*x + leftLin.constant = rightLin.coef*x + rightLin.constant
        // => (leftLin.coef - rightLin.coef)*x = (rightLin.constant - leftLin.constant)
        double A = leftLin.coef - rightLin.coef;
        double B = rightLin.constant - leftLin.constant;
        if (fabs(A) < 1e-9) 
        {
            if (fabs(B) < 1e-9)
                printf("[DEBUG] Уравнение имеет бесконечное множество решений.\n");
            else
                printf("[DEBUG] Уравнение не имеет решений.\n");
        } 
        else 
        
        {
            double solution = B / A;
            printf("[DEBUG] Решение уравнения: %s = %.2f\n", unknownVar, solution);
        }
        return;
    }
    
    // 3. Если знак '=' отсутствует:
    //    Вычисляем выражение как линейное. Если коэффициент равен 0 – выводим число,
    //    иначе – выводим в виде A*x + B.
    const char *pExpr = str;
    // По умолчанию неизвестная переменная имеет имя "x"
    Linear exprLin = parseLinearExpression(&pExpr, "x");
    if (fabs(exprLin.coef) < 1e-9)
        printf("[DEBUG] Вычислено значение: %.2f\n", exprLin.constant);
    else
        printf("[DEBUG] Выражение имеет вид: %.2f*x + (%.2f)\n", exprLin.coef, exprLin.constant);
}

//* ======================== INPUT ========================
void input_int_var(int* a, int t, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
		printf("Выберите тип выполняемой операции:\n\t 1 - проверка синтаксиса скобок\n\t 2 - проверка синтаксиса скобок + вычисление выражения\n");
		check = scanf("%d", a);								   				// ввод значения
		if (check != 1)														// проверка правильности типа введённого значения
			printf("\033[1;31m Ошибка ввода: не целое десятичное число, попробуйте ввести еще раз.\033[0m\n");  // вывод сообщения об ошибке
		else if (*a <= 0)
		{
			printf("\033[1;31m Ошибка ввода: не положительное число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		else if (*a >= 1000)
		{
			printf("\033[1;31m Ошибка ввода: Слишком большое число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		if (min != 0 && max != 0) 
		{
			if (*a < min || *a > max) 
			{
				printf("\033[1;31m Ошибка ввода: Число не в пределе 1-12, попробуйте ввести еще раз.\033[0m\n");// вывод сообщения об ошибке
				check--;
			}
		}
		while ((c = getchar()) != '\n' && c != EOF);						// очистка буфера ввода
	}
}

void input_str(char* destination)										// функция ввода названия фирмы
{
	char buffer[MAX_SIZE];											// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода
		int valid = 0;													// переменная для проверки ввода
		printf("Input string\n");
		while (!valid) 													// цикл ввода и проверки
		{
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) 			// ввод строки
			{
				printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			buffer[strcspn(buffer, "\n")] = '\0';						// Удаление символа новой строки, если он есть

			int char_count = 0;											// счётчик длины UTF-8 строки
			for (char* p = buffer; *p != '\0';) 						// Вычисление количества UTF-8 символов
			{
				if ((*p & 0x80) == 0)
					p += 1; // 1-байтовый символ (ASCII)
				else if ((*p & 0xE0) == 0xC0)
					p += 2; // 2-байтовый символ
				else if ((*p & 0xF0) == 0xE0)
					p += 3; // 3-байтовый символ
				else if ((*p & 0xF8) == 0xF0)
					p += 4; // 4-байтовый символ
				else 
				{
					printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
					char_count = -1;
					break;
				}
				char_count++;
			}

		if (char_count == -1)											// проверка наневерную кодировку
				continue;
			
			int buffer_len = strlen(buffer); 							// получение длины строки	

			if (char_count == 0) 										// проверка на пустую строку
			{
				printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			// if (!is_valid(buffer))                                                 // проверка на бытиё чистом
			// {
            //     printf("\033[1;31m Введено не числовое значение. Пожалуйста, повторите ввод.\033[0m\n");
            //     continue;
			// }

			if (char_count > MAX_SIZE/2-1)							// проверка на слишком большую строку
			{   
				printf("\033[1;31m Название слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);			// очистка буфера ввода			
			strncpy(destination, buffer, MAX_SIZE);				// копирование буффера в поле структуры
			valid = 1; 													// Ввод корректен, выход из цикла
		}
}

//* ======================== Restart ========================
void restart_program(int* flag)																				    // функция перезапуска программы
{
	printf("\nДля завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
	scanf("%d", flag);																							// ввод значения флага цикла программы
	if (*flag != 0)																								// проверка значения флага
		printf("Перезапуск программы...\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}