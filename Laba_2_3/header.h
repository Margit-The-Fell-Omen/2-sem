#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры ====================
#define MAX_NAME_SIZE 62

//*  =================== STRUCT & UNION =================== 
typedef struct                      //
{
    char name[MAX_NAME_SIZE];       //
    char surname[MAX_NAME_SIZE];    //
    char patronymic[MAX_NAME_SIZE]; //
} Full_name;

typedef union                       //
{
    struct                          //
    {
        Full_name father;           //
        Full_name mother;           //
        Full_name brother;          //
    } optionA;
    struct 
    {
        Full_name father;           //
        Full_name mother;           //
        Full_name brother;          //
        Full_name sister;           //
    } optionB;
    struct 
    {
        Full_name mother;           //
        Full_name brother;          //
        Full_name sister;           //
    } optionC;
} FamilyUnion;

typedef struct Student_with_family                                                                      //
{
    Full_name full_name;                                                                                //
    FamilyUnion family;                                                                                 //

    void (*output_1)(struct Student_with_family* students, int number_of_students, int family_choise);  //
    void (*output_2)(struct Student_with_family* students, int number_of_students, int family_choise);  //
    void (*output_3)(struct Student_with_family* students, int number_of_students, int family_choise);  //
} Student_with_family;

//* ==================== MEMORY =================== 
Full_name* students_alloc(int number_of_students);                                                                              // функция выделения памяти для 1 структуры
Student_with_family* students_with_families_alloc(int number_of_students);                                                      // функция выделения памяти для 2 структуры
void students_free(Full_name* students);                                                                                        // функция освобождения памяти 1 структуры
void students_with_families_free(Student_with_family* students);                                                                // функция освобождения памяти 2 структуры

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                                                            // функция ввода целого числа

void input_students_names(Full_name* students, int number_of_students);                                                         // 
int input_students_with_families_names( Student_with_family* students, int number_of_students, int family_choice);              //

void input_str(char* destination);				                                                                                // функция ввода строки

//* ==================== OUTPUT =================== 
void output_student_full_name(Full_name* students, int number_of_students);                                                     //
void output_student_with_family(Student_with_family* students, int number_of_students, int family_choice, int function_choice); //
void output_1(Student_with_family* students, int number_of_students, int family_choise);                                        //
void output_2(Student_with_family* students, int number_of_students, int family_choise);                                        //
void output_3(Student_with_family* students, int number_of_students, int family_choise);                                        //

//* ==================== RESTART =================== 
void restart_program(int* flag);													                                            // функция перезапуска программы
#endif