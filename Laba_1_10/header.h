#ifndef MY_HEADER
#define MY_HEADER

// информация о фирме
typedef struct Firm_info 
{
    char* name;
    char* taxes;
    char* taxes_deadline;
    char* taxes_payed;
} firm_info;

firm_info* firm_info_alloc(int number_of_firms);

void input_int_var(int* a, int t);
void input_firm_name(firm_info* list_of_firms, int number_of_firms);
void input_firm_taxes(firm_info* list_of_firms, int number_of_firms);
void input_firm_dates(firm_info* list_of_firms, int number_of_firms);

void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t);
void restart_program(int* flag);
#endif