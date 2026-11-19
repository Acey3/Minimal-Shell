#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ARGS 64

int main(){
    size_t len= 0; 
    char *input = NULL;
    char *args[MAX_ARGS];
// a loop to enable the shell to work since when i print the prompt wtht the loop it printed then returned command to the os 
    while(1){
        printf("msh>");
        fflush(stdout); //this is used to flush the buffer/memory since sometimes when you printf they may stay in the memory

        if(getline(&input,&len , stdin)== -1){
              //this is used to get the input it can fit any length of input as it needs to feet then updates len of the allocated size
            if(feof(stdin)){
                printf("\n");
                break;
            }
            perror("getline failed");
            break;
        }
        char *token = strtok(input, " \t\r\n");  // this converts string to tokens for command execution
        int i= 0;

        while(token != NULL && i < MAX_ARGS){
            args[i] = token;
            token = strtok(NULL, " \t\r\n");   //splits line on spaces and tabs
            i++;
        }
        args[i]=NULL;

        if (i == 0){ //this is for skipping white spaces and blank lines
            continue;
        }
   
    printf("%s\n",input);

    }
    free(input); // this is used to free the buffer on exit so that it doesn't leak .Since getline() allocates heap memory

    return 0;

}

