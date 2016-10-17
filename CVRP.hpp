#include <iostream>
#include <list>
using namespace std;
const double PI = 3.1415926535;
const double PI_180 = PI / 180;
const double RRR = 6378.388;

class CVRPData {
private:
	int Capacity;
	int NbNodes;
	int ** Distances;
	int * Demands;

public:
	//********** Constructeur & Destructeur **********//
	CVRPData(const char* filename);
	~CVRPData();

	//********** Accesseurs **********//
	int GetCapacity() const;
	int GetSize() const;
	int** GetDistances() const;
	int GetDistance(const int & i, const int & j) const;
	int* GetDemands() const;
	int GetDemand(const int & i) const;

	//********** Clark & Wright **********//
	list<pair<list<int>,int>> ClarkWright() const;

	friend ostream & operator<<(ostream & os, const CVRPData & cvrp);
};

