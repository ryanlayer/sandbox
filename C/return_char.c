#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* test(char *s) {
	char *t = strtok(s, ",");
	t = strtok(NULL, ",");
	return t;
}

int main(int argc, char *argv[]) {
	printf("%s\n", test(argv[1]));
}
