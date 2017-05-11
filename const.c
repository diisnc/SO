#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//argv[0] = const
//argv[1] = valor
//nao é preciso ciclo, depois o processor geral é q chama mts vezes


/*
void main(int argc, char **argv){ 
	int x, i=0;
	char buffer[256];

		do{
			x=read(0, &buffer[i], 1);
			i++;
		}
		while( x>0 && buffer[i-1]!='\n' );

		strcpy(&buffer[i-1], ":");
		strcat(buffer, argv[1]);
		strcat(buffer,"\n");
		write(1, buffer, strlen(buffer));
}
*/
int main(int argc, char** argv){
int x;
char buffer[256];

	if(argc == 2){
    	
		while( (( x = read(0,buffer,256)) > 0)){
			strcpy(&buffer[x-1], ":");
			strcat(buffer, argv[1]);
			strcat(buffer, "\n");

			write(1,buffer,strlen(buffer));
		
		}
	}

	else{
		write(1, "Too many or very few arguments!\n", 32);
		return EXIT_FAILURE;
	}

return EXIT_SUCCESS;
}
