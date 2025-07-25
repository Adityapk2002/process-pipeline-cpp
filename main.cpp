#include <iostream>
#include <unistd.h>

void pipeline(const char *process1, const char *process2)
{
    int fd[2];
    pipe(fd);
    int id = fork();
    if (id != 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("/bin/cat", "cat", "main.cpp", nullptr);
        std::cerr << "Failed to execute" << process1 << std::endl;
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execlp("/usr/bin/grep", "grep", "hello", nullptr);
        std::cerr << "Failed to execute " << process2 << std::endl;
    }
}
int main()
{
    pipeline("cat main.cpp", "grep hello");
    return 0;
}