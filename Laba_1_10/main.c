#include "header.h"

int main()
{
    int number_of_firms;
    int flag = 1;
    firm_info* list_of_firms;
    while (flag)
    {
        input_int_var(&number_of_firms, 1);

        list_of_firms = firm_info_alloc(number_of_firms);

        input_firm_name(list_of_firms, number_of_firms);
        input_firm_taxes(list_of_firms, number_of_firms);
        input_firm_dates(list_of_firms, number_of_firms);

        output_firm_info(list_of_firms, number_of_firms, 1);

        restart_program(&flag);
    }
    return 0;
}
