/*
System Programming 4 - Processes
./a.out system/fork ls -a
*/
#include <stdio.h>      /*fork*/
#include <stdlib.h>     /*system*/
#include <unistd.h>     /*fork*/
#include <sys/types.h>  /*getpid*/
#include <string.h>     /*strcmp*/

int WatchDog(char *program, char **arg_list)
{
    char **sys_call = arg_list + 1;
    char **program_start = arg_list + 2;
    char **option = arg_list + 3;
    pid_t child_pid;
    
    if (0 == strcmp(*sys_call, "system"))
    {
        int return_value = 0;
        char buffer[50] = {0};
        size_t option_len = 0;
        
        if (NULL != *option)
        {
            option_len = strlen(*option) + 1;
        }
        
        strncpy(buffer, program, strlen(program));
        strncpy(buffer + strlen(program), " ", 1);
        strncpy(buffer + strlen(program) + 1, *option, option_len);
        
        return_value = system(buffer);
        
        return return_value;
    }

    else if (0 == strcmp(*sys_call, "fork"))
    {
        child_pid = fork();
        if (0 != child_pid)
        {
            return child_pid;
        }
        else
        {
            execvp(program, program_start);
            abort();
        }
    }
    
    else
    {
        printf("Invalid process create argument\n");
    }
    
    return 0;
}

int main(int argc, char **argv)
{
    char **program = argv + 2;
    
    WatchDog(*program, argv);

    return 0;
}
