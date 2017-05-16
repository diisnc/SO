#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int getVal(char *line, int coluna, long *val){

  int two_dots = 0, reachEnd = 0, idx = 0, i;
  int size = strlen(line);
  char saved_elem[21];   /*20 dígitos do maior unsigned int representável em 64 bits mais 1 do \0.*/

  for (idx = 0; idx < size && (two_dots < (coluna-1)); idx++)
    if(line[idx] == ':')
      two_dots++;

    if (idx == size)
      reachEnd = 1;
    else {
      for (i = 0; line[idx]!=':' && line[idx]!='\n' && line[idx]!='\0'; idx++, i++)
        saved_elem[i] = line[idx];
      saved_elem[i] = '\0';
      (*val) = atol(saved_elem);
    }

    return (reachEnd) ? -1 : idx;
}

int stackOrg(long *stack, int i, int nlines, long valCol) {

  if (i == nlines) {
    nlines--;
    while (nlines > 0) {
      stack[nlines-1] = stack[nlines];
      nlines--;
    }
    stack[i-1] = valCol;
  }
  else {
    stack[i] = valCol;
    i++;
  }

  return i;
}

int countDigits(long n){

  int count = 0;

  if (n <= 0){     // contar o sinal -
    count++;
  }

  while(n != 0){
    n /= 10;    // n = n/10
    count++;
  }

  return count;
}

long operate(long *stack, int i, const char* op){

  int it = 0;
  long res = 0;

  if (i > 0) {
    if (!strcmp(op, "avg")) {
      for (; it < i; it++)
        res += stack[it];
      res /= i;
    }
    else if (!strcmp(op, "sum"))
      for (; it < i; it++)
        res += stack[it];
    else if (!strcmp(op, "max")) {
      res = stack[0];
      for (it = 1; it < i; it++)
        if (stack[it] > res)
          res = stack[it];
    }
    else if (!strcmp(op, "min")) {
      res = stack[0];
      for (it = 1; it < i; it++)
        if (stack[it] < res)
          res = stack[it];
    }
  }

  return res;
}

int checkOP(const char* op) {

  return (!strcmp(op, "avg") || !strcmp(op, "max") || !strcmp(op, "min") || !strcmp(op, "sum"));
}

int read_line(int fd, char *buf, int count) {
  int x, endOfLine = 0, nbytes = 0;
  char c;
  while (!endOfLine && read(fd, &c, 1) == 1) {
    if (nbytes < count)
      buf[nbytes++] = c;
    if (c == '\n')
      endOfLine = 1;
  }
  buf[nbytes] = '\0';
  return nbytes;
}

/*
PIPE_BUF = 4096

argv[0] -> ./window
argv[1] -> número da coluna sobre a qual iremos fazer operações;
argv[2] -> operação - pode ser 'avg', 'max', 'min' ou 'sum';
argv[3] -> número máximo de linhas sobre o qual a operação se irá efetuar.

*/
int main(int argc, const char *argv[]) {

    int x, ins, i = 0;
    long valCol, res;
    int ncol = atoi(argv[1]);
    int nlines = atoi(argv[3]);
    const char* operation = argv[2];
    char buf[PIPE_BUF];
    long stack[nlines];

    if (argc != 4) {
      write(1, "Too many or not enough arguments passed to the program.\n", 56);
      return EXIT_FAILURE;
    }
    if (!checkOP(operation)) {
      write(1, "The operation is not supported by this program. Try 'avg', 'max', 'min' or 'sum'.\n", 82);
      return EXIT_FAILURE;
    }
    while (x = read_line(0, buf, PIPE_BUF)) {
      res = operate(stack, i, operation);
      if (x + countDigits(res) + 2 > PIPE_BUF) {
        write(1, "The length of the newly created line exceeds PIPE_BUF and, therefore, is too long.\n", 83);
        return EXIT_FAILURE;
      }
      ins = getVal(buf, ncol, &valCol);
      if (ins >= 0)
        i = stackOrg(stack, i, nlines, valCol);

      sprintf(buf+x-1, ":%ld\n", res);
      write(1, buf, strlen(buf));
    }

    return EXIT_SUCCESS;
}
