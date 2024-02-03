#include "headers.h"

void bgTofg(char** cmd_seq, int cmd_cnt)
{
    if ( cmd_cnt < 2 )
    {
        printf("bgTofg:missing arguments\n");
    }
    else if ( cmd_cnt == 2 )
    {
        if( is_integer_string(cmd_seq[1]) )
        {
            int pid = atoi(cmd_seq[1]);
            if( kill(pid, 0) == - 1 )
            {
                printf("No such Process found\n");
                return;
            }
            if ( strcmp(cmd_seq[0], "fg") == 0 )
            {
                
                struct termios saved_attributes;
                int kfg = kill(pid,SIGCONT);
                RemoveProc(&bg_list, pid);
                
                if( kfg == -1 )
                {
                   perror("failed to run the process");
                }
                tcgetattr(STDIN_FILENO, &saved_attributes);

    
                int ck = tcsetpgrp(STDIN_FILENO,getpgid(pid));
                if ( ck == -1) {
                    perror("bgTofg:tcsetpgrp() failed");
                }
                
                int status;
                int wfg = waitpid(pid, &status, 0);
            

                if( ck != -1 )
                {
                  if(tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
                  {
                     perror("bgTofg:tcsetpgrp() failed");
                  }
                }
            }
            if ( strcmp(cmd_seq[0], "bg") == 0 )
            {
                if ( kill(pid, SIGCONT) == -1)
                {
                    perror("fail to Run");
                    return;
                }  
                ChangeState(&bg_list, pid, 0, "RUNNING");
            }
        }
        else
        {
            printf("bgTofg:invalid argument");
        }
    }
    else
    {
        printf("bgTofg:too many arguments\n");
    }
}