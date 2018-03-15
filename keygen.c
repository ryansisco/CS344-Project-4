#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
	if (argc != 2) {	//Not 2 command line args
		fprintf(stderr, "ERROR\nSYNTAX: keygen [key length]\n");
		exit(1);
	}
	else {	// 2 command line args
		srand(time(NULL));
		int i, keylength = atoi(argv[1]);
		if (keylength == 0) {
			fprintf(stderr, "ERROR\nSYNTAX: keygen [key length]\n");
			exit(1);
		}
		else {
			for (i = 0; i < keylength; i++) {
				char key = (rand() % (91 - 65 + 1)) + 65;
				// ascii capital 65-90 + '['
				if (key == '[') {	// changes '[' to ' '
					printf(" ");
				}
				else {	// prints key if not '['
					printf("%c", key);
				}
			}
			printf("\n");
		}
	}
	return 0;
}