#include "headers.h"

void Readinput(char* inp){
    char c;
    setbuf(stdout, NULL);
    enableRawMode();
    memset(inp, '\0', 100);
    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (iscntrl(c)) {
            if (c == 10) break;
            else if (c == 127)
            {                                 // backspace
                if (pt > 0) {
                    if (inp[pt-1] == 9) {
                        for (int i = 0; i < 7; i++) {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            } 
            else if (c == 9)
            {                             // TAB character
                inp[pt++] = c;
                for (int i = 0; i < 8; i++)
                {                        // TABS should be 8 spaces
                    printf(" ");
                }
            } 
            else if (c == 4)
            {
                printf("\n");
                exit(0);
            } 
        } 
        else {
            inp[pt++] = c;
            printf("%c", c);
        }
        
    }
    printf("\n");
    disableRawMode();
}
void prompt() {

    char system[MAX_PATH_LENGTH];
    char user[MAX_PATH_LENGTH];
    char Commands[MAX_INPUT_SIZE]; 
    gethostname(system,MAX_PATH_LENGTH);
    getlogin_r(user,MAX_PATH_LENGTH);
    getcwd(homePath,MAX_PATH_LENGTH);
    bg_list = NULL;
    bool fg_flag = false;
    int time = 0;\
    struct timeval t1, t2;
    int elapsedTime = 0;
    while(1)
    {
        printf("\033[0;32m");
        printf("\x1B[1m");
        if( fg_flag == true && elapsedTime > 2 )
        {
             printf("<%s@%s:",user,system);
             printf("\033[0;34m");
             printf("\x1B[1m");
             printf("%s %s %ds",CurrPath,firstfg,elapsedTime);
             printf("\033[0;32m");
             printf("> ");
        } 
        else
        {
             printf("<%s@%s:",user,system);
             printf("\033[0;34m");
             printf("\x1B[1m");
             printf("%s",CurrPath);
             printf("\033[0;32m");
             printf("> ");
        }
        printf("\033[0m");
        printf("\x1B[0m");

        Readinput(Commands);
        fg_flag = false;
        Check_bg_Processes(&bg_list);
        
        gettimeofday(&t1, NULL);
        fg_flag = Separate(Commands);
       
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec);  
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;
         
                    
    }
}

