
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "list.h"

#define BUFFER_SIZE 64
extern int kill(__pid_t __pid, int __sig);
void signal1_handler() {

}
void signal2_handler() {

}

int main(int argc, char** argv, char** envp) {
    if (argc < 2)
        return EXIT_FAILURE;

    signal(SIGUSR1, signal1_handler);
    signal(SIGUSR2, signal2_handler);

    int child_count = 0;
    pid_t child_pid;
    Node* child_pid_list = createNode(getpid());

    char buffer[BUFFER_SIZE] = "\0";

    while (scanf(" %s", buffer)) {
        char* child_argv[2] = {(char*)0};

        switch (buffer[0])
        {
            case '+':
            {
                switch (child_pid = fork())
                {
                    case -1:
                    {
                        child_count--;
                        perror("Fork error");
                        break;
                    }
                    case 0:
                    {
                        child_argv[0] = (char*)malloc(sizeof(char) * 6);
                        sprintf(child_argv[0], "C_%d", child_count);

                        if (execve(argv[1], child_argv, envp) == -1)
                        {
                            perror("Execve");
                            free(child_argv[0]);
                            exit(1);
                        }
                        break;
                    }
                    default:
                        addNode(child_pid_list, createNode(child_pid));
                        child_count++;
                        break;
                }

                break;
            }
            case '-':
            {
                if (child_count > 0)
                {
                    kill(pop(child_pid_list), SIGKILL);
                    child_count--;
                    printf("C_%d was killed. %d left\n", child_count, child_count);
                }
                else
                    puts("Nothing to kill");

                break;
            }
            case 'l':
            {
                printf("PPID: %d\n", getpid());
                displayList(child_pid_list);
                break;
            }
            case 'k':
            {
                while(child_count > 0) {
                    kill(pop(child_pid_list), SIGKILL);
                    child_count--;
                }
                printf("All child process were succesfully killed\n");
                break;
            }
            case 's':
            {
                if (strlen(buffer) > 1)
                {
                    if(child_count < atoi(buffer + 1))
                        puts("Child with this number are not exitst");
                    else
                        kill(getNode(child_pid_list, atoi(buffer + 1))->pid, SIGUSR1);
                }
                else
                    kill(0, SIGUSR1);

                break;
            }
            case 'g':
            {
                if (strlen(buffer) > 1)
                {
                    if(child_count < atoi(buffer + 1))
                        puts("Child with this number are not exitst");
                    else
                        kill(getNode(child_pid_list, atoi(buffer + 1))->pid, SIGUSR2);
                }
                else
                    kill(0, SIGUSR2);

                break;
            }
            case 'p':
            {
                if (strlen(buffer) <= 1) {
                    break;
                }

                kill(0, SIGUSR1);
                kill(getNode(child_pid_list, atoi(buffer + 1))->pid, SIGUSR2);

                sleep(1);

                kill(getNode(child_pid_list, atoi(buffer + 1))->pid, SIGUSR1);

                sleep(5);

                kill(0, SIGUSR2);

                break;
            }
            case 'q':
            {
                kill(0, SIGKILL);
                return 0;
            }
            default:
                break;
        }
    }
}