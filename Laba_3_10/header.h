#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_SIZE 62            // максимальная длина имени/фамилии/отчества

//*  =================== STRUCT =================== 
typedef struct Stack 
{
    char data;
    struct Stack* next;
} Stack;

typedef struct Var {
    char name[64];
    double value;
    struct Var* next;
} Var;

// Структура контекста, содержащая список переменных
typedef struct {
    Var* varList;
} Context;

//* ==================== MEMORY =================== 
Stack* createNode(char data);

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                                                            // функция ввода целого числа

void input_str(char* destination);				                                                                                // функция ввода строки

//* ==================== STACK =================== 
int isEmpty(Stack* stack);
void push(Stack** stack, char new_data);
void pop(Stack** stack) ;
int peek(Stack* stack);

//* ==================== MATH =================== 
void check_brackets(Stack* stack, char* destination);
void do_math(Stack* stack, char* str, Context* ctx);
void simplify(char* str, int open, int close, Context* ctx);
//* ==================== OUTPUT =================== 
void display_stack(Stack* stack);

//* ==================== RESTART =================== 
void restart_program(int* flag);													                                            // функция перезапуска программы
#endif