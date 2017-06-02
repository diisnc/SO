#include <unistd.h> 	/* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "auxs.h"

/*
argv[0] = const
argv[1] = valor
*/

int read_line(int fd, char *buf, int count) {

    int endOfLine = 0, nbytes = 0;
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

int main(int argc, char** argv){

		int x;
		char buffer[PIPE_BUF];

		if (argc == 2) {
			while ((x = read_line(0,buffer,PIPE_BUF)) > 0) {
<<<<<<< HEAD
				if (argc == 2) {
					sprintf(buffer+x-1, ":%s\n", argv[1]);
=======
				
					strcpy(&buffer[x-1], ":");
					strcat(buffer, argv[1]);
					strcat(buffer, "\n");
>>>>>>> 04b015994e719a731adc5a808b7aecb35ee92963
					write(1,buffer,strlen(buffer));
				
			}
		}
		else
			write(1, "Too many or very few arguments!\n", 32);

		return EXIT_SUCCESS;
}
