#include <stdio.h>
#include <stdlib.h>


int* array_stack (void);
int* array_heap (void);


int main (void)
{
    int array[5];

    printf("uninitialisierter Wert array[0] == %d\n", array[0]);
    printf("ungültiges Element array[5] == %d\n", array[5]);
    printf("Wert aus Funktion array_stack()[0] == %d\n", array_stack()[0]);
    printf("Wert aus Funktion array_heap()[0] == %d\n", array_heap()[0]);

    return EXIT_SUCCESS;
}


int* array_stack (void)
{
    int array[] = { 0, 1, 2, 3, 4};
    return array;
}


int* array_heap (void)
{
    int* buff = (int*) malloc(5 * sizeof(int));
    for (int i = 0; i < 5; ++i)
        buff[0] = i;
    return buff;
}
