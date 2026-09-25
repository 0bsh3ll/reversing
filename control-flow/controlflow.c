#include <stdio.h>

/*
 * FOR LOOP
 */
void for_loop()
{
    volatile int i;

    for (i = 0; i < 5; i++)
    {
        printf("FOR: i = %d\n", i);
    }
}

/*
 * WHILE LOOP
 */
void while_loop()
{
    volatile int i = 0;

    while (i < 5)
    {
        printf("WHILE: i = %d\n", i);
        i++;
    }
}

/*
 * DO-WHILE LOOP
 */
void do_while_loop()
{
    volatile int i = 0;

    do
    {
        printf("DO-WHILE: i = %d\n", i);
        i++;
    }
    while (i < 5);
}

/*
 * MAIN
 */
int main()
{
    for_loop();

    while_loop();

    do_while_loop();

    return 0;
}
