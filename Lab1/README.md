# Group Project 1: Warm up of Interprocess Communication
In this project, you will design and implement a file-copying program named filecopy.c that utilizes
ordinary pipes for inter-process communication. The program will be passed two parameters: the name of
the file to be copied and the name of the destination file. It will create a pipe (please find more info of pipe
for interprocess communication by yourself), write the contents of the source file to the pipe, and then have
a child process read from the pipe and write to the destination file.

## How-to run (Using Linux & gcc)
Ensure `gcc` is installed, if it isn't run:__
`sudo apt-get install gcc`__
Then run:__
`gcc filecopy.c -o run`__
(Optionally generate a text file)__ 
`echo some text > input.txt`__
`./run input.txt output.txt`
