#include "System.h"
//#include "Simu.h"


using namespace std;

extern vector <cCircuit> vModule_list;
extern vector <cGate*> Topological_list;
extern cLib_Cell LibHead;


void CalAreaPower(){
  double AnsArea=0, AnsPower=0;
  cGate *gptr;
  for(int i=0 ; i<vModule_list.size() ; i++){
    gptr = vModule_list[i].Gate_list_head.pNext;
    while( gptr!= NULL){
      if(gptr->dArea == 0 || gptr->dPower == 0){
        cout<<"error\n";
        int x;
        cin>>x;
      }
      AnsArea += gptr->dArea;
      AnsPower += gptr->dPower;
      gptr = gptr->pNext;
    }
    cout<<"Module "<<i<<", cell area: "<<AnsArea<<endl;
  }
}

void CalSignalprobability(cGate *Gptr){
  if( Gptr->dSigDenominator != 0){
    Gptr->dSig0probability = Gptr->dSig0Nominator / Gptr->dSigDenominator;
    Gptr->dSig1probability = Gptr->dSig1Nominator / Gptr->dSigDenominator;
  }

}

void PassSignal(cGate *Gptr){
  vector<bool> vStack;
  //cout<<Gptr->sName<<", "<<Gptr->sFootprint<<", "<<Gptr->sFunc<<endl;
  for(int i=0 ; i<Gptr->sFunc.length() ; i++ ){
    if( Gptr->sFunc[i] == 42 ){// *
      bool v1 = vStack[ vStack.size()-1 ];
      bool v2 = vStack[ vStack.size()-2 ];
      bool v3 = v1 & v2;
      vStack.pop_back();
      vStack.pop_back();
      vStack.push_back(v3);
    }
    else if( Gptr->sFunc[i] == 43 ){// +
      bool v1 = vStack[ vStack.size()-1 ];
      bool v2 = vStack[ vStack.size()-2 ];
      bool v3 = v1 | v2;
      vStack.pop_back();
      vStack.pop_back();
      vStack.push_back(v3);
    }
    else if( Gptr->sFunc[i] == 39 ){// '
      bool v1 = vStack[ vStack.size()-1 ];
      bool v2 = !v1;
      vStack.pop_back();
      vStack.push_back(v2);
    }
    else if( Gptr->sFunc[i] == 94 ){// ^
      bool v1 = vStack[ vStack.size()-1 ];
      bool v2 = vStack[ vStack.size()-2 ];
      bool v3 = v1 ^ v2;
      vStack.pop_back();
      vStack.pop_back();
      vStack.push_back(v3);
    }
    /*else if( Gptr->sFunc[i] == 33 ){// !
      cout<<"for !"<<endl;
      int x;
      cin>>x;
      Gptr->vWire_list[ (int)(Gptr->sFunc[i+1]-48) ]->value = !Gptr->vWire_list[ (int)(Gptr->sFunc[i+1]-48) ]->value;
    }*/
    else{
      vStack.push_back( Gptr->vWire_list[ (int)(Gptr->sFunc[i]-48) ]->value );
      //cout<<Gptr->vWire_list[(int)(Gptr->sFunc[i]-48)]->sName<<", "<<Gptr->vWire_list[ (int)(Gptr->sFunc[i]-48) ] ->value<<endl;
    }
  }


  if( Gptr->sFootprint == "SDFF" ){//把值寫回去output net
    if( Gptr->vWire_list[ Gptr->vWire_list.size()-1 ] != NULL ){//for QN
      //cout<<"QN net: "<<Gptr->vWire_list[ Gptr->vWire_list.size()-1 ]->sName<<", value: "<<!vStack[ vStack.size()-1 ]<<endl;
      Gptr->vWire_list[ Gptr->vWire_list.size()-1 ]->value = !vStack[ vStack.size()-1 ];// QN
    }

    if( Gptr->vWire_list[ Gptr->vWire_list.size()-2 ] != NULL ){//for Q
      //cout<<"Q net: "<<Gptr->vWire_list[ Gptr->vWire_list.size()-2 ]->sName<<", value: "<<vStack[ vStack.size()-1 ]<<endl;
      Gptr->vWire_list[ Gptr->vWire_list.size()-2 ]->value = vStack[ vStack.size()-1 ];//Q
    }
  }
  else{
    //cout<<"Output net: "<<Gptr->vWire_list[ Gptr->vWire_list.size()-1 ]->sName<<", value: "<<vStack[ vStack.size()-1 ]<<endl;
    Gptr->vWire_list[ Gptr->vWire_list.size()-1 ]->value = vStack[ vStack.size()-1 ];
  }

  Gptr->value =  vStack[ vStack.size()-1 ];
  if( !Gptr->value ){//出現訊號0 訊號0的次數++
    Gptr->dSig0Nominator += 1 ;
  }
  else{//出現訊號1 訊號0的次數++
    Gptr->dSig1Nominator += 1 ;
  }
  Gptr->dSigDenominator += 1 ;//分母一直++

  vStack.pop_back();
}

void TestTPLGOrder(){
  int k=0;
	bool flag =false;
	cGate *gptr = &vModule_list[vModule_list.size()-1].Gate_list_head;
	while( gptr->pNext != NULL ){
		//cout<<"Gate Name: "<<gptr->sName<<endl;
		gptr = gptr->pNext;
		flag =false;
		for(int j=0 ; j<Topological_list.size() ;j++){
			if( !gptr->sName.compare(Topological_list[j]->sName) ){
				flag =true;
        break;
			}
		}
		if(flag == false){
      cout<<gptr->sFootprint<<" loss gate name: "<<gptr->sName<<endl;
      for(int i=0 ; i<gptr->vWire_list.size() ;i++){
          cout<<gptr->vWire_list[i]->sName<<endl;
      }
			k++;
		}

	}
  cout<<"Topological_list size: "<<Topological_list.size()<<endl;
	cout<<"Loss gate count: "<<k<<endl;
  /*
  for(int i=Topological_list.size()-1 ; i>=0 ;i--){
		cout<<"Order: "<<Topological_list[i]->sName<<endl;
	}
  */
}
void FindTPLGOrder(cWire *Wptr){
  cGate *Gptr;
  //cout<<"Wire: "<<Wptr->sName<<endl;

  if( Wptr->Type == PO && Wptr->vCnnctGate_list.size() == 0 ){ //表示net is PO
    return ;//add gates into order list
  }
  else{
    for(int i=0 ; i < Wptr->vCnnctGate_list.size() ; i++){
      Gptr = Wptr->vCnnctGate_list[i];

      if( Gptr->TPLGVisit ){//如果這個gate被走訪過就直接換下一個gate
        continue;
      }
      Gptr->TPLGVisit = true;

      if( Gptr->sFootprint == "SDFF" ){ //for SDFF with two outputs
        int OptNum1 = Gptr->vWire_list.size()-1; //QN
        int OptNum2 = Gptr->vWire_list.size()-2; //Q

        /*if( Gptr->vWire_list.size()!=6 ){
          cout<<"error size: "<<Gptr->vWire_list.size()<<", Name: "<<Gptr->sName<<endl;
          for(int j=0 ; j<Gptr->vWire_list.size() ;j++){
            cout<<"Net: "<<Gptr->vWire_list[j]->sName<<endl;
          }
          int x;
          cin>>x;
        }*/
        if(Gptr->vWire_list[OptNum2]!=NULL ){ //maybe no nets connected to port for Q
          FindTPLGOrder(Gptr->vWire_list[OptNum2]);
        }

        if(Gptr->vWire_list[OptNum1]!=NULL ){ //maybe no nets connected to port for QN
          FindTPLGOrder(Gptr->vWire_list[OptNum1]);
        }
      }
      else{ //for other gates with only one output
        int OptNum = Gptr->vWire_list.size()-1; //get output net of the gate
        if( Gptr->vWire_list[OptNum] != NULL ){
          FindTPLGOrder(Gptr->vWire_list[OptNum]);
        }
      }
      Topological_list.push_back(Gptr);
    } //end for

    return ;
  }
  return ;
}

void TestSimution(){
  for(int j=0 ; j < vModule_list.size() ; j++){
    for(int k=0 ; k < vModule_list[j].vWire_list.size() ;k++){
      if( vModule_list[j].vWire_list[k].Type == PI ){
        cout<<"PI: "<<vModule_list[j].vWire_list[k].sName<<", value: "<<vModule_list[j].vWire_list[k].value<<endl;
      }
    }
    //cout<<"ramdom num: "<<rand()%2<<endl;
  }
}

void Simulation(){ // for input pattern
  unsigned seed = (unsigned)time(NULL);
  srand(seed);
  for(int i=0 ; i<10000 ; i++){
    for(int j=0 ; j < vModule_list.size() ; j++){
      for(int k=0 ; k < vModule_list[j].vWire_list.size() ;k++){
        if( vModule_list[j].vWire_list[k].Type == PI ){
          vModule_list[j].vWire_list[k].value = rand()%2;
        }
      }
      //cout<<"size: "<<Topological_list.size()<<endl;
      for( int k=Topological_list.size()-1 ; k >=0 ; k--){ //pass signal according to Topological order
        PassSignal( Topological_list[k] );
      }
      //int x;
      //cin>>x;//?????
      //cout<<"ramdom num: "<<rand()%2<<endl;
    }
  }
  for(int i=Topological_list.size()-1 ; i>=0 ; i--){
    CalSignalprobability( Topological_list[i] );
    //cout<<"Signal 0 probability: "<<Topological_list[i]->dSig0probability<<" "<<Topological_list[i]->sName<<endl;
    /*if(Topological_list[i]->dSig0probability == 0){
      cout<<"signal probability is 0: "<<Topological_list[i]->sName<<endl;
      cout<<"count: "<<Topological_list[i]->dSigDenominator<<endl;
    }*/
  }
}
