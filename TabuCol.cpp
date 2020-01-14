#include<fstream>
#include<iostream>
#include<vector>
#include<utility> 
#include<set> 
#include<random>
#include<map>
#include<set>
#include<iterator>
using namespace std;

int** mac_sas(int n) {
	int w, k, x;
	char o;
	int** A;
	A = new int* [n];
	for (int i = 0; i < n; i++) {
		A[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = 0;
		}
	}
	fstream file;
	file.open("le450_5a.txt", ios::in);
	if (file.good()) {
		file >> x;
		cout<< x << endl;
	
		while (!file.eof()) {
			file >> o;
			//cout << o;
			file >> w;
			file >> k;
		//	cout << w << " " << k << endl;
			A[w - 1][k - 1] = 1;
			A[k - 1][w - 1] = 1;
		}
		file.close();
	}
	return A;
}


double getRandom(double start, double finish) {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(start, finish);
	return dist(mt);
}

bool checkTabuList(int p, int c,vector<pair<int,int>> tabulist) {
	bool in_tabu_list = false;

	for (int i = 0; i < tabulist.size() && !in_tabu_list; i++) {

		if (tabulist[i].first == p && tabulist[i].second == c)
			in_tabu_list = true;
	}

	return in_tabu_list;
};
int indexTabuList(int p, int c, vector<pair<int, int>> tabulist) {
	bool in_tabu_list = false;
	int i;
	for ( i= 0; i < tabulist.size() && !in_tabu_list; i++) {

		if (tabulist[i].first == p && tabulist[i].second == c)
			return i;
	}

	return -1;
};

int calculateConflicts(int** graph, int n,vector<int>checksolution) {
	int conflict = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (graph[i][j] == 1) {
				if (checksolution.size() > 0) {
					if (checksolution[i] == checksolution[j]) {
						conflict += 1;
					}
				}
			}
		}

	}
	return conflict;

}
vector<int> firstConflicts(int** graph, int n, vector<int>checksolution) {
	int conflictnode = 0;
	set<int>con;
	vector<int>con2;
//cout << "haloo";
	bool found = false;
	//while (!found) {
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (graph[i][j] == 1 && checksolution[i] == checksolution[j]) {
					//conflictnode = i;
					//found = true;
					con.insert(i);
					con.insert(j);
					//cout << i << j;
				}
			}

		}
	
		//cout << con.size() << endl;
		set<int>::iterator itr;
		for (itr =con.begin(); itr !=con.end(); ++itr)
		{
			con2.push_back(*itr);
		//	cout << " " << *itr;
		}
	//	cout << endl;
			
	//	cout << getRandom(0, con2.size())<<endl;
	//	conflictnode = con2.at(getRandom(0, con2.size()));
	//	cout << conflictnode << endl;
	//}*/
	return con2;

}

vector<int> tabucol( int ** graph, int k,int n,int tabu_size,vector <int>currentsolution) {

	//solution<V,C>
	int kk = k-1;
	vector<pair<int, int>>tabulist;
	vector<int>move;
	vector<int>local_current;
	vector<int>empty;
	pair<int, int>bestmove;

	int nbiter = 0;
	int maxbiter = 10000;
	int conflicts = 0;

	int index = 0;
	//sprawdzanie konfliktów
	map<int, int>aspiration;
	//	int aspiration=0;
	//jeżeli rozwiązanie greedy daje kolor większy niż cel k
	for (int u = 0; u < n; u++) {
		if (currentsolution.at(u) >= k)
			currentsolution.at(u) = getRandom(0, k);
	}

	for (int u = 0; u < n; u++) {
		cout<<currentsolution.at(u)<<" ";
	}
	cout << endl;

	//zaczynamy tabu
	while (nbiter < maxbiter) {
		//cout << "kurrrqa" << endl;


		conflicts = calculateConflicts(graph, n, currentsolution);
		//tworzymy liste wierzchołków, które są w konflikcie z innym
		move= firstConflicts(graph, n, currentsolution);

		if (conflicts == 0) {
			cout << "done "<< conflicts <<"kolor: "<<k<<endl;
			
			cout << "znaleziono rozwiązanie jakie było życzeniem"<<endl;
				for (int u = 0; u < n; u++) {
					cout << currentsolution.at(u) << " ";
				}
				return currentsolution;
		}
	

		pair<int, int> best_move;
		int node;

	for(int h=0;h<100;h++){
		// node =firstConflicts(graph,n,currentsolution);
		node = move.at(getRandom(0, move.size()));
		//cout << "wylosowany node: " << node << endl;
		local_current = currentsolution;
		
			//losowanie nowego koloru
		if (kk == 0)
			kk = k-1;
		int color = getRandom(0, k);
			//kk;
	//	kk--;
				//getRandom(0, k);
			while (currentsolution.at(node) == color)
				color = getRandom(0, k);
			
			//cout << "new color: " << color << endl;
			int localcal;
				
				//if (!checkTabuList(node, color, tabulist)) {
						local_current.at(node) = color;
						 localcal = calculateConflicts(graph, n, local_current);
						// cout <<"conflicts: "<<conflicts<< " localcal: " << localcal << endl;
						if (localcal < conflicts) {
							if (aspiration[conflicts] == 0) {
								aspiration[conflicts] = conflicts - 1;
							}
							//cout << "aspiracja " << aspiration[conflicts] << endl;
							if (localcal <= aspiration[conflicts]) {
								aspiration[conflicts] = localcal - 1;
								int t = indexTabuList(node, color, tabulist);
								//cout << "t; " << t << endl;
								if (t >= 0) {
									/*for (int u = 0; u < tabulist.size(); u++) {
										cout << tabulist.at(u).first << " "
											<< tabulist.at(u).second << ";";
									}
									cout << endl;*/
									tabulist.erase(tabulist.begin() + t);
									/*for (int u = 0; u < tabulist.size(); u++) {
										cout << tabulist.at(u).first << " "
											<< tabulist.at(u).second << ";";
									}
									cout << endl;*/
								}
								}
							else {
								if (checkTabuList(node, color, tabulist)) {
									continue;
								}
							}
							break;
						}

		
	}

	

		bestmove.first = node;
		bestmove.second = currentsolution.at(node);
		//bestmove.second = local_current.at(node);
		//zmiana tabulisty
		if (tabulist.size() > tabu_size) {
			tabulist.erase(tabulist.begin());
			//cout << endl;
		}

		tabulist.push_back(bestmove);
		/*cout << endl;
		for (int u = 0; u < tabulist.size(); u++) {

			cout<< tabulist.at(u).first << " "
				<< tabulist.at(u).second  << ";";
		}
		cout << endl;*/
		currentsolution = local_current;

	
		//teracje na plus
		nbiter += 1;
	}



	
	return empty;
}

vector<int>  Greedy(int** A, int n) {
	vector<int>solution;
	//int* Col = new int[n];
	bool* Av = new bool[n];
	solution.push_back(0);
	for (int i = 1; i < n; i++) {
		solution.push_back(-1);
	}
	//Col[0] = 0; //pierwszy wierzcho³ek z góry ma kolor 0
	for (int j = 0; j < n; j++) {
		Av[j] = false;
	}
	for (int i = 1; i < n; i++) {

		for (int k = 0; k < n; k++) {
			if (solution.at(k) != -1 && A[i][k] == 1) {
				Av[solution.at(k)] = true;
			}
		}
		int l;
		for (l = 0; l < n; l++) {
			if (Av[l] == false)
				break;
		}
		solution.at(i) = l;
		for (int k = 0; k < n; k++) {
			if (solution.at(k) != -1 && A[i][k] == 1)
				Av[solution.at(k)] = false;
		}
	}
	cout << "greedy:" << endl;
	for (int u = 0; u < n; u++) {
		cout 			<< solution.at(u) <<" ";
	}
	int m = 0;
	for (int i = 0; i < n; i++) {
		if ((solution.at(i) + 1) > m) {
			m = solution.at(i) + 1;

		}
	}
	delete[] Av;
	cout<<"MAX: "<<m<<endl;
	return solution;

}





int main()
{
	int n,k;
	n = 450;
	int** tab = mac_sas(n);
	vector<int>currentsolution;
	bool result;
	/*for (int i = 0; i < 50; i++) {
		//int x = getRandom(0, 14);
		cout <<getRandom(0,14) << endl;
	}*/
	currentsolution = Greedy(tab, n);
	//return 0;
	//currentsolution.at(1) = 0;
	result = true;
	k = 13;
	
	
	while(currentsolution.size()>0){
		currentsolution =tabucol(tab,k, n, 7, currentsolution);
		k--;
	}
	//cout << "v: " << v;
	delete[]tab;
	return 0;
}

