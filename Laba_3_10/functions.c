#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== Memory ===========================
Stack* createNode(char data) 
{
    Stack* new_node = malloc(sizeof(Stack));
    if (new_node) 
	{
        new_node->data = data;
        new_node->next = NULL;
    }
    return new_node;
}
//* ======================== STACK ========================
int isEmpty(Stack* stack) {
    return stack == NULL;
}

void push(Stack** stack, char new_data) {
    Stack* new_node = createNode(new_data);
    if (!new_node) {
        printf("\n Ошибка выделения памяти\n");
        return;
    }

    new_node->next = *stack;
    *stack = new_node;
}

void pop(Stack** stack) 
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

int peek(Stack* stack) 
{
    if (!isEmpty(stack)) 
	{
        return stack->data;
    }
    return 404;
}

//* ======================== VARIABLE ========================

// Поиск переменной в контексте
Var* findVariable(Context* ctx, const char* name) {
    Var* current = ctx->varList;
    while (current) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// Получение значения переменной
double getVariableValue(Context* ctx, const char* name) {
    Var* var = findVariable(ctx, name);
    if (var)
        return var->value;
    else {
        printf("Предупреждение: переменная \"%s\" не определена. Использую значение 0.\n", name);
        return 0.0;
    }
}

// Установка значения переменной
void setVariable(Context* ctx, const char* name, double value) {
    Var* var = findVariable(ctx, name);
    if (var) {
        var->value = value;
    } else {
        Var* newVar = (Var*)malloc(sizeof(Var));
        if (!newVar) {
            printf("Ошибка выделения памяти для переменной.\n");
            return;
        }
        strncpy(newVar->name, name, sizeof(newVar->name) - 1);
        newVar->name[sizeof(newVar->name)-1] = '\0';
        newVar->value = value;
        newVar->next = ctx->varList;
        ctx->varList = newVar;
    }
}

// Функция для освобождения памяти, занятой переменными
void freeVariables(Context* ctx) {
    Var* cur = ctx->varList;
    while (cur) {
        Var* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

//* ======================== Checks ============================
int is_valid(const char* str)                     	// функция проверки на наличие знака равенства
{
    while (*str)                            		// цикл по элементам строки
    {
        if (*str == '=') return 1;
        str++;
    }
    return 0;
}

int is_numeric(const char* str)                     	// функция проверки на бытиё числом
{
    while (*str)                            		// цикл по элементам строки
    {
        if (!isdigit(*str) && *str != '.' && *str != ',' && *str != '+' && *str != '-' && *str != '*' && *str != '/') return 0;
        str++;
    }
    return 1;
}
//* ======================== MATH ========================
void check_brackets(Stack* stack, char* str)
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
	display_stack(stack);
	if (isEmpty(stack))
	{
		printf("All good!");
	}
	
}


//TODO MATH with simplification
//TODO поддержка систем уравнений
double parseExpression(const char **s, Context* ctx);
double parseTerm(const char **s, Context* ctx);
double parseFactor(const char **s, Context* ctx);

double parseFactor(const char **s, Context* ctx) {
    while (isspace(**s)) (*s)++;
    double result = 0;
    if (**s == '(') {
        (*s)++;  // пропускаем '('
        result = parseExpression(s, ctx);
        while (isspace(**s)) (*s)++;
        if (**s == ')')
            (*s)++;
        else
            printf("Ошибка: ожидалась закрывающая скобка ')'\n");
        return result;
    } else if (isalpha(**s) || **s == '_') {
        // Разбор имени переменной: поддержка букв, цифр и '_'
        char varName[64];
        int pos = 0;
        while (isalnum(**s) || **s == '_') {
            if (pos < 63)
                varName[pos++] = **s;
            (*s)++;
        }
        varName[pos] = '\0';
        return getVariableValue(ctx, varName);
    } else {
        char* end;
        result = strtod(*s, &end);
        *s = end;
        return result;
    }
}

double parseTerm(const char **s, Context* ctx) {
    double result = parseFactor(s, ctx);
    while (1) {
        while (isspace(**s)) (*s)++;
        if (**s == '*') {
            (*s)++;
            double factor = parseFactor(s, ctx);
            result *= factor;
        } else if (**s == '/') {
            (*s)++;
            double factor = parseFactor(s, ctx);
            result /= factor;
        } else {
            break;
        }
    }
    return result;
}

double parseExpression(const char **s, Context* ctx) {
    double result = parseTerm(s, ctx);
    while (1) {
        while (isspace(**s)) (*s)++;
        if (**s == '+') {
            (*s)++;
            double term = parseTerm(s, ctx);
            result += term;
        } else if (**s == '-') {
            (*s)++;
            double term = parseTerm(s, ctx);
            result -= term;
        } else {
            break;
        }
    }
    return result;
}

double evaluate_expression(const char *expr, Context* ctx) {
    const char *s = expr;
    return parseExpression(&s, ctx);
}

// --- Функция simplify ---
// Извлекаем подстроку между скобками open и close, вычисляем её значение,
// затем подставляем число вместо скобочной группы.
void simplify(char* str, int open, int close, Context* ctx) {
    char expr[256];
    int length = close - open - 1; // содержимое между скобками
    strncpy(expr, str + open + 1, length);
    expr[length] = '\0';
    
    double value = evaluate_expression(expr, ctx);
    
    char value_str[64];
    sprintf(value_str, "%.2f", value);
    
    // Собираем новую строку: часть до открывающей скобки + значение + остаток
    char new_str[256] = {0};
    strncpy(new_str, str, open);
    new_str[open] = '\0';
    strcat(new_str, value_str);
    strcat(new_str, str + close + 1);
    
    strcpy(str, new_str);
}

// --- Функция do_math ---
// 1. Проверяем баланс скобок с использованием заданного стека.
// 2. Последовательно упрощаем внутренние скобочные группы.
// 3. Вычисляем итоговое значение выражения.
void do_math(Stack* stack, char* str, Context* ctx) {
    // Если присутствует знак '=', обрабатываем присваивание
    char* eq = strchr(str, '=');
    if (eq != NULL) {
        // Левая часть до '=' – имя переменной
        char varName[64];
        int len = eq - str;
        strncpy(varName, str, len);
        varName[len] = '\0';
        // Обрезаем пробелы по краям
        int start = 0;
        while (isspace(varName[start]))
            start++;
        int end = strlen(varName) - 1;
        while (end >= start && isspace(varName[end])) {
            varName[end] = '\0';
            end--;
        }
        // Правая часть – выражение для вычисления
        char* expression = eq + 1;
        double value = evaluate_expression(expression, ctx);
        setVariable(ctx, varName, value);
        printf("Установлена переменная %s = %.2f\n", varName, value);
        return;
    }
    
    // Проверка баланса скобок с использованием стека
    Stack* checkStack = NULL;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (c == '(') {
            push(&checkStack, c);
        } else if (c == ')') {
            if (isEmpty(checkStack)) {
                printf("Синтаксическая ошибка в позиции %d: лишняя закрывающая скобка '%c'\n", i, c);
                return;
            }
            char top = peek(checkStack);
            if (top == '(')
                pop(&checkStack);
            else {
                printf("Синтаксическая ошибка в позиции %d.\n", i);
                return;
            }
        }
    }
    if (!isEmpty(checkStack)) {
        printf("Синтаксическая ошибка: не все скобки закрыты\n");
        return;
    }
    
    // Последовательное упрощение выражения внутри скобок
    int open, close;
    char open_ch, close_ch;
    int found = 1;
    while (found) {
        found = 0;
        len = strlen(str);
        int last_open = -1;
        char last_open_ch = '\0';
        for (int i = 0; i < len; i++) {
            if (str[i] == '(') {
                last_open = i;
                last_open_ch = str[i];
            }
        }
        if (last_open != -1) {
            open = last_open;
            open_ch = last_open_ch;
            if (open_ch == '(')
                close_ch = ')';
            // Ищем соответствующую закрывающую скобку
            for (int i = open + 1; i < len; i++) {
                if (str[i] == close_ch) {
                    close = i;
                    found = 1;
                    break;
                }
            }
            if (found)
                simplify(str, open, close, ctx);
        }
    }
    
    // Вычисляем итоговое выражение (без скобок, переменные поддерживаются)
    double result = evaluate_expression(str, ctx);
    printf("Результат: %.2f\n", result);
}

//* ======================== INPUT ========================
void input_int_var(int* a, int t, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
		switch (t)															// вывод сообщений с описанием вводимой величины
		{
		case 1:
			printf("Введите количество студентов(целое положительное десятичное число): \n");
			break;
		case 2:
			printf("Выберите тип вводимой структуры: ФИО студента или ФИО студента и иформация о семье (1-2): \n");
			break;
		case 3:
			printf("Выберите состав семьи:\n\t1. отец, мать, брат\n\t2. отец, мать, сестра, брат\n\t3. отец, мать, сестра\n (1-3): \n");
			break;
		}
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

//* ======================== OUTPUT ========================
void display_stack(Stack* stack)
{
    printf("Stack:\n\t");
    while (stack != NULL)
    {
        printf("%c", stack->data);
        stack = stack->next;
    }
    printf("\n");
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