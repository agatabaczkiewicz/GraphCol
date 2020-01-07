#include<fstream>
#include<iostream>
#include<vector>
#include<utility>
#include<set>
#include<random>

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


int getRInt(int x){
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,x);
    return distribution(generator);

}

bool checkTabuList(int p, int c,vector<pair<int,int>> tabulist) {
	bool in_tabu_list = false;

	for (int i = 0; i < tabulist.size() && !in_tabu_list; i++) {

		if (tabulist[i].first == p && tabulist[i].second == c)
			in_tabu_list = true;
	}

	return in_tabu_list;
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
int firstConflicts(int** graph, int n, vector<int>checksolution) {
	int conflictnode = 0;
	bool found = false;
	while (!found) {
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (graph[i][j] == 1&& checksolution[i] == checksolution[j]) {
						conflictnode = i;
						found = true;

				}
			}

		}
	}
	return conflictnode;

}

void tabucol( int ** graph, int k,int n,int tabu_size,vector <int>currentsolution) {

	//solution<V,C>
	int kk = k;
	vector<pair<int, int>>tabulist;
	vector<vector<int>>critical_solutions;
	vector<int>bestsolution;
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

	//jeżeli rozwiązanie greedy daje kolor większy niż cel k
	for (int u = 0; u < n; u++) {
		if (currentsolution.at(u) >= k)
			currentsolution.at(u) = getRInt(k);
	}

	for (int u = 0; u < n; u++) {
		cout<<currentsolution.at(u)+1<<" ";
	}
	cout << endl;

	//zaczynamy tabu
	while (nbiter < maxbiter) {
		cout << "kurrrqa" << endl;


		conflicts = calculateConflicts(graph, n, currentsolution);
		cout << conflicts;

		if (conflicts == 0) {
			cout << "done" << endl;
			if (bestsolution.size() > 0) {
				for (int u = 0; u < n; u++) {
					cout << bestsolution.at(u) + 1 << " ";

				}
				cout << "znaleziono rozwiązanie jakie było życzeniem";
				break; //znaleziono rozwiązanie jakie było życzeniem
			}
			else
				break;

		}

		//looking for the neibours
		int failed_neighborhood = 0;



		set<int> neighborhood;
		pair<int, int> best_move;
		set<int>::iterator it;
		local_current = bestneighbour = currentsolution;
		//numer pierwszego wierzchołka który powoduje konflikt
		int node =firstConflicts(graph,n,currentsolution);
	//	cout << "node: " << node << endl;

	//	while (neighborhood.size() != k) {

	for(int h=0;h<100;h++){
			if (failed_neighborhood == neighborhood_size * 2) {
				node = getRInt(n);
				//if (critical_solutions.size() > 0) {
				//	local_current = critical_solutions.at(critical_solutions.size() - 1);
				//	critical_solutions.pop_back();
				//}
				failed_neighborhood = 0;
			}

			found = false;
			//losowanie nowego koloru
			if (kk == 0)
				kk = k;
			int color = kk;
			kk--;
				//getRandom(0, k);
			while (currentsolution.at(node) == color)
				color = getRInt(k);
			//Check if that color is already in the neighborhood
			if (neighborhood.size() > 0) {
				it = neighborhood.find(color);
				if (it != neighborhood.end())
					found = true;
			}


			//jeżeli koloru nie ma w sąsiedztwie i ruch nie jest tabu
			if (!found && !checkTabuList(node, color, tabulist)) {
				//cout << "jestem";
				neighborhood.insert(color);
				local_current.at(node) = color;   //lokalne rozwiązanie zmienia wierzchołek na ten kolor
				int localcal = calculateConflicts(graph, n, local_current);  //oblicza ilość konfliktów tego rozwiązania

				if (bestneigbourcal > localcal||index==0) { //jeżeli dotychczas najlepszy sąsiad miał więcej konflikót to teraz jest nim local lub przy pierwszej iteracji
					index++;
					bestneighbour = local_current;
					bestneigbourcal = localcal;
				}
			}
			else {
				//neighborhood.clear();
				failed_neighborhood++;


			}
		}



		bestmove.first = node;
		bestmove.second = bestneighbour.at(node);

		//zmiana tabulisty
		if (tabulist.size() > tabu_size) {
			tabulist.erase(tabulist.begin());
		}

		tabulist.push_back(bestmove);

		for (int u = 0; u < tabulist.size(); u++) {

			cout << "tabu" << endl << "Wierzcholek " << tabulist.at(u).first + 1 << " Kolor: "
				<< tabulist.at(u).second + 1 << endl;
		}

		currentsolution = bestneighbour;

		if (calculateConflicts(graph, n,bestsolution) > calculateConflicts(graph, n, currentsolution)) {
			bestsolution = currentsolution;
			//critical_solutions.push_back(currentsolution);
		}
		//teracje na plus
		nbiter += 1;
	}




	cout << "po tabu "<<"conflicts: "<<conflicts << endl;

	cout << "koniec";
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
	int n;
	n = 450;
	int** tab = mac_sas(n);
	vector<int>currentsolution;
	currentsolution = Greedy(tab, n);
	//currentsolution.at(1) = 2;
	tabucol(tab,12 , n, 7, currentsolution);
	delete[]tab;
	return 0;
}

