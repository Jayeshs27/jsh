#include "headers.h"



bool is_integer_string(const char *s) {
    if (!s || *s == '\0')
    {
        return false;
    }
    while (*s != '\0') 
    {
        if (!isdigit(*s)) 
        {
            return false;
        }
        s++;
    }
    
    return true;
}
void proclore(char** cmd_seq, int cmd_cnt )

{
    int pid;
    if( cmd_cnt == 1 )
    {
        pid = getpid();
    }
    else if( cmd_cnt == 2 )
    {
        if( is_integer_string(cmd_seq[1]) )
        {
            pid = atoi(cmd_seq[1]);
        }
        else
        {
            printf("proclore:Invalid arguments\n");
            return;
        }
    }
    else
    {
        printf("proclore:too many arguments\n");
        return;
    }
    printf("PID: %d\n", pid);

    char status_path[64];

    snprintf(status_path,sizeof(status_path),"/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if(status_file == NULL) 
    {
        printf("proclore:No Such process\n");
        return;  
    }

    char proc_path[32];
    char line[256];
    pid_t fg_pid = tcgetpgrp(STDOUT_FILENO);
    char sign;
    if( fg_pid == pid )
    {
        sign = '+';
    }
    else
    {
        sign = ' ';
    }
    while(fgets(line,sizeof(line),status_file)) 
    {
        if( strncmp(line, "State:", 6) == 0 ) 
        {
            strcpy(proc_path,line + 7);
            printf("Process Status: %c%c\n",proc_path[0],sign);
        } 
        else if( strncmp(line, "VmSize:", 7) == 0 )
        {
            printf("Virtual Memory: %s", line + 10);
        }
    }
    
    char exe_path[32];

    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);

    int len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);
    exe_path[len]='\0';

    printf("Path: %s\n",exe_path);
  
    
    fclose(status_file);

}
