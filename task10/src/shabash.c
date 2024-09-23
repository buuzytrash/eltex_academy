#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 32

void exec_cmd(char* input)
{
  char* args[MAX_ARGS];
  char* tok = strtok(input, " ");
  int arg_count = 0;

  while (tok != NULL && arg_count < MAX_ARGS - 1) {
    args[arg_count++] = tok;
    tok = strtok(NULL, " ");
  }

  args[arg_count] = NULL;

  if (arg_count == 0) {
    perror("No input command");
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {
    // Дочерний процесс
    if (execvp(args[0], args) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  }
  else {
    // Родительский процесс
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
      printf("Child process exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
      printf("Child process terminated by signal %d\n", WTERMSIG(status));
    }
  }
}

int main()
{
  char input[MAX_INPUT];

  while (1) {
    printf("shabash> ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
      break;
    }

    input[strcspn(input, "\n")] = 0;

    if (strcmp(input, "exit") == 0) {
      break;
    }

    if (strlen(input) == 0) {
      printf("Please enter a command\n");
      continue;
    }

    exec_cmd(input);
  }

  return 0;
}