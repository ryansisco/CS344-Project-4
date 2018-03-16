#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
			break;
		}
		stn[q] = character;
		q++;
	}
	fclose(r);
}

void execute(char *text, char *key, int port) {
	printf("VALIDATED\n");
}

int validate(char *text, char *key, int port) {
	int i;
	if (strlen(text) > strlen(key)) {	// checks key is greater than text
		fprintf(stderr, "ERROR\nKey is too small!\n");
		exit(1);
	}
	for (i = 0; i < strlen(text); i++) {	// checks textfile
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
	for (i = 0; i < strlen(key); i++) {	// checks keyfile
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
	int port;
	if (argc != 4) {
		fprintf(stderr, "ERROR\nSYNTAX: otp_dec [text] [key] [port]\n");
		return(1);
	}
	else {
		int i, size, q;
		for (i = 1; i < 4; i++) {	// for command args
			if (i == 1) {	// text to decrypt
				size = scanfile(argv[i]);
				text = (char*)malloc(size);
				fillarray(argv[i], text, size);
			}
			if (i == 2) {	// key
				size = scanfile(argv[i]);
				key = (char*)malloc(size);
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