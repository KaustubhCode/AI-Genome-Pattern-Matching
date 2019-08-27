#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

//Global variable
vector<vector<int> > globalcosts;		// Cost matrix
vector<char> units; 					//{'A','T','G','C','_'};
unordered_map<char, int> umap; 			//{'A':0,'T':1,'G':2,'C':3,'_':4};
int cc;									// Cost of inserting '_'
time_t start_time;						// Starting time
float total_time;						// Time the program should run
int k;									// Number of strings
int m;									// Number of units (excluding '_')
int maxlen;								// Max Length of strings given

//State Class
class state{
	public:
		int score;
		vector<string> all_seq;
		int n; //Length of strings

		state(int length_strings){
			n = length_strings;
			score = 0;
		}

		state(int length_strings, vector<string> new_seq){
			n = length_strings;
			all_seq = new_seq;
			score = 0;
			compute_cost();
		}

		void add(string seq){
			all_seq.push_back(seq);
		}

		void add(state prev){
			all_seq = prev.all_seq;
		}

		void add_score(int new_score){
			score = new_score;
		}

		void compute_cost(){
			score = 0;
			for (int i = 0; i < n; i++){
				for (int j = 0; j < k; j++){
					if (all_seq.at(j).at(i) == '_'){
						score += cc;
					}
					for (int p = j+1; p < k; p++){
						score += globalcosts.at(umap[all_seq.at(j).at(i)]).at(umap[all_seq.at(p).at(i)]);
					}
				}
			}
		}

		void print_state(){
			for (int i = 0; i < k; i++){
				cout << all_seq.at(i) << " ";
			}
			cout << endl;
			cout << "Score is: " << score << endl;  
		}
};

state greedy_choose(state current, int ind){
	string str = current.all_seq.at(ind);
	int min_score = current.score;
	state best_state = current;
	for (int i = 0; i < str.size();i++){
		if (str.at(i) == '_'){
			if (i-1 >= 0 && str.at(i-1) != '_'){
				swap(str[i-1],str[i]);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = str;
				state new_state(current.n, new_seq);
				if (new_state.score <= min_score){
					best_state = new_state;
					min_score = new_state.score;
				}
				swap(str[i-1],str[i]);
			}
			if (i+1 < str.size() && str.at(i+1) != '_'){
				swap(str[i+1],str[i]);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = str;
				state new_state(current.n, new_seq);
				if (new_state.score <= min_score){
					best_state = new_state;
					min_score = new_state.score;
				}
				swap(str[i+1],str[i]);	
			}
		}
	}
	return best_state;
}

state greedy_local_search(state current){
	//Find next best state
	int count = 0;
	while (true){
		// cout << "Working:" << count << endl;
		state next = greedy_choose(current,count);
		next.print_state();

		if (next.score > current.score){
			return current;
		}
		else{
			current = next;
		}
		count = (count+1)%k;
	}
}

int main(){

	start_time = time(NULL);

	ifstream infile; 
	infile.open("input.txt"); 
	cout << "Reading from the file" << endl; 
	infile >> total_time ;
	infile >> m ;
	
	string str;
	getline(infile,str);
	getline(infile,str);

	//Reading units
	int count = 0;
	for(int i = 0 ; i < str.length() ; i++){
		if(str.at(i) == ' ' || str.at(i) == ','){
			continue ;
		}
	    units.push_back(str.at(i));
	    umap[str.at(i)] = count;
	    count++;
	}
	units.push_back('_');
	umap['_'] = count;

	infile >> k;
	string s[k] ;
    for(int i = 0 ; i < k ; i++){
    	infile >> s[i];
    	int cmp = s[i].length();
    	if (maxlen < cmp){
    		maxlen = cmp;
    	}
    }

    //Storing costs
    infile >> cc ;
	for(int i = 0 ; i <= m ; i++){
		vector<int> temp(m+1,0);
	    for(int j = 0 ; j <= m ; j++){
	        infile >> temp.at(j);
	    }
	    globalcosts.push_back(temp);
	}

	maxlen += 1;

	state first(maxlen);

	for (int i = 0; i < k; i++){
		while (s[i].size() < maxlen){
			s[i] = s[i].append("_");
		}
		first.add(s[i]);
	}
	first.compute_cost();
	first.print_state();

	// string test = s[0];
	// cout << "String initially:" << test << endl;
	// swap(test[2],test[3]);
	// cout << "String finally:" << test << endl;
	greedy_local_search(first);
	// printf("Program will run for %f seconds\n",total_time*60);
	// while (true){
		// cout << "Time passed is: " << (time(NULL) - start_time) << endl;
		// if (time(NULL) - start_time + 5 < total_time*60 ){
		// 	break;
		// }
	// }

	infile.close();
	return 0;
}