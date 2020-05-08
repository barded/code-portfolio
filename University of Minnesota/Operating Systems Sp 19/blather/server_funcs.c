#include "blather.h"
#include <string.h>
#include <sys/time.h>

client_t *server_get_client(server_t *server, int idx) {
// Gets a pointer to the client_t struct at the given index. If the
// index is beyond n_clients, the behavior of the function is
// unspecified and may cause a program crash.

	if(idx > server->n_clients-1){
		return NULL;
	} else {
		return &server->client[idx];
	}
}

void server_start(server_t *server, char *server_name, int perms) {
// Initializes and starts the server with the given name. A join fifo
// called "server_name.fifo" should be created. Removes any existing
// file of that name prior to creation. Opens the FIFO and stores its
// file descriptor in join_fd._

	char fifo_name[MAXPATH];
	strncpy(fifo_name, server_name, MAXPATH);
	server->n_clients = 0;
	server->join_ready = 0;

	strcat(fifo_name,".fifo");
	strncpy(server->server_name, server_name, MAXPATH);

	remove(fifo_name);                      //  remove existing fifo if there is one
	mkfifo(fifo_name, DEFAULT_PERMS);       //   create the new server fifo

	server->join_fd = open(fifo_name, DEFAULT_PERMS);

	if (server->join_fd == -1) {
		perror("Could not initialize the join fifo\n");
		exit(1);
	}

// ADVANCED: create the log file "server_name.log" and write the
// initial empty who_t contents to its beginning. Ensure that the
// log_fd is position for appending to the end of the file. Create the
// POSIX semaphore "/server_name.sem" and initialize it to 1 to
// control access to the who_t portion of the log.

}

void server_shutdown(server_t *server) {
// Shut down the server. Close the join FIFO and unlink (remove) it so
// that no further clients can join. Send a BL_SHUTDOWN message to all
// clients and proceed to remove all clients in any order.

	char fifo_name[MAXPATH];
	if (server->server_name != NULL) {
		strncpy(fifo_name, server->server_name, MAXPATH);
	} else {
		perror("server_name is NULL\n");
		exit(1);
	}

	strcat(fifo_name, ".fifo");
  int close_ret = close(server->join_fd);   //    close server join fifo
	if (close_ret == -1) {
		perror("failed to close server join fd\n");
	}

	int remove_ret = remove(fifo_name);                         //    remove fifo
	if (remove_ret == -1 ) {
		perror("failed to remove server fifo\n");
	}

	// SEND BL_SHUTDOWN
	mesg_t message;
	memset(&message, 0, sizeof(mesg_t));
	message.kind = BL_SHUTDOWN;
	int ret_val = server_broadcast(server, &message);      // broadcast shutdown to clients
	if (ret_val != 0) {
		perror("Problem with server_broadcast\n");
	}

	for (int i = server->n_clients - 1; i >= 0; i--) {   //  remove all clients
		server_remove_client(server, i);
	}
// ADVANCED: Close the log file. Close the log semaphore and unlink
// it.

}

int server_add_client(server_t *server, join_t *join){
// Adds a client to the server according to the parameter join which
// should have fileds such as name filed in.  The client data is
// copied into the client[] array and file descriptors are opened for
// its to-server and to-client FIFOs. Initializes the data_ready field
// for the client to 0. Returns 0 on success and non-zero if the
// server as no space for clients (n_clients == MAXCLIENTS).

	if (server->n_clients == MAXCLIENTS) {
		return -1;                              //  no more room to add a client
	} else {
		client_t* new_client = &server->client[server->n_clients];

		if (join->name != NULL) {
			strncpy(new_client->name, join->name, MAXPATH);
		} else {
		  perror("join name is NULL\n");
		  exit(1);
		}

		if (join->to_client_fname != NULL) {
			strncpy(new_client->to_client_fname, join->to_client_fname, MAXPATH);
		} else {
		  perror("join to_client_fname is NULL\n");
		  exit(1);
		}

		char toClient_fifo[MAXPATH];

		if (new_client->to_client_fname != NULL) {
			strncpy(toClient_fifo, new_client->to_client_fname, MAXPATH);
		} else {
		  perror("new_client to_client_fname is NULL\n");
		  exit(1);
		}

		new_client->to_client_fd = open(toClient_fifo, DEFAULT_PERMS);   // open toClient_fifo
		if (new_client->to_client_fd == -1) {
			perror("Could not open to_Client fifo\n");
			return -1;
		}

		if (join->to_server_fname != NULL) {
			strncpy(new_client->to_server_fname, join->to_server_fname,MAXPATH);
		} else {
		  perror("join to_server_fname is NULL\n");
		  exit(1);
		}

		char toServer_fifo[MAXPATH];

		if (new_client->to_server_fname != NULL) {
			strncpy(toServer_fifo, new_client->to_server_fname, MAXPATH);
		} else {
		  perror("new_client to_server_fname is NULL\n");
		  exit(1);
		}

		new_client->to_server_fd = open(toServer_fifo, DEFAULT_PERMS);   // open toServer_fifo
		if (new_client->to_server_fd == -1) {
			perror("Could not open to_Server fifo\n");
			return -1;
		}

		new_client->data_ready = 0;

		server->n_clients++;


		return 0;
	}
}

int server_remove_client(server_t *server, int idx) {
// Remove the given client likely due to its having departed or
// disconnected. Close fifos associated with the client and remove
// them.  Shift the remaining clients to lower indices of the client[]
// preserving their order in the array; decreases n_clients.

	client_t* burn_client = server_get_client(server, idx);

  int close_ret = close(burn_client->to_client_fd);
	if (close_ret == -1) {
		perror("failed to close removed client to_client_fd\n");
	}

	close_ret = close(burn_client->to_server_fd);
	if (close_ret == -1) {
		perror("failed to close removed client to_client_fd\n");
	}

	int remove_ret = remove(burn_client->to_server_fname);
	if (remove_ret == -1 ) {
		perror("failed to remove to_server_fname fifo\n");
	}

	remove_ret = remove(burn_client->to_client_fname);
	if (remove_ret == -1 ) {
		perror("failed to remove to_client_fname fifo\n");
	}

	// Shift clients
	for(int i = idx; i < server->n_clients-1; i++) {
		server->client[i] = server->client[i+1];
	}

	server->n_clients--;

	return 0;
}

int server_broadcast(server_t *server, mesg_t *mesg) {
// Send the given message to all clients connected to the server by
// writing it to the file descriptors associated with them.

	for (int i = 0; i < server->n_clients; i++) {
		client_t* c = server_get_client(server, i);
		int written = write(c->to_client_fd, mesg, sizeof(mesg_t));
		if (written <= 0) {
			perror("Broadcast failed\n");
			return -1;
		}
	}

// ADVANCED: Log the broadcast message unless it is a PING which
// should not be written to the log.

	return 0;
}

void server_check_sources(server_t *server){
// Checks all sources of data for the server to determine if any are
// ready for reading. Sets the servers join_ready flag and the
// data_ready flags of each of client if data is ready for them.
// Makes use of the select() system call to efficiently determine
// which sources are ready.

	fd_set read_set;                                                 // set of file descriptors for select()
	FD_ZERO(&read_set);                                              // init the set
	int maxfd = server->join_fd;									 // need some maximum fd
	FD_SET(server->join_fd, &read_set);

	for (int i = 0; i < server->n_clients; i++) {
		FD_SET(server->client[i].to_server_fd, &read_set);           // set fds to contain client fifos
		maxfd = (maxfd < server->client[i].to_server_fd) ? server->client[i].to_server_fd : maxfd;
	}

	int check = select(maxfd + 1, &read_set, NULL, NULL, NULL);
	if (check == -1){
		perror("\nIssue with select()");
	} else {
		if (FD_ISSET(server->join_fd, &read_set)) {
			server->join_ready = 1;                          //  there's a client to be added
		}

		for (int i = 0; i < server->n_clients; i++) {
			client_t* c = server_get_client(server, i);
			if (FD_ISSET(c->to_server_fd, &read_set)) {
				c->data_ready = 1;                           //  there's data from a client to broadcast
			}
		}
	}
}

int server_join_ready(server_t *server) {
// Return the join_ready flag from the server which indicates whether
// a call to server_handle_join() is safe.

	return server->join_ready;
}

int server_handle_join(server_t *server) {
// Call this function only if server_join_ready() returns true. Read a
// join request and add the new client to the server. After finishing,
// set the servers join_ready flag to 0.

	join_t request;
	int success1 = read(server->join_fd, &request, sizeof(join_t));
	int success2 = server_add_client(server, &request);
	if (success1 <= 0 || success2 == -1){
		perror("Could not process join request\n");
		return -1;
	}

	// Send message that client has joined
	mesg_t message;
	memset(&message, 0, sizeof(mesg_t));
	message.kind = BL_JOINED;

	if (request.name != NULL) {
		strncpy(message.name, request.name, MAXNAME);
	} else {
		perror("request name is NULL\n");
		exit(1);
	}

	int ret_val = server_broadcast(server, &message);     //  broadcast message to clients that a client has been added

	if (ret_val != 0) {
		perror("Problem with server_broadcast\n");
	}

	server->join_ready = 0;

	return 0;
}

int server_client_ready(server_t *server, int idx) {
// Return the data_ready field of the given client which indicates
// whether the client has data ready to be read from it.

	client_t* from_client = server_get_client(server, idx);
	return from_client->data_ready;
}

int server_handle_client(server_t *server, int idx) {
// Process a message from the specified client. This function should
// only be called if server_client_ready() returns true. Read a
// message from to_server_fd and analyze the message kind. Departure
// and Message types should be broadcast to all other clients.  Ping
// responses should only change the last_contact_time below. Behavior
// for other message types is not specified. Clear the client's
// data_ready flag so it has value 0.

	mesg_t message;
	client_t* handle_client = server_get_client(server, idx);

	int did_read = read(handle_client->to_server_fd, &message, sizeof(mesg_t));
	if (did_read <= 0) {
		perror("Could not read message\n");
		return -1;
	}

  int ret_val = -1;
	// Handle message type
	if (message.kind == BL_MESG){
		ret_val = server_broadcast(server, &message);
		if (ret_val != 0) {
			perror("Problem with server_broadcast\n");
		}

		handle_client->data_ready = 0;
	} else if (message.kind == BL_DEPARTED) {
		ret_val = server_remove_client(server, idx);
		if (ret_val != 0) {
			perror("Problem with server_remove_client\n");
		}

		ret_val = server_broadcast(server, &message);
		if (ret_val != 0) {
			perror("Problem with server_broadcast\n");
		}

	} else if (message.kind == BL_SHUTDOWN) {
		server_shutdown(server);
  	} /* else if (message->kind == BL_DISCONNECTED) {
	} */

// ADVANCED: Update the last_contact_time of the client to the current
// server time_sec.

	return 0;
}

void server_tick(server_t *server);
// ADVANCED: Increment the time for the server

void server_ping_clients(server_t *server);
// ADVANCED: Ping all clients in the server by broadcasting a ping.

void server_remove_disconnected(server_t *server, int disconnect_secs);
// ADVANCED: Check all clients to see if they have contacted the
// server recently. Any client with a last_contact_time field equal to
// or greater than the parameter disconnect_secs should be
// removed. Broadcast that the client was disconnected to remaining
// clients.  Process clients from lowest to highest and take care of
// loop indexing as clients may be removed during the loop
// necessitating index adjustments.

void server_write_who(server_t *server);
// ADVANCED: Write the current set of clients logged into the server
// to the BEGINNING the log_fd. Ensure that the write is protected by
// locking the semaphore associated with the log file. Since it may
// take some time to complete this operation (acquire semaphore then
// write) it should likely be done in its own thread to preven the
// main server operations from stalling.  For threaded I/O, consider
// using the pwrite() function to write to a specific location in an
// open file descriptor which will not alter the position of log_fd so
// that appends continue to write to the end of the file.

void server_log_message(server_t *server, mesg_t *mesg);
// ADVANCED: Write the given message to the end of log file associated
// with the server.
