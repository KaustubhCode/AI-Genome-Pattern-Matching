#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class state{
public:
	int f;
	int g;
	vector<int> indices ;

	state(){
		f = 0 ;
		g = 0 ;
	}
	state(int x,int y,vector<int> v){
		f = x ;
		g = y ;
		indices = v ;
	}
	// vector<vector<int>>	expand(){
	// 	vector<vector<int>>
	// }


}

int main(){
	ifstream infile; 
	infile.open("input.txt"); 

	cout << "Reading from the file" << endl; 
	string str;
	float time;
	int m;
	int k;
	int cc ;
	infile >> time ;
	infile >> m ;


	char units[m] ;
	int costmap[m+1][m+1];
	char temp;

	getline(infile,str);
	getline(infile,str);

	//Reading units
	int count = 0;
	for(int i = 0 ; i < str.length() ; i++){
		if(str.at(i) == ' ' || str.at(i) == ','){
			continue ;
		}
	    units[count] = str.at(i);
	    count++;
	}

	infile >> k;
	string s[k] ;
    for(int i = 0 ; i < k ; i++){
    	infile >> s[i];
    }
    infile >> cc ;
	for(int i = 0 ; i <= m ; i++){
	    for(int j = 0 ; j <= m ; j++){
	        infile >> costmap[i][j] ;
	    }
	}

	// cout << time << endl;
	// cout << m << endl;
	// cout << k << endl;
	// for (int i = 0; i < m; i++){
	// 	cout << units[i] << " ";
	// }
	// cout << endl;
	// for(int i = 0 ; i < k ; i++){
 	//    cout << s[i] << endl ;
 	// }


 //    infile >> temp ;
	// cout << temp << endl;
	// // cout << "Received Costs" << endl ;
 //    for(int i = 0 ; i <= m ; i++){
 //        for(int j = 0 ; j <= m ; j++){
 //            cout << costmap[i][j] << " " ;
 //        }
 //        cout << endl ;
 //    }
	infile.close();
	return 0;
}