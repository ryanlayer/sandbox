#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sfx 
{
	int origin_node, first_char_index, last_char_index;
};

struct sfx_edge
{
	int first_char_index, last_char_index;
	sfx_node *start_node, *end_node;
};


void add_prefix(struct sfx *active, int i)
{
	int parent_node, last_parent_node = -1; // keep for sfx pointers

	while (true) {
		sfx_edge e;
		e.start_node = 0;


	}
}


int main(int arc, char *argv[])
{

	int N = strlen(argv[1]) - 1;
	char *T = (char *) malloc( N * sizeof(char) + 1);
	strcpy(T, argv[1]);
	T[N + 1] = '#';

	printf("%s\n", T);

	struct sfx active_point;
	active.origin_node = 0;
	active.first_char_index = 0;
	active.last_char_index = 0;

	int i;
	for (i = 0; i <= N; i++)
		add_prefix(&active, i);

