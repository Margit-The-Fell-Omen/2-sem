#ifndef MY_HEADER
#define MY_HEADER
#define MAX_SIZE 10

// информация о фирме
typedef struct Firm_info 
{
    wchar_t* name;
    wchar_t* taxes;
    wchar_t* taxes_deadline;
    wchar_t* taxes_payed;
} firm_info;


firm_info** input_firm_name(int number_of_firms);
#endif