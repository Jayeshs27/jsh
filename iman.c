#include "headers.h"
#define MAX_OUTPUT 800000




void extractTextFromHTML(const char *html) {
    bool inTag = false;
    bool isPrinted = false;
    for (int i = 0; html[i] != '\0'; i++) {
        if (html[i] == '<') {
            inTag = true;
        } else if (html[i] == '>') {
            inTag = false;
        } else if (!inTag) {
            putchar(html[i]);
            isPrinted = true;
        }
    }
    if( isPrinted == false )
    {
        printf("ERROR\n");
        printf("\tNo such command");
    }
}

void iman(char** cmd_seq, int cmd_cnt)
{
    if( cmd_cnt < 2 )
    {
        printf("iman:missing argument\n");
    }
    else
    {
        int socket_desc;
        struct sockaddr_in server_addr;
        char host[50] = "man.he.net";
        char server_message[MAX_OUTPUT];
        char client_message[200];
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));

        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0){
            printf("Unable to create socket\n");
            return;
        }
      
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET; 
        server_addr.sin_port = htons(80);
        struct in_addr addr;

        server_addr.sin_addr.s_addr = inet_addr("65.19.140.5");

        if( connect( socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("Unable to connect\n");
            close(socket_desc);
            return;
        }

        
        
        snprintf(client_message,sizeof(client_message),"GET /?topic=%s&section=all HTTP/1.1\r\n"
                                                        "Host:%s\r\n"
                                                        "\r\n",cmd_seq[1],host);
        
        if(send(socket_desc, client_message, strlen(client_message), 0) < 0)
        {
            printf("Unable to send message\n");
            close(socket_desc);
            return;
        }
        
        int cnt = 0;
        while(recv(socket_desc, server_message, sizeof(server_message), 0) > 0)
        {
            
            printf("server_reading\n");
            if( cnt == 0 )
            {
                char* separator = strstr(server_message,"NAME\n");
                if( separator == NULL )
                {
                    printf("ERROR\n\tNo Such command\n");
                }
                else
                {
                    extractTextFromHTML(separator);
                } 
            }
            else
            {
                extractTextFromHTML(server_message);
            }
            cnt ++;
           
        }
        
        printf("\n");
 
        close(socket_desc);
       

    }
}