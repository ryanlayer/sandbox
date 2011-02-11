#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
	int i,f = 0,s = 0, w = 1;
	char first[LINE_MAX];
	char second[LINE_MAX];

	fgets(first, LINE_MAX, stdin);

	while( (i = getc(stdin)) != EOF ) {
		if( (char)i == ',' ) {
			first[f] = '\0';
			f = 0;
			w = 2;
		} else if ( ((char)i == ';') || 
					((char)i == '\n') ||
					((char)i == '\0') ) {
			second[s] = '\0';
			s = 0;
			w = 1;
			printf("%s\t%s\n", first, second);
		} else if (w == 1)
			first[f++] = (char)i;
		else 
			second[s++] = (char)i;
	}
}
