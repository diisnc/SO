#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "auxs.h"

int elemIndexInicial(char *buffer, int coluna){
    int two_dots = 0, i;
    int tam = strlen(buffer);


    for( i = 0; i < tam && (two_dots < (coluna-1) ) ; i++){
                
        if(buffer[i] == ':') 
            two_dots++;
    }

    return (i == tam) ? -1 : i;
}

char* elem_column(char *buffer, int tamanho, int index){

    int first_index = elemIndexInicial(buffer, index);
    char guarda_elem[256];
    int i = 0;
    
    //if(first_index == -1)
      //  return -1;

    while( buffer[first_index]!=':'  &&  buffer[first_index]!='\n'  &&  buffer[first_index]!='\0' ){

        guarda_elem[i] = buffer[first_index];
        i++;
        first_index++;
    }

    guarda_elem[i]='\0';

    //int elemfinal = atoi(guarda_elem);
    //return elemfinal;
    return strdup(guarda_elem);
}

int main(int argc, char **argv)
{
    char buffer[256];
    int ins[100];
    char args[100][100];
    int i=1;
    int j=0;
    int k;
    int tamanho;
    int index_col;//= atoi(argv[1]);
    //printf("yyy1\n");
    //printf("%s ",argv[1]);
    while(i<argc){
        if(argv[i][0]=='$'){ins[j]=atoi(argv[i]+1);j++;}//printf("%s ",argv[i]+1);
        i++;
        
    }
    //printf("yfghyy2\n");
    while((( tamanho = read(0,buffer,256)) > 0)){
        i=0;
        while(i<j){
            char* elem = elem_column(buffer, tamanho, ins[i]); //conteudo coluna argv[1]
            printf("%s\n",elem );
            //printf("222222222\n");
            strcpy(args[i],elem);
            //printf("333333333\n");
            i++;
        }
        //printf("444444444\n");
        i=0;
        k=0;
        while(i<argc){
            //printf("555555555\n");
            if(argv[i][0]=='$'){argv[i]=args[k++];}
            //printf("qwertyui\n");
            i++;
        }
        //printf("666666666\n");
        i=0;
        //chat argv2[100][100];
        while(i<argc){
            printf("%s ",argv[i]);
            i++;
        }
        printf("\n");
        int r;
        int x;
        if((x=fork())==0){
            x = execvp(argv[1],&argv[1]);
            exit(x);
        }

        write(1,buffer,WEXITSTATUS(r));

    }

    /*

    
//EX5-FICHA3
void main(int argc, char** argv){
int x, r;

for(int i = 1; i < argc; i++){
    if( (x=fork()) == 0 ){
        printf("Criou o filho nmr %d\n", i);
        execlp(argv[i], argv[i], NULL);
        exit(i);
    }
}

while(wait(&r)>0)//retorna -1 se n tiver sucesso, valor se for id do filho q acabou
    printf("MENOS UM FILHO!! numero dele: %d", WEXITSTATUS(r));
//é suposto dar sempre zero no codigo de saida, pq se faz o exec,
//vai fazer o exit
}


    */
    
    printf("yyy3\n");
    return 0;
}
