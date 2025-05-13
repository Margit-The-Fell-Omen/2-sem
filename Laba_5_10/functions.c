#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


//* ======================== BIN_TREE ===========================

Node* createNode(int data) 
{
    // Выделяем память под новый узел
    Node* newNode = (Node*)malloc(sizeof(Node));

    // Проверяем, успешно ли выделилась память
    if (newNode == NULL) 
    {
        fprintf(stderr, "Ошибка: не удалось выделить память для нового узла\n");
        exit(EXIT_FAILURE); // Завершаем программу в случае ошибки выделения памяти
    }

    newNode->data = data;   // Присваиваем данные новому узлу
    newNode->left = NULL;   // Инициализируем левого потомка как NULL
    newNode->right = NULL;  // Инициализируем правого потомка как NULL
    return newNode;         // Возвращаем указатель на созданный узел
}

Node* insertNode(Node* root, int data) 
{
    // Если дерево (или поддерево) пусто, создаем новый узел и возвращаем его
    if (root == NULL) 
    {
        return createNode(data);
    }

    // Если вставляемое значение меньше значения текущего узла,
    // рекурсивно вставляем в левое поддерево
    if (data < root->data) 
    {
        root->left = insertNode(root->left, data);
    }
    // Если вставляемое значение больше значения текущего узла,
    // рекурсивно вставляем в правое поддерево
    else if (data > root->data) 
    {
        root->right = insertNode(root->right, data);
    }
    // Если значение уже существует в дереве, ничего не делаем (можно добавить обработку дубликатов)
    // и возвращаем текущий корень без изменений.

    return root; // Возвращаем (возможно, обновленный) указатель на корень
}

// Функция для поиска значения в бинарном дереве поиска
// Возвращает узел с искомым значением или NULL, если значение не найдено
Node* searchNode(Node* root, int data) 
{
    // Базовые случаи: корень равен NULL или значение найдено в корне
    if (root == NULL || root->data == data) 
    {
        return root;
    }

    // Если искомое значение меньше значения текущего узла,
    // ищем в левом поддереве
    if (data < root->data) 
    {
        return searchNode(root->left, data);
    }
    // Иначе (искомое значение больше), ищем в правом поддереве
    else 
    {
        return searchNode(root->right, data);
    }
}

// Симметричный (in-order) обход дерева: Левый -> Корень -> Правый
// Выводит элементы в отсортированном порядке для BST
void inorderTraversal(Node* root) 
{
    if (root != NULL) 
    {
        inorderTraversal(root->left);    // Сначала обходим левое поддерево
        printf("%d ", root->data);       // Затем посещаем (печатаем) корень
        inorderTraversal(root->right);   // Затем обходим правое поддерево
    }
}

// Прямой (pre-order) обход дерева: Корень -> Левый -> Правый
void preorderTraversal(Node* root)
{
    if (root != NULL) 
    {
        printf("%d ", root->data);       // Сначала посещаем (печатаем) корень
        preorderTraversal(root->left);   // Затем обходим левое поддерево
        preorderTraversal(root->right);  // Затем обходим правое поддерево
    }
}

// Обратный (post-order) обход дерева: Левый -> Правый -> Корень
void postorderTraversal(Node* root)
{
    if (root != NULL) 
    {
        postorderTraversal(root->left);  // Сначала обходим левое поддерево
        postorderTraversal(root->right); // Затем обходим правое поддерево
        printf("%d ", root->data);      // Затем посещаем (печатаем) корень
    }
}

// Функция для освобождения всей памяти, выделенной под дерево
void freeTree(Node* root)
{
    if (root == NULL) 
    {
        return; // Если узел пуст, ничего не делаем
    }

    // Рекурсивно освобождаем левое и правое поддеревья
    freeTree(root->left);
    freeTree(root->right);

    // Освобождаем текущий узел
    // printf("Освобождаем узел: %d\n", root->data); // Для отладки
    free(root);
}

// Функция для поиска минимального узла в дереве (левый потомок до конца)
Node* findMin(Node* node) 
{
    while (node && node->left != NULL) 
    {
        node = node->left;
    }
    return node;
}

// Функция удаления узла по ключу (значению) в BST
Node* deleteNode(Node* root, int key) 
{
    // Если дерево пустое, возвращаем NULL
    if (root == NULL) 
    {
        return root;
    }

    // Ищем узел для удаления по правилу BST
    if (key < root->data) 
    {
        // Если ключ меньше текущего, идём в левое поддерево
        root->left = deleteNode(root->left, key);
    } 
    else if (key > root->data) 
    {
        // Если ключ больше текущего, идём в правое поддерево
        root->right = deleteNode(root->right, key);
    } 
    else 
    {
        // Найден узел, соответствующий ключу
        // Обработка трёх основных случаев

        // 1. Узел не имеет потомков (листовой)
        if (root->left == NULL && root->right == NULL) 
        {
            free(root);
            return NULL;
        }
        // 2. Узел имеет только одного потомка
        else if (root->left == NULL) 
        {
            Node* temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL) 
        {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        // 3. Узел имеет двух потомков
        else 
        {
            // Находим минимальный узел в правом поддереве (наследник)
            Node* temp = findMin(root->right);
            // Копируем значение наследника в текущий узел
            root->data = temp->data;
            // Удаляем наследника из правого поддерева
            root->right = deleteNode(root->right, temp->data);
        }
    }
    return root;
}
//* ======================== INPUT ========================
void input_int_var(int* a, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
		check = scanf("%d", a);								   				// ввод значения
		if (check != 1)														// проверка правильности типа введённого значения
			printf("\033[1;31m Ошибка ввода: не целое десятичное число, попробуйте ввести еще раз.\033[0m\n");  // вывод сообщения об ошибке
		else if (*a >= 1000)
		{
			printf("\033[1;31m Ошибка ввода: Слишком большое число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		if (min != 0 && max != 0) 
		{
			if (*a < min || *a > max) 
			{
				printf("\033[1;31m Ошибка ввода: Число не в пределе 1-12, попробуйте ввести еще раз.\033[0m\n");// вывод сообщения об ошибке
				check--;
			}
		}
		while ((c = getchar()) != '\n' && c != EOF);						// очистка буфера ввода
	}
}

void input_str(char* destination)										// функция ввода названия фирмы
{
	char buffer[MAX_STR_SIZE];											// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода
		int valid = 0;													// переменная для проверки ввода
		while (!valid) 													// цикл ввода и проверки
		{
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) 			// ввод строки
			{
				printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			buffer[strcspn(buffer, "\n")] = '\0';						// Удаление символа новой строки, если он есть

			int char_count = 0;											// счётчик длины UTF-8 строки
			for (char* p = buffer; *p != '\0';) 						// Вычисление количества UTF-8 символов
			{
				if ((*p & 0x80) == 0)
					p += 1; // 1-байтовый символ (ASCII)
				else if ((*p & 0xE0) == 0xC0)
					p += 2; // 2-байтовый символ
				else if ((*p & 0xF0) == 0xE0)
					p += 3; // 3-байтовый символ
				else if ((*p & 0xF8) == 0xF0)
					p += 4; // 4-байтовый символ
				else 
				{
					printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
					char_count = -1;
					break;
				}
				char_count++;
			}

		if (char_count == -1)											// проверка наневерную кодировку
				continue;
			
			int buffer_len = strlen(buffer); 							// получение длины строки	

			if (char_count == 0) 										// проверка на пустую строку
			{
				printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			if (char_count > MAX_STR_SIZE/2-1)							// проверка на слишком большую строку
			{   
				printf("\033[1;31m Название слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_STR_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);			// очистка буфера ввода			
			strncpy(destination, buffer, MAX_STR_SIZE);				// копирование буффера в поле структуры
			valid = 1; 													// Ввод корректен, выход из цикла
		}
}

Node* input_bin_tree() 
{
    int input_num = 0;
    int elem = 0;
    Node* root = NULL;

    printf("----------- Ввод элементов дерева ------------\n");
    printf("Сколько элементов ввести?\n");
    input_int_var(&input_num, 1, 1000);
    printf("Введите элементы дерева:\n");
    
    for (int i = 0; i < input_num; i++) 
	{
        input_int_var(&elem, 0, 0);
        root = insertNode(root, elem);
    }
    
    return root;
}

// Функция меню для взаимодействия с пользователем
// Принимает УКАЗАТЕЛЬ НА УКАЗАТЕЛЬ на текущий корень дерева (Node** root_ptr)
// и указатель на флаг продолжения работы программы
void menu(Node** root_ptr, int* program_running_flag) 
{
    // Используем локальную переменную current_root для удобства,
    // которая является разыменованным указателем root_ptr.
    // Все изменения, которые должны затронуть корень дерева в main,
    // будут сделаны через присваивание current_root, а затем
    // в конце функции значение current_root будет записано обратно в *root_ptr.
    while (*program_running_flag) 
    {
        Node* current_root = *root_ptr;

        int choice = 0;
        printf("\nМеню управления бинарным деревом:\n"
               "1. Добавить элементы в дерево\n"
               "2. Удалить элемент из дерева\n"
               "3. Найти элемент в дереве\n"
               "4. Вывести дерево (обходы)\n"
               "5. Очистить (удалить) всё дерево\n"
               "0. Выход\n"
               "Выберите действие: ");
        input_int_var(&choice, 0, 5);    
        switch (choice) 
        {
            case 1: 
            { // Добавить элементы
                int sub_choice = 0;
                printf("\nДобавление элементов:\n"
                    "\t1 - Создать новое дерево (старое будет удалено, если существует)\n"
                    "\t2 - Добавить элементы в текущее дерево\n"
                    "Выберите действие: ");
                input_int_var(&sub_choice, 1, 2);

                if (sub_choice == 1) 
                { // Создать новое дерево
                    if (current_root != NULL) 
                    {
                        printf("Удаление существующего дерева...\n");
                        freeTree(current_root); // Освобождаем память старого дерева
                        current_root = NULL;    // Обнуляем указатель
                    }
                    current_root = input_bin_tree(); // input_bin_tree возвращает новый корень
                    if (current_root != NULL) printf("Создано новое дерево.\n");
                    else printf("Новое дерево не было создано (возможно, было введено 0 элементов).\n");

                } 
                else 
                { // sub_choice == 2: Добавить в текущее дерево
                    if (current_root == NULL) 
                    {
                        printf("Текущее дерево пусто. Создаём новое дерево...\n");
                        current_root = input_bin_tree();
                        if (current_root != NULL) printf("Создано новое дерево.\n");
                        else printf("Новое дерево не было создано (возможно, было введено 0 элементов).\n");
                    } 
                    else 
                    {
                        int add_count = 0;
                        printf("Сколько элементов добавить в текущее дерево? (1-100): ");
                        input_int_var(&add_count, 1, 100);
                        for (int i = 0; i < add_count; i++) 
                        {
                            int elem_to_add = 0;
                            printf("Введите элемент #%d для добавления: ", i + 1);
                            input_int_var(&elem_to_add, 0, 0);
                            current_root = insertNode(current_root, elem_to_add); // insertNode вернет (возможно измененный) корень
                        }
                        if (add_count > 0) printf("%d элемент(ов) успешно добавлен(ы).\n", add_count);
                    }
                }
                break;
            }
            case 2: 
            { // Удалить элемент
                if (current_root == NULL) 
                {
                    printf("Дерево пусто. Нечего удалять.\n");
                } 
                else 
                {
                    int key_to_delete = 0;
                    printf("Введите значение элемента для удаления: ");
                    input_int_var(&key_to_delete, 0, 0);
                    
                    Node* node_exists = searchNode(current_root, key_to_delete);
                    if (node_exists != NULL) 
                    {
                        current_root = deleteNode(current_root, key_to_delete); // deleteNode вернет новый корень
                        printf("Элемент %d удален.\n", key_to_delete);
                    } 
                    else 
                    {
                        printf("Элемент %d не найден в дереве. Удаление не произведено.\n", key_to_delete);
                    }
                }
                break;
            }
            case 3: 
            { // Найти элемент
                if (current_root == NULL) 
                {
                    printf("Дерево пусто. Нечего искать.\n");
                } 
                else 
                {
                    int key_to_find = 0;
                    printf("Введите значение элемента для поиска: ");
                    input_int_var(&key_to_find, 0, 0);
                    Node* foundNode = searchNode(current_root, key_to_find);
                    if (foundNode != NULL) 
                    {
                        printf("Элемент %d найден в дереве.\n", key_to_find);
                    } 
                    else 
                    {
                        printf("Элемент %d не найден в дереве.\n", key_to_find);
                    }
                }
                break;
            }
            case 4: 
            { // Вывести дерево (обходы)
                if (current_root == NULL) 
                {
                    printf("Дерево пусто. Нечего выводить.\n");
                } 
                else 
                {
                    int traversal_choice = 0;
                    printf("\nВыберите тип обхода для вывода дерева:\n"
                        "\t1 - Симметричный (In-order) - элементы в отсортированном порядке\n"
                        "\t2 - Прямой (Pre-order) - корень, лево, право\n"
                        "\t3 - Обратный (Post-order) - лево, право, корень\n"
                        "Выберите действие: ");
                    input_int_var(&traversal_choice, 1, 3);
                    switch (traversal_choice) 
                    {
                        case 1:
                            printf("Симметричный обход (In-order): ");
                            inorderTraversal(current_root);
                            printf("\n");
                            break;
                        case 2:
                            printf("Прямой обход (Pre-order): ");
                            preorderTraversal(current_root);
                            printf("\n");
                            break;
                        case 3:
                            printf("Обратный обход (Post-order): ");
                            postorderTraversal(current_root);
                            printf("\n");
                            break;
                    }
                }
                break;
            }
            case 5: 
            { // Очистить (удалить) всё дерево
                if (current_root == NULL) 
                {
                    printf("Дерево уже пусто.\n");
                } 
                else 
                {
                    freeTree(current_root);
                    current_root = NULL; // Обновляем локальную копию
                    printf("Дерево успешно очищено.\n");
                }
                break;
            }
            case 0: 
            { // Выход
                *program_running_flag = 0;
                restart_program(program_running_flag);					                // функция перезапуска программы
                // *root_ptr будет обновлен значением current_root в конце функции
                break;
            }
        }
        // ВАЖНО: Обновляем указатель в main значением current_root
        // Это нужно сделать один раз в конце функции, после всех операций.
        *root_ptr = current_root;
    }
}


//* ======================== Restart ========================
void restart_program(int* flag)																				    // функция перезапуска программы
{
	printf("\nДля завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
	scanf("%d", flag);																							// ввод значения флага цикла программы
	if (*flag != 0)																								// проверка значения флага
		printf("Перезапуск программы...\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}