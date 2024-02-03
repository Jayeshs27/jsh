#include "headers.h"


char CurrPath[MAX_PATH_LENGTH]="~";
char PrevPath[MAX_PATH_LENGTH]="~";
char homePath[MAX_PATH_LENGTH];
char firstfg[128];
int currfg = -1;

void handle_ctrlC(int s)
{
    if( currfg != -1 )
    {
       int ck = kill(currfg,SIGINT);
       if( ck == -1 )
       {
           perror("kill failed()");
       }
    }
}


void handle_ctrlZ(int s)
{
    signal(SIGTSTP, handle_ctrlZ);
}

int main()
{
    signal(SIGINT, handle_ctrlC);
    signal(SIGTSTP, handle_ctrlZ);
    prompt();
}




void Sep_cmd_Execute(char** single_cmd, int ct, char* commands, int Is_bg_flag,  bool* pastevents_flag, bool* fg_flag)
{ 
    if( strcmp(single_cmd[0], "warp") == 0 )
    {
        warp(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "pastevents") == 0 )
    {
        PastEvents(single_cmd, ct, commands);
        *pastevents_flag = true;
    }
    else if( strcmp(single_cmd[0], "peek") == 0 )
    {
        peek(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "exit") == 0 )
    {
        exit(0);
    }
    else if( strcmp(single_cmd[0], "seek") == 0 )
    {
        seek(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "proclore") == 0 )
    {
        proclore(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "ping") == 0 )
    {
        Signal(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "neonate") == 0 )
    {
        neonate(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "activities") == 0 )
    {
        activities(&bg_list);
    }
    else if( strcmp(single_cmd[0], "iMan") == 0 )
    {
        iman(single_cmd, ct);
    }
    else if( strcmp(single_cmd[0], "bg") == 0 || strcmp(single_cmd[0], "fg") == 0 )
    {
        bgTofg(single_cmd, ct);
    }
    else
    {
            if( Is_bg_flag == 0 && *fg_flag == false )
            {
                *fg_flag = true;
                strcpy(firstfg, single_cmd[0]);
            }
            char* tmp = single_cmd[ct];
            Excute_sys( single_cmd, ct, Is_bg_flag);
            single_cmd[ct] = tmp;
    }
}

bool Separate(char* commands)
{
    bool fg_flag = false;
    bool is_last_bg = false;
    int i = strlen(commands);
    while( i > 0 )
    {
        i--;
        if( commands[i] == ' ' ) continue;
        else if( commands[i] == '\t' ) continue;
        else if( commands[i] == '\0' ) continue;
        else if( commands[i] == '\n' ) continue;
        else if( commands[i] == '&' ) 
        {
            is_last_bg = true;
        }
        else
        {
            break;
        }
    }
    bool pastevents_flag = false;
    char sep1[4] = "&";
    char sep2[4] = ";";
    char and_sep_list[MAX_CMD_COUNT][MAX_CMD_LENGTH];
    char* items = strtok(commands, sep1);
    int cnt = 0;
    while( items != NULL )
    {
        if( and_sep_list[cnt] == NULL )
        {
            printf("Separate:Execeptional Error\n");
        }
        strcpy(and_sep_list[cnt], items);
        items = strtok(NULL, sep1);
        cnt++;
    }
    char sep_cmds[MAX_CMD_COUNT][MAX_CMD_LENGTH];
    int Is_bg[32] = {0};
    int cmd_cnt = 0;
    for( int i = 0 ; i < cnt ; i++ )
    {
        char* cmd = strtok(and_sep_list[i], sep2);
        while( cmd != NULL )
        {
            if( sep_cmds[cmd_cnt] == NULL )
            {
                printf("Separate:Execeptional Error\n");
            }
            strcpy(sep_cmds[cmd_cnt], cmd);
            cmd = strtok(NULL, sep2);
            if( cmd == NULL && i != cnt - 1 )
            {
                Is_bg[cmd_cnt] = 1;

            }
            else if( cmd == NULL && i == cnt - 1 )
            {
                if( is_last_bg ) Is_bg[cmd_cnt] = 1;
            }
            cmd_cnt++;
        }
    }
    
    char** single_cmd = MakeStringArr(MAX_WORD_COUNT, MAX_WORD_SIZE);
    if( single_cmd == NULL )
    {
        printf("Separate:malloc Error\n");
        return false;
    }
    int ct = 0;
    char space[2] = " \t";
    for( int i = 0 ; i < cmd_cnt ; i++ )
    {
        ct = 0;
        char* tok = strtok(sep_cmds[i],space);
        while( tok != NULL )
        {
            if( single_cmd[ct] == NULL )
            {
                printf("Separate:Execeptional Error\n");
                return false;
            }
            strcpy( single_cmd[ct] , tok); 
            tok = strtok(NULL, space);
            ct++;
        }

        single_cmd = pipeSeparate(single_cmd,&ct);
        if( ct == 0 || single_cmd == NULL )
        {
            continue;
        }
        
        bool bg_flag = false;
        if( Is_bg[i] == true )
        {
            bg_flag = true;
        }
        
        Pipe_And_Exe(single_cmd, commands, ct, &pastevents_flag, &fg_flag, bg_flag);
    }

    if( !pastevents_flag )
    {
        Add_to_history(commands);
    }

    return fg_flag;
}