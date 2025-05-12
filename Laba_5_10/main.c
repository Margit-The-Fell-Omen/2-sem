#include "header.h"
#include <stdio.h>

int main()
{
    
    int flag = 1;											            // флаг главного цикла программы
    int behaviour_choice;                                               // выбор выполняемых действий
    while (flag)                                                        // главный цикл программы
    {
        Node* root = NULL; // Инициализируем пустое дерево

        menu(&root, &flag);
    }
    return 0;
}