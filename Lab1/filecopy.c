/**
 * filecopy.c
 * 
 * This program copies files using a pipe.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define READ_END 0
#define WRITE_END 1

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Error: Expected 2 arguments \n");
		exit(1);
	}

	char *sourceFile = argv[1];
	char *destFile = argv[2];

	int fd[2];

	if (pipe(fd) == -1 ) {
		printf("Failed to create Pipe \n");
		exit(1);
	}
	
	pid_t pid = fork();

	if (pid < 0){
		printf("Failed to fork process \n");
		exit(1);
	}

	if (pid == 0){
		close(fd[1]);

		if (dup2(fd[0], STDIN_FILENO) == -1) {
			printf("Failed to duplicate file descriptor \n");
			exit(1);
        }

		close(fd[0]);

		execl("./child", "./child", destFile, (char *)NULL);

		printf("Failed to exec child process\n");
        exit(1);
} else {
        // Parent process

        // Close the read end of the pipe
        close(fd[0]);

        // Open the source file
        int src_fd = open(sourceFile, O_RDONLY);
        if (src_fd < 0) {
            perror("Failed to open source file");
            // Close the write end before exiting
            close(fd[1]);
            exit(1);
        }

        // Read from source file and write to the pipe
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
            ssize_t bytes_written = write(fd[1], buffer, bytes_read);
            if (bytes_written != bytes_read) {
                perror("Failed to write to pipe");
                close(src_fd);
                close(fd[1]);
                exit(1);
            }
        }

        if (bytes_read < 0) {
            perror("Failed to read from source file");
            close(src_fd);
            close(fd[1]);
            exit(1);
        }

        // Close the source file and the write end of the pipe
        close(src_fd);
        close(fd[1]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
