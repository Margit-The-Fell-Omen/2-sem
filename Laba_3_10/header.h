#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_NAME_SIZE 62            // максимальная длина имени/фамилии/отчества

//*  =================== STRUCT =================== 
typedef struct Stack 
{
    char data;
    struct Stack* next;
} Stack;

//* ==================== MEMORY =================== 
Stack* createNode(char new_data);

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                                                            // функция ввода целого числа

void input_str(char* destination);				                                                                                // функция ввода строки

//* ==================== STACK =================== 
void initializeStack(Stack* stack);
int isEmpty(Stack* stack);
void push(Stack* stack, char new_data);
void pop(Stack* stack);
int peek(Stack* stack);

//* ==================== MATH =================== 
void check_brackets(Stack* stack, char* destination);

//* ==================== OUTPUT =================== 
void display_stack(Stack* stack)

//* ==================== RESTART =================== 
void restart_program(int* flag);													                                            // функция перезапуска программы
#endif