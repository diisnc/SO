#include <stdlib.h>
#include <unistd.h>

/**
 * Lê uma linha até ao tamanho máximo 'count'(corresponderá a PIPE_BUF).
 */
 int read_line(int fd, char *buf, int count) {

	 int endOfLine = 0, nbytes = 0;
   char c;

	 while (!endOfLine && read(fd, &c, 1) == 1 ) {
     if (nbytes < count)
       buf[nbytes] = c;
     nbytes++;
     if (c == '\n')
       endOfLine = 1;
   }
	 buf[nbytes] = '\0';

	 return nbytes;
 }

 
 char* read_words(char *buffer){
 	char c;
 	char **string = malloc(sizeof(char)*10);
 	int i, j;

 	while(buffer[i] != '\n'){
 		for(i = 0, j =; buffer[i]!=' '; i++){
 		*string[i] = buffer[i];
 		}
 	*string[i] = '\0';
 	i++;
 	}

 	return *string;

}