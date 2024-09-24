#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	//Create a new process and save its PID
	pid_t pid;
	pid = fork();

	if (pid < 0) {
		// Error happened while forking
		perror("fork");
		return 1;
	} else if (pid == 0) {
		// This is the child process
		printf("My favorite TV show is Rick and Morty. (PID %d)\n", getpid());
	} else {
		// This is the parent process 
		// Wait for child to complete
		printf("My name is Jayden Alton. (PID %d)\n", getpid());
		wait(NULL);
		printf("Child process has finished.\n");
	}

	return 0;

}
