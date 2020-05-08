#include "blather.h"
#include <string.h>
#include <signal.h>

int signaled = 0;

void handle_signals(int signo) {
	signaled = 1;
	return;
}

int main(int argc, char* argv[]) {
	// Signal Handling
	struct sigaction my_sa = {};			// portable signal handling setup with sigaction()
	sigemptyset(&my_sa.sa_mask);			// don't block any other signals during handling
	my_sa.sa_handler = handle_signals;		// run function handle_signals
	my_sa.sa_flags = SA_RESTART;			// restart system calls on signals if possible
	sigaction(SIGTERM, &my_sa, NULL);		// register SIGTERM with given action
	sigaction(SIGINT, &my_sa, NULL);		// register SIGINT with given action

	server_t server;
	memset(&server, 0, sizeof(server_t));
	strcpy(server.server_name, argv[1]);

	server_start(&server, argv[1], DEFAULT_PERMS);

	// REPEAT:
	while(!signaled){
		// Check all sources
		server_check_sources(&server);

		// Handle a join request if one is ready
		if (server_join_ready(&server)) {
			if(server_handle_join(&server) == -1){
				perror("Join failed\n");
			}
		}

		// For each client, handle data from it if ready
		for (int i = 0; i < server.n_clients; i++) {
			if (server_client_ready(&server, i)) {
				if(server_handle_client(&server, i) == -1){
					perror("Could not handle client\n");
				}
			}
		}
	}   //  while loop

	server_shutdown(&server);
	exit(0);
}
