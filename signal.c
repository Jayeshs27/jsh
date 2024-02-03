#include "headers.h"

void Signal(char** cmd_seq, int cmd_cnt )
{
    int pid, sig;
    if ( cmd_cnt < 3 )
    {
        printf("ping:missing arguments\n");
    }
    else if ( cmd_cnt == 3 )
    {
        if( is_integer_string(cmd_seq[1]) && is_integer_string(cmd_seq[2]))
        {
            pid = atoi(cmd_seq[1]);
            sig = atoi(cmd_seq[2]);
            if( kill(pid, 0) == -1 )
            {
                printf("No such Process Running\n");
                return;
            }
            int fk = kill(pid, sig % 32);
            if ( fk < 0 )
            {
                perror("signal:kill() failed\n");
            }
            else
            {
                printf("Sent signal %d to process with pid %d\n", sig, pid);
            }
        }
        else
        {
            printf("ping:invalid argument\n");
        }
    }
    else
    {
        printf("ping:too many arguments\n");
    }
}

// void KeySignal()
// {
//     char c;
//     struct timeval tv;
//     tv.tv_sec = 0;
//     tv.tv_usec = 0;
//     fd_set rfds;
//     FD_ZERO(&rfds);
//     FD_SET(STDIN_FILENO, &rfds);
//     int flags = fcntl(STDIN_FILENO, F_GETFL, 0); // read stdin flags
//     fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);  

//     ///////////////////
//     // first argument( STDIN_FILENO + 1 ) --> limit to which it has to check if files descriptors are ready 
//     //  rfds -->  set of file descriptors to check 
//     // tv --> time interval to which it has to wait ins sec and usec(microsecond)
//     ///////////////////
     
//     if( select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv) > 0)
//     {
//         if( read(STDIN_FILENO, &c, 1) == 1 )
//         {
//             // printf("waiting for input\n");
//             if ( c == 'x')
//             {
//                 printf("press x\n");
//             }
//         }
//     }

//     fcntl(STDIN_FILENO, F_SETFL, flags);
            
// }