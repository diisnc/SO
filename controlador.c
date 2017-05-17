#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "comuns.c"

void node(){
//cria 2 filhos

}
void connect(){}
void disconnect(){}
void inject(){}

int main(int argc, char **argv){
	char buffer[PIPE_BUF];
	char *args[10]; //se isto n chegar , depois aumentar
	int tam;



	while((tam = read_line(0, buffer, PIPE_BUF)) > 0){
		*args = read_words(buffer);
		for(int i = 0; i < 10; i++)
		printf("%s", args[i]);



		
		if( strcmp(args[0],"node") ){
			//node();


		}
		
		else	
		
			if( strcmp(args[0],"connect") ){}
		
		else	
		
			if( strcmp(args[0],"disconnect") ){}
		
		else
		
			if( strcmp(args[0],"inject") ){}	






return EXIT_SUCCESS;
}

}