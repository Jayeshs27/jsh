#include "headers.h"


char curr_bg_Process[256];
struct Process* bg_list = NULL;
struct ProcInfo* bgInfo;

void handleChildExit(int signum) 
{
    pid_t child_pid;
    int status;
    
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            ChangeState(&bg_list, child_pid, WIFEXITED(status),"EXITED");
    }
}


void ChangeState(struct Process** head, int pid, int status, char* state) {

    struct Process* current = *head;
    // printf("status : %d\n",status);
    while (current != NULL) 
    {
        if( current->pid == pid )
        {
            current->state = strdup(state);
            current->status = status;
             
        }
        current = current->next;
    }
}

void addProcess(struct Process** head, int pid, char* State, char* name) {
    // printf("added %s %s\n",name,State);
    struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));
    newProcess->pid = pid;
    newProcess->state = strdup(State);
    newProcess->name = strdup(name);
    newProcess->next = NULL;

    if (*head == NULL)
    {
        *head = newProcess;
    } 
    else
    {
        struct Process* temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newProcess;
    }

}
// void addProcess(struct Process** head, int pid, int status, const char* name) {
//     struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));
//     newProcess->pid = pid;
//     newProcess->status = status;
//     newProcess->next = NULL;

//     struct ProcInfo* current = bgInfo;
//     struct ProcInfo* prev = NULL;

//     while (current != NULL) 
//     {
//         if( current->pid == pid )
//         {
//             newProcess->name = strdup(current->name);
//             if (prev == NULL) 
//             {
//                bgInfo = current->next;
//                free(current);
//                break;
//             }
//             else
//             {
//                 prev->next = current->next;
//                 free(current);
//                 break;
//             }
             
//         }
//         current = current->next;
//     }

//     if (*head == NULL)
//     {
//         *head = newProcess;
//     } 
//     else
//     {
//         struct Process* temp = *head;
//         while (temp->next != NULL)
//         {
//             temp = temp->next;
//         }
//         temp->next = newProcess;
//     }

// }
// void addProcInfo(struct ProcInfo** head, int pid, const char* name) {
//     struct ProcInfo* newProcess = (struct ProcInfo*)malloc(sizeof(struct ProcInfo));
//     newProcess->pid = pid;
//     newProcess->name = strdup(name);
//     newProcess->next = NULL;

//     if (*head == NULL) {
//         *head = newProcess;
//     } 
//     elsed
//      {
//         struct ProcInfo* current = *head;
//         while (current->next != NULL) 
//         {
//             current = current->next;
//         }
//         current->next = newProcess;
//     }
// }

void RemoveProc(struct Process** head,int pid)
{
    if (*head == NULL)
    {
        return;
    }
    struct Process* current = *head;
    struct Process* prev = NULL;

    while (current != NULL) 
    {
        if( current->pid == pid )
        {
            if(prev == NULL)
            {
                *head = current->next;
                free(current);
                current = *head;
            } 
            else
            {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            return;
        }
        current = current->next;
    }
}

void Check_bg_Processes(struct Process** head) {

    if (*head == NULL) {
        return;
    }

    struct Process* current = *head;
    struct Process* prev = NULL;

    while (current != NULL) 
    {
        // sleep(2);
        if( strcmp(current->state,"EXITED") == 0 )
        {
            if( current->status == 1 )
            {
                printf("%s exited normally (%d)\n",current->name,current->pid);
            }
            else
            {
                printf("%s exited abnorally (%d)\n",current->name,current->pid);
            }

            if(prev == NULL)
            {
                *head = current->next;
                free(current);
                current = *head;
                continue;
            } 
            else
            {
                prev->next = current->next;
                free(current);
                current = prev->next;
                continue;
            }
        }

        current = current->next;
    }
}




void Excute_sys(char** one_cmd, int ct, int bg_flag)
{

    signal(SIGCHLD, handleChildExit);
  
    one_cmd[ct] = NULL;
    // ct++;

        if( bg_flag == 0 )
        {
            int c = fork();
            int stat;
            int err_num = 0;
            if( c == 0 )
            {
            //   signal(SIGCONT, SIG_DFL);
              signal(SIGTTOU, SIG_IGN);
              signal(SIGTSTP, SIG_DFL);

              int check = execvp(one_cmd[0],one_cmd);
              if( check == - 1 && errno == 2 )
              {  
                 printf("ERROR :'%s' is not a valid command\n",one_cmd[0]);
              }
              exit(0);
            }
            currfg = c;
            // printf("below exe\n");
             waitpid(c, &stat, WUNTRACED);
             if ( WIFSTOPPED(stat) ) {
               
                setpgid(c, c);  
                addProcess(&bg_list, c, "STOPPED", one_cmd[0]);
    
             }
            
            currfg = -1;
            return;
            
        }
        else
        {
            int flag = fork();
            if( flag == 0 )
            { 
               freopen("/dev/null", "w", stderr);
               int check =  execvp(one_cmd[0],one_cmd);
               if( check == - 1 )
               {
                   exit(1);
               }
            }
            else
            {
                int stat;
                pid_t cpid = waitpid(flag, &stat, WNOHANG);
                addProcess(&bg_list,flag,"RUNNING",one_cmd[0]);
                printf("%d\n",flag);
            }
        }

    
}