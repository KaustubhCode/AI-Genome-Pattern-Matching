#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <limits.h>
#include <math.h>

using namespace std;

//Global variable
vector<vector<int> > globalcosts;		// Cost matrix
vector<char> units; 					// {'A','T','G','C','_'};
map<char, int> umap; 			// {'A':0,'T':1,'G':2,'C':3,'-':4};
int cc;									// Cost of inserting '-'
time_t start_time;						// Starting time
float total_time;						// Time the program should run
int k;									// Number of strings
int m;									// Number of units (excluding '_')
int maxlen;								// Max Length of strings given


float RandomFloat()
{
	// this  function assumes max > min, you may want
	// more robust error checking for a non-debug build
	// assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	return random;
}


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
			// cout << "Check 1" << endl;
			all_seq = new_seq ;
			// cout << "Check 2" << endl;
			score = 0;
			// compute_cost();
		}

		//virtual ~state() = default;

		state create_new(int length_strings, vector<string> new_seq, int p){
			state new_state(n, new_seq);
			new_state.score = score;
			// for (int i = p; i < length_strings; i++){
			// 	new_state.score = new_state.score - compute_cost_ind(i) + new_state.compute_cost_ind(i);	
			// }
			new_state.compute_cost();
			return new_state;
		}

		state create_new(int length_strings, vector<string> new_seq, int p, int q){
			state new_state(n, new_seq);
			new_state.score = score - compute_cost_ind(p) - compute_cost_ind(q) + new_state.compute_cost_ind(p) + new_state.compute_cost_ind(q);
			// new_state.compute_cost();
			return new_state;
		}

		void add(string seq){
			all_seq.push_back(seq);
		}

		void add(state prev){
			for (int i = 0; i < prev.all_seq.size();i++){
				all_seq.push_back(prev.all_seq[i]);
			}
		}

		void add_score(int new_score){
			score = new_score;
		}

		void compute_cost(){

			score = 0;
			for (int i = 0; i < n; i++){
				 score += compute_cost_ind(i);
			}
			// cout << "New Score: " << score << endl;

			// score = 0;
			// for (int i = 0; i < n; i++){
			// 	for (int j = 0; j < k; j++){
			// 		if (all_seq[j].at(i) == '_'){
			// 			score += cc;
			// 			// cout << score << " ";
			// 		}
			// 		for (int p = j+1; p < k; p++){
			// 			score += globalcosts[umap[all_seq[j].at(i)]][umap[all_seq[p].at(i)]];
			// 			// cout << score << " ";
			// 		}
			// 	}
			// 	// cout << endl;
			// }
			// cout << "Old Score: " << score << endl;
		}

		int compute_cost_ind(int idx){
			// map<char, int> count_map;
			int counts[m+1] ;
			// cout << "Check 5" << endl ;
			for (int i = 0; i < units.size(); i++){
				counts[i] = 0;
			}
			// cout << "Check 2" << endl ;
			for (int j = 0; j < k; j++){
				counts[umap[all_seq[j].at(idx)]]++;
			}
			// cout << "Check 69" << endl ;
			int ans = 0;
			for (int i = 0 ; i < m+1 ; i++){
				for (int j = i ; j < m+1 ; j++){
					// cout << j->first << " " << p->first << endl;
					// cout << "Check 71" << endl ;
					if (i == j){
						ans += (globalcosts[i][i] * (counts[i]*((counts[i]) -1))/2);
					}
					else{
						ans += (globalcosts[i][j] * counts[i] * counts[j]);
					}
				}
				// cout << "Check 72" << endl ;
			}
			// cout << "Check 70" << endl ;
			ans += cc * counts[m];

			return ans;
		}

		void print_state(){
			for (int i = 0; i < k; i++){
				cout << all_seq.at(i) << " ";
			}
			cout << endl;
			cout << "Score is: " << score << endl;
		}

		string return_output(){
			string str = "";
			for (int i = 0; i < k; i++){
				str.append(all_seq.at(i));
				str.append("\n");
			}
			return str;
		}
};

string swap_char(string str, int i, int j){
		str.erase(str.begin()+i);
		str.insert(j,"-");
		return str;
}


//Neighbours of '_' are all elements
state greedy_choose_whole(state current, int ind){
	string str = current.all_seq.at(ind);
	int min_score = current.score;
	state best_state = current;
	for (int i = 0; i < str.size(); i++){
		if (str.at(i) == '-'){
			for (int j = 0; j < i; j++){
				string new_str = swap_char(str,i,j);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = new_str;
				state new_state = current.create_new(current.n, new_seq, j);
				if (new_state.score < min_score){
					best_state = new_state;
					min_score = new_state.score;
				}
			}

			for (int j = i+1; j < str.size()-1; j++){
				string new_str = swap_char(str,i,j);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = new_str;
				state new_state = current.create_new(current.n, new_seq, i);
				if (new_state.score < min_score){
					best_state = new_state;
					min_score = new_state.score;
				}
			}
		}
	}
	return best_state;
}


//Neighbours of '_' are the elements beside it
state greedy_choose(state current, int ind){
	string str = current.all_seq.at(ind);
	int min_score = INT_MAX ;
	// int min_score = current.score;
	state best_state = current;
	for (int i = 0; i < str.size();i++){
		if (str.at(i) == '-'){
			if (i-1 >= 0 && str.at(i-1) != '-'){
				swap(str[i-1],str[i]);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = str;
				state new_state = current.create_new(current.n, new_seq, i-1, i);
				if (new_state.score <= min_score){
					
					best_state = new_state;
					min_score = new_state.score;
				}

				swap(str[i-1],str[i]);
			}
			if (i+1 < str.size() && str.at(i+1) != '-'){
				swap(str[i+1],str[i]);
				vector<string> new_seq = current.all_seq;
				new_seq.at(ind) = str;
				state new_state = current.create_new(current.n, new_seq, i, i+1);
				if (new_state.score <= min_score){
					
					best_state = new_state;
					min_score = new_state.score;
				}

				swap(str[i+1],str[i]);
			}
		}
	}
	// cout << "New state score: " << best_state.score << endl;
	if(min_score <= current.score){
		return best_state;
	}
	// return best_state;
	return current ;

}

state create_random_state(string s[], int length){
	state first(length);
	string new_s[k];
	for (int i = 0; i < k; i++){
		new_s[i] = s[i];
	}
	// cout << "Check 8" << endl ;
	for (int i = 0; i < k; i++){
		while (new_s[i].length() < length){
			int rand_int = (rand() % (new_s[i].size()));
			if (rand_int ==  new_s[i].size()-1){
				new_s[i].append("-");
			}
			else{
				new_s[i].insert(rand_int,"-");
			}
		}
		first.add(new_s[i]);
	}
	// cout << "Check 6" << endl ;
	first.compute_cost();
	// cout << "Check 7" << endl ;
	return first;
}

state generateFromPrev(state prev, int length){
	state first(length);

	string new_s[k];
	for (int i = 0; i < k; i++){
		new_s[i] = prev.all_seq[i];
	}
	
	for (int i = 0; i < k; i++){
		while (new_s[i].length() < length){
			int rand_int = (rand() % new_s[i].length());
			if (rand_int ==  new_s[i].length()-1){
				new_s[i].append("-");
			}
			else{
				new_s[i].insert(rand_int,"-");
			}
		}
		first.add(new_s[i]);
	}
	first.compute_cost();
	return first;
}

state greedy_local_search(string s[], int maxlen){
	// cout << "Check 9" << endl;
	state current = create_random_state(s,maxlen+1);
	int min_score = current.score;
	state min_state = current;
	int i = maxlen;

	// cout << "Check 10" << endl;

	while (i <= maxlen*k){
		// cout << "reaching " << i << endl;
		if (time(NULL) - start_time + 5 >= total_time*60 ){
			return min_state;
		}
		if ((i-maxlen)*k*cc >= min_score || (i==maxlen*k)){
			i = maxlen;
		}
		if(i == maxlen){
            current = create_random_state(s,i);
		}
		else{
            current = generateFromPrev(current,i) ;
		}
		// current = create_random_state(s,i);
		// cout << "Initial state:" << endl;
		// current.print_state();
		// cout << "--------------------" << endl;

		//Find next best state
		int count = 0;
		int tabu_count = 0;
		while (true){
			// cout << "Working:" << count << endl;
			state next = greedy_choose(current,count);
			// next.print_state();

			if (next.score < min_score){
				// next.print_state();
				min_score = next.score;
				min_state = next;
			}

			//min_state.print_state();

			// if (next.score > current.score){
			// 	break;
			// }
			if (next.score >= current.score){
				tabu_count++;
				if (tabu_count == 2*k){
					break;
				}
			}
			else{
				tabu_count = 0;
						current = next;
			}
			count = (count+1)%k;
		}
		i++;
	}
	// cout << "Min score: " << min_score << endl;

	return min_state;
}

int main(int argc,char** argv){
	srand(time(NULL));
	start_time = time(NULL);

	ifstream infile;
	infile.open(argv[1]);
	// infile.open("example.txt");
	// cout << "Reading from the file" << endl;
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
	units.push_back('-');
	umap['-'] = count;

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
	for(int i = 0 ; i <= m; i++){
		vector<int> temp(m+1,0);
	    for(int j = 0 ; j <= m ; j++){
	        infile >> temp[j];
	    }
	    globalcosts.push_back(temp);
	}

	// cout << umap['A'] << " " << umap['C'] << " " << umap['T'] << " " << umap['G'] << " " << umap['_'] << " " <<endl;

	// state test = create_random_state(s,maxlen+2);
	// test = create_random_state(s,maxlen+1);
	// test.print_state();
	// test.print_state();
	// string str = current.all_seq.at(1);
	// swap(str[i-1],str[i]);
	// vector<string> new_seq = current.all_seq;
	// new_seq.at(ind) = str;
	// state new_state(current.n, new_seq);

	// for (int i = 0; i <= m; i++){
	// 	for (int j = 0; j <= m; j++){
	// 		cout << globalcosts[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// first.compute_cost();
	// first.print_state();

	state min_state = greedy_local_search(s,maxlen);

	// cout << "reach 2" << endl;
	// cout << "Min state is below" << endl;
	// min_state.print_state();

	ofstream outdata; // outdata is like cin

	outdata.open(argv[2]); // opens the file
	outdata << min_state.return_output();

	// cout << min_state.return_output();
	// out


	// string test = s[0];
	// cout << "String initially:" << test << endl;
	// swap(test[2],test[3]);
	// cout << "String finally:" << test << endl;

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
