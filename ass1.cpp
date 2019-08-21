#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void int main(){
	ifstream infile; 
	infile.open("input.txt"); 

	cout << "Reading from the file" << endl; 
	string str;
	float time;
	int m;
	int k;
	string s[k] ;
	char units[m] ;
	char temp ;
	int costmap[m+1][m+1];
	int cc ;
	infile >> time ;
	infile >> m ;
	for(int i = 0 ; i < m ; i++){
	    infile >> units[i] ;
	    infile >> temp ;
	}
	infile >> k ;
    for(int i = 0 ; i < k ; i++){
        infile >>  s[i] ;
    }
    infile >> cc ;
	for(int i = 0 ; i < = m ; i++){
	    for(int j = 0 ; j <= m ; j++){
	        infile >> costmap[i][j] ;
	    }
	}
    infile >> temp ;
	cout << temp ;
	cout << "Received Costs" << endl ;
    for(int i = 0 ; i < = m ; i++){
        for(int j = 0 ; j <= m ; j++){
            cout << costmap[i][j] << " " ;
        }
        cout << endl ;
    }
	return 0;
}