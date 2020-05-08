#include "commando.h"
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

// cmdcol.c: functions related to cmdcol_t collections of commands.

void cmdcol_add(cmdcol_t *col, cmd_t *cmd){
// Add the given cmd to the col structure. Update the cmd[] array and
// size field. Report an error if adding would cause size to exceed
// MAX_CMDS, the maximum number commands supported.

  // Check for space in the cmdcol cmd array to add another cmd
  if (col->size + 1 > MAX_CMDS) {
    perror("Cannot add another cmd to col. At capacity.\n");
  } else {
    col->cmd[col->size] = cmd; 		// Store the command in cmdcol cmd array
    col->size += 1;					// Update the size of the cmdcol cmd array
  }
}

void cmdcol_print(cmdcol_t *col) {
// Print all cmd elements in the given col structure.  The format of
// the table is
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 0    #17434       0    EXIT(0) 2239 ls -l -a -F
// 1    #17435       0    EXIT(0) 3936 gcc --help
// 2    #17436      -1        RUN   -1 sleep 2
// 3    #17437       0    EXIT(0)  921 cat Makefile
//
// Widths of the fields and justification are as follows
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 1234  12345678 1234 1234567890 1234 Remaining
// left  left    right      right rigt left
// int   int       int     string  int string
//
// The final field should be the contents of cmd->argv[] with a space
// between each element of the array.

  // Format output
  printf("%-4s #%-8s %4s %10s %4s %-s\n","JOB","PID","STAT","STR_STAT","OUTB","COMMAND" );

  for (int i = 0; i < col->size; i++) {
    cmd_t *temp = col->cmd[i];				// Get the cmd from the cmdcol cmd array
    printf("%-4d #%-8d %4d %10s %4d",
      i, temp->pid, temp->status, temp->str_status, temp->output_size);

    int j = 0;
    while (temp->argv[j] != NULL) {           // Print the command
      printf(" %-s", temp->argv[j]);
      j++;
    }
    printf(" \n");  // the space is needed
  }
}


void cmdcol_update_state(cmdcol_t *col, int block){
// Update each cmd in col by calling cmd_update_state() which is also
// passed the block argument (either NOBLOCK or DOBLOCK)

  for (int i = 0; i < col->size; i++) {
    cmd_update_state(col->cmd[i], block);
  }
}

void cmdcol_freeall(cmdcol_t *col){
// Call cmd_free() on all of the constituent cmd_t's.

  for (int i = 0; i < col->size; i++) {
    cmd_free(col->cmd[i]);
  }

}
