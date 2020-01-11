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
	file.open("queen6.txt", ios::in);
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

bool tabucol( int ** graph, int k,int n,int tabu_size,vector <int>currentsolution) {

	//solution<V,C>
	int kk = k;
	vector<pair<int, int>>tabulist;
	vector<vector<int>>critical_solutions;
	vector<int>move;
	vector<int>bestneighbour;
	vector<int>local_current;
	pair<int, int>bestmove;
	//vector<int>neigbours;
	int nbiter = 0,bestneigbourcal=0;
	int maxbiter = 10000;
	int conflicts = 0,neighborhood_size=n/2;
	bool found;
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
		cout<<currentsolution.at(u)+1<<" ";
	}
	cout << endl;

	//zaczynamy tabu
	while (nbiter < maxbiter) {
		//cout << "kurrrqa" << endl;


		conflicts = calculateConflicts(graph, n, currentsolution);
	//	;
		move= firstConflicts(graph, n, currentsolution);
		if (conflicts == 0) {
			cout << "done "<< conflicts <<"kolor: "<<k<<endl;
			//if (bestsolution.size() > 0) {
				for (int u = 0; u < n; u++) {
					cout << currentsolution.at(u) + 1 << " ";

				}
				cout << "znaleziono rozwiązanie jakie było życzeniem";
				//break; //znaleziono rozwiązanie jakie było życzeniem
				return true;
			//}
			//else
			//	break;
	
		}

		//looking for the neibours
		int failed_neighborhood = 0;
	

		set<int> neighborhood;
		pair<int, int> best_move;
		set<int>::iterator it;
		
		bestneighbour = currentsolution; 
		//numer pierwszego wierzchołka który powoduje konflikt
		
	//	cout << "node: " << node << endl;

	//	while (neighborhood.size() != k) {
		int node;
	for(int h=0;h<100;h++){
		// node =firstConflicts(graph,n,currentsolution);
		node = move.at(getRandom(0, move.size()));
		// cout << "node " << node << endl;
			/*if (failed_neighborhood == neighborhood_size * 2) {
				node = getRandom(0, n);
				//if (critical_solutions.size() > 0) {
				//	local_current = critical_solutions.at(critical_solutions.size() - 1);
				//	critical_solutions.pop_back();
				//}
				failed_neighborhood = 0;
			}*/
		local_current = currentsolution;
		found = false;
			//losowanie nowego koloru
		if (kk == 0)
			kk = k;
		int color = kk; //= getRandom(0, k);
			//kk;
		kk--;
				//getRandom(0, k);
			while (currentsolution.at(node) == color)
				color = getRandom(0, k);
			//Check if that color is already in the neighborhood
			//cout << "color " << color << endl;
		/*	if (neighborhood.size() > 0) {
				it = neighborhood.find(color);
				if (it != neighborhood.end())
					found = true;
			}*/

		//	if (!found) {
			//	neighborhood.insert(color);
				local_current.at(node) = color;
				int localcal = calculateConflicts(graph, n, local_current);
				
				if (localcal < conflicts) {
					aspiration[conflicts] = conflicts - 1;
					if (localcal <= aspiration[conflicts]) {
						aspiration[conflicts] = localcal - 1;
						int t = indexTabuList(node, color, tabulist);
						if (t>=0) {
							tabulist.erase(tabulist.begin()+t);
						}
					}

				}
		//	}

			//jeżeli koloru nie ma w sąsiedztwie i ruch nie jest tabu
			/*if (!found && !checkTabuList(node, color, tabulist)) {
				//cout << "jestem";
				neighborhood.insert(color);
				local_current.at(node) = color;   //lokalne rozwiązanie zmienia wierzchołek na ten kolor
				int localcal = calculateConflicts(graph, n, local_current);  //oblicza ilość konfliktów tego rozwiązania

				if (bestneigbourcal >= localcal||index==0) { //jeżeli dotychczas najlepszy sąsiad miał więcej konflikót to teraz jest nim local lub przy pierwszej iteracji
					index++;
					bestneighbour = local_current;
					bestneigbourcal = localcal;
				}
			}*/
			else {
				//neighborhood.clear();
				//failed_neighborhood++;
				if (checkTabuList(node, color, tabulist)) {
					continue;
				}


			}
	}

	

		bestmove.first = node;
		bestmove.second = local_current.at(node);

		//zmiana tabulisty
		if (tabulist.size() > tabu_size) {
			tabulist.erase(tabulist.begin());
		}

		tabulist.push_back(bestmove);

		/*for (int u = 0; u < tabulist.size(); u++) {

			cout << "tabu" << endl << "Wierzcholek " << tabulist.at(u).first + 1 << " Kolor: "
				<< tabulist.at(u).second + 1 << endl;
		}*/

		currentsolution = local_current;

	//	if (calculateConflicts(graph, n,bestsolution) >= calculateConflicts(graph, n, currentsolution)) {
	//		bestsolution = currentsolution;
			//critical_solutions.push_back(currentsolution);
	//	}
		//teracje na plus
		nbiter += 1;
	}



	
	cout << "po tabu "<<"conflicts: "<<conflicts << endl;
	
	cout << "koniec";
	return false;
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
		cout 			<< solution.at(u) + 1 <<" ";
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
	n = 36;
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
	k = 11;
	//int v = firstConflicts(tab, n, currentsolution);
	while(result==true){
		result=tabucol(tab,k, n, 7, currentsolution);
		k--;
	}
	//cout << "v: " << v;
	delete[]tab;
	return 0;
}

