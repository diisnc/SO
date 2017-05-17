#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "comuns.c"
/*
argv[0] = const
argv[1] = valor
nota: em vez de ler 256, tamanho vai ser PIPE_BUF
*/
int main(int argc, char** argv){

		int x;
		char buffer[PIPE_BUF];

		//if (argc == 2) {

			while (((x = read_line(0,buffer,PIPE_BUF)) > 0)) {
				
				if(argc == 2){

				strcpy(&buffer[x-1], ":");
				strcat(buffer, argv[1]);
				strcat(buffer, "\n");

				write(1,buffer,strlen(buffer));
				}
			}
		//}

		//else {
			//write(1, "Too many or very few arguments!\n", 32);
			//return EXIT_FAILURE;
		//}

		return EXIT_SUCCESS;
}
