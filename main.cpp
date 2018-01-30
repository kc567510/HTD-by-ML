#include "System.h"
#include "Read_File.h"
#include "Simu.h"
#include "Segment.h"

using namespace std;
vector <cCircuit> vModule_list;
cLib_Cell LibHead;
vector<cGate*> Topological_list;

int main(int argc, char* argv[] )
{
	clock_t start, finish;//update
	start = clock();   //update

	cout<<"./Dectector [Circuit.v]"<<endl;

	cout<<"Reading Library....."<<endl;
	Read_Lib();
	cout<<"Reading Library Finish."<<endl;
	TestLIB();

	cout<<"Reading Circuit....."<<endl;
	Read_Circuit(argv[1]);
	cout<<"Reading Circuit Finish."<<endl;


	int i=0;
	cGate *gptr = &vModule_list[vModule_list.size()-1].Gate_list_head;
	while( gptr->pNext != NULL ){
		//cout<<"Gate Name: "<<gptr->sName<<endl;
		gptr = gptr->pNext;
		i++;
	}
	cout<<"Gate count: "<<i<<endl;


	cout<<"Find TPLG Order....."<<endl;
	for(int i=0 ; i < vModule_list.size()  ; i++){
    for(int j=0 ; j < vModule_list[i].vWire_list.size() ; j++){
      if( vModule_list[i].vWire_list[j].Type == PI ){
				if(vModule_list[i].vWire_list[j].sName == "CK" || vModule_list[i].vWire_list[j].sName == "test_se"){
	        continue; //Pass net: CK
	      }
        FindTPLGOrder(&vModule_list[i].vWire_list[j]);//從PI開始
			}
		}
	}
	cout<<"Find TPLG Order Finish"<<endl;
	TestTPLGOrder();

	cout<<"Start Simulation....."<<endl;
	Simulation();
	cout<<"Finish Simulation"<<endl;
 	//TestSimution();
	//CalAreaPower();

	cout<<"Start Find Each Features of Every Path....."<<endl;
	string OutFileName;
	for(int i=0 ; i<strlen(argv[1])-2 ; i++ ){
		OutFileName += argv[1][i];
	}
	OutFileName += "_path_feature.txt";
	FindPath(OutFileName);
 	cout<<"Finish Find Each Features of Every Path....."<<endl;

	finish = clock();
	cout<<(finish-start)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
