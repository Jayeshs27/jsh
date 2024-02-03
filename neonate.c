#include "headers.h"



void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO );
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


// int main() {
//     char *inp = malloc(sizeof(char) * 100);
//     char c;
//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();
//         printf("Prompt>");
//         memset(inp, '\0', 100);
//         int pt = 0;
//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 27) {
//                     char buf[3];
//                     buf[2] = 0;
//                     if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
//                         printf("\rarrow key: %s", buf);
//                     }
//                 } else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         if (inp[pt-1] == 9) {
//                             for (int i = 0; i < 7; i++) {
//                                 printf("\b");
//                             }
//                         }
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 9) { // TAB character
//                     inp[pt++] = c;
//                     for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
//                         printf(" ");
//                     }
//                 } else if (c == 4) {
//                     exit(0);
//                 } 
//                 // else {
//                 //     printf("%d\n", c);
//                 // }
//              } 
//             //  else {
//             //     inp[pt++] = c;
//             //     printf("%c", c);
//             // }
//             if ( c == 'x' )
//             {
//                 exit(0);
//             }
            
//         }
//         disableRawMode();

//         printf("\nInput Read: [%s]\n", inp);
//     }
//     return 0;
// }

void PrintPids(int time_arg) {

    // while (1) {
    
    int first = 1;
    int curr_time;
    DIR *dir;
    struct dirent *ent;
    
    while(1)
    {
        long newest_start_time = -1;
        int newest_pid = -1;
        if ((dir = opendir("/proc")) != NULL) {

            while ((ent = readdir(dir)) != NULL)
            {
                char path[128];
                snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                // printf("path :%s\n",path);
                FILE *file = fopen(path, "r");
                if (file) {
                    int pid;
                    char comm[128];
                    char state;
                    long start_time;
                   
                    if (fscanf(file, "%d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %ld",  // first argument reads pid last read start time (utime) 
                            &pid, &start_time) == 2) {
                        if (first == 1 && start_time > newest_start_time) {
                            newest_start_time = start_time;
                            newest_pid = pid;
                        }
                        else if( start_time > newest_start_time && start_time < curr_time)
                        {
                            newest_start_time = start_time;
                            newest_pid = pid;
                        }
                    }
                    // printf("%ld\n",start_time);
                    fclose(file);
                }
            }
            if (newest_pid != -1) {
                printf("%d\n", newest_pid,newest_start_time);
                curr_time = newest_start_time;
                first = 0;
            }
            else
            {
                printf("Done!\n");
                // exit(0);
                return;
            }
            char c;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(STDIN_FILENO, &rfds);
            // first argument( STDIN_FILENO + 1 ) --> limit to which it has to check if files descriptors are ready 
            //  rfds -->  set of file descriptors to check 
            // tv --> time interval to which it has to wait ins sec and usec(microsecond)
            if( select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv) > 0)
            {
                if( read(STDIN_FILENO, &c, 1) == 1 )
                {
                    // printf("waiting for input\n");
                    if ( c == 'x')
                    {
                        return;
                    }
                }
            }
            

            closedir(dir);
        }
        sleep(time_arg);
    }

}
int key = 0;
void key_handler(int sig)
{
    key = 1;
}
void Printpds(int time_arg) {

    // while (1) {
    
    
    
    // while(1)
    // {
    //     // long newest_start_time = -1;
        // int newest_pid = -1;
        // if ((dir = opendir("/proc")) != NULL) {

        //     while ((ent = readdir(dir)) != NULL)
            key_handler(1);
            signal(SIGALRM, key_handler);


            alarm(time_arg);
            while(1)
            {
                    
                    if(key)
                    {
                        FILE *file = fopen("/proc/loadavg", "r");
                        if( file == NULL)
                        {
                            perror("can't open /proc/loadavg");
                            return;
                        }
                        int pid;
                        fscanf(file, "%*f %*f %*f %*s %d",&pid);
                        printf("%d\n",pid);
                        fclose(file);
                        alarm(time_arg);
                        key = 0;
                    }


                    char c;
                    if( read(STDIN_FILENO, &c, 1) == 1 )
                    {
                    
                        if ( c == 'x')
                        {
                            return;
                        }
                    }

                    

            }
                // }
            // }
            // if (newest_pid != -1) {
            //     printf("%d\n", newest_pid,newest_start_time);
            //     curr_time = newest_start_time;
            //     first = 0;
            // }
            // else
            // {
            //     printf("Done!\n");
            //     // exit(0);
            //     return;
            // }
            // char c;
            // struct timeval tv;
            // tv.tv_sec = 0;
            // tv.tv_usec = 0;
            // fd_set rfds;
            // FD_ZERO(&rfds);
            // FD_SET(STDIN_FILENO, &rfds);
            // // first argument( STDIN_FILENO + 1 ) --> limit to which it has to check if files descriptors are ready 
            // //  rfds -->  set of file descriptors to check 
            // // tv --> time interval to which it has to wait ins sec and usec(microsecond)
            // if( select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv) > 0)
            // {
            //     if( read(STDIN_FILENO, &c, 1) == 1 )
            //     {
            //         // printf("waiting for input\n");
            //         if ( c == 'x')
            //         {
            //             return;
            //         }
            //     }
            // }
            

    //         closedir(dir);
    //     }
    //     sleep(time_arg);
    // }

}

void neonate(char** cmd_seq, int cmd_cnt)
{
    if( cmd_cnt < 3 )
    {
        printf("neonate:missing arguments\n");
    }
    else if( cmd_cnt == 3 )
    {
        if ( strcmp(cmd_seq[1],"-n") == 0 )
        {
           int interval = atoi(cmd_seq[2]);
           if( !is_integer_string(cmd_seq[2]))
           {
              printf("neonate:invalid arguments\n");
              return;
           }
           int flags = fcntl(STDIN_FILENO, F_GETFL, 0); // read stdin flags
           fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);   // stdin --> apply flag O_NONBLOCK
           
           enableRawMode();

           Printpds(interval);

           disableRawMode();

           fcntl(STDIN_FILENO, F_SETFL, flags);    // stdin --> remove flag O_NONBLOCK
        }
        else
        {
            printf("neonate:invalid arguments\n");
            
        }
    }
    else
    {
        printf("neonate:too many arguments\n");
    }
}