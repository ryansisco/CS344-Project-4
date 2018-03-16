#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void execute(char *text, char *key, int port) {
	printf("VALIDATED\n");
}

int validate(char *text, char *key, int port) {
	int i;
	// printf("%s\n", text);
	// printf("%s\n", key);
	// printf("%d\n", port);
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
				FILE *r = fopen(argv[i], "r");
				if (r == NULL) {
					fprintf(stderr, "cannot open %s\n", argv[i]);
					fclose(r);
					exit(1);
				}
				fseek(r, 0L, SEEK_END);
				size = ftell(r);
				fseek(r, 0L, SEEK_SET);
				text = (char*)malloc(size);
				q = 0;
				while(!(feof(r))) {
					int character = fgetc(r);
					if (character == '\n') {
						break;
					}
					text[q] = character;
					q++;
				}
				fclose(r);
			}
			if (i == 2) {	// key
				FILE *r = fopen(argv[i], "r");
				if (r == NULL) {
					fprintf(stderr, "cannot open %s\n", argv[i]);
					fclose(r);
					exit(1);
				}
				fseek(r, 0L, SEEK_END);
				size = ftell(r);
				fseek(r, 0L, SEEK_SET);
				key = (char*)malloc(size);
				q = 0;
				while(!(feof(r))) {
					int character = fgetc(r);
					if (character == '\n') {
						break;
					}
					key[q] = character;
					q++;
				}
				fclose(r);
			}
			if (i == 3) {
				port = atoi(argv[3]);
			}
		}
	}
	if (validate(text, key, port)) {
		execute(text, key, port);
	}
	free(text);
	free(key);
	return 0;
}