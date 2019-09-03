// #include<bits/stdc++.h>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <ctime>
#include <limits>

using namespace std;

float RandomFloat(float min, float max)
{
	// this  function assumes max > min, you may want
	// more robust error checking for a non-debug build
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}

string generateString(char c[], int len) {
	string s = "";
	int index;
	for (int j = 0; j < len; j++) {
		index = rand() % 4;
		s += c[index];
	}

	return s;
}

int main() {
    srand(time(NULL));
	float f;
	f = RandomFloat(0.5, 4.0) ;
	int k = 4;
	char c[] = {'A','C','T','G'} ;
	int m = 0;
	while (m == 0) {
		m = rand() % 21;
	}
	string s[m];
	int maxlen = 0;
	while (maxlen < 10) {
		maxlen = rand() % 31;
	}
	int len;
	for (int i = 0; i < m; i++) {
		len = 0;
		while (len <= 0.7 * maxlen) {
			len = rand() % (maxlen + 1);
		}
		s[i] = generateString(c, len);
	}
	int cc;
	cc = rand() % 7;
	int cost[m + 1][m + 1];
	for (int i = 0; i < m + 1; i++) {
		cost[i][i] = 0;
		for (int j = i + 1; j < m + 1; j++) {
			cost[i][j] = rand() % 10;
			cost[j][i] = cost[i][j];
		}
	}
	ofstream myfile;
	myfile.open("E:/Course Content/COL333/AI-Genome-Pattern-Matching/example.txt");
	myfile << f << endl;
	myfile << k << endl;
	/*for (int i = 0; i < 3; i++) {
		myfile << c
	}*/
	myfile << "A, C, T, G" << endl;
	myfile << m << endl;
	for (int i = 0; i < m; i++) {
		myfile << s[i] << endl;
	}
	myfile << cc << endl;
	for (int i = 0; i < m + 1; i++) {
		for (int j = 0; j < m + 1; j++) {
			myfile << cost[i][j] << " ";
		}
		myfile << endl;
	}

	return 0 ;
}
