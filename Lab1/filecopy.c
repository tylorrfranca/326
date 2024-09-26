#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 50

int main(int argc, char* argv[])
{
    int pipe_fd[2];
    pid_t child_pid;
    char buffer[BUFFER_SIZE];

    // Make sure two arguments are passed
    if (argc != 3) 
    {
        fprintf(stderr, "ERROR: Need exactly 2 parameters.\n");
        exit(1);
    }

    // Open source and target files
    int sourceFile = open(argv[1], O_RDONLY);
    int targetFile = open(argv[2], O_WRONLY | O_CREAT |O_TRUNC, 0666);

    if (sourceFile == -1 || targetFile == -1) 
    {
        perror("ERROR: Unable to open file");
        exit(1);
    }

    // Create pipe
    if (pipe(pipe_fd) == -1) 
    {
        perror("ERROR: Pipe creation failed");
        exit(1);
    }

    // Fork to create a child process
    child_pid = fork();

    if (child_pid == -1) 
    {
        perror("ERROR: Fork failed");
        exit(1);
    }

    if (child_pid == 0) // Child process
    {
        close(pipe_fd[1]); // Close write-end of pipe

        // Read from pipe and write to target file
        ssize_t bytesRead;
        while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) 
        {
            write(targetFile, buffer, bytesRead);
        }

        close(pipe_fd[0]); // Close read-end of pipe
        close(targetFile);  // Close target file
    } 
    else // Parent process
    {
        close(pipe_fd[0]); // Close read-end of pipe

        // Read from source file and write to pipe
        ssize_t bytesRead;
        while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0) 
        {
            write(pipe_fd[1], buffer, bytesRead);
            memset(buffer, 0, sizeof(buffer));
        }

        close(pipe_fd[1]); // Close write-end of pipe
        close(sourceFile);  // Close source file
        wait(NULL);        // Wait for child process to finish
    }

    return 0;
}
