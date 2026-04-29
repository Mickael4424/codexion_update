#include "codexion.h"


int check_nb_arguments (int ac)
{
    if (ac != 9)
    {
        printf("Invalid arguments\n");
        printf("./codexion nb_of_coders t_to_burnout t_to_compile ");
        printf("t_to_debug t_to_refactor nb_of_compiles_required ");
        printf("dongle_cooldown scheduler\n");
        return(1);
    }
    else
    {
        return(0);
    }
}

int check_positiv_int(char **av)
{
    int i;
    int j;

    for (i = 1; i < 8; i++)
    {
        if (av[i][0] == '\0')
        {
            printf("Argument %d must be a positive integer.\n", i);
            return(1);
        }
        for(j = 0; av[i][j]; j++)
        {
            if (av[i][j] < '0' || av[i][j] > '9')
            {
                printf("Argument %d must be a positive integer.\n", i);
                return(1);
            }
        }
        if (atoi(av[i]) <= 0)
        {
            printf("Argument %d must be a positive integer.\n", i);
            return(1);
        }
    }
    return (0);
}

int check_last_arg(char **av)
{
    if (strcmp(av[8], "fifo") == 0 || (strcmp(av[8], "edf") == 0))
        return(0);

    printf("Last argument must be fifo or edf.\n");
    return (1);
}

int parsing(int ac, char **av)
{
    if (check_nb_arguments (ac)
    || check_positiv_int(av)
    || check_last_arg(av))
        return(1);
    else
    {
        return(0);
    }
}