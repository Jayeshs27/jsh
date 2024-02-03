#include "headers.h"


int Execute_cnt = 0;

char* RemoveExten(char* str)
{
    for( int i = 1 ; i < strlen(str) ; i++ )
    {
        if(str[i] == '.')
        {
            str[i] = '\0';
        }
    }
    return str;
}
void Seek_recursive(char* Path,char** Exec,char* target, int flag1, int flag2)
{
    DIR *d;
    struct dirent *dir;
    struct dirent *entry;
    struct stat fileStat;
    char temp[256];
    d = opendir(Path);

    if (d) 
    {

        while ((dir = readdir(d)) != NULL)
        {
            if( dir->d_name[0] == '.' )
            {
                continue;
            }
            strcpy(temp, Path);
            strcat(Path,"/");
            strcat(Path,dir->d_name);

            if( ( flag1 == 1 || flag1 == -1 ) && strcmp(target,RemoveExten(dir->d_name)) == 0 && isDirect(Path))
            {
                printf("%s\n", Path);
                strcpy(Exec[1],Path);
                Execute_cnt++;
            }
            if( ( flag1 == 0 || flag1 == -1 ) && strcmp(target,RemoveExten(dir->d_name)) == 0 && !isDirect(Path))
            {
                printf("%s\n", Path);
                strcpy(Exec[1],Path);
                Execute_cnt++;
            }
            Seek_recursive(Path, Exec, target, flag1, flag2);
            strcpy(Path, temp);
        }
        closedir(d);
    }
}
void cat(char* filePath)
{
    int fdold,count;
    char buffer[2048]; 
    fdold=open(filePath, O_RDONLY);
    if(fdold == -1)
    {
    //   perror("cannot open file");
      if( errno == EACCES )
      {
         printf("Missing permissions for task!\n");
      }
      else
      {
         perror("cannot open file");
      }
      
      return;  
    }
    while( (count=read(fdold,buffer,sizeof(buffer))) > 0 ) 
    {
       printf("%s",buffer);
    }

}
void Execute_seek(char* Path,char* target, int flag1, int flag2)
{
    Execute_cnt = 0;
    char** Exec = MakeStringArr(2,256);
    if( Exec == NULL )
    {
        perror("Execute_seek:can't allocate memory");
        return;
    }
    Seek_recursive(Path, Exec, target, flag1, flag2);
    if( Execute_cnt == 1 && flag2 == 1 )
    {
       if( flag1 == 0 )
       {
           cat(Exec[1]);
       }
       else
       {
           warp(Exec,2);
       }
    }
}

bool Doub_strcmp(char* a1, char* a2, char* b1, char* b2)
{
    if( strcmp(a1,b1) == 0 && strcmp(a2,b2) == 0 )
    {
        return true;
    }
    return false;
}
bool Not_D_flags(char* str)
{
    if( strcmp(str,"-d") == 0 || strcmp(str,"-f") == 0 ||  strcmp(str,"-e") == 0 || strcmp(str,"seek") == 0 )
    {
        return false;
    }
    return true;
}
void seek(char** cmd_seq, int cmd_cnt)
{
    // printf("Enter seek\n");
    char Path[256] = ".";
    char target[256];
    if( cmd_cnt == 1 )
    {
       printf("seek:Target Missing\n");
    }
    else
    {
        if( isDirect(cmd_seq[cmd_cnt - 1]) && Not_D_flags(cmd_seq[cmd_cnt - 2]))
        {
           strcpy(Path, cmd_seq[cmd_cnt - 1]);
           if( cmd_cnt == 3 )
           {
               strcpy(target,cmd_seq[1]);  
               Execute_seek(Path,target,-1,-1);
               // excute // NO flag 
           }
           else if( cmd_cnt == 4 )
           {
               strcpy(target, cmd_seq[2]);

               if( strcmp(cmd_seq[1],"-f") == 0 )
               {
                  Execute_seek(Path,target,0,-1);
               }
               else if( strcmp(cmd_seq[1],"-d") == 0 )
               {
                  Execute_seek(Path,target,1,-1);
               }
               else 
               {
                   printf("Invalid flags!\n");
               }
           }
           else if( cmd_cnt == 5 )
           {
               strcpy(target, cmd_seq[3]);
               if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-d") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-d") )
               {
                   printf("Invalid flags!\n");
               }
               else if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-e") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-e","-f") )
               {
                   Execute_seek(Path,target,0,1);
               }
               else if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-e","-d") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-d","-e") )
               {
                   Execute_seek(Path,target,1,1);
               }
               else
               {
                   printf("Invalid flags!\n");
               }
           }
        }
        else 
        {
            if( cmd_cnt == 2 )
            {
                strcpy(target, cmd_seq[1]);
                Execute_seek(Path,target,-1,-1);
            }
            else if( cmd_cnt == 3 )
            {
               strcpy(target, cmd_seq[2]);
               if( strcmp(cmd_seq[1],"-f") == 0 )
               {
                  Execute_seek(Path,target,0,-1);
               }
               else if( strcmp(cmd_seq[1],"-d") == 0 )
               {
                  Execute_seek(Path,target,1,-1);
               }
               else 
               {
                   printf("Invalid flags!\n");
               }
           }
           else if( cmd_cnt == 4 )
           {
               strcpy(target, cmd_seq[3]);
               if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-d") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-d") )
               {
                   printf("Invalid flags!\n");
               }
               else if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-f","-e") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-e","-f") )
               {
                   Execute_seek(Path,target,0,1);
               }
               else if( Doub_strcmp(cmd_seq[1],cmd_seq[2],"-e","-d") || Doub_strcmp(cmd_seq[1],cmd_seq[2],"-d","-e") )
               {
                   Execute_seek(Path,target,1,1);
               }
               else
               {
                   printf("Invalid flags!\n");
               }
           }

        }
        if ( Execute_cnt == 0 )
        {
            printf("No Match Found!\n");
        }
    }
}