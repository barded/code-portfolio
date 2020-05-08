#include "blather.h"
#include <pthread.h>

simpio_t simpio_actual;
simpio_t *simpio = &simpio_actual;

client_t client_actual;
client_t *client = &client_actual;

pthread_t user_thread;          // thread managing user input
pthread_t background_thread;

// Worker thread to manage user input
void *user_worker(void *arg){
  while(!simpio->end_of_input){
    simpio_reset(simpio);
    iprintf(simpio, "");                                        // print prompt
    while(!simpio->line_ready && !simpio->end_of_input){        // read until line is complete
      simpio_get_char(simpio);
    }

    if(simpio->line_ready){                          // send data to server
  	  mesg_t message;
  	  memset(&message, 0, sizeof(mesg_t));
  	  message.kind = BL_MESG;
  	  strncpy(message.name, client->name, MAXNAME);
  	  strncpy(message.body, simpio->buf, MAXLINE);

      int bytes_written = write(client->to_server_fd, &message, sizeof(mesg_t));
      if (bytes_written <= 0) {
		  perror("Could not send message to server\n");
	  } else {
		  iprintf(simpio,"Message sent!");
	    }
    }
  }

  mesg_t messageD;
  memset(&messageD, 0, sizeof(mesg_t));
  messageD.kind = BL_DEPARTED;
  strncpy(messageD.name, client->name, MAXNAME);
  strncpy(messageD.body, "", MAXLINE);

  int bytes_written = write(client->to_server_fd, &messageD, sizeof(mesg_t));
  if (bytes_written <= 0) {
	  perror("Could not depart from server\n");
	  exit(1);
  }

  pthread_cancel(background_thread); // kill the background thread
  return NULL;
}

// Worker thread to listen to the info from the server.
void *background_worker(void *arg){
  while(1) {
	  mesg_t message;
	  int bytes_read = read(client->to_client_fd, &message, sizeof(mesg_t));
	  if (bytes_read <= 0) {
		  perror("Could not read from server\n");
		  exit(1);
	  }

	  // Process messages from server
	  if (message.kind == BL_MESG){
		  iprintf(simpio, "[%s] : %s\n", message.name, message.body);
	  } else if (message.kind == BL_JOINED){
		  iprintf(simpio, "-- %s JOINED --\n", message.name);
	  } else if (message.kind == BL_DEPARTED){
		  iprintf(simpio, "-- %s DEPARTED --\n", message.name);
	  } else if (message.kind == BL_SHUTDOWN){
		  iprintf(simpio, "!!! server is shutting down !!!\n");
		  pthread_cancel(user_thread);	// kill the user thread
		  break;
	  }
  }
  return NULL;
}

int main(int argc, char *argv[]){
  strncpy(client->name, argv[2], MAXNAME);
  char prompt[MAXNAME];
  strncpy(prompt, client->name, MAXNAME); // create a prompt string
  strcat(prompt, ">> ");

// Initialize the simplified terminal I/O
  simpio_set_prompt(simpio, prompt);         // set the prompt
  simpio_reset(simpio);                      // initialize io
  simpio_noncanonical_terminal_mode();       // set the terminal into a compatible mode

// Open server's join-FIFO
  char *fifoname = argv[1];
  strcat(fifoname, ".fifo");
  int join_fd = open(fifoname, DEFAULT_PERMS);
  if(join_fd == -1){
	  perror("Could not open server\n");
	  exit(1);
  }

// Initialize client
  // Make unique-client fifos and open them
  char pid[MAXPATH];
  sprintf(pid,"%d",getpid());
  strncpy(client->to_client_fname, pid, MAXPATH);
  strncpy(client->to_server_fname, pid, MAXPATH);

  strcat(client->to_client_fname, "_toClient.fifo");
  strcat(client->to_server_fname, "_toServer.fifo");

  remove(client->to_client_fname);
  remove(client->to_server_fname);
  mkfifo(client->to_client_fname, DEFAULT_PERMS);
  mkfifo(client->to_server_fname, DEFAULT_PERMS);

  client->to_client_fd = open(client->to_client_fname, DEFAULT_PERMS);
  client->to_server_fd = open(client->to_server_fname, DEFAULT_PERMS);

  if(client->to_client_fd == -1 || client->to_client_fd == -1) {
	  perror("Could not make/open client fifos\n");
	  exit(1);
  }
  // Set join flags
  client->data_ready = 0;

// Create and send join-request
  join_t join_request;
  memset(&join_request, 0, sizeof(join_t));
  strncpy(join_request.name, client->name, MAXPATH);
  strncpy(join_request.to_client_fname, client->to_client_fname, MAXPATH);
  strncpy(join_request.to_server_fname, client->to_server_fname, MAXPATH);
  int bytes_written = write(join_fd, &join_request, sizeof(join_t));
  if (bytes_written <= 0){
	  perror("Could not send join request\n");
	  exit(1);
  }
  close(join_fd);


// Create threads
  pthread_create(&user_thread,   NULL, user_worker,   NULL);     // start user thread to read input
  pthread_create(&background_thread, NULL, background_worker, NULL);
  pthread_join(user_thread, NULL);
  pthread_join(background_thread, NULL);

  close(client->to_client_fd);
  close(client->to_server_fd);
  remove(client->to_client_fname);
  remove(client->to_server_fname);
  simpio_reset_terminal_mode();
  printf("\n");                 // newline just to make returning to the terminal prettier
  return 0;
}
