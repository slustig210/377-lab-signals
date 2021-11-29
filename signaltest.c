#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

// signal handlers
void sigchld_handler() {
    printf(1, "SIGCHLD %d\n", getpid());
}

void sigusr1_handler() {
    printf(1, "SIGUSR1 %d\n", getpid());
}

int main(int argc, char **argv) {
    // set signal handlers using the signal system call
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGCHLD, sigchld_handler);

    printf(1, "Parent pid: %d\n", getpid());

    int pid = fork();
    if (pid) {
        // parent
        printf(1, "Child process created with PID %d\nSending SIGUSR1 and then going to sleep:\n", pid);
        sigsend(pid, SIGUSR1);
        sleep(10000);
    } else {
        // child
        // wait for SIGUSR1, then exit.
        sleep(10000);
        printf(1, "Child process exiting.\n");
        exit(); // will send SIGCHLD to parent
    }

    printf(1, "Parent process waiting for child.\n");
    wait();
    printf(1, "Parent process exiting.\n");
    return 0;
}