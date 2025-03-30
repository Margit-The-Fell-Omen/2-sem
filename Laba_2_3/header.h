#ifndef MY_HEADER
#define MY_HEADER
 
//* ==================== максимальные размеры полей структуры ====================
#define MAX_NAME_SIZE 62            // максимальная длина имени/фамилии/отчества

//*  =================== STRUCT & UNION =================== 
typedef struct                      // структура ФИО
{
    char name[MAX_NAME_SIZE];       // имя
    char surname[MAX_NAME_SIZE];    // фамилия
    char patronymic[MAX_NAME_SIZE]; // отчество
} Full_name;

typedef union                       // объединение семья
{
    struct                          // а)
    {
        Full_name father;           // ФИО отца
        Full_name mother;           // ФИО матери
        Full_name brother;          // ФИО брата
    } optionA;
    struct                          // б)
    {
        Full_name father;           // ФИО отца
        Full_name mother;           // ФИО матери
        Full_name brother;          // ФИО брата
        Full_name sister;           // ФИО сестры
    } optionB;
    struct                          // в)
    {
        Full_name mother;           // ФИО матери
        Full_name brother;          // ФИО брата
        Full_name sister;           // ФИО сестры
    } optionC;
} FamilyUnion;

typedef struct Student_with_family                                                                      // структура стедент с семьёй
{
    Full_name full_name;                                                                                // ФИО студента
    FamilyUnion family;                                                                                 // семья студента

    void (*output_1)(struct Student_with_family* students, int number_of_students, int family_choise);  // указатель на функцию вывода ФИО студента с семьёй
    void (*output_2)(struct Student_with_family* students, int number_of_students, int family_choise);  // указатель на функцию вывода ФИО семьи
    void (*output_3)(struct Student_with_family* students, int number_of_students, int family_choise);  // указатель на функцию вывода ФИО студента и семьи
} Student_with_family;

//* ==================== MEMORY =================== 
Full_name* students_alloc(int number_of_students);                                                                              // функция выделения памяти для 1 структуры
Student_with_family* students_with_families_alloc(int number_of_students);                                                      // функция выделения памяти для 2 структуры
void students_free(Full_name* students);                                                                                        // функция освобождения памяти 1 структуры
void students_with_families_free(Student_with_family* students);                                                                // функция освобождения памяти 2 структуры

//* ==================== INPUT =================== 
void input_int_var(int* a, int t, int min, int max);                                                                            // функция ввода целого числа

void input_students_names(Full_name* students, int number_of_students);                                                         // функция ввода ФИО студентов без семей
void input_students_with_families_names( Student_with_family* students, int number_of_students, int family_choice);              // ввод ФИО студентов с семьями

void input_str(char* destination);				                                                                                // функция ввода строки

//* ==================== OUTPUT =================== 
void output_student_full_name(Full_name* students, int number_of_students);                                                     // функция вывода ФИО студентов без семей
void output_student_with_family(Student_with_family* students, int number_of_students, int family_choice, int function_choice); // функция вызова выбранной функции вывода
void output_1(Student_with_family* students, int number_of_students, int family_choise);                                        // функция вывода ФИО студента с семьёй
void output_2(Student_with_family* students, int number_of_students, int family_choise);                                        // функция вывода ФИО семьи
void output_3(Student_with_family* students, int number_of_students, int family_choise);                                        // функция вывода ФИО студента и семьи

//* ==================== RESTART =================== 
void restart_program(int* flag);													                                            // функция перезапуска программы
#endif