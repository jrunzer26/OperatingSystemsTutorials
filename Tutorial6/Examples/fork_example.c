#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

/**
 * A simple forking example. Forks and both child and
 * parent print out different messages.
 */
int main(int argc, char *argv[]) {

    pid_t PID = 0;

    // Create the child.
    PID = fork();

    // Not the child or parent
    if (PID == -1) {
        return 0;
    }

    //Is it a child process or the parent?
    if (PID == 0) {
        //Child process:
        printf("Child Says Hi!\n");

        //Finish on success
        return 0;
    } else {
        //Parent process:
        printf("Parent Says Hello\n");
    }

    return 0;
}