#include "headers.h"

char temp[256];

void PrintDir()
{
    char Dir[256];
    char* check = getcwd(Dir,236);
    if( check != NULL)
    {
        printf("%s\n",Dir);
    }
    else
    {
        printf("Error While printing Directory\n");
    }
   
}
bool isAbsolute(char* Path)
{
    char* ptr = malloc(sizeof(char)*(strlen(Path) + 1));
    strcpy(ptr,Path);
    if (ptr[0] != '/')
    {
        return false;
    }
    char* tok = strtok(ptr,"/");
    if( strcmp(tok,"home") == 0 )
    {
        free(ptr);
        return true;
    }
    free(ptr);
    return false;
}

bool homeDir(char* homePath, char* CurrPath)
{
    StorePrev();
    int Dircheck = chdir(homePath);
    if( Dircheck == 0 )
    {
        strcpy(CurrPath,"~");
        return true;
    }
    else
    {
        RestorePrev();
        printf("Error Occured while excuting '~'\n");
        return false;
    }
}
bool ChangeDir(char* path)
{
    
    int Dircheck = chdir(path);
    if( Dircheck == 0 )
    {
        return true;
    }
    else {
        perror("chdir() failed");
        return false;
    }
}

void StorePrev()
{
    getcwd(temp,256);
}

void RestorePrev()
{
    strcpy(PrevPath,temp);
}

void StringMinus(char* a, char* b)
{
   int c = 0;
   while( a[c] == b[c] && a[c] != '\0' && b[c] != '\0' )
   {
      c++;
   }
   int i=0;
   while(a[i + c] != '\0')
   {
      a[i] = a[i + c];
      i++;
   }
   while(a[i] != '\0')
   {
      a[i] = '\0';
      i++;
   }
   return;
}
void warp(char** cmd_seq, int cmd_cnt)
{
    if(cmd_cnt == 1)
    {
        homeDir(homePath, CurrPath);
    }
    else
    {
        for(int i=1 ; i < cmd_cnt ; i++ )
        {
            warp_Execute(cmd_seq[i]);
        }
    }
}
  
void warp_Execute(char* cmd)
{
    if( strcmp(cmd,"~") == 0 )
    {
        if( homeDir(homePath, CurrPath))
        {
            PrintDir();
        }
    }
    else if( strcmp(cmd,"-") == 0 )
    {
        if( strcmp(PrevPath,"~") == 0 )
        { 
            printf("OLDPWD not set\n");
            return;
        }
        warp_Execute(PrevPath);
    }
    else if( strcmp(cmd,"/") == 0 )
    {
         StorePrev();
         if( ChangeDir(cmd) )
         {
            PrintDir();
            strcpy(CurrPath,"/");
            RestorePrev();
         }
    }
    else if( strcmp(cmd,"..") == 0 )
    {
        StorePrev();
        if( ChangeDir(cmd) )
        {
            PrintDir();
            if( strcmp(CurrPath, "~") == 0)
            {
                char newPath[256];
                strcpy(newPath, homePath);
                int c = strlen(newPath);

                while( newPath[c] != '/' )
                {
                    c--;
                }
                newPath[c]= '\0';
                strcpy(CurrPath,newPath);
            }
            else
            {
                int c = strlen(CurrPath);
                while( CurrPath[c] != '/' )
                {
                    c--;
                }
                CurrPath[c] = '\0';
                if( c == 0 )
                {
                    CurrPath[0] = '/';
                    CurrPath[1] = '\0';
                }
               
            }
            RestorePrev();
                
        }

    }
    else
    {
        StorePrev();
        if( ChangeDir(cmd) )
        {
            PrintDir();
            if( isAbsolute(cmd) )
            {
                char* tok;
                if( strcmp(homePath,cmd) == 0 )
                {
                    homeDir(homePath,CurrPath);
                }
                else if( strlen(homePath)  > strlen(cmd) )
                {  
                    strcpy(CurrPath, cmd);
                    
                }
                else
                {
                    StringMinus(cmd,homePath);
                    strcpy(CurrPath, "~");
                    strcat(CurrPath, cmd);
                }
            }
            else
            {
                if( cmd[0] == '.' )
                {
                    int i = 1;
                    while(cmd[i] != '\0')
                    {
                        cmd[i - 1] = cmd[i];
                        i++;
                    }
                    cmd[i-1] = '\0';
                    i++;
                    cmd[i-1] = '\0';
                    strcat(CurrPath,cmd);
                }
                else
                {
                    if( strcmp(CurrPath,"/") != 0 )
                    {
                        strcat(CurrPath,"/");
                    }
                    strcat(CurrPath,cmd);
                }
            }
            RestorePrev();
        }
        
    }
}




// void warp(char* CurrPath, char* homePath, char* PrevPath)
// {

//     char Path[256];
//     fgets(Path, 256, stdin);
//     Seq_Run(Path, CurrPath, homePath, PrevPath);

// }
// void Seq_Run(char** cmd_seq, int cmd_cnt)
// {
//     char temp[256];
//     char* tok;
//     char slash[2] = "/";
//     for( int i = 1 ; i < cmd_cnt ; i++ )
//     {  
//         tok = strtok(cmd_seq[i], slash);
//         int j = 0;
//         while( tok != NULL )
//         {
//             // printf("%s | %d\n",tok,strlen(tok));
//             if( strlen(tok) == 1 )
//             {
//                 if( tok[0] == '\n' )
//                 {
//                     if( i + j == 0 )
//                     {
//                         strcpy(temp, CurrPath);
//                         if( Execute("~", CurrPath, homePath, PrevPath) )
//                         {
//                             strcpy(PrevPath, temp);          
//                         }
//                     }
//                 }
//                 else if( tok[0] == '.' )
//                 {
//                     tok = strtok(NULL, slash);
//                     j++;
//                     continue;
//                 }
//                 else
//                 {
//                     strcpy(temp, CurrPath);
//                     if( Execute(tok, CurrPath, homePath, PrevPath) )
//                     {
//                         strcpy(PrevPath, temp);
//                     }
//                 }
//             }
//             else if( strlen(tok) > 1 )
//             {
//                 if( tok[strlen(tok) - 1] == '\n' )
//                 {
//                     tok[strlen(tok) - 1] = '\0';
//                 }
//                 strcpy(temp, CurrPath);
//                 if( Execute(tok, CurrPath, homePath, PrevPath) )
//                 {
//                     strcpy(PrevPath, temp);
//                 }
//             }
//             // printf("%s | %d\n",tok,strlen(tok));

           
//             tok = strtok(NULL, slash);
//             j++;
//         } 

//     }
// }
// bool Execute(char* Path, char* CurrPath, char* homePath, char* PrevPath)
// {
//     //  char check[256];
//         // char path[256];
//     //  scanf("%s",check);
//     //  if(strcmp(check,"warp") == 0){
//         int Dircheck;
//         // scanf("%s",path);
//         if( strcmp(Path,"~") == 0 )
//         {
//             if( homeDir(homePath, CurrPath) )
//             {
//                 return true;
//             }
//             else{
//                 return false;
//             }
//         }
//         else if( strcmp(Path,"home") == 0 )
//         {
//             int dirCheck = chdir(getenv("HOME"));
//             if( dirCheck == 0 )
//             {
//                 strcpy(CurrPath,"~");
//                 ChangeDir(Path,CurrPath);
//             }
//             else
//             {
//                 perror("Error while executing");
//             }
//         }
//         else if( strcmp(Path,"-") == 0 )
//         {

//             Seq_Run(&PrevPath,1);       //CHECK IS UNCOMPLETE FOR SEQ_RUN AS DIR IS PRINTED TWICE IN THIS CASE
//         }
//         else if( strcmp(Path, "..") == 0 )
//         {
//             int i = strlen(CurrPath);
//             while( i >= 0 && CurrPath[i] != '/' )
//             {
//                 i--;
//             }
//             CurrPath[i] = '\0';
//             char absPath[256];
//             getcwd(absPath,256);
//             i = strlen(absPath);
//             while( i >= 0 && absPath[i] != '/' )
//             {
//                 i--;
//             }
//             absPath[i] = '\0';

//             if( chdir(absPath) == 0 )                // Executing the '..' command
//             {
//                 PrintDir();
//                 return true;
//                 // printf("%s\n",absPath);
//             }
//             else
//             {
//                 printf("Error While Executing '..'\n");
//                 return false;
//             }
//         }
//         else
//         {
//             if( ChangeDir(Path, CurrPath) )
//             {
//                 return true;
//             }
//             else
//             {
//                 return false;
//             }
//         }
//         // printf("%s\n",path);
//     //  }
//     //  else{
//     //     printf("ERROR : %s is not a valid command\n",check);
            
//     //  }
// }