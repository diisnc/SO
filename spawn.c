#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


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
    while( buffer[first_index]!=':'  &&  buffer[first_index]!='\n'  &&  buffer[first_index]!='\0' ){
        guarda_elem[i] = buffer[first_index];
        i++;
        first_index++;
    }

    guarda_elem[i]='\0';
    return strdup(guarda_elem);
}

char* concat(const char *s1, const char *s2)
{
    char *result =(char*) malloc(strlen(s1)+strlen(s2)+1);

    strcpy(result, s1);
    strcat(result, s2);
    return result;
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

    while(i<argc){
        if(argv[i][0]=='$'){ins[j]=atoi(argv[i]+1);j++;}
        i++;
    }
    while((( tamanho = read(0,buffer,256)) > 0)){
        i=0;
        while(i<j){
            char* elem = elem_column(buffer, tamanho, ins[i]); //conteudo coluna argv[1]
            strcpy(args[i],elem);
            i++;
        }
        i=0;
        k=0;
        while(i<argc){
            if(argv[i][0]=='$'){argv[i]=args[k++];}
            i++;
        }
        i=0;
        printf("\n");
        int x;
        char* output = (char*) malloc(256);
        if((x=fork())==0){
            x = execvp(argv[1],&argv[1]);
            exit(x);
        }
        int exitStatus;
        int result;
        while (wait(&exitStatus) > 0) {
            result = WEXITSTATUS(exitStatus);
        }

        char* res = concat(buffer,":0");
        //res = concat(res,result);

        write(1, res, strlen(res));
    }

    //printf("yyy3\n");
    return EXIT_SUCCESS;
}
