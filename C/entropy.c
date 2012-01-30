#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// <summary>
///// returns bits of entropy represented in a given string, per 
///// http://en.wikipedia.org/wiki/Entropy_(information_theory) 
///// </summary>
/*
public static double ShannonEntropy(string s)
{
	var map = new Dictionary<char, int>();
	foreach (char c in s) {
		if (!map.ContainsKey(c))
		map.Add(c, 1);
		else
		map[c] += 1;
	}

	double result = 0.0;
	int len = s.Length;

	foreach (var item in map) {
		var frequency = (double)item.Value / len;
		result -= frequency * (Math.Log(frequency) / Math.Log(2));
	}
	return result;
}
*/

int main(int argc, char *argv[])
{

	int i;
	char *input = argv[1];
	for (i = 0; i < strlen(input); i++)
		printf("%c\n", input[i]);

}
