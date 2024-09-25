/*
 * child.c
 *
 * This program reads the contents of the pipe and writes it to a file.
 */
  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    if (argc != 2){
        fprintf(stderr, "Usage: %s destination_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *destFile = argv[1];

    // Open the destination file
    int dest_fd = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Failed to open destination file");
        exit(EXIT_FAILURE);
    }

    // Read from stdin and write to the destination file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Failed to write to destination file");
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read < 0) {
        perror("Failed to read from stdin");
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    // Close the destination file
    close(dest_fd);

    return 0;
}
