#include "headers.h"

#define HISTORY_LIMIT 15

int History_count;


int Remove_Past(const char *filename) {
     
    FILE* fptr = fopen(filename, "r");
    if( fptr == NULL )
    {
        perror("Remove_Past:can't open file\n");
        return -1;
    }
    char ch = fgetc(fptr);
    int flag = 0;
    char str[1024];
    int cnt = 0;
    while( ch != EOF )
    {
        if( flag == 1 )
        {
            str[cnt] = ch;
            cnt++;
        }
        if( ch == '\n' )
        {
            flag = 1;
        }

        ch = fgetc(fptr);

    }
    str[cnt++] = '\0';
    fclose(fptr);

    fptr = fopen(filename, "w");
    if( fptr == NULL )
    {
        perror("Remove_Past:can't open file\n");
        return -1;
    }
    else
    {
        fprintf(fptr,"%s",str);
        fclose(fptr);
    }
    History_count--;

    return 1;
}
int Count_lines(const char* file,char* buf)
{
    int History_count = 0;
    FILE* fptr = fopen(file, "r");
    if( fptr == NULL )
    {
        perror("Add_to_history:Error opening file");
        return -1;
    }
    else
    {
        strcpy(buf,"\0");
        while ( fgets(buf, 256, fptr) != NULL )
        {
           History_count ++;
        }
        fclose(fptr);
    }
    return History_count;
}

void replaceSubstring(char *original, const char *target, const char *replacement) {
    char *position = strstr(original, target);
    if (position != NULL) 
    {
        size_t targetLength = strlen(target);
        size_t replacementLength = strlen(replacement);
        size_t remainingLength = strlen(position + targetLength);

        memmove(position + replacementLength, position + targetLength, remainingLength + 1);
        memcpy(position, replacement, replacementLength);
    }
}

void Past_Execute(int index, char* filePath, char* commands)
{
    char buf[256];
    int n = Count_lines(filePath,buf);
    int cnt = 0;
    FILE* fptr = fopen(filePath, "r");
    if( fptr == NULL )
    {
        perror("Past_Execute:Error opening file");
        return;
    }
    else if( index > n )
    {
        printf("pastevents execute:History not Found\n");
        return;
    }
    else
    {
        strcpy(buf,"\0");
        while ( fgets(buf, 256, fptr) != NULL )
        {
           cnt++;
           if( cnt == n - index + 1 )
           {
            //   Separate(buf);
              break;
           }
       }
       fclose(fptr);
       Separate(buf);
       buf[strlen(buf) - 1] = '\0';
       char ind[6];
       sprintf(ind, "%d",index);  
       char* replace = strdup("pastevents execute ");
       strcat(replace,ind);
       replaceSubstring(commands,replace,buf);
       Add_to_history(commands);
    }
}


void Add_to_history(char* cmds)
{
    char filePath[1024];
    strcpy(filePath,homePath);
    strcat(filePath,"/pastevents.txt");
    char buf[256];
    History_count = Count_lines(filePath,buf);
    // printf("last string ;%s\n",buf);
    if( History_count == -1 )
    {
        return;
    }
    
   
    // printf("hist end %d,buf %s and cmds %s\n",History_count,buf,cmds);
    FILE* fptr = fopen(filePath, "a");

    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }
    else
    {
        if( strcmp(cmds,buf) != 0 )
        {
            while( History_count >= HISTORY_LIMIT )
            {
                if( Remove_Past(filePath)  < 0 )
                {
                    return;
                }
            }
            fprintf(fptr, "%s\n", cmds);
            History_count++;
        }
        fclose(fptr);
    }
    
 
}

void PastEvents(char** cmd_seq, int cmd_cnt, char* commands)
{
    char filePath[1024];
    strcpy(filePath, homePath);
    strcat(filePath,"/pastevents.txt");
    if( cmd_cnt == 1 )
    {
         FILE* fptr = fopen(filePath,"r");
         if( fptr == NULL )
         {
             perror("pastevents:can't open file");
             return;
         }
         else
         {
            char ch = fgetc(fptr);
            while( ch != EOF )
            {
               printf("%c",ch);
               ch = fgetc(fptr);
            }
            fclose(fptr);
         }
    }
    else if( cmd_cnt == 2 )
    {
        if( strcmp(cmd_seq[1],"purge") == 0 )
        {
            FILE* fptr = fopen(filePath,"w");
            if( fptr == NULL )
            {
                perror("pastevents purge:Can't open file");
                return;
            }
            else
            { 
                fclose(fptr);
            }
        }
        else if( strcmp(cmd_seq[1],"execute") == 0 )
        {
            printf("pastevents execute:missing integer argument\n");
        }
        else
        {
            printf("pastevents:invalid argument");
        }
    }
    else if( cmd_cnt == 3 )
    {
        if( strcmp(cmd_seq[1],"purge") == 0 )
        {
            printf("pastevents:too many arguments\n");
        }
        else if( strcmp(cmd_seq[1],"execute") == 0 )
        {
            int c = atoi(cmd_seq[2]);
            if( c == 0 && strcmp(cmd_seq[2],"0") != 0 )
            {
                printf("Pastevents execute:invalid numeric arguments");
            }
            else
            {
                 Past_Execute(c,filePath,commands);
            }
        }
        else
        {
            printf("pastevents execute:missing integer argument\n");
        }
    }
    else
    {
        printf("pastevents:too many arguemtns\n");
    }

}
