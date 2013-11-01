#include "timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{

	if (argc != 2) {
		printf("%s N\n", argv[0]);
		return 1;
	}

	int N = atoi(argv[1]);

	// create a test file
	FILE *file;
	start();
	file = fopen(".test", "w");
	int i;
	for (i = 0; i < N; ++i)
		fprintf(file, "%d\n", rand());
	fclose(file);
	stop();
	//printf("write:\t%ld\n", report());

	start();
	string line;
	ifstream myfile (".test");
	vector<double> tmp_vector;
	if (myfile.is_open()) {
		while ( myfile.good() ) {
			getline (myfile,line);
			tmp_vector.push_back( atof(line.c_str()) );
		}
		myfile.close();
	} else
		cerr << "Unable to open " << ".test" << endl;
	stop();
	unsigned long cpp = report();

	start();
	int file_len = 0;
	char c_line[LINE_MAX];
	FILE *file_2 = fopen(".test", "r");
	while ( fgets(c_line, LINE_MAX, file_2) )
		++file_len;

	fclose(file_2);

	int *ints  = (int *) malloc(file_len * sizeof(int));

	file_2 = fopen(".test", "r");
	i = 0;
	while ( fgets(c_line, LINE_MAX, file_2) ) {
		int a = atoi(strtok(c_line, "\t"));
		ints[i] = a;
		++i;
	}
	fclose(file_2);

	stop();
	unsigned long c = report();
	printf("%d\t%ld\t%ld\n", N, cpp, c);

	return 0;
}
