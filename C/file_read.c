#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *file_name = argv[1];
    FILE *fp;
    char line[256];
    char *l_p;
    char *save_ptr1;

    fp = fopen(file_name, "r");

    if (fp == NULL)
        perror("Error opening file.");

    int i = 0;
    while( fgets(line, 256, fp) != NULL ) {
        l_p = strtok_r(line, "\t", &save_ptr1);
        while (l_p != NULL) {
            char *save_ptr2;
            int a = atoi(strtok_r(l_p, " ", &save_ptr2));
            int b = atoi(strtok_r(NULL, " ", &save_ptr2));
            printf("%d,%d\n", a, b);

            l_p = strtok_r(NULL, "\t", &save_ptr1);
        }
    }

    return 0;
}
