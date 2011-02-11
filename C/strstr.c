#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
	bool i = 1 == 0;
	char *a = (char *) calloc(strlen(argv[1]), sizeof(char));
	strcpy(a, argv[1]);
	char *b = (char *) calloc(strlen(argv[2]), sizeof(char));
	strcpy(b, argv[2]);

	if (strlen(a) < strlen(b)) {
		printf("usage: temp <haystack> <needle>\n");
		return 0;
	}

	char *c = strstr(a, b);
	int d = strcoll(a, b);

	if (c == NULL) {
		printf("Not found\n");
		printf("%i\n", d);
	} else {
		printf("%s\n", c);
		printf("%i\n", d);
		printf("%s\n", (strchr(a, 'X') + 1));
	}

	return 0;
}
