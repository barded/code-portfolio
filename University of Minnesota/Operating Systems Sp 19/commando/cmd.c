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

// cmd.c: functions related the cmd_t struct abstracting a
// command. Most functions maninpulate cmd_t structs.

cmd_t *cmd_new(char *argv[]){
// Allocates a new cmd_t with the given argv[] array. Makes string
// copies of each of the strings contained within argv[] using
// strdup() as they likely come from a source that will be
// altered. Ensures that cmd->argv[] is ended with NULL. Sets the name
// field to be the argv[0]. Sets finished to 0 (not finished yet). Set
// str_status to be "INIT" using snprintf(). Initializes the remaining
// fields to obvious default values such as -1s, and NULLs.

  cmd_t *cmd = malloc(sizeof(cmd_t));	// allocate memory for new command
  int i=0;

  while (argv[i] != NULL ) {
    cmd->argv[i] = strdup(argv[i]);		// copy the command-line into the cmd data structure
    i++;
    if (i >= ARG_MAX + 1) {
      perror("Too many args in cmd_new\n");
      return NULL;
    }
  }

  cmd->argv[i] = NULL;					// create an identifiable termination to the string

  if (cmd->argv[i] != NULL) {
    printf("argv doesn't end with NULL, its %s\n", cmd->argv[i-1]);
    return NULL;
  }

  // Fill in the cmd data structure
  strcpy(cmd->name, cmd->argv[0]);
  snprintf(cmd->str_status, STATUS_LEN + 1, "INIT");
  cmd->pid = -1;
  cmd->out_pipe[0] = -1;
  cmd->out_pipe[1] = -1;
  cmd->finished = 0;
  cmd->status = -1;
  cmd->output = NULL;
  cmd->output_size = -1;

  return cmd;
}

void cmd_free(cmd_t *cmd){
// Deallocates a cmd structure. Deallocates the strings in the argv[]
// array. Also deallocats the output buffer if it is not
// NULL. Finally, deallocates cmd itself.

  int i = 0;
  while (cmd->argv[i] != NULL) {
    free(cmd->argv[i]);
    i++;
  }

  if (cmd->output != NULL) {
    free(cmd->output);
  }

  free(cmd);
}

void cmd_start(cmd_t *cmd){
// Forks a process and starts executes command in cmd in the process.
// Changes the str_status field to "RUN" using snprintf().  Creates a
// pipe for out_pipe to capture standard output.  In the parent
// process, ensures that the pid field is set to the child PID. In the
// child process, directs standard output to the pipe using the dup2()
// command. For both parent and child, ensures that unused file
// descriptors for the pipe are closed (write in the parent, read in
// the child).

  int pipe_result = pipe(cmd->out_pipe);  // create a pipe

  // Check that pipe opened correctly
  if(pipe_result != 0) {
    perror("Failed to create pipe");
    exit(1);
  }

  snprintf(cmd->str_status, STATUS_LEN+1, "RUN");

  // Fork a child command
  cmd->pid = fork();

  // Check that fork was successful
  if (cmd->pid < 0) {
    perror("Failed to fork");
    exit(1);
  }

  // CHILD CODE
  if (cmd->pid == 0) {
    dup2(cmd->out_pipe[PWRITE], STDOUT_FILENO);    // redirect stdout to pipe

    close(cmd->out_pipe[PREAD]);                  // disables the child's ability
												  // to read from the pipe

    execvp(cmd->name, cmd->argv);  //  execute command
    exit(errno);   // only executes if execvp fails
  } else{
    // PARENT CODE
    close(cmd->out_pipe[PWRITE]);			 	  // disables the parent's ability
												  // to write to the pipe
  }

}

void cmd_update_state(cmd_t *cmd, int block){
// If the finished flag is 1, does nothing. Otherwise, updates the
// state of cmd.  Uses waitpid() and the pid field of command to wait
// selectively for the given process. Passes block (one of DOBLOCK or
// NOBLOCK) to waitpid() to cause either non-blocking or blocking
// waits.  Uses the macro WIFEXITED to check the returned status for
// whether the command has exited. If so, sets the finished field to 1
// and sets the cmd->status field to the exit status of the cmd using
// the WEXITSTATUS macro. Calls cmd_fetch_output() to fill up the
// output buffer for later printing.
//
// When a command finishes (the first time), prints a status update
// message of the form
//
// @!!! ls[#17331]: EXIT(0)
//
// which includes the command name, PID, and exit status.

  if (!cmd->finished) {  // if not finished
    int status;
    int waitret = waitpid(cmd->pid, &status, block);

    if (waitret == -1) {   // check if error
      perror("Wait pid error.");
      exit(1);
    } else if (waitret>0) {  // status changed
      if (WIFEXITED(status)) {
        cmd->status = WEXITSTATUS(status);
        cmd->finished = 1;
        snprintf(cmd->str_status,STATUS_LEN + 1, "EXIT(%d)",cmd->status);
        cmd_fetch_output(cmd);
        printf("@!!! %s[#%d]: %s\n", cmd->name, cmd->pid, cmd->str_status);
      }
    } else {
      return;  // child not finished, do nothing
    }
  }
}

char *read_all(int fd, int *nread){
// Reads all input from the open file descriptor fd. Stores the
// results in a dynamically allocated buffer which may need to grow as
// more data is read.  Uses an efficient growth scheme such as
// doubling the size of the buffer when additional space is
// needed. Uses realloc() for resizing.  When no data is left in fd,
// sets the integer pointed to by nread to the number of bytes read
// and return a pointer to the allocated buffer. Ensures the return
// string is null-terminated. Does not call close() on the fd as this
// is done elsewhere.

  int cur_pos = 0;           // initial position in input buffer
  int max_size = 1024;         // initial size of input buffer
  char *input = malloc(max_size * sizeof(char));    // allocate memory

  while(1){                                                  // loop until end of input is indicated
    if(cur_pos >= max_size){                                 // input buffer full
      max_size = max_size * 2;                               // double in size
      input = realloc(input, max_size);                      // reallocate buffer to larger size

      if(input == NULL){                                     // check for failure
        printf("Could not expand input buffer.\n");
        exit(1);
      }
    }

    int max_read = max_size - cur_pos;    // calculate maximum read size
    int nread = read(fd, input + cur_pos, max_read); // perform read()

    if(nread == 0){     // 0 bytes read indicates end of file/input
      break;
    }
    else if(nread == -1){
      perror("read failed");
      exit(1);
    }

    cur_pos += nread;   // succesful read, advance input buffer position
  }

  *nread = cur_pos;
  input[cur_pos] = '\0';

  return input;
}

void cmd_fetch_output(cmd_t *cmd){
// If cmd->finished is zero, prints an error message with the format
//
// ls[#12341] not finished yet
//
// Otherwise retrieves output from the cmd->out_pipe and fills
// cmd->output setting cmd->output_size to number of bytes in
// output. Makes use of read_all() to efficiently capture
// output. Closes the pipe associated with the command after reading
// all input.

  // Check that the command finished
  if (cmd->finished == 0) {
    printf("%s[#%d] not finished yet\n", cmd->name, cmd->pid);
  } else {
    int n = -1;
    cmd->output = read_all(cmd->out_pipe[PREAD], &n); // Read output from pipe
    close(cmd->out_pipe[PREAD]);                      // Close the reading end of
													  // the out-pipe

    cmd->output_size = n;
  }
}


void cmd_print_output(cmd_t *cmd){
// Prints the output of the cmd contained in the output field if it is
// non-null. Prints the error message
//
// ls[#17251] : output not ready
//
// if output is NULL. The message includes the command name and PID.

  // Command is not finished/output is not initialized
  if (cmd->output==NULL) {
    printf("%s[#%d] : output not ready\n", cmd->name, cmd->pid);
  } else {

    int bytes_written = write(STDOUT_FILENO, cmd->output, cmd->output_size);

    if(bytes_written == -1){                              // check for errors
      perror("Failed to write to STDOUT_FILENO\n");
      exit(1);
    }
  }

}
