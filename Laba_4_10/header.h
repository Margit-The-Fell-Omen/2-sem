#ifndef MY_HEADER
#define MY_HEADER


//* ==================== максимальные размеры полей структуры ====================
#define MAX_STR_SIZE 256            // максимальная длина строки

//*  =================== STRUCT =================== 
typedef struct Q_node             // структура очереди
{
    int clinic_number;          //
    int total_beds;             //
    int free_beds;              //
    char** patients;
    char location[MAX_STR_SIZE];
    //TODO pointer

    struct Q_node* next;          //
} q_node;

typedef struct Queue            // структура очереди
{
    q_node* front;                //
    q_node* rear;                 // 
} Queue;


//* ==================== QUEUE =================== 
Queue* create_queue();
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int field, int data);
q_node* dequeue(Queue* queue);
int peek(Queue* queue, int field);
void free_queue(Queue* queue);

//* ==================== FUNCTIONAL =================== 
void add_patient(Queue* queue);
void delete_patients(Queue* queue);

//* ==================== INPUT =================== 
void input_int_var(int* a, int min, int max);                    // функция ввода целого числа
void input_str(char* destination, int check_num);				                        // функция ввода строки
int validateLatLon(const char *input);
int input_queue(Queue* queue);

//* ==================== OUTPUT ===================
void output_beds(Queue* queue, int queue_len);
void output_patients(Queue* queue, int patient_num, int nospital_num);
//* ==================== RESTART =================== 
void restart_program(int* flag);		                                // функция перезапуска программы
#endif