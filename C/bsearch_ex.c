#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ex {
	char *name;
	int *data;
};


int compare_ex(const void *a, const void *b)
{   
	struct ex *a_i = (struct ex *)a;
	struct ex *b_i = (struct ex *)b;
	return strcmp(a_i->name, b_i->name);
}



int main(int argc, char *argv[]) {

	struct ex ex_a[4];

	ex_a[0].name = (char *) malloc(strlen("one") * sizeof(char));
	strcpy(ex_a[0].name, "one");
	ex_a[0].data = (int *) calloc(3, sizeof(int));

	ex_a[1].name = (char *) malloc(strlen("two") * sizeof(char));
	strcpy(ex_a[1].name, "two");
	ex_a[1].data = (int *) calloc(5, sizeof(int));

	ex_a[2].name = (char *) malloc(strlen("three") * sizeof(char));
	strcpy(ex_a[2].name, "three");
	ex_a[2].data = (int *) calloc(2, sizeof(int));

	ex_a[3].name = (char *) malloc(strlen("four") * sizeof(char));
	strcpy(ex_a[3].name, "four");
	ex_a[3].data = (int *) calloc(1, sizeof(int));

	qsort(ex_a, 4, sizeof(struct ex),  compare_ex);

	ex_a[1].data[0] = -1;
	ex_a[1].data[1] = -1;
	ex_a[1].data[2] = -1;

	struct ex tmp;
	tmp.name = "two";
	struct ex *pos = (struct ex *) bsearch((void *)&tmp, ex_a, 4, 
			sizeof(struct ex), compare_ex);

	if (pos) 
		printf("found\n");
	else
		printf("not found\n");

	pos->data[2] = 5;

	tmp.name = "one";
	pos = (struct ex *) bsearch((void *)&tmp, ex_a, 4, 
			sizeof(struct ex), compare_ex);

	printf("%d\n", pos->data[2]);

	tmp.name = "two";
	pos = (struct ex *) bsearch((void *)&tmp, ex_a, 4, 
			sizeof(struct ex), compare_ex);



	printf("%d %d %d\n", pos->data[0], pos->data[1], pos->data[2]);
}
