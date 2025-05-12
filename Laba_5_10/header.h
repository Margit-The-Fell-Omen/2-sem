#ifndef MY_HEADER
#define MY_HEADER


//* ==================== максимальные размеры полей структуры ====================
#define MAX_STR_SIZE 1000            // максимальная длина строки

//*  =================== STRUCT =================== 

typedef struct Node 
{
    int data;           // Данные, хранящиеся в узле
    struct Node *left;  // Указатель на левый дочерний узел
    struct Node *right; // Указатель на правый дочерний узел
} Node;

//* ==================== BIN_TREE =================== 
Node* createNode(int data);
Node* insertNode(Node* root, int data);
Node* searchNode(Node* root, int data);
void inorderTraversal(Node* root);
void preorderTraversal(Node* root);
void postorderTraversal(Node* root);
void freeTree(Node* root);

void menu(Node** root_ptr, int* program_running_flag);
//* ==================== INPUT =================== 
void input_int_var(int* a, int min, int max);                           // функция ввода целого числа
void input_str(char* destination);				                        // функция ввода строки
Node* input_bin_tree();

//* ==================== RESTART =================== 
void restart_program(int* flag);		                                // функция перезапуска программы
#endif