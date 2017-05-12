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

nota: em vez de ler 256, tamanho vai ser PIPE_BUF(1024)

*/

//------------------------------------------------------------------------------------

/*

Função que dá o indice da coluna pedida(1º indice)

*/
int elemIndexInicial(char *buffer, int coluna){
	int two_dots = 0, i;
	int tam = strlen(buffer);


	for( i = 0; i < tam && (two_dots < (coluna-1) ) ; i++){
				
		if(buffer[i] == ':') 
			two_dots++;
	}

	return (i == tam) ? -1 : i;
}

//------------------------------------------------------------------------------------

/*

Função que dado o indice da coluna pedida, dá o elemento que queremos
para depois comparar

*/
int elem_column(char *buffer, int tamanho, int index){

	int first_index = elemIndexInicial(buffer, index);
	char guarda_elem[256];
	int i = 0;
	
	if(first_index == -1)
		return -1;

	while( buffer[first_index]!=':'  &&  buffer[first_index]!='\n'  &&  buffer[first_index]!='\0' ){

		guarda_elem[i] = buffer[first_index];
		i++;
		first_index++;
	}

	guarda_elem[i]='\0';

	int elemfinal = atoi(guarda_elem);
	return elemfinal;
}

//-------------------------------------------------------------------------------------

int main(int argc, char** argv){
char buffer[256];

	if(argc == 4){
		int tamanho;
		int index_col1= atoi(argv[1]);
		int index_col2= atoi(argv[3]);
		char* op = argv[2];


		if( index_col1 <=0 || index_col2 <= 0 ){
			write(1, "Columns numbers can't be zero or less!\n", 38);
			return EXIT_FAILURE;
		}

		
		if( (strcmp(op, "<")!=0) && (strcmp(op, ">")!=0) && (strcmp(op, "<=")!=0) && (strcmp(op, ">=")!=0) && (strcmp(op, "=")!=0) && (strcmp(op, "!=")!=0) ){
			write(1, "Invalid operator!\n", 18);
			return EXIT_FAILURE;
		}



		while( (( tamanho = read(0,buffer,256)) > 0)){
			int elem1 = elem_column(buffer, tamanho, index_col1); //conteudo coluna argv[1]
			int elem2 = elem_column(buffer, tamanho, index_col2); //conteudo coluna argv[3]

			if(elem1 == -1 || elem2 == -1){
				write(1, "There are less columns in the input than the ones you entered initially.\n", 73);
				return EXIT_FAILURE;
			}



			//a ação começa aqui, até aqui foram quase só verificações


			if(!strcmp(op, "<")){

				if(elem1 < elem2)
					write(1, buffer, tamanho);
			}


			if(!strcmp(op, ">")){

				if(elem1 > elem2)
					write(1, buffer, tamanho);
			}


			if(!strcmp(op, "<=")){

				if(elem1 <= elem2)
					write(1, buffer, tamanho);
			}


			if(!strcmp(op, ">=")){

				if(elem1 >= elem2)
					write(1, buffer, tamanho);
			}


			if(!strcmp(op, "=")){

				if(elem1 == elem2)
					write(1, buffer, tamanho);
			}


			if(!strcmp(op, "!=")){

				if(elem1 != elem2)
					write(1, buffer, tamanho);
			}
		}
	}
	
	else{
		write(1, "Too many or very few arguments!\n", 32);
		return EXIT_FAILURE;
	}


return EXIT_SUCCESS;
}