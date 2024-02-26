#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

#define Server_PortNumber 5557
#define Server_Address "140.120.222.149"
#define Num 4

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *Client_handler(void *arg) {

	struct sockaddr_in server_addr;
	int sock, byte_sent, server_addr_length = sizeof(server_addr);
	char send_buffer[50];
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("Error creating socket!\n");	
	}
	bzero(&server_addr, server_addr_length);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Server_PortNumber);
	server_addr.sin_addr.s_addr = inet_addr(Server_Address);

	if(connect(sock,(struct sockaddr *)&server_addr, server_addr_length) < 0) {
		printf("connect failed!\n");
		close(sock);
	}

	pthread_mutex_lock( &mutex1 );
	printf("For thread : %lu\n", pthread_self());
	fgets(send_buffer, sizeof(send_buffer), stdin);
	pthread_mutex_unlock( &mutex1 );

	byte_sent = send(sock, send_buffer, sizeof(send_buffer), 0);
	if(byte_sent < 0) {
		printf("Error sending packet!\n");
	}
	close(sock);
	pthread_exit(NULL);
}
void *Server_handler(void *arg) {
	struct sockaddr_in server_addr, client_addr;
	int sock, byte_recv, recfd;
	int server_addr_length = sizeof(server_addr);
	int client_addr_length = sizeof(client_addr);
	char recv_buffer[50];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("Error creating socket!\n");	
	}
	bzero(&server_addr, server_addr_length);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(Server_PortNumber);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *)&server_addr, server_addr_length) == -1) {
		printf("Error binding!\n");
		close(sock);
	}
	if(listen(sock, 20) == -1) {
		printf("Listen failed!\n");
		close(sock);
	}
	int i = 0;
	while(i < Num) {
		if((recfd = accept(sock, (struct sockaddr *)&client_addr, &client_addr_length)) == -1) {
			printf("Accept failed!\n");
			close(sock);
		}
	
		byte_recv = recv(recfd, recv_buffer, sizeof(recv_buffer), 0);
		
		pthread_mutex_lock( &mutex1 );
		if(byte_recv < 0) {
			printf("Error recving packet!\n");
		}
		else {	/*command*/
			int buflen = strlen(recv_buffer);			
			if(recv_buffer[buflen-1]=='\r' || recv_buffer[buflen-1]=='\n')
				recv_buffer[buflen-1] = '\0';
			
			if(!strcmp("ls", recv_buffer)){
				/*				
				DIR* pDir = opendir(".");
				struct dirent* pEntry = NULL;
				while((pEntry = readdir(pDir)) != NULL) {
					if(!strcmp(pEntry->d_name, ".") || !strcmp(pEntry->d_name, "..")){
				continue;
			}
					printf("%s\n", pEntry->d_name);
				}
				printf("\n");
				*/
				printf("List the files and directories in the current directory.\n\n");			
				i++;
			}	
			else if(!strcmp("cd", recv_buffer)){
				printf("Change the current working directory\n\n");
				i++;
			}
			else if(!strcmp("download", recv_buffer)){
				printf("Download a file from current working directory\n\n");
				i++;
			}
			else if(!strcmp("upload", recv_buffer)){
				printf("Upload a file to current working directory.\n\n");
				i++;
			}
			else {
				i++;
			}
		}
		pthread_mutex_unlock( &mutex1 );
		sleep(3);
	}
	close(sock);
	pthread_exit(NULL);
}
int main(int argc, char** argv) {
	
	pthread_t server, client[Num];
	
	pthread_create( &server , NULL, Server_handler, NULL);
	int i;
	for(i = 0; i < Num; i++){	
		pthread_create( &client[i] , NULL, Client_handler, NULL);
	}
	for(i = 0; i < Num; i++){	
		pthread_join( client[i], NULL);
	}
	pthread_join( server, NULL);
	return 0;
}

