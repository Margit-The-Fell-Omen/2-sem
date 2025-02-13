#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "header.h"


firm_info** input_firm_name(int number_of_firms)
{
    
    firm_info** list_of_firms = memory_alloc(number_of_firms);
    wchar_t* name = (wchar_t*)calloc(30, sizeof(wchar_t));
    for (int i = 0; i < list_of_firms; i++)
    {
        wprintf(L"Введите название %d фирмы:", i+1);
        getws(name);
        list_of_firms[i]->name = name;
    }
}

firm_info** memory_alloc(int number_of_firms)
{
    firm_info** list_of_firms = (firm_info**)calloc(number_of_firms, sizeof(firm_info*));
    for (int i = 0; i < list_of_firms; i++)
    {
        *(list_of_firms + i) = malloc(sizeof(firm_info));
    }
    return list_of_firms;
}