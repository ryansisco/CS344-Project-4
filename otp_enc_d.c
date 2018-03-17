#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char text[200000];
	char key[200000];
	struct sockaddr_in serverAddress, clientAddress;

	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	if (establishedConnectionFD < 0) error("ERROR on accept");

	// Get the message from the client and display it
	memset(text, '\0', 200001);
	char readBuffer[10000];
	//charsRead = recv(establishedConnectionFD, text, 10000, 0); // Read the client's message from the socket
	while (strstr(text, "@@") == NULL) {
		memset(readBuffer, '\0', sizeof(readBuffer));
		recv(establishedConnectionFD, readBuffer, 10000, 0); // Read the client's message from the socket
		strcat(text, readBuffer);
	}
	printf("SERVER: I received this from the client: \"%s\"\n", text);
	printf("%c\n", text[0]);
	
	memset(key, '\0', 200001);
	memset(readBuffer, '\0', 10001);
	while (strstr(key, "@@") == NULL) {
		memset(readBuffer, '\0', sizeof(readBuffer));
		recv(establishedConnectionFD, readBuffer, 10000, 0); // Read the client's message from the socket
		strcat(key, readBuffer);
	}
	printf("SERVER: I received this from the client: \"%s\"\n", key);
	int i;
	for (i = 0; i < sizeof(text); i++) {
		int temp1, temp2, temp3;
		if (text[i] == '@') {
			break;
		}
		else if (text[i] == 32) {
			temp1 = 26;
		}
		else if (key[i] == 32) {
			temp2 = 26;
		}
		else {
			if (text[i] > 64) {
				temp1 = (text[i] - 65);
			}
			if (key[i] > 64) {
				temp2 = (key[i] - 65);
			}
		}
		temp3 = (temp2 + temp1);
		if (temp3 > 26) {
			temp3 = (temp3 - 26);
		}
		text[i] = (temp3 + 65);
	}
	// Send a Success message back to the client
	charsRead = send(establishedConnectionFD, text, i+2, 0); // Send success back
	if (charsRead < 0) error("ERROR writing to socket");
	close(establishedConnectionFD); // Close the existing socket which is connected to the client
	close(listenSocketFD); // Close the listening socket
	return 0; 
}
