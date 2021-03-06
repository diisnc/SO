#include <unistd.h>     /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h>   /* chamadas wait*() e macros relacionadas */
#include <fcntl.h>      /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "auxs.h"

void node(char** args, pid_t nodes[]) {

    char in_node[28];
    char out_node[28];
    int fd_rd_in;
    int fd_wr_in;
    int fd_rd_out;
    int fd_wr_out;
    pid_t node_proc;
    pid_t rd_proc;
    pid_t exec_proc;
    ssize_t br;             /*br -> bytes read*/
    int feed_exec_fd[2];    /*Array para os file descriptors do pipe anónimo que irá alimentar o processo de execução.*/
    int out_exec_fd[2];     /*Array para os file descriptors do pipe anónimo que irá devolver o resultado da execução para que este possa sair do nodo.*/
    pipe(feed_exec_fd);
    pipe(out_exec_fd);
    char rd_wr_buf[PIPE_BUF];
    //int err;

    strcpy(in_node, "fifo_in_");
    strcat(in_node, args[1]);
    /*printf("%s\n", in_node);*/

    strcpy(out_node, "fifo_out_");
    strcat(out_node, args[1]);
    /*printf("%s\n", out_node);*/

    mkfifo(in_node, 0666);
    mkfifo(out_node, 0666);

    node_proc = fork();                                                /*Criação de um filho do processo principal, que será o processo correspondente ao nodo.*/
    if (node_proc == 0) {                                               /*Sendo 0, estamos no processo filho do nodo criado.*/
      rd_proc = fork();                                                /*Criação de um processo de leitura dos dados que chegam ao node.*/
      if (rd_proc == 0) {                                           	 /*Sendo 0, estamos no processo filho de leitura criado.*/
        fd_rd_in = open(in_node, O_RDONLY);    		 /*Abre o named pipe de entrada para leitura.*/
        while ((br = read_line(fd_rd_in, rd_wr_buf, PIPE_BUF)) >= 0) {   /*A leitura dá-se quando o número de bytes lidos é maior ou igual a 0 de modo a que a rede não pare.*/
          if (br > 0)                                               	 /*Se for maior do que 0, estamos perante um evento, e dá-se uma execução.*/
            write(feed_exec_fd[1], rd_wr_buf, br);
        }
      }
      else if (rd_proc > 0) {                                 /*Sendo maior do que 0, estamos no processo pai, correspondente ao processo do nodo.*/
        exec_proc = fork();                                   /*Criação de um processo de execução de um comando que chegou ao node.*/
        if (exec_proc == 0) {                                 /*Sendo 0, estamos no processo filho de execução criado.*/
          dup2(feed_exec_fd[0], 0);
          dup2(out_exec_fd[1], 1);
          if (!strcmp(args[2], "const") || !strcmp(args[2], "filter") || !strcmp(args[2], "window") || !strcmp(args[2], "spawn"))
            execv(args[2], args+2);
          else
            execvp(args[2], args+2);
        }
        else if (exec_proc > 0) {                             /*Sendo maior do que 0, estamos ainda no processo pai, correspondente ao processo do nodo.*/
          fd_wr_out = open(out_node, O_WRONLY);  		 /*Abre o named pipe de saída para escrita.*/
          while ((br = read_line(out_exec_fd[0], rd_wr_buf, PIPE_BUF)) > 0)
            write(fd_wr_out, rd_wr_buf, br);
        }
      }
    }
    else
      nodes[atoi(args[1])] = node_proc;
}

void connect(char *out, char *in, pid_t connections[512][512]) {

    ssize_t br;
    pid_t connection;
    char in_node[28];
    char out_node[28];
    char connection_buffer[PIPE_BUF];

    strcpy(out_node, "fifo_out_");
    strcat(out_node, out);

    strcpy(in_node, "fifo_in_");
    strcat(in_node, in);


    connection = fork();
    if (connection == 0) {  /*Estamos no processo filho, que será responsável por criar uma ligação entre nodos.*/
      int fd_rd_out = open(out_node, O_RDONLY);  /*Abre o named pipe de saída para leitura.*/
      int fd_wr_in = open(in_node, O_WRONLY);    /*Abre o named pipe de entrada para escrita.*/

      while ((br = read_line(fd_rd_out, connection_buffer, PIPE_BUF)) > 0) {
        write(fd_wr_in, connection_buffer, br);
      }
    }
    else {
      connections[atoi(out)][atoi(in)] = connection;
    }
}

void disconnect(char** args, pid_t connections[512][512]) {

    pid_t to_kill;
    int id_1 = atoi(args[1]);
    int id_2 = atoi(args[2]);

    if (to_kill = connections[id_1][id_2]) {
      kill(to_kill, SIGTERM);   /*SIGTERM (15) - pede ao processo que termine. Poderia ser SIGKILL (9) que acaba logo com tudo.*/
      /*waitpid(to_kill, NULL, WEXITED)*/     /*Para termos a certeza de que o processo morreu. Com SIGKILL não seria necessário.*/
      connections[id_1][id_2] = 0;
    }
}

void inject(char** args) {

    pid_t exec;
    int exec_com[2];
    pipe(exec_com);
    ssize_t br;
    char buffer[PIPE_BUF];
    char injection_node[28];

    strcpy(injection_node, "fifo_in_");
    strcat(injection_node, args[1]);

    int injection_fd = open(injection_node, O_WRONLY);

    exec = fork();
    if (exec == 0) {      /*Aqui estaremos no processo filho, que irá executar um dado programa.*/
      dup2(exec_com[1], 1);
      if (!strcmp(args[2], "const") || !strcmp(args[2], "filter") || !strcmp(args[2], "window") || !strcmp(args[2], "spawn"))
        execv(args[2], args+2);
      else
        execvp(args[2], args+2);
    }
    else if (exec > 0) {  /*Aqui estaremos no processo pai, que ficará de ler do pipe anónimo onde o filho escreverá.*/
      while ((br = read_line(exec_com[0], buffer, PIPE_BUF)) > 0) {
        write(injection_fd, buffer, br);
      }
    }
}

/*prctl(PR_SET_PDEATHSIG, SIGTERM); Para matar os filhos todos matando o pai.*/

int main(int argc, char const *argv[]) {

    pid_t nodes[512] = {0};
    pid_t connections[512][512] = {0};
    char mainbuffer[PIPE_BUF] = {0};
    char **args;
    int i;
    /*printf("%d\n", nodes[28]);*/
    /*printf("%d\n", connections[28][28]);*/

  	int config_file_fd = open("config.txt", O_RDONLY);

    while (read_line(config_file_fd, mainbuffer, PIPE_BUF) > 0) {
      args = parse_cmd(mainbuffer);
      if (!strcmp(args[0], "node"))
        node(args, nodes);
      else if (!strcmp(args[0], "connect")) {
        for (i = 2; args[i]; i++) {
          connect(args[1], args[i], connections);
        }
      }
      else if (!strcmp(args[0], "disconnect"))
        disconnect(args, connections);
      else if (!strcmp(args[0], "inject"))
        inject(args);
    }

    while (read_line(0, mainbuffer, PIPE_BUF) > 0) {
      args = parse_cmd(mainbuffer);
      if (!strcmp(args[0], "node"))
        node(args, nodes);
      else if (!strcmp(args[0], "connect")) {
        for (i = 2; args[i]; i++) {
          connect(args[1], args[i], connections);
        }
      }
      else if (!strcmp(args[0], "disconnect"))
        disconnect(args, connections);
      else if (!strcmp(args[0], "inject"))
        inject(args);
    }

    return 0;
}
