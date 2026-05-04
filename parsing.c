#include "codexion.h"

static int check_positiv_int(char *str)
{
    int i;

    i = 0;

    if (!str || !str[0])
        return (1);

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (1);
        i++;
    }
    if (atoi(str) < 0)
        return (1);

    return (0);
}

static int check_arguments (int ac, char **av)
{
    if (ac != 9)
    {
        printf("Usage:\n");
        printf("./codexion nb_of_coders t_to_burnout t_to_compile ");
        printf("t_to_debug t_to_refactor nb_of_compiles_required ");
        printf("dongle_cooldown scheduler\n");
        return(1);
    }
    for (int i = 1; i < 8; i++)
    {
        if (check_positiv_int(av[i]))
        {
            printf("Argument %d must be a positive integer.\n", i);
            return (1);
        }
    }
    if (strcmp(av[8], "fifo") != 0 && (strcmp(av[8], "edf") != 0))
    {
        printf("Error: scheduler must be fifo or edf.\n");
        return (1);
    }
    return (0);
}

int parsing(int ac, char **av)
{
    if (check_arguments (ac, av))
        return (1);
    return (0);
}