#include "header.h"

int main()
{
    int number_of_firms;									// количество фирм
    int flag = 1;											// флаг главного цикла программы
    firm_info* list_of_firms;								// объявление массива структур
    const char* month;                                      // название месяца

    while (flag)                                            // главный цикл программы
    {
        input_int_var(&number_of_firms, 1, 0, 0);           // функция ввода целого числа

        list_of_firms = firm_info_alloc(number_of_firms);	// функция выделения памяти для структуры

        input_firm_name(list_of_firms, number_of_firms);	// функция ввода названия фирмы
        output_firm_info(list_of_firms, number_of_firms, 1);// функция вывода данных о фирмах

        input_firm_taxes(list_of_firms, number_of_firms);	// функция ввода налогов
        output_firm_info(list_of_firms, number_of_firms, 2);// функция вывода данных о фирмах

        input_firm_dates(list_of_firms, number_of_firms);	// функция ввода дат дедлайна и оплаты налогов
        output_firm_info(list_of_firms, number_of_firms, 3);// функция вывода данных о фирмах

        month = input_month();					            // функция ввода месяца
        output_top_5(list_of_firms, number_of_firms, month);// функция вывода топ-5 фирм по налогам

        firm_info_free(list_of_firms);						// функция освобождения памяти структуры
        restart_program(&flag);								// функция перезапуска программы
    }
    return 0;
}
