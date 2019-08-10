#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

static int reversed = 0;
static int mseck = 0;
static int timed = 0;

struct answer
{
    int recieved;
    int first_num;
    int second_num;
    int time;
};

static
void
set_timeout_option (int *argc, char **(*argv))
{
    if (--*argc <= 0 || *(++*argv)[0] == '-')
    {
        printf ("expected number mseck to timeout\n");
    }
    else
    {
        mseck = atoi ((*argv)[0]);
    }
}

struct long_option
{
    char *option_name;
    void (*option_func) (int *, char ***);
};

static
struct long_option options[] =
{
    {
        .option_name = "set-timeout",
        .option_func = set_timeout_option
    }
};

static
void (*find_option_func(char *option_name)) (int *, char ***)
{
    for (size_t i = 0; i < sizeof (options) / sizeof (options[0]); ++i)
    {
        if (!strcmp (options[i].option_name, option_name))
        {
            return options[i].option_func;
        }
    }
    return NULL;
}

static
int
reverse (int val)
{
    int result = 0;
    int i = 0;

    while ((val / (int) pow (10, i)) > 0)
    {
        ++i;
    }

    do
    {
        --i;
        result += (val % 10) * pow (10, i);
        val /= 10;
    }
    while (i > 0);

    return result;
}

static
void
main_loop (int n_first_digits, int n_second_digits, int n_expressions, struct answer *results)
{
    int max_first_number  = pow (10, n_first_digits);
    int min_first_number  = pow (10, n_first_digits - 1);
    int max_second_number = pow (10, n_second_digits);
    int min_second_number = pow (10, n_second_digits - 1);

    int result = 0;

    time_t start = 0;
    time_t end = 0;

    for (int i = 0; i < n_expressions; ++i)
    {
        results[i].first_num  = rand () % (max_first_number  - min_first_number)  + min_first_number;
        results[i].second_num = rand () % (max_second_number - min_second_number) + min_second_number;

        printf ("expression №%d\n", i);

        printf ("%*d\n", MAX (n_first_digits, n_second_digits) + 1, results[i].first_num);
        printf ("*\n");
        printf ("%*d\n", MAX (n_first_digits, n_second_digits) + 1, results[i].second_num);

        printf (" ");
        start = time (NULL);
        if (scanf ("%d", &(results[i].recieved)) == -1)
        {
            break;
        }
        end = time (NULL);

        results[i].time = end - start;

        if (reversed)
        {
            results[i].recieved = reverse (results[i].recieved);
        }
    }
}

static
void
parse_arg (int *argc, char ***argv)
{
    while (--*argc > 0)
    {
        if (*(++(*argv))[0] == '-')
        {
            while (*++(*argv)[0] != '\0' && *(*argv)[0] != '-')
            {
                switch (*(*argv)[0])
                {
                    case 'r':
                        reversed = 1;
                        break;
                    case 't':
                        timed = 1;
                        break;
                    default:
                        printf ("unknown option -%c\n", *(*argv)[0]);
                        break;
                }
            }

            if (*(*argv)[0] == '-')
            {

                void (*option_func) (int *, char ***) = find_option_func (++(*argv)[0]);
                if (!option_func)
                {
                    printf ("unknown option long name --%s\n", (*argv)[0]);
                }
                else
                {
                    option_func (argc, argv);
                }
                break;
            }
        }
    }
}

static
void
print_res (int n_expressions, struct answer *results)
{
    printf ("\nRESULTS:\n\n");
    for (int i = 0; i < n_expressions; ++i)
    {
        printf ("expression - %d * %d\n\n", results[i].first_num, results[i].second_num);
        printf ("expected - %d\n", results[i].first_num * results[i].second_num);
        printf ("recieved - %s%d\x1b[0m\n", (results[i].first_num * results[i].second_num == results[i].recieved) ? "\x1b[32m" : "\x1b[31m",
                results[i].recieved);

        if (timed)
        {
            printf ("time result - %d sec\n", results[i].time);
        }

        putchar ('\n');
    }
}

int
main (int argc, char *argv[])
{
    parse_arg (&argc, &argv);

    srand (time (NULL));

    int n_first_digit  = 0;
    int n_second_digit = 0;
    int n_expressions  = 0;

    printf ("enter number of digits for first and second number: ");
    scanf ("%d %d", &n_first_digit, &n_second_digit);

    printf ("enter number of expressions: ");
    scanf ("%d", &n_expressions);

    struct answer results[n_expressions];

    memset (results, 0, sizeof (results));

    main_loop (n_first_digit, n_second_digit, n_expressions, results);

    print_res (n_expressions, results);

    return 0;
}
