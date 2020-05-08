#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "commando.h"
// global variable to control echoing, 0: echo off, 1: echo on
int ECHO = 0;

// Print the given string if echoing is turned on
void echo(char *s){
  if(ECHO){
    printf("%s",s);
  }
}

void help(){
printf("COMMANDO COMMANDS\n");
printf("help               : show this message\n");
printf("exit               : exit the program\n");
printf("list               : list all jobs that have been started giving information on each\n");
printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
printf("output-for int     : print the output for given job number\n");
printf("output-all         : print output for all jobs\n");
printf("wait-for int       : wait until the given job number finishes\n");
printf("wait-all           : wait for all jobs to finish\n");
printf("command arg1 ...   : non-built-in is run as a job \n");
}

int main(int argc, char const *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering

  if(argc > 1 && strcmp("--echo",argv[1]) == 0) {
    ECHO=1;
  }

  char *env_echo = getenv("COMMANDO_ECHO");  // check environment var
  if(env_echo != NULL){
    ECHO=atoi(env_echo);
  }

  char cmd[MAX_LINE];
  char *tokens[ARG_MAX];					 // holds tokens to be made into commands
  int ntok;									 // stores the number of tokens
  cmdcol_t *col = malloc(sizeof(cmdcol_t));  // holds all commands
  col->size = 0;

  while(1){
    printf("@> ");                 		       // print prompt
    char *input = fgets(cmd, MAX_LINE, stdin); // read input from command-line
    if (input == NULL) {
      printf("\nEnd of input\n" );
      break;
    }
    echo(input);  							// print input, for testts

    input[strlen(input)-1] = '\0'; 			// Eliminate newline in buffer
    if(strlen(input) == 0){
      // return was hit, so skip to updating col commands
    } else {
      parse_into_tokens(cmd, tokens, &ntok); // Get tokens from command-line
	  // "exit" command
      if( strcmp("exit", tokens[0]) == 0 ){    // check for exit command
        break;                               // break from loop
      }
      // "help" command
      else if( strcmp("help", tokens[0]) == 0 ){
        help();
      }
      // "list" command
      else if( strcmp("list", tokens[0]) == 0 ){
        cmdcol_print(col);
      }
      // "puase" command
      else if( strcmp("pause", tokens[0]) == 0 ){
        char **ptr = NULL;   // needed for strtol
        pause_for(strtol(tokens[1], ptr, 10), atoi(tokens[2]));
      }
      // "output-for" command
      else if( strcmp("output-for", tokens[0]) == 0 ){
        cmd_t *cmd = col->cmd[atoi(tokens[1])];
        printf("@<<< Output for %s[#%d] (%d bytes): \n", cmd->name, cmd->pid, cmd->output_size);
        printf("---------------------------------------- \n");
        cmd_print_output(cmd);
        printf("---------------------------------------- \n");
      }
      // "output-all" command
      else if( strcmp("output-all", tokens[0]) == 0 ){
        for (int i = 0; i < col->size; i++) {
          cmd_t *cmd = col->cmd[i];
          printf("@<<< Output for %s[#%d] (%d bytes): \n", cmd->name, cmd->pid, cmd->output_size);
          printf("---------------------------------------- \n");
          cmd_print_output(cmd);
          printf("---------------------------------------- \n");
        }
      }
      // "wait-for" command
      else if( strcmp("wait-for", tokens[0]) == 0 ){
        cmd_update_state(col->cmd[atoi(tokens[1])], DOBLOCK);
      }
      // "wait-all" command
      else if( strcmp("wait-all", tokens[0]) == 0 ){
        cmdcol_update_state(col, DOBLOCK);
      }
      else {  // new command to be executed
        cmd_t *temp = NULL;
        temp = cmd_new(tokens);
        cmdcol_add(col, temp);
        cmd_start(col->cmd[col->size - 1]);
      }
    }

    cmdcol_update_state(col, NOBLOCK);  // update all commands

  } // end while-loop/commando-input

  //  free memory
  cmdcol_freeall(col);
  free(col);

  return 0;
}
