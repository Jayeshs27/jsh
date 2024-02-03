#include "headers.h"


static int myCompare(const void* a, const void* b)
{
 
    return strcmp(*(const char**)a, *(const char**)b);
}
 

void sort(char** arr, int n)
{
    qsort(arr, n, sizeof(char*), myCompare);
}
char** MakeStringArr(int n,int size)
{
    char** str = (char**)malloc(sizeof(char*)*n);
    if( str == NULL )
    {
        return NULL;
    }
    for( int i = 0 ; i < n ; i++ )
    {
        str[i] = (char*)malloc(sizeof(char)*size);
        if( str[i] == NULL )
        {
            return NULL;
        }
    }
    return str;
}
void printFileInfo(char *file, struct stat *fileStat) 
{

    printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");
    
    printf(" %ld", fileStat->st_nlink);
    printf(" %s", getpwuid(fileStat->st_uid)->pw_name);
    printf(" %s", getgrgid(fileStat->st_gid)->gr_name);
    printf(" %lld", fileStat->st_size);
    
    char time[100];
    strftime(time, sizeof(time), "%b %d %H:%M", localtime(&(fileStat->st_mtime)));
    printf(" %s", time);
    printf(" %s", file);
    printf("\n");
}
bool isFlag(char* str)
{
    if( strcmp(str,"-l") == 0 || strcmp(str,"-a") == 0 || strcmp(str,"-la") == 0 || strcmp(str,"-al") == 0  )
    {
        return true;
    }
    return false;
}

bool IsTHIR_flag(char* str)
{
    if( strcmp(str,"-l") == 0 || strcmp(str,"-a") == 0 )
    {
        return true;
    }
    return false;
}
bool isDir(char* PATH)
{  
    DIR* dir = opendir(PATH);
    FILE* ptr = fopen(PATH,"r");
    if (dir || ptr != NULL ) 
    {
        closedir(dir);
        return true;printf("::%s\n",PrevPath);
    }
    return false;
}
bool isDirect(char* PATH)
{  
    DIR* dir = opendir(PATH);
    if (dir) 
    {
        closedir(dir);
        return true;
    }
    return false;
}

void Execute_cmd(char* Path,int Allflag, int infoFlag)
{

    DIR *d;
    struct dirent *dir;
    struct dirent *entry;
    struct stat fileStat;
    d = opendir(Path);
    int t = 0;
    char** list = MakeStringArr(256,128);
    if( list == NULL )
    {
        printf("Execute_cmd:MakeStringArr returned NULL\n");
        return;
    }
    if (d) 
    {
        t = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if( Allflag == 0 && dir->d_name[0] == '.')
            {
                continue;
            }
            sprintf(list[t], "%s", dir->d_name);
            t++;
        }
        closedir(d);
    }
    else
    {
        FILE* ptr = fopen(Path,"r");
        if( ptr != NULL )
        {
            if( infoFlag == 0 )
            {
                printf("%s\n",Path);
            }
            else
            {
                if (stat(Path, &fileStat) == -1) 
                {
                   perror("Unable to get file info");
                }
                else
                {   
                   printFileInfo(Path, &fileStat);
                }
            }
        }
        else
        {
            printf("No Such File Or Dir Exist\n");
        }
    }
 
    sort(list,t);
    int exeCheck[t];
    for(int i = 0 ; i < t ; i++ )
    {
        
        struct stat fileStat;

        if (stat(list[i], &fileStat) == -1) {
            exeCheck[i] = 0;
        }

        // Check if the file is executable for the owner, group, or others
        if (fileStat.st_mode & S_IXUSR || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXOTH) {
            exeCheck[i] = 1;
        }  
        else
        {
            exeCheck[i] = 0;
        }
    }
    if( infoFlag == 0)
    {
        for( int i = 0 ; i < t ; i++ )
        {
            if( isDirect(list[i]) )
            {
               printf("\033[0;34m");
               printf("%s\n",list[i]);
               printf("\033[0m");
            }
            else if( exeCheck[i] == 1 )
            {
               printf("\033[0;32m");
               printf("%s\n",list[i]);
               printf("\033[0m");

            }
            else
            {
               printf("\033[0;37m");
               printf("%s\n",list[i]);
               printf("\033[0m");
            //    printf("fcolor\n");
            }

        }
    }
    else{
        for( int i = 0 ; i < t ; i++ )
        {
            char realpath[256];
            strcpy(realpath,Path);
            strcat(realpath,"/");
            strcat(realpath,list[i]);
            if (stat(realpath, &fileStat) == -1) {
                perror("Unable to get file info");
            }
            else
            {   
                if( isDirect(list[i]) )
                {
                    printf("\033[0;34m");
                    printFileInfo(list[i], &fileStat);
                    printf("\033[0m");
                }
                else if( exeCheck[i] == 1 )
                {
                    printf("\033[0;32m");
                    printFileInfo(list[i], &fileStat);
                    printf("\033[0m");

                }
                else
                {
                    printf("\033[0;37m");
                    printFileInfo(list[i], &fileStat);
                    printf("\033[0m");
                }
                
            }
        }
    }
    free(list);
   
}


void peek(char** cmd_seq, int cmd_cnt)
{
    char Path[256]=".";
    char newPath[256];
    strcpy(newPath, cmd_seq[cmd_cnt - 1]);
    if( isDir(newPath) )
    {
        strcpy(Path, newPath);  
        cmd_cnt --;
    }
    if( cmd_cnt == 1 )
    {
        Execute_cmd(Path,0,0);
    }
    else if( cmd_cnt == 2 )
    {
        if( strcmp(cmd_seq[cmd_cnt - 1],"-l") == 0 )
        {
            Execute_cmd(Path,0,1);
        }
        else if( strcmp(cmd_seq[cmd_cnt - 1],"-a") == 0 )
        {
            Execute_cmd(Path,1,0);
        }
        else if( strcmp(cmd_seq[cmd_cnt - 1],"-la") == 0 )
        {
            Execute_cmd(Path,1,1);
        }
        else if( strcmp(cmd_seq[cmd_cnt - 1],"-al") == 0 )
        {
            Execute_cmd(Path,1,1);
        }
        else
        {
                printf("Invalid flags!\n");
        }
    }
    else if( cmd_cnt == 3 )
    {
        if( strcmp(cmd_seq[cmd_cnt - 2],"-l") == 0 && strcmp(cmd_seq[cmd_cnt - 1],"-a") == 0)
        {
            Execute_cmd(Path,1,1);
        }
        else if( strcmp(cmd_seq[cmd_cnt - 2],"-a") == 0 && strcmp(cmd_seq[cmd_cnt - 1],"-l") == 0)
        {
            Execute_cmd(Path,1,1);
        }
        else
        {
            printf("Invalid flags!\n");
        }
    }
    else
    {
        printf("peek:too many arguments\n");
    }
        
}