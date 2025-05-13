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
Node* createNode(int data);                             // функция инициализации узла дерева
Node* insertNode(Node* root, int data);                 // функция добавления узла в дерево
Node* searchNode(Node* root, int data);                 // функция поиска узла в дереве по значению
void inorderTraversal(Node* root);                      // функция симметрического обхода дерева
void preorderTraversal(Node* root);                     // функция прямого обхода дерева
void postorderTraversal(Node* root);                    // функция обратного обхода дерева
void freeTree(Node* root);                              // функция освобождения памяти дерева

void menu(Node** root_ptr, int* program_running_flag);  // функция взаимодействия с пользователем
//* ==================== INPUT =================== 
void input_int_var(int* a, int min, int max);           // функция ввода целого числа
void input_str(char* destination);				        // функция ввода строки
Node* input_bin_tree();                                 // функция ввода элементов дерева

//* ==================== RESTART =================== 
void restart_program(int* flag);		                // функция перезапуска программы
#endif