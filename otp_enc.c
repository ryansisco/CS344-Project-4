#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int validate(char *text, char *key, int port) {
	printf("%s\n", text);
	printf("%s\n", key);
	printf("%d\n", port);
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
					return(1);
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
					return(1);
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
	validate(text, key, port);
	free(text);
	free(key);
	return 0;
}