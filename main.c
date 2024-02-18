#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
  pid_t pid;
  int status;

  // Fork the process
  pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    // We are in the child process

    // Open the error file, with write-only permissions and create it if it doesn't exist
    int newerr = open("/error.log", O_WRONLY | O_CREAT, 0666);
    if (newerr == -1) {
      perror("open");
      return 1;
    }

    // Open the output file, with write-only permissions and create it if it doesn't exist
    int newout = open("/out.log", O_WRONLY | O_CREAT, 0666);
    if (newout == -1) {
      perror("open");
      return 1;
    }

    // Make stderr file descriptor point to the error file
    if (dup2(newerr, STDERR_FILENO) == -1) {
      perror("dup2");
      return 1;
    }

    // Make stdout file descriptor point to the output file
    if (dup2(newout, STDOUT_FILENO) == -1) {
      perror("dup2");
      return 1;
    }

    // Replace the current process with the print-fd program
    // Notice that we are replacing the process of the forked child, not the original process
    char * newargv[] = {
      "/print-fd",
      NULL
    };
    execvp(newargv[0], newargv);

    // execvp only returns if there is an error
    perror("execvp");
    return 1;
  } else {
    // We are in the parent process
    // Wait for the child to finish
    if (waitpid(pid, & status, 0) == -1) {
      perror("waitpid");
      return 1;
    }

    // Check child's exit status
    if (WIFEXITED(status)) {
      printf("Child exited with status %d\n", WEXITSTATUS(status));
    } else {
      printf("Child did not exit cleanly\n");
    }
  }

  return 0;
}
