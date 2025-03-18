#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_SIZE 256            // максимальная длина имени/фамилии/отчества

//*  =================== STRUCT =================== 
typedef struct Stack            // структура стека
{
    char data;                  // поле данных элемента стека
    struct Stack* next;         // указатель на следующий элемент стека
} Stack;

typedef struct                  // структура линейного выражения
{
    double coef;                // коэффициент при неизвестной переменной
    double constant;            // свободный член
} Linear;

//* ==================== STACK =================== 
Stack* createNode(char data);               // функция создания элемента стека
int isEmpty(Stack* stack);                  // функция проверки стека на пустоту
void push(Stack** stack, char new_data);    // функция добавления элемента в стек
void pop(Stack** stack) ;                   // функция удаления элемента из стека
int peek(Stack* stack);                     // функция возвращения верхнего элемента стека
void display_stack(Stack* stack);           // функция вывода элементов стека

//* ==================== MATH =================== 
Linear parseLinearFactor(const char **s, const char *unknownVar);       // функция вычисления линейного фактора
Linear parseLinearTerm(const char **s, const char *unknownVar);         // функция вычисления линейного терма
Linear parseLinearExpression(const char **s, const char *unknownVar);   // функция вычисления линейного выражения

void skipSpaces(const char **s);                                        // функция пропуска пробелов в строке
int isSimpleVariable(const char *str);                                  // функция проверки на простую переменную
void check_brackets(Stack* stack, char* destination);                   // функция проверки синтаксиса скобок
void do_math(char* str);                                                // функция вычисления линейного уравнения

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                    // функция ввода целого числа
void input_str(char* destination);				                        // функция ввода строки

//* ==================== RESTART =================== 
void restart_program(int* flag);		                                // функция перезапуска программы
#endif