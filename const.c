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

int main(int argc, char** argv){

		int x;
		char buffer[PIPE_BUF];

		if (argc == 2) {
			while ((x = read_line(0,buffer,PIPE_BUF)) > 0) {
				if (argc == 2) {
					sprintf(buffer+x-1, ":%s\n", argv[1]);
					write(1,buffer,strlen(buffer));
			  }
		  }
    }
		else
			write(1, "Too many or very few arguments!\n", 32);

		return EXIT_SUCCESS;
}
