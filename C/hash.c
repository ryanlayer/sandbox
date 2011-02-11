#include <stdio.h>
#include <math.h>

static unsigned long sdbm(unsigned char *str) {
//int sdbm(unsigned char *str) {
	unsigned long hash = 0;
	//int hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
	}

int main(int argc, char *argv[]) {

	char *l[] = {
		"AACO03000001",
		"AACO03000002",
		"AACO03000003",
		"AACO03000004",
		"AACO03000005",
		"AACO03000006",
		"AACO03000007",
		"AACO03000008",
		"AACO03000009",
		"AACO03000010",
		"AACO03000011",
		"AACO03000012",
		"AACO03000013",
		"AACO03000014"};

	int i;
	for (i = 0; i < 14; i++) {
		unsigned long  s = sdbm(l[i]);
		printf("%d\n", s);
	}

}
