#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char *s = (char *) malloc((strlen("hello\n") + 1) * sizeof(char));
	strcpy(s, "one\ttwo\tthree\tfour\n");

	char *one_p, *two_p, *three_p, *four_p, *one, *two;

	one_p = strtok(s, "\t");
	two_p = strtok(NULL, "\t");
	three_p = strtok(NULL, "\t");
	four_p = strtok(NULL, "\t");

	one = (char *) malloc((strlen(one_p) + 1) * sizeof(char));
	strcpy(one, one_p);
	two = (char *) malloc((strlen(two_p) + 1) * sizeof(char));
	strcpy(two, two_p);

	printf("one:%s one_p:%s two:%s two_p:%s s:%s four_p:%s\n",
			one, one_p, two, two_p, s, four_p);

	return 0;
}
