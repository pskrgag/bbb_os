#include <sys/socket.h>
#include <syslog.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <bone/olaf_api.h>
#include "olaf.h"

static void *__attribute__((noreturn)) user_main(void *data)
{
	sock_t socket = (sock_t) data;
	olaf_code_t code;
	int is_logged = 0;

	code = olaf_get_code(socket);
	if (code != OLAF_LOGIN)
		goto error;

	/* only these commands can be first commands send by user */
	switch (code) {
	case OLAF_LOGIN:
		is_logged = olaf_login(socket);
		if (!is_logged)
			goto error;

		break;
	case OLAF_GET_DEVICE_INFO:
		break;
		
	}

	while (1) {
		code = olaf_get_code(socket);
		if (code == OLAF_WRONG_CODE)		/* no need to talk with broken connection */
			goto error;
	}
error:
	close(socket);
	pthread_exit(NULL);
}

static void __attribute__((noreturn)) olaf_main(sock_t master)
{
	sock_t new_conn;
	pthread_t user_thread;
	struct user_args *args;
	int res;

	while (1) {
		new_conn = accept(master, NULL, NULL);
		if (new_conn < 0) {
			log_err("Failed to accept new connection");
			continue;
		}

		res = olaf_enable_keep_alive(new_conn);
		if (res) {
			log_err("Failed to set keep_alive for client");
			goto err;
		}

		res = olaf_check_handshake(new_conn);
		if (res) {
			log_err("Failed to check handshake with user");
			goto err;
		}

		pthread_create(&user_thread, NULL, user_main, (void *) new_conn);

		DBG("Accepted new connection\n");
		continue;
err:
		close(new_conn);
	}

	/* unreachable */
	exit(0);
}

int main(int argc, char **argv)
{
	sock_t master_socket;
	int res;
	struct sockaddr_in addr;
	const int yes = 1;

	res = daemon(0, 0);
	if (res) {
		fprintf(stderr, "Failed to daemonize\n");
		exit(-11);
	}

	openlog("daemon", LOG_CONS, LOG_DAEMON);

	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket < 0) {
		log_err("Failed to create master socket\n");
		exit(-1);
	}

	res = setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (res) {
		log_err("Failed to setsockopt for master\n");
		exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_addr.s_addr = inet_addr(IP);

	res = bind(master_socket, (struct sockaddr *) &addr, sizeof(addr));
	if (res) {
		log_err("Failed to bind master socket\n");
		exit(-2);
	}

	res = listen(master_socket, SOMAXCONN);
	if (res) {
		log_err("Failed to create master socket");
		exit(-3);
	}

	olaf_main(master_socket);
}