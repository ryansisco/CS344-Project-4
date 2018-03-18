#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
//******************************************************************************
// FUNCTION NAME: error
// AUTHOR: Ryan Sisco
// PURPOSE: prints recieved error message
// PARAMETERS: character array to print
//****************************************************************************** 
	fprintf(stderr, "%s\n", msg); 
	exit(1); 
}

int main(int argc, char *argv[]) {
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char text[200000];	// space for text
	char key[200000];	// space for key
	struct sockaddr_in serverAddress, clientAddress;
	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections
	int l;
	for (l = 0; l < 5; l++) {
		int status;
		pid_t pid = -5;
		pid = fork();	// forks
		if (pid < 0) {	// if unsuccessful fork
			perror("ERROR");
			exit(1);
		}
		if (pid == 0) {	// if successful fork
			setpgid(0,0);
			while (1) {
				sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
				establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
				if (establishedConnectionFD < 0) {
					error("ERROR on accept");
				}
				char readBuffer[10000];	// creates buffer
				memset(readBuffer, '\0', 10001);	// clears
				recv(establishedConnectionFD, readBuffer, 10000, 0); // Read the client's message from the socket
				if (strcmp(readBuffer, "enc") != 0) {
					error("ERROR\nThis is encryption port");
				}
				memset(text, '\0', 200001);	// clears
				memset(readBuffer, '\0', 10001);	// clears
				while (strstr(text, "@@") == NULL) {
					memset(readBuffer, '\0', sizeof(readBuffer));
					recv(establishedConnectionFD, readBuffer, 10000, 0); // Read the client's message from the socket
					strcat(text, readBuffer);	// appends buffer to text
				}
				char first = text[0];
				memset(key, '\0', 200001);	// clears
				memset(readBuffer, '\0', 10001);	// clears
				while (strstr(key, "@@") == NULL) {
					memset(readBuffer, '\0', sizeof(readBuffer));
					recv(establishedConnectionFD, readBuffer, 10000, 0); // Read the client's message from the socket
					strcat(key, readBuffer);	// appends buffer to key
				}
				char firstkey = key[0];
				int i;
				for (i = 0; i < sizeof(text); i++) {
					int temp1, temp2, temp3;
					if (i == 0) {	// resets first var due to clear
						text[i] = first;
						key[i] = firstkey;
					}
					if (text[i] == '@') {	// breaks if EOF
						break;
					}
					if (text[i] > 64) {	// regular char
						temp1 = (text[i] - 65); // 0-25
					}
					if (key[i] > 64) {	// regular char
						temp2 = (key[i] - 65);	// 0-25
					}
					if (text[i] == 32) {	// space
						temp1 = 26;
					}
					if (key[i] == 32) {	// space
						temp2 = 26;
					}
					temp3 = (temp2 + temp1);
					if (temp3 > 26) {
						temp3 = (temp3 - 27);	// resets to range
					}
					if (temp3 < 26) { 
						text[i] = (temp3 + 65);	// resets to ascii
					}
					if (temp3 == 26) {
						text[i] = ' ';	// sets space
					}
				}
				charsRead = send(establishedConnectionFD, text, i+2, 0); // Send success back
				if (charsRead < 0) {
					error("ERROR writing to socket");
				}
				close(establishedConnectionFD); // Close the existing socket which is connected to the client
			}
		}	
		else {
			waitpid(pid, &status, WNOHANG);
		}
	}
	return 0; 
}
