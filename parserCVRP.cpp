#include "CVRP.hpp"
#include<iostream>
#include<list>
using namespace std;

int main(int argc, char** argv){

  if (argc > 1) {
    CVRPData cvrp (argv[1]);
    std::cout << cvrp << std::endl;
    cvrp.ClarkWright();
/*
    cout<<"la taille est: "<<cvrp.GetSize()<<endl;
    
  

cout<<"ma matrice distance "<<endl;
for (int i = 0; i < cvrp.GetSize(); i++){
	  for (int j =0 ; j < cvrp.GetSize(); j++){
          cout<<cvrp.GetDistance(i,j)<<"\t";
      }
      cout<<endl;
 }
 //*/
  }
  return 0;

}
