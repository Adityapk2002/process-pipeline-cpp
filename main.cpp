#include <iostream>
#include <unistd.h>

void pipeline(const char *process1, const char *process2)
{
    int fd[2];
    pipe(fd);
    int id = fork();
    if (id != 0) // This is the parent process (which will run cat).
    {
        close(fd[0]);               // close the read end of the pipe
        dup2(fd[1], STDOUT_FILENO); // redirect standard output to write end of pipe
        close(fd[1]);               // close the write end of the pipe

        // [execute process]
        execlp("/bin/cat", "cat", "main.cpp", nullptr);
        std::cerr << "Failed to execute" << process1 << std::endl;
    }
    else // This is the child process (which will run grep hello).
    {
        close(fd[1]);              // close the write end of the pipe
        dup2(fd[0], STDIN_FILENO); // redirect standard input to the read end of the pipe
        close(fd[0]);              // close the read end of the pipe

        //[execute process2]
        execlp("/usr/bin/grep", "grep", "hello", nullptr);
        std::cerr << "Failed to execute " << process2 << std::endl;
    }
}
int main()
{
    pipeline("cat main.cpp", "grep hello");
    return 0;
}