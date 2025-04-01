#ifndef MY_HEADER
#define MY_HEADER


//* ==================== максимальные размеры полей структуры ====================
#define MAX_STR_SIZE 256            // максимальная длина строки
#define MAX_GRAPH_SIZE 256          // максимальный размер графа

//*  =================== STRUCT =================== 
typedef struct Q_node             // структура очереди
{
    int clinic_number;          //
    int total_beds;             //
    int free_beds;              //
    //TODO location
    //TODO pointer

    struct Q_node* next;          //
} q_node;

typedef struct Queue            // структура очереди
{
    q_node* front;                //
    q_node* rear;                 // 
} Queue;

// typedef struct G_node 
// {
//     int vertex;
//     struct G_node* next;
// } g_node;

// typedef struct Graph 
// {
//     int numVertices;
//     g_node** adjLists; // Array of pointer to Node lists
// } Graph;

//* ==================== QUEUE =================== 
Queue* create_queue();
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int field, int data);
q_node* dequeue(Queue* queue);
int peek(Queue* queue, int field);
void free_queue(Queue* queue);

//* ==================== GRAPH =================== 


//* ==================== FUNCTIONAL =================== 


//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                    // функция ввода целого числа
void input_str(char* destination);				                        // функция ввода строки
void input_queue(Queue* queue);

//* ==================== RESTART =================== 
void restart_program(int* flag);		                                // функция перезапуска программы
#endif