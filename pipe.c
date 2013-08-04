#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int pipefd[2] = {0};
	pid_t pid;
	char buf;
	int status;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		return -1;
	}
	
	if (pipe(pipefd) < 0) {
		perror("[e]pipe");
		return -2;
	}
	
	pid = fork();
	if (pid < 0) {
		perror("[e]fork");
		return -3;
	} 
	
	if (pid == 0) {
		close(pipefd[1]);
		
		while (read(pipefd[0], &buf, 1) > 0) {
			write(STDOUT_FILENO, &buf, 1); 
		} 
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);				
	} else {
		close(pipefd[0]);
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);
		wait(&status);	
		printf("status:%d\n", status);
	}
	
	return 0;
}
