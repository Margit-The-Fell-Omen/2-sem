#ifndef MY_HEADER
#define MY_HEADER

// информация о фирме
typedef struct Firm_info 
{
    char name[32];
    char taxes[9];
    char taxes_deadline[12];
    char taxes_payed[12];
} firm_info;

firm_info* firm_info_alloc(int number_of_firms);

int is_numeric(const char* str);
int is_valid_num(const char* str);

void input_int_var(int* a, int t, int min, int max)	;
const char* input_month();

void input_firm_name(firm_info* list_of_firms, int number_of_firms);
void input_firm_taxes(firm_info* list_of_firms, int number_of_firms);
void input_firm_dates(firm_info* list_of_firms, int number_of_firms);

void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t);
void output_top_5(firm_info* list_of_firms, int number_of_firms, const char* month);

void firm_info_free(firm_info* list_of_firms);

void restart_program(int* flag);
#endif