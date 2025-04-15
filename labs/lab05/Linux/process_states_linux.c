// process_states_linux.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child Process
        printf("Child Process (PID: %d) created.\n", getpid());

        sleep(5); // Move to 'Sleeping' state
        printf("Child Process waking up from sleep.\n");

        raise(SIGSTOP); // Move to 'Stopped' state

        sleep(10); // Continue after resuming

        printf("Child Process Exiting.\n");
        exit(0);
    }
    else {
        // Parent Process
        printf("Parent Process (PID: %d) observing child (PID: %d).\n", getpid(), pid);

        sleep(2);
        system("ps -aux | grep process_states_linux");  // Check states

        sleep(6);
        printf("Resuming child process...\n");
        kill(pid, SIGCONT); // Move child to 'Running' state

        wait(NULL);  // Wait for child process to complete
        printf("Child process finished.\n");

        system("ps -aux | grep process_states_linux"); // Check again

    }
    return 0;
}
