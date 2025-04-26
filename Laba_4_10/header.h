#ifndef MY_HEADER
#define MY_HEADER


//* ==================== максимальные размеры полей структуры ====================
#define MAX_STR_SIZE 1000            // максимальная длина строки

//*  =================== STRUCT =================== 

typedef struct Q_node    // структура элемента очереди
{
    int clinic_number;                                                    // номер больницы
    int total_beds;                                                       // всего мест
    int free_beds;                                                        // всего свободных мест
    char** patients;                                                      // пациенты больницы
    char location[MAX_STR_SIZE];                                          // местоположение больницы
    void (*calculate_distance)\
    (const char *coord1_str, const char *coord2_str);                     // Указатель на функцию для расчета расстояния
    struct Q_node* next;                                                  // следующий элемент очереди
} q_node;

typedef struct Queue    // структура очереди
{
    q_node* front;      // начало очереди
    q_node* rear;       // конец очереди
} Queue;


//* ==================== QUEUE =================== 
Queue* create_queue();
int isEmpty(Queue* queue);
int enqueue(Queue* queue, int clinic_number, int total_beds, int free_beds, const char* location);
q_node* dequeue(Queue* queue);
int peek(Queue* queue, int field);
void free_queue(Queue* queue);

//* ==================== FUNCTIONAL =================== 
void menu(Queue* queue);

void add_patient(Queue* queue);
void delete_patient(Queue* queue);

int parse_coordinates(const char *coord_str, double *latitude, double *longitude);
double degrees_to_radians(double degrees);
double calculate_distance(const char *coord1_str, const char *coord2_str);
double find_nearest_hospital(Queue* queue, const char* location);

//* ==================== INPUT =================== 
void input_int_var(int* a, int min, int max);                    // функция ввода целого числа
void input_str(char* destination, int check_num);				                        // функция ввода строки
int validateLatLon(const char *input);
int input_queue(Queue* queue);

//* ==================== OUTPUT ===================
void output_beds(Queue* queue);
void output_patients(Queue* queue, int nospital_num);
//* ==================== RESTART =================== 
void restart_program(int* flag);		                                // функция перезапуска программы
#endif