#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * Lê uma linha até ao tamanho máximo 'count'(corresponderá a PIPE_BUF).
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

/**
  * Recebe um char* correspondente à linha passada ao programa (evento) e devolve um char**, do qual cada char* corresponde a uma palavra.
  */
char** parse_cmd(char *buffer) {

 	  char **words = malloc(sizeof(char*) * 10);  /*Array que guarda os endereços das palavras.*/
    char auxword[40];
    char *word;
    int i = 0, j , k = 0;

 	  while (buffer[i] != '\n') {
 		  for (j = 0; buffer[i] > 32; j++, i++)     /*buffer[i] ser maior que 32 significa que o caracter não é um caracter de controlo, ou seja, não é espaço (32), nem newline (12).*/
 		    auxword[j] = buffer[i];                 /*A palavra no buffer é copiada para um array estático auxiliar.*/
 	    auxword[j] = '\0';                        /*A palavra é dada como terminada ao ser encontrado um caracter com código <= 32.*/
      word = strdup(auxword);                   /*A palavra é copiada para espaço alocado na memória.*/
      words[k] = word;                          /*É guardado o endereço da palavra no array de palavras.*/
 	    k++;                                      /*É incrementado o índice no array de palavras guardadas.*/
 	  }
    words[k] = NULL;                            /*Marca-se o fim do array de palavras com um NULL pointer.*/

 	  return words;                               /*Devolve-se o endereço das palavras guardadas.*/
}
