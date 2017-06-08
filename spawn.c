#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "auxs.h"

char* getVal(char *line, int coluna) {

    int two_dots = 0, reachEnd = 0, idx = 0, i;
    int size = strlen(line);
    char saved_elem[28];
    char *colElem;

    for (idx = 0; idx < size && two_dots < (coluna-1); idx++)
      if(line[idx] == ':')
        two_dots++;

    if (idx == size)
      reachEnd = 1;
    else {
      for (i = 0; line[idx] != ':' && line[idx] != '\n' && line[idx] != '\0'; idx++, i++)
        saved_elem[i] = line[idx];
      saved_elem[i] = '\0';
      colElem = strdup(saved_elem);
    }

    return (reachEnd) ? NULL : colElem;
}

int main(int argc, char **argv) {

    int i;
  	ssize_t br;
  	pid_t exec_proc;
    char buf[PIPE_BUF];
  	int argsCol[argc];
  	char *args[argc];					//argc para incluir o NULL no final
  	int status;

    for (i = 1; i < argc; i++) {
      args[i-1] = argv[i];
      if (argv[i][0] == '$')
        argsCol[i-1] = atoi(argv[i]+1);
      else
        argsCol[i-1] = 0;
    }
  	args[i] = NULL;

  	while (br = read_line(0, buf, PIPE_BUF) > 0) {
      for (i = 0; i < argc; i++)
        if (argsCol[i])
        	args[i] = getVal(buf, argsCol[i]);
      exec_proc = fork();
      if (exec_proc == 0)
      	execvp(args[0], args);
      else {
      	wait(&status);
        sprintf(buf+br-1, ":%d\n", WEXITSTATUS(status));
				write(1, buf, strlen(buf));
    	}
  	}

}
