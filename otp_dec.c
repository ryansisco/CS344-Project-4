#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void clean(char *str1,char *str2) {
	free(str1);
	free(str2);
}

int scanfile(char *filename) {
	FILE *r = fopen(filename, "r");
	if (r == NULL) {
		fprintf(stderr, "cannot open %s\n", filename);
		exit(1);
	}
	fseek(r, 0L, SEEK_END);
	int size = ftell(r), q = 0;
	fseek(r, 0L, SEEK_SET);
	fclose(r);
	return size;
}

void fillarray(char *filename, char *stn, int size) {
	FILE *r = fopen(filename, "r");
	int q = 0;
	while(!(feof(r))) {
		int character = fgetc(r);
		if (character == '\n') {
			stn[q] = '@';
			stn[q+1] = '@';
			break;
		}
		stn[q] = character;
		q++;
	}
	fclose(r);
}

void execute(char *text, char *key, int port) {
	char *enctext = (char*)malloc(strlen(text));
	char ID[3] = "dec"; 
	int socketFD;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(port); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) { // Connect socket to address
		fprintf(stderr, "ERROR\nBad port\n");
		exit(1);
	}
	send(socketFD, ID, 3, 0);
	sleep(1);
	send(socketFD, text, strlen(text), 0); // Write to the server
	sleep(3);
	send(socketFD, key, strlen(key), 0);
	memset(enctext, '\0', strlen(enctext)); // Clear out the text again for reuse
	recv(socketFD, enctext, strlen(enctext) - 1, 0); // Read data from the socket, leaving \0 at end
	int newsize = strlen(enctext)-1;
	enctext[newsize] = '\0';
	enctext[newsize-1] = '\0';
	printf("%s\n", enctext);
	close(socketFD); // Close the socket
}

int validate(char *text, char *key, int port) {
	int i;
	if (strlen(text) > strlen(key)) {	// checks key is greater than text
		fprintf(stderr, "ERROR\nKey is too small!\n");
		exit(1);
	}
	for (i = 0; i < strlen(text)-2; i++) {	// checks textfile
		if (text[i] > 90) {	// checks outside ascii bounds
			fprintf(stderr, "ERROR\nText has illegal characters!\n");
			exit(1);
		}
		else if (text[i] == ' ') { // allows spaces
		}
		else if (text[i] < 65) {	// checks outside ascii bounds
			fprintf(stderr, "ERROR\nText has illegal characters!\n");
			exit(1);
		}
	}
	for (i = 0; i < strlen(key)-2; i++) {	// checks keyfile
		if (key[i] > 90) {	// checks outside ascii bounds
			fprintf(stderr, "ERROR\nKey has illegal characters!\n");
			exit(1);
		}
		else if (key[i] == ' ') {	// allows spaces
		}
		else if (key[i] < 65) {	// checks outside ascii bounds
			fprintf(stderr, "ERROR\nKey has illegal characters!\n");
			exit(1);
		}
	}
	if (port == 0) {	// checks port
		fprintf(stderr, "ERROR\nPort must be a number!\n");
		exit(1);
	}
	return 1;
}


int main(int argc, char **argv) {
	char *text, *key;
	int port, size, i, q;
	if (argc != 4) {
		fprintf(stderr, "ERROR\nSYNTAX: otp_dec [text] [key] [port]\n");
		return(1);
	}
	else {
		for (i = 1; i < 4; i++) {	// for command args
			if (i == 1) {	// text to decrypt
				size = scanfile(argv[i]);
				text = (char*)malloc(size+2);
				fillarray(argv[i], text, size);
			}
			if (i == 2) {	// key
				size = scanfile(argv[i]);
				key = (char*)malloc(size+2);
				fillarray(argv[i], key, size);
			}
			if (i == 3) {
				port = atoi(argv[3]);
			}
		}
	}
	if (validate(text, key, port)) {
		execute(text, key, port);
	}
	clean(text, key);
	return 0;
}
