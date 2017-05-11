#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
argv[0]=filter
argv[1]=coluna
argv[2]=operador
argv[3]=coluna

operadores: =, >=, <=, >, <, !=

nota: em vez de ler 256, tamanho vai ser PIPE_BUF

*/


int main(int argc, char** argv){


	if(argc == 4){

		while( (( x = read(0,buffer,256)) > 0)){
			int vc1 = elemIndex(); //valor coluna argv[1]
			int vc2 = elemIndex(); //valor coluna argv[3]

			if(atoi(&buffer[vc1]) < atoi(&buffer[vc2])){


				
			}

		}










	}
	
	else{
		write(1, "Too many or very few arguments!\n", 32);
		return EXIT_FAILURE;
	}


return EXIT_SUCCESS;
}