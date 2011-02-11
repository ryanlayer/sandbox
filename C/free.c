#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct aa {
	char *x;
	int y;
};

int main() {
	struct aa a;
	a.x = (char *) malloc(5*sizeof(char));
	strcpy(a.x, "hello");
	a.y = 10;
	printf("%s\n", a.x);
	free(a.x);
	printf("%s\n", a.x);
	a.x = (char *) malloc(10*sizeof(char));
	strcpy(a.x, "hello you");
	printf("%s\n", a.x);

}
		

