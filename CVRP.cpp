#include <iostream>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include"CVRP.hpp"

//************************************************//
//********** Constructeurs Destructeurs **********//
//************************************************//
CVRPData::CVRPData(const char* filename) : Capacity(0), NbNodes(0)
{
	ifstream infile;
	int tmp;
	infile.open(filename, ios::in);

	/* Reading of the data in file filename */
	char* ligne = new char[256];
	do {
		infile >> ligne >> ws;
	} while ((strcmp(ligne, "TYPE:") != 0) && (!infile.eof()));
	if (infile.eof()) { cout << "Bad file (type not specified) !!!" << endl;		abort(); }

	infile >> ligne >> ws;
	if (strcmp(ligne, "CVRP") == 0) {
		do {
			infile >> ligne >> ws;
		} while ((strcmp(ligne, "DIMENSION:") != 0) && (!infile.eof()));

		if (infile.eof()) { cout << "Bad File (dimension not specified) !!!" << endl;			abort(); }

		infile >> NbNodes >> ws;
		//Alloc
		Distances = new int*[NbNodes];
		for (auto i = 0; i < NbNodes; i++)
			Distances[i] = new int[NbNodes];
		Demands = new int[NbNodes];

		do {
			infile >> ligne >> ws;
		} while ((strcmp(ligne, "EDGE_WEIGHT_TYPE:") != 0) && (!infile.eof()));

		infile >> ligne >> ws;

		if (strcmp(ligne, "EXPLICIT") == 0) {
			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "EDGE_WEIGHT_FORMAT:") != 0) && (!infile.eof()));

			infile >> ligne >> ws;

			if (strcmp(ligne, "FULL_MATRIX") == 0) {
				do {
					infile >> ligne >> ws;
				} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

				infile >> Capacity >> ws;
				do {
					infile >> ligne >> ws;
				} while ((strcmp(ligne, "EDGE_WEIGHT_SECTION") != 0) && (!infile.eof()));

				if (infile.eof()) {
					cout << "Bad File (no arc specified) !!!" << endl;
					abort();
				}

				infile >> ws;

				for (auto i = 0; i < NbNodes; i++) {
					for (auto j = 0; j < NbNodes; j++) {
						infile >> Distances[i][j] >> ws;
					}
					Distances[i][i] = numeric_limits<int>::max();
				}

				do {
					infile >> ligne >> ws;
				} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

				for (auto i = 0; i < NbNodes; i++) {
					infile >> tmp >> Demands[i] >> ws;
				}


			}
			else {
				if (strcmp(ligne, "UPPER_ROW") == 0) {

					do {
						infile >> ligne >> ws;
					} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

					infile >> Capacity >> ws;

					do {
						infile >> ligne >> ws;
					} while ((strcmp(ligne, "EDGE_WEIGHT_SECTION") != 0) && (!infile.eof()));

					if (infile.eof()) {
						cout << "Bad File (no arc specified) !!!" << endl;
						abort();
					}

					infile >> ws;
					for (auto i = 0; i < NbNodes - 1; i++) {
						for (auto j = i + 1; j < NbNodes; j++) {
							infile >> Distances[i][j] >> ws;
							Distances[j][i] = Distances[i][j];
						}
						Distances[i][i] = numeric_limits<int>::max();
					}
					Distances[NbNodes - 1][NbNodes - 1] = numeric_limits<int>::max();

					do {
						infile >> ligne >> ws;
					} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

					for (auto i = 0; i < NbNodes; i++) {
						infile >> tmp >> Demands[i] >> ws;
					}

				}
				else {
					if (strcmp(ligne, "LOWER_DIAG_ROW") == 0) {
						do {
							infile >> ligne >> ws;
						} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

						infile >> Capacity >> ws;

						do {
							infile >> ligne >> ws;
						} while ((strcmp(ligne, "EDGE_WEIGHT_SECTION") != 0) && (!infile.eof()));

						if (infile.eof()) {
							cout << "Bad File (no arc specified) !!!" << endl;
							abort();
						}

						infile >> ws;
						for (auto i = 0; i < NbNodes; i++) {
							for (auto j = 0; j <= i; j++) {
								infile >> Distances[i][j] >> ws;
								Distances[j][i] = Distances[i][j];
							}
							Distances[i][i] = numeric_limits<int>::max();
						}

						do {
							infile >> ligne >> ws;
						} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

						for (auto i = 0; i < NbNodes; i++) {
							infile >> tmp >> Demands[i] >> ws;
						}

					}
					else {
						if (strcmp(ligne, "LOWER_ROW") == 0) {
							do {
								infile >> ligne >> ws;
							} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

							infile >> Capacity >> ws;

							do {
								infile >> ligne >> ws;
							} while ((strcmp(ligne, "EDGE_WEIGHT_SECTION") != 0) && (!infile.eof()));

							if (infile.eof()) {
								cout << "Bad File (no arc specified) !!!" << endl;
								abort();
							}

							infile >> ws;
							for (auto i = 0; i < NbNodes; i++) {
								for (auto j = 0; j < i; j++) {
									infile >> Distances[i][j] >> ws;
									Distances[j][i] = Distances[i][j];
								}
								Distances[i][i] = numeric_limits<int>::max();
							}

							do {
								infile >> ligne >> ws;
							} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

							for (auto i = 0; i < NbNodes; i++) {
								infile >> tmp >> Demands[i] >> ws;
							}

						}
						else {
							cout << "Edge weight format not supported" << endl;
							abort();
						}
					}
				}
			}

		}
		else if (strcmp(ligne, "EUC_2D") == 0) {
			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

			infile >> Capacity >> ws;

			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "NODE_COORD_SECTION") != 0) && (!infile.eof()));

			if (infile.eof()) {
				cout << "Bad File (no node specified) !!!" << endl;
				abort();
			}

			vector<double> coordx(NbNodes);
			vector<double> coordy(NbNodes);
			int temp;

			infile >> ws;
			for (auto i = 0; i < NbNodes; i++) {
				infile >> temp >> coordx[i] >> coordy[i] >> ws;
			}

			for (auto i = 0; i < NbNodes - 1; i++) {
				for (auto j = i + 1; j < NbNodes; j++) {
					auto xd = coordx[i] - coordx[j];
					auto yd = coordy[i] - coordy[j];
					Distances[i][j] = int(floor(sqrt(xd*xd + yd*yd) + 0.5));
					Distances[j][i] = Distances[i][j];
				}
				Distances[i][i] = numeric_limits<int>::max();
			}
			Distances[NbNodes - 1][NbNodes - 1] = numeric_limits<int>::max();

			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

			for (auto i = 0; i < NbNodes; i++) {
				infile >> tmp >> Demands[i] >> ws;
			}

		}
		else if (strcmp(ligne, "GEO") == 0) {

			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "CAPACITY:") != 0) && (!infile.eof()));

			infile >> Capacity >> ws;

			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "NODE_COORD_SECTION") != 0) && (!infile.eof()));

			if (infile.eof()) {
				cout << "Bad File (no node specified) !!!" << endl;
				abort();
			}

			vector<double> latitude(NbNodes);
			vector<double> longitude(NbNodes);
			int temp, deg;
			double xi, yi, min;

			infile >> ws;
			for (auto i = 0; i < NbNodes; i++) {
				infile >> temp >> xi >> yi >> ws;
				deg = int(floor(xi + 0.5));
				min = xi - deg;
				latitude[i] = PI_180 * (deg + 5.0 * min / 3.0);
				deg = int(floor(yi + 0.5));
				min = yi - deg;
				longitude[i] = PI_180 * (deg + 5.0 * min / 3.0);
			}

			for (auto i = 0; i < NbNodes; i++) {
				for (auto j = 0; j < NbNodes; j++) {
					if (i != j) {
						auto q1 = cos(longitude[i] - longitude[j]);
						auto q2 = cos(latitude[i] - latitude[j]);
						auto q3 = cos(latitude[i] + latitude[j]);
						Distances[i][j] = int(floor(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1)*q3)) + 1.0));
					}
				}
				Distances[i][i] = numeric_limits<int>::max();
			}

			do {
				infile >> ligne >> ws;
			} while ((strcmp(ligne, "DEMAND_SECTION") != 0) && (!infile.eof()));

			for (auto i = 0; i < NbNodes; i++)
				infile >> tmp >> Demands[i] >> ws;
		}
		else {
			cout << "Edge weight type not supported" << endl;
			abort();
		}
	}
	else cout << "Problem not supported" << endl;
	infile.close();
}

CVRPData::~CVRPData() {
	for (auto i = 0; i < NbNodes; i++)
		delete[] Distances[i];
	delete[] Distances;
	delete[] Demands;
}

//********************************//
//********** Accesseurs **********//
//********************************//
int CVRPData::GetCapacity() const { return Capacity; }
int CVRPData::GetSize() const { return NbNodes; }
int** CVRPData::GetDistances() const { return Distances; }
int CVRPData::GetDistance(const int & i, const int & j) const { return Distances[i][j]; }
int* CVRPData::GetDemands() const { return Demands; }
int CVRPData::GetDemand(const int & i) const { return Demands[i]; }

list<pair<list<int>,int>> CVRPData::ClarkWright() const
{
   list<pair<list<int>,int>> tournes;								//

	for (int i=1; i<= NbNodes; i++){
		if(Demands[i-1]!=0){
			list<int> matourne(1,i);
	 		pair<list<int>,int> mapaire(matourne,Demands[i-1]);
     		tournes.push_back(mapaire); 
	 	}
	 }

	 for(auto it=tournes.begin();it!=tournes.end();it++){
		auto it2=it;
		it2++;
		for(it2;it2!=tournes.end();it2++){ 
			if(it->second + it2->second<=Capacity){
				it->first.splice(it->first.end(),it2->first);
				it->second+=it2->second;
				it2->second=0;
			}
		}
	 }

 	 while(tournes.rbegin()->second==0){
		  tournes.pop_back();
	  }
 
 	for(auto it=tournes.begin();it!=tournes.end();it++){
		 it->first.push_back(1);
         it->first.push_front(1);
	 }
	//affichage tournees
	for(auto it=tournes.begin();it!=tournes.end();it++){
	cout<<"liste de ville : ";
	//lister les villes
	for(auto it2=it->first.begin();it2!=it->first.end();it2++)
		cout<<*it2<<"\t";	
	cout<<endl<<"Demande totale : "<<it->second<<endl;

}

	 cout<<endl;
	return tournes;
}

//*********************************************//
//********** Surcharges d'Op�rateurs **********//
//*********************************************//
ostream & operator<<(ostream & os, const CVRPData & cvrp) {
	os << "Size = " << cvrp.GetSize() << endl;
	os << "Capacity = " << cvrp.GetCapacity() << endl;
	os << "Distances :" << endl;
	for (auto i = 0; i < cvrp.GetSize(); i++) {
		for (auto j = 0; j < cvrp.GetSize(); j++) {
			os << cvrp.GetDistance(i, j) << " - ";
		}
		os << endl;
	}
	os << "Demands :" << endl;
	for (auto i = 0; i < cvrp.GetSize(); i++) {
		os << cvrp.GetDemand(i) << " - ";
	}

	return os;
}
