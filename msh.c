#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_ARGS 64

int main(){
    size_t len= 0; 
    char *input = NULL;
    char *args[MAX_ARGS];
    int last_status = 0;  //this stores the exit status of the previous command
   // a loop to enable the shell to work since when i print the prompt wtht the loop it printed then returned command to the os 
    while(1){
        while(waitpid(-1, NULL, WNOHANG) > 0){  //this is for waiting for background child processes, its work is to look for any 
                                       //child background process that have finished so that it can be cleaned b4 it enters the shell

        }
        printf("msh[%d]>", last_status);
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

        while(token != NULL && i < MAX_ARGS - 1){
            args[i] = token;
            token = strtok(NULL, " \t\r\n");   //splits line on spaces and tabs
            i++;
        }
        args[i]=NULL;

        if (i == 0){ //this is for skipping white spaces and blank lines
            continue;
        }
        int background = 0;  //used to check for trailing '&' so that it can run the command in the background.
        if(strcmp(args[i-1], "&")==0){
            background = 1;
            args[i-1] = NULL; //used to remove '&' so it is not passed to execvp
            i--;
            if(i == 0){
                continue;
            }
        }

        // Check for output redirection '>'
        char *outfile = NULL;
         // O_WRONLY opens for writing only, O_CREAT makes the file if it is not there,
                // O_TRUNC empties the file if it already has something,
        for (int j = 0; args[j] != NULL; j++) {
            if (strcmp(args[j], ">") == 0) {
                if (args[j + 1] == NULL) {
                    printf("syntax error: expected file after '>'\n");
                    last_status = 1;
                    outfile = (char *)-1; // mark syntax error
                    break;
                }
                outfile = args[j + 1];
                args[j] = NULL; // cut off '>' and filename so execvp doesn't pass them as arguments
                break;
            }
        }
        if (outfile == (char *)-1) {
            continue;
        }

        if (strcmp(args[0],"cd")==0){    //chdir is used to change the directory
            char *target_dir = args[1];
            if (target_dir == NULL) {
                target_dir = getenv("HOME"); // if the cd has no argument it will go directly to home
            }
                if (target_dir == NULL) {
                    printf("cd: HOME not set\n");
                    continue;
                }else if(chdir(target_dir) !=0 ){
                    perror("cd failed");
                }
                continue;
            }
        
        if (strcmp(args[0], "help") == 0) { //this is like a manual for one who is lost and doesn't know their way around
            printf("Built-in commands:\n");
            printf("  cd <dir>   Change the current working directory\n");
            printf("  help       Display this help message\n");
            printf("  exit       Exit the shell\n");
            continue;
        }
        if (strcmp(args[0], "exit") == 0) {
            break; //used to get out of the loop
        }

        pid_t pid = fork(); //fork used to create a new process (a child process) from a parent process.
        
        if(pid == 0){      //This is a child process
            if (outfile != NULL) {
                int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("open failed");
                    exit(EXIT_FAILURE);
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("dup2 failed");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
                close(fd); // close the extra file descriptor after duplicating onto stdout since stdout points to the file
            }
            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);

        }else if(pid > 0){       //This is a parent process
            if(background){
                printf("[%d]\n", pid); 
                last_status = 0;
            }else{
            int status;
            if(waitpid(pid,&status,0) == -1){ 
                perror("waitpid failed");//waiting for the childprocess to terminate so that it cannot be left to roam and be a zombie process
                last_status = 1;
                } else if (WIFEXITED(status)) {
                    last_status = WEXITSTATUS(status); // normal exit
                } else if (WIFSIGNALED(status)) {
                    last_status = 128 + WTERMSIG(status); //this is killed by signal eg SIGKILL
                }
            }
            
            

        }else{
            perror("fork failed");
        }
   

    }
    free(input); // this is used to free the buffer on exit so that it doesn't leak .Since getline() allocates heap memory

    return 0;


}

