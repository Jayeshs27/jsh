#include "headers.h"

int Separate_from_pipe(char* sstr,char** new_str, int ncnt)
{
    char* pipePos = strchr(sstr,'|');
    if( pipePos == NULL )
    {
        return ncnt + 1;
    }
    else
    {
        if( sstr != pipePos )
        {
            strncpy(new_str[ncnt ++],sstr, (strlen(sstr) - strlen(pipePos)));
            new_str[ncnt - 1][strlen(sstr) - strlen(pipePos)] = '\0';
        }
        strcpy(new_str[ncnt ++],"|");
    
        if( strlen(pipePos) != 1 )
        {
            strcpy(new_str[ncnt ++], pipePos + 1);

            ncnt = Separate_from_pipe(new_str[ncnt - 1], new_str,ncnt - 1);
        }
    }
    return ncnt;
}


bool Redirect(int new, int old)
{
    if( new == old )
    {
        return true;
    }
    if( dup2(new, old) == -1 )
    {
        perror("cannot redirect stdout");
        return false;
    }
    return true;
}

bool pipeError(char* str, int word_cnt, bool* ptr, int* counter)
{
    if( strcmp(str,"|") == 0 && word_cnt == 0 && *ptr == true )
    {
        *counter += 1;
        printf("change counter %d",*counter);
        return false;
    }
    if( strcmp(str,"|") == 0  || strcmp(str,">") == 0 || strcmp(str,"<") == 0 || strcmp(str,">>") == 0)
    {
        if( word_cnt == 0 )
        {
            return true;
        }
    }
    return false;
}

bool Pipe_And_Exe(char** single_cmd,char* commands,int ct, bool* pastevents_flag,bool* fg_flag,bool bg_flag)
{
     int Stdin = dup(0);
        int Stdout = dup(1);
        bool prevfile = false;
        int CurrIn = 0;
        int Currout = 1;
        char** CurrCmd = MakeStringArr(MAX_WORD_COUNT,MAX_WORD_SIZE);
        int j = 0;
        int word_cnt = 0;
        while( j < ct )
        {
             

             int pfd[2];
             int pf = pipe(pfd);
             if( pf == -1 )
             {
                 perror("can't open pipe");
                 return false;
             }

             if( pipeError(single_cmd[j],word_cnt, &prevfile, &j) )
             {
                 printf("invalid commmand\n");
                 return false;
             }
            
             int Stdin = dup(0);
             int Stdout = dup(1);

             if( strcmp(single_cmd[j],"|") == 0 )
             {
                Redirect(CurrIn, 0);
                Redirect(pfd[1], 1);
                Sep_cmd_Execute(CurrCmd, word_cnt, commands, false,pastevents_flag,fg_flag);
                close(pfd[1]);
                Redirect(Stdin, 0);
                Redirect(Stdout, 1);
                CurrIn = pfd[0];
                word_cnt = -1;
             }
             else if( strcmp(single_cmd[j],">") == 0  ||  strcmp(single_cmd[j],">>") == 0 )
             {
                int flags = O_CREAT | O_WRONLY | O_TRUNC  ;

                if( strcmp(single_cmd[j],">>") == 0 )
                {
                    flags = O_CREAT | O_WRONLY | O_APPEND ;
                }

                int fd = open(single_cmd[++j], flags,0644);

                if  ( fd == -1 )
                {
                    perror("Error Opening file");
                    return false;
                }
                Redirect(CurrIn, 0);
                Redirect(fd, 1);

                Sep_cmd_Execute(CurrCmd, word_cnt, commands, false, pastevents_flag,fg_flag);
                
                close(fd);
                close(CurrIn);
                Redirect(Stdin, 0);
                Redirect(Stdout, 1);

                prevfile = true;
                CurrIn = Stdin;
                word_cnt = -1;
                
             }
             else if( strcmp(single_cmd[j],"<") == 0 )
             {

                int fd = open(single_cmd[++j], O_RDONLY);
                if  ( fd == -1 )
                {
                    perror("Error Opening file1");
                    return false;
                }

                if( j < ct - 1  && strcmp(single_cmd[j + 1],">") == 0 )
                {
                    j += 2;
                    int fd2 = open(single_cmd[j], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if  ( fd2 == -1 )
                    {
                        perror("Error Opening file2");
                        return false;
                    }
                    Redirect(fd2, 1);
                }
                else if( j < ct - 1  && strcmp(single_cmd[j + 1],">>") == 0 )
                {
                    j += 2;
                    int fd2 = open(single_cmd[j], O_CREAT | O_WRONLY | O_APPEND, 0644);
                    if  ( fd2 == -1 )
                    {
                        perror("Error Opening file3");
                        return false;
                    }
                    Redirect(fd2, 1);
                }
                else if( j == ct - 1 )
                {
                    Redirect(Stdout, 1);
                }
                else
                {
                    Redirect(pfd[1], 1);
                }
                close(CurrIn);
                Redirect(fd, 0);

                Sep_cmd_Execute(CurrCmd, word_cnt, commands, false, pastevents_flag,fg_flag);
                
                close(pfd[1]);
                close(fd);
                Redirect(Stdin, 0);
                Redirect(Stdout, 1);

                prevfile = true;
                CurrIn = pfd[0];
                word_cnt = -1;
                
             }
             else
             {
                prevfile = false;
                strcpy(CurrCmd[word_cnt], single_cmd[j]);
                if( j == ct - 1 )
                {
                    word_cnt++;
                    Redirect(CurrIn,0);
                    Redirect(Stdout,1);
                    
                    Sep_cmd_Execute(CurrCmd, word_cnt, commands, bg_flag, pastevents_flag,fg_flag);
                    Redirect(Stdin, 0);
                    close(Stdout);
                    close(Stdin);
                }
             }
             word_cnt++;
             j++;
        }

}


char** pipeSeparate(char** str, int* cnt_ptr)
{

    int cnt = *cnt_ptr;

    char** new_str = MakeStringArr(MAX_WORD_COUNT, MAX_WORD_SIZE);
    if ( new_str == NULL )
    {
        perror("pipeSeparate:malloc error");
        return NULL;
    }
    int ncnt = 0;
    for( int i = 0 ; i < cnt ; i ++ )
    {
        char* pipePos = strchr(str[i],'|');
        if( pipePos != NULL )
        {
            ncnt = Separate_from_pipe(str[i],new_str,ncnt);
            continue;
        }
        strcpy(new_str[ncnt++],str[i]);
        
    }
    for( int i = 0 ; i < cnt ; i++ )  
    {
          free(str[i]);
    }
    free(str);
    *cnt_ptr = ncnt;
    
    return new_str;
}

