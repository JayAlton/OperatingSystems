#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	
	// Create new processes and save their PIDs
	pid_t pid1, pid2, pid3;
	pid1 = fork();

	if (pid1 < 0) {
		// Error happened while forking
		perror("fork");
		return 1;
	} else if (pid1 == 0) {
		// This is the first child process
		printf("My major is Computer Systems. (PID %d)\n", getpid());
	} else {
		// Fork again and wait for both child processes to complete
		pid2 = fork();

		if (pid2 < 0) {
			// Error happened while forking
			perror("fork");
			return 1;
		} else if (pid2 == 0) {
			// This is the second child process
			printf("I except to graduate in Spring/Summer 2025. (PID %d)\n", getpid());
		} else {
			// This is the parent process
			// Fork again and wait for all child processes to complete
			printf("My name is Jayden Alton. (PID %d)\n", getpid());

			pid3 = fork();

			if (pid3 < 0) {
				// Error happened while forking
				perror("fork");
				return 1;
			} else if (pid3 == 0) {
				// This is the third child process
				printf("My concentration is Systems Administration. (PID %d)\n", getpid());
			} else {

				// This is the parent process
				// Wait for both processes to finish
				waitpid(pid1, NULL, 0);
				waitpid(pid2, NULL, 0);
				waitpid(pid3, NULL, 0);
				printf("Child processes has finished.\n");
			}
		}
	}
	
	return 0;
}

