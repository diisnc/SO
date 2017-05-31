#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "comuns.c"

//vars globais
int nodes[20];//dps altera-se
int connects[20][20];
//(...)




//faz um fork,este fork faz 3 forks, um para o read, outro para o
//write, outro para os execs e dups (ver imagem)

//estao todos os closes feitos?
//casos de exceçao - fazer ainda
int node(int node_ID, char* node_ID1, char* cmd, char** args){
	int id_filho;
	int id_neto_read;
	int id_neto_write;
	int id_neto_exec;

	//------teste, apagar depois------
	printf("Nodo %d criado com as componentes %s , e argumentos ", node_ID, cmd);
	for(int i = 0; args[i]; i++)
		printf("%s", args[i]);
	printf("\n");
	//-------------fim teste-----------


	if( (id_filho = fork()) == 0){ //pai faz um filho apenas, isto é o node
		mkfifo( strcat("fifo_in", node_ID1) , 666); //fifo de entrada
		mkfifo( strcat("fifo_out", node_ID1) , 666); //fifo de saida
		int fd_in[2]; //pipe anonimo
		int fd_out[2]; //pipe anonimo
		pipe(fd_in);
		pipe(fd_out);


		/*---criaçao dos 3 filhos----*/
		int read_id = fork();
		int write_id = fork();
		int dupexec_id = fork();
		/*--------------------------*/


		if(!read_id){//fork de leitura
			close(fd_in[0]); //fechar pipe anon p leitura. vou
						  //ler apenas e depois escrever
						  //para pipe anon


			int fich_i = open( strcat("fifo_in", node_ID1) , O_RDONLY);//abrir fifo de entrada
			char buffer[PIPE_BUF];
			int x;
			while( (x=read_line(fich_i, buffer, 1)) > 0)
				write(fd_in[1], buffer, x); //escrever o q leu

		}

		else{

			if(!dupexec_id){
				dup2(fd_in[0], 0);
				dup2(fd_out[1], 1);
				execv(cmd, args); //testar este exec
			}

			else{

				if(!write_id){
					close(fd_out[1]);//fechar pipe anon p escrita
							//só vou ler de lá p depois fazer write p
							//pipe de saida
					int fich_o = open(strcat("fifo_out", node_ID1) , O_WRONLY);
					char buffer[PIPE_BUF];
					int x;
					while( (x=read_line(fd_out[0], buffer, 1)) > 0)
						write(fich_o, buffer, x); //escrever o q leu
				}
			}
		}
	}


	else{ //pai nao faz qs nada
		//wait(); n é preciso
		nodes[node_ID] = id_filho;//vai ser preciso guardar a info de todos os nodos q temos no momento
									//por array ou depois no return
	}

	return 0; //pai vai retornar id dele mesmo(1, 2, 3, ...)
}



/*
Sempre q houver um connect entre 2 nodos, vai haver um "intermediário"
ou seja, um pipe a ligar(no caso de 2 nodos) a saida do primeiro
À entrada do segundo (node). só fiz isto para no caso de haver uma ligaçao de
mts nodos, isto vai ser preciso.
Depois, no disconnect, é só fazer close? do intermediario e criar outro c
menos ligacoes


*/

//Para já, só está o caso q conectamos um a outro !
//p mais, fazer como na "node", por nomes nos pipes com
//os id's ou qualquer coisa assim -ESQUECER, CONECTOR VAI SER
//SEMPRE O MESMO !!
void connect(char* node_ID1, char* node_ID2){

	//fazer tipo um if nao houver conecçao para efeitos de verificaçao, verificar no array connects

	int fdi = open( strcat("fifo_out", node_ID1), O_RDONLY);//abrir saida do primeiro(read)
	int fdo = open( strcat("fifo_in", node_ID2), O_WRONLY);//mandar para entrada do segundo(write)

	mkfifo("conector", 666);
	int fdc = open("conector", O_RDONLY | O_WRONLY);

	int x; char buffer[PIPE_BUF];
	while( (x=read_line(fdi, buffer , 1)) > 0 ){
		write(fdc, buffer , x);
	}

	while( (x=read_line(fdc, buffer , 1)) > 0 ){
		write(fdo, buffer, x);
	}

	connects[atoi(node_ID1)][atoi(node_ID2)] = 1; //pode n ser preciso
}




void disconnect(){}
void inject(){}

int main(int argc, char **argv){
	char buffer[PIPE_BUF];
	char *args[10]; //se isto n chegar , depois aumentar
	int tam;

	//inicializar os arrays globais a 0 ou -1(fazer)

	while(read_line(0, buffer, PIPE_BUF) > 0){ //le um comando de cada vez
		*args = read_words(buffer);
		//for(int i = 0; i < 10; i++)
		//printf("%s", args[i]);




		if( (strcmp(args[0],"node")) == 0 ){

			//estou a passar 2x o args[1] id do node pq vou precisar, ver funcao
			node(atoi(args[1]),args[1], args[2], &args[3]); //&args[3] funciona???
			//args[0] node
			//args[1] id do node
			//args[2] filter (por exemplo)
			//a partir de arg[3] estao os argumentos


		}

		else

			if( (strcmp(args[0],"connect")) == 0 ){}

		else

			if( (strcmp(args[0],"disconnect")) == 0 ){}

		else

			if( (strcmp(args[0],"inject")) == 0 ){}




		}

return EXIT_SUCCESS;


}
