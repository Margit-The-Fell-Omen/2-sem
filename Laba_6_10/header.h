#ifndef MY_HEADER
#define MY_HEADER

//* ==================== максимальные размеры полей структуры firm_info ====================
#define MAX_NAME_SIZE 62
#define MAX_TAX_SIZE 9
#define MAX_DATE_SIZE 32

//*  =================== информация о фирме =================== 
typedef struct Firm_info 
{
    char name[MAX_NAME_SIZE];
    char taxes[MAX_TAX_SIZE];
    char taxes_deadline[MAX_DATE_SIZE];
    char taxes_payed[MAX_DATE_SIZE];
} firm_info;

//* =================== File Operations =====================
int save_firms_to_file(firm_info* list_of_firms, int number_of_firms, const char* filename);
firm_info* load_firms_from_file(int* number_of_firms_ptr, const char* filename);

//* ==================== MEMORY =================== 
firm_info* firm_info_alloc(int number_of_firms);                                    // функция выделения памяти для структуры
void firm_info_free(firm_info* list_of_firms);                                      // функция освобождения памяти структуры

//* ==================== CHECKS =================== 
int is_numeric(const char* str);                                                    // функция проверки на бытиё числом
int is_valid_num(const char* str);                                                  // функция проверки числа
int str_compare(const char* str1, const char* str2);                                // функция сравнения строк
int is_valid_date(const char* str);                                                 // функция проверки даты

//* ==================== TOP-5 =================== 
int compare(const void* a, const void* b);                                          // функция сравнения
int what_month(const char* str);                                                    // функция определения порядкогого номера месяца
int* find_top_5(firm_info* list_of_firms, int number_of_firms, const char* month);  // функция нахождения топ-5 фирм по налогам
void sort_top_5_by_name(firm_info* list_of_firms, int* top_5, int count);           // функция сортировки в алфавитном порядке

//* ==================== INPUT =================== 
void input_all_firm_data(firm_info* list_of_firms, int number_of_firms);
void input_int_var(int* a, int t, int min, int max);                                // функция ввода целого числа
const char* input_month();															// функция ввода месяца
void input_firm_name(firm_info* list_of_firms, int number_of_firms);				// функция ввода названия фирмы
void input_firm_taxes(firm_info* list_of_firms, int number_of_firms);				// функция ввода налогов
void input_firm_dates(firm_info* list_of_firms, int number_of_firms);				// функция ввода дат дедлайна и оплаты налогов

//* ==================== OUTPUT =================== 
void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t);		// функция вывода данных о фирмах
void output_top_5(firm_info* list_of_firms, int number_of_firms, const char* month);// функция вывода топ-5 фирм по налогам

//* ==================== RESTART =================== 
void restart_program(int* flag);													// функция перезапуска программы
#endif