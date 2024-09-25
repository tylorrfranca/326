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

    // Check if the correct number of arguments is provided
    if (argc != 3) 
    {
        fprintf(stderr, "ERROR: Need exactly 2 parameters.\n");
        exit(EXIT_FAILURE);
    }

    // Open source and target files
    int source_fd = open(argv[1], O_RDONLY);
    int target_fd = open(argv[2], O_RDWR | O_CREAT | O_APPEND, 0666);

    if (source_fd == -1 || target_fd == -1) 
    {
        perror("ERROR: Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Create pipe
    if (pipe(pipe_fd) == -1) 
    {
        perror("ERROR: Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork to create a child process
    child_pid = fork();

    if (child_pid == -1) 
    {
        perror("ERROR: Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) // Child process
    {
        close(pipe_fd[1]); // Close write-end of the pipe

        // Read from the pipe and write to the target file
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) 
        {
            write(target_fd, buffer, bytes_read);
        }

        close(pipe_fd[0]); // Close read-end of the pipe
        close(target_fd);  // Close target file
    } 
    else // Parent process
    {
        close(pipe_fd[0]); // Close read-end of the pipe

        // Read from the source file and write to the pipe
        ssize_t bytes_read;
        while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) 
        {
            write(pipe_fd[1], buffer, bytes_read);
            memset(buffer, 0, sizeof(buffer));
        }

        close(pipe_fd[1]); // Close write-end of the pipe
        close(source_fd);  // Close source file
        wait(NULL);        // Wait for child process to finish
    }

    return 0;
}
