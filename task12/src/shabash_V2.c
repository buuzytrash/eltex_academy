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
  char* tok = strtok(input, "|");
  int arg_count = 0;
  int fd[2];

  while (tok != NULL && arg_count < MAX_ARGS - 1) {
    args[arg_count++] = tok;
    tok = strtok(NULL, "|");
  }

  args[arg_count] = NULL;

  if (arg_count == 0) {
    perror("No input command");
  }

  for (int i = 0; i < arg_count; ++i) {
    char* cmd_args[MAX_ARGS];
    int cmd_args_count = 0;
    tok = strtok(args[i], " ");

    while (tok != NULL && cmd_args_count < MAX_ARGS - 1) {
      cmd_args[cmd_args_count++] = tok;
      tok = strtok(NULL, " ");
    }

    cmd_args[cmd_args_count] = NULL;

    if (i < arg_count - 1) {
      if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }
    }

    pid_t pid = fork();

    if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
      if (i > 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
      }

      if (i < arg_count - 1) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
      }

      if (execvp(cmd_args[0], cmd_args) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
      }
    }
    else {
      if (i > 0) {
        close(fd[0]);
      }

      if (i < arg_count - 1) {
        close(fd[1]);
      }

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