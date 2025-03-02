#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_NAME_SIZE 62

//*  =================== информация о фирме =================== 
typedef struct 
{
    char name[MAX_NAME_SIZE];
    char surname[MAX_NAME_SIZE];
    char patronymic[MAX_NAME_SIZE];
} Full_name;

typedef union 
{
    struct 
    {
        Full_name father;
        Full_name mother;
        Full_name brother;
    } optionA;
    struct 
    {
        Full_name father;
        Full_name mother;
        Full_name brother;
        Full_name sister;
    } optionB;
    struct 
    {
        Full_name mother;
        Full_name brother;
        Full_name sister;
    } optionC;
} FamilyUnion;

typedef struct Student_with_family
{
    Full_name full_name;
    FamilyUnion family;
    void (*output_student_full_name)(struct Student_with_family*, int);
    void (*output_student_with_family)(struct Student_with_family*, int, int, int);
} Student_with_family;

//* ==================== MEMORY =================== 
Full_name* students_alloc(int number_of_students);
Student_with_family* students_with_families_alloc(int number_of_students);              // функция выделения памяти для структуры
void students_free(Full_name* students);                                                   // функция освобождения памяти структуры
void students_with_families_free(Student_with_family* students);

//* ==================== CHECKS =================== 
int is_numeric(const char* str);                                                    // функция проверки на бытиё числом
int is_valid_num(const char* str);                                                  // функция проверки числа
int str_compare(const char* str1, const char* str2);                                // функция сравнения строк

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                // функция ввода целого числа

void input_students_names(Full_name* students, int number_of_students);
int input_students_with_families_names( Student_with_family* students, int number_of_students, int family_choice);

void input_str(char* destination);				    // функция ввода строки

//* ==================== OUTPUT =================== 
void output_student_full_name(Full_name* students, int number_of_students);
void output_student_with_family(Student_with_family* students, int number_of_students, int family_choice, int function_choice);

//* ==================== RESTART =================== 
void restart_program(int* flag);													// функция перезапуска программы
#endif