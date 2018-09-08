#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void exit_error(char* error_message)
{
    //Prints an error message and exits
    printf("%s\n",error_message);
    exit(0);
}

void check_for_errors(int check_value, char *error_message)
{
    //Pass arguments to exit_error if check value is null
    if (!check_value)
        exit_error(error_message);
}

void check_arguments_for_nm(char **EVERY, char *argv[], int *start_of_file_arguments)
{
    //Exit if no arguments are provided
    if (!argv[1])
        return;
    //Check if first argument provides values for n & m
    if ( strncmp(argv[1], "-", 1) == 0 )
    {
        *start_of_file_arguments = 2; 
        //Set EVERY to values specified by the first argument
        *EVERY = argv[1];
        return;
    }
    //Specify where the file arguments start
    *start_of_file_arguments = 1;
}

char* check_environmental_variable(char *EVERY)
{
    //Check if environmental variable EVERY exists
    if ( getenv("EVERY") )
        return getenv("EVERY");
    else
        //Else set to DEFAULT values
        return "-1,1";
}

int find_comma_position(char *EVERY)
{
    //Find the index of the comma in EVERY
    for (int i = 0; *EVERY != '\0'; EVERY++, i++)
    {
        if ( *EVERY == ',' )
            return i;
    }
    //If there is no comma, return 0
    return 0;
}

char* copy_n_chars(char *EVERY, int n)
{
    //Copy n characters of EVERY into a new string
    char *copy = malloc ( sizeof (char) * sizeof(n) );
    strcpy(copy, EVERY);
    copy[n] = '\0';
    return copy;
}

void convert_to_integers(char *n_str, char *m_str, int *n, int *m)
{
    //Convert N and M to integers
    *n = atoi(n_str);
    check_for_errors(*n, "N value cannot be converted to an integer");
    *m = atoi(m_str);
    check_for_errors(*m, "M value cannot be converted to an integer");
    //Free the str values that were holding n and m
    free(n_str); free(m_str);
}

int* convert_string_to_numbers(char *EVERY, int index, int *n_int, int *m_int)
{
    //If only N is defined
    if (index == -1)
    {
        char *n_str = copy_n_chars(EVERY, sizeof(EVERY));
        *n_int = atoi(n_str);
        check_for_errors(*n_int, "N value cannot be converted to an integer");
        *m_int = 1;
        free(n_str);
    }
    //If N & M are defined start here
    char *n_str = copy_n_chars(EVERY, index);
    //Increment EVERY so it now points to where M will start
    EVERY = EVERY+index + 1;
    char *m_str = copy_n_chars(EVERY, index++);
    //Convert N and M to integers
    convert_to_integers(n_str, m_str, n_int, m_int);
}

void check_number_values(int n, int m)
{
    //Make sure that n and m have acceptable values
    if ( n < m )
        exit_error("N must be greater than or equal to M");
}

void do_M_work(FILE *fp, char *buf, int m, int *line)
{
    //Print out sequential lines
    for (int reps = 1; (fgets(buf, sizeof(buf), fp)) != NULL && reps < m; reps++)
    {
        printf("%s", buf);
        (*line)++;
    }
    (*line)++;
}

void file_work( char *file, int n, int m )
{
    FILE *fp;
    char buf[1024];
    size_t len = 0;
    fp = fopen(file, "r");
    //Check for errors opening files
    if ( !fp )
    {
        printf("Invalid filename: %s\n", file);
        return;
    }
    //Iterate through lines in the file
    int line = 0;
    for (; (fgets(buf, sizeof(buf), fp)) != NULL; line++ )
    {
       //Catch N
       if ( line % n == 0 )
       {
           printf("%s", buf);
           //Do repetitions for M
           do_M_work(fp, buf, m, &line);
       }
    }
    fclose(fp);
}

void iterate_files(char *argv[], int argc, int n, int m, int start_of_file_arguments)
{
    char file[256];
    //If no file arguments, accept input from stdin
    if (argc == 1 || (argc == 2 && start_of_file_arguments ==2) )
    {
        printf("What file would you like to use?  ");
        fgets(file, sizeof(file), stdin);
        file[strlen(file)-1]='\0';
        file_work(file, n, m);
        return;
    }
    //Iterate through file arguments
    for (int i = start_of_file_arguments; i < argc; ++i)
        file_work(argv[i], n, m);
}

int main(int argc, char *argv[], char *envp[])
{
    //Find values for n and m
    int start_of_file_arguments;
    char *EVERY = check_environmental_variable(EVERY);
    check_arguments_for_nm(&EVERY, argv, &start_of_file_arguments);

    //Convert string value for n & m to integers
    int comma = find_comma_position(EVERY) - 1;
    int n,m;
    convert_string_to_numbers(++EVERY, comma, &n, &m);

    //Check n and m have acceptable values
    check_number_values(n,m);

    //Go through file arguments and print out lines according to N & M values
    iterate_files(argv, argc, n, m, start_of_file_arguments);
    return 0;
 }
