#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

static int reversed = 0;

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
main_loop (int n_first_digits, int n_second_digits, int n_expressions)
{
    int max_first_number  = pow (10, n_first_digits);
    int min_first_number  = pow (10, n_first_digits - 1);
    int max_second_number = pow (10, n_second_digits);
    int min_second_number = pow (10, n_second_digits - 1);

    int result = 0;

    for (int i = 0; i < n_expressions; ++i)
    {
        int first_number  = rand () % (max_first_number  - min_first_number)  + min_first_number;
        int second_number = rand () % (max_second_number - min_second_number) + min_second_number;

        printf ("expression №%d\n", i);

        printf ("%*d\n", MAX (n_first_digits, n_second_digits) + 1, first_number);
        printf ("*\n");
        printf ("%*d\n", MAX (n_first_digits, n_second_digits) + 1, second_number);

        printf ("enter your answer: ");
        if (scanf ("%d", &result) == -1)
        {
            break;
        }

        if (reversed)
        {
            result = reverse (result);
        }

        if (result == first_number * second_number)
        {
            printf ("correct\n");
        }
        else
        {
            printf ("wrong, expected - %d, recieved - %d\n", first_number * second_number, result);
        }
    }
}

int main (int argc, char *argv[])
{
    while (--argc > 0)
    {
        if (*(++argv)[0] == '-')
        {
            while (*++argv[0] != '\0')
            {
                switch (*argv[0])
                {
                    case 'r':
                        reversed = 1;
                        break;
                    default:
                        printf ("unknown option -%c\n", *argv[0]);
                        break;
                }
            }
        }
    }

    srand (time (NULL));

    int n_first_digit  = 0;
    int n_second_digit = 0;
    int n_expressions  = 0;

    printf ("enter nuber of digits for first and second number: ");
    scanf ("%d %d", &n_first_digit, &n_second_digit);

    printf ("enter number of expressions: ");
    scanf ("%d", &n_expressions);

    main_loop (n_first_digit, n_second_digit, n_expressions);

    return 0;
}
