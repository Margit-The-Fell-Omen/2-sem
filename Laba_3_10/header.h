#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_SIZE 256            // максимальная длина имени/фамилии/отчества

//*  =================== STRUCT =================== 
typedef struct Stack 
{
    char data;
    struct Stack* next;
} Stack;

typedef struct {
    double coef;      // коэффициент при неизвестной переменной
    double constant;  // свободный член
} Linear;

//* ==================== MEMORY =================== 
Stack* createNode(char data);

//* ==================== STACK =================== 
int isEmpty(Stack* stack);
void push(Stack** stack, char new_data);
void pop(Stack** stack) ;
int peek(Stack* stack);

//* ==================== MATH =================== 
Linear parseLinearFactor(const char **s, const char *unknownVar);
Linear parseLinearTerm(const char **s, const char *unknownVar);
Linear parseLinearExpression(const char **s, const char *unknownVar);

int isSimpleVariable(const char *str);
void check_brackets(Stack* stack, char* destination);
void do_math(char* str);
void simplify(char* str, int open, int close);

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                                                            // функция ввода целого числа
void input_str(char* destination);				                                                                                // функция ввода строки

//* ==================== OUTPUT =================== 
void display_stack(Stack* stack);

//* ==================== RESTART =================== 
void restart_program(int* flag);													                                            // функция перезапуска программы
#endif