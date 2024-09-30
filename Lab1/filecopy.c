/* 
Authors : Michael Jarrah & Tylor Franca
Program : Group Project 1, Warm Up of Interprocess Communication
*/
#include <stdio.h>     // i/o
#include <sys/types.h> // pid_t
#include <sys/stat.h>  // open()
#include <fcntl.h>     // file control
#include <string.h>    // memset()
#include <stdlib.h>    // exit()
#include <unistd.h>    // fork(), pipe(), read(), write(), close()
#include <sys/wait.h>  // wait()

#define BUFFER_SIZE 200 // arbitrary buffer size

int main(int argc, char* argv[])
{
    int pipe_fd[2];
    pid_t child_pid;
    char buffer[BUFFER_SIZE];

    // Make sure two arguments are passed
    if (argc != 3) 
    {
        printf("Error: Need exactly 2 parameters.\n");
        exit(1);
    }

    // open input/output files
    int sourceFile = open(argv[1], O_RDONLY);
    int targetFile = open(argv[2], O_WRONLY | O_CREAT |O_TRUNC, 0666);

    // handle errors for source and target files
    if (sourceFile == -1) 
    {
        fprintf(stderr, "Error: Unable to open source file '%s'.\n", argv[1]);
        exit(1);
    }

    if (targetFile == -1)
    {
        fprintf(stderr, "Error: Unable to open target file '%s'.\n", argv[2]);
        exit(1);
    }

    // handle pipe errors
    if (pipe(pipe_fd) == -1) 
    {
        printf("Error: Pipe creation failed");
        exit(1);
    }

    // fork to make child process
    child_pid = fork();

    // ensure fork works
    if (child_pid == -1) 
    {
        printf("Error: Fork failed");
        exit(1);
    }

    // child process
    if (child_pid == 0) 
    {
        close(pipe_fd[1]); 
        
        // read pipe, write to target, close all
        ssize_t bytesRead;
        while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) 
        {
            write(targetFile, buffer, bytesRead);
        }

        close(pipe_fd[0]); 
        close(targetFile);  
    } 
    else // parent process
    {
        close(pipe_fd[0]); // close read

        // read source, write to pipe, close all and wait for child
        ssize_t bytesRead;
        while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0) 
        {
            write(pipe_fd[1], buffer, bytesRead);
            memset(buffer, 0, sizeof(buffer));
        }

        close(pipe_fd[1]);
        close(sourceFile);  
        wait(NULL);        

        // print status
        printf("File successfully copied from %s to %s.\n", argv[1], argv[2]);
    }

    return 0;
}
