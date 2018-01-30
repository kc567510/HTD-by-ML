#include "System.h"
//#include "Read_File.h"


using namespace std;
extern vector <cCircuit> vModule_list;
extern cLib_Cell LibHead;
cGate *GTailptr = NULL;


void TestLIB(){
  int i=0;
	cLib_Cell *lptr = LibHead.pNext;
	while( lptr != NULL ){
    i++;
    //cout<<i<<" "<<lptr->sName<<", area: "<<lptr->dArea<<",\t power: "<<lptr->dPower<<",\t #ports: "<<lptr->iNumPort<<endl;
		lptr = lptr->pNext;

	}
	cout<<"Lib Cell count: "<<i<<endl;
}

string BuildPostfix(string sFunc){//lib cell->sFunc 傳進來可以做比較少次
  vector <char> vFuncStack;
  string sAns;
  sAns.clear();
  //cout<<sFunc<<endl;
  for(int i=0 ; i<sFunc.length() ; ){

    if( sFunc[i] == 40 ){// (
      vFuncStack.push_back(sFunc[i]);
      i++;
    }
    else if( sFunc[i] == 41){// )
      for(int j=vFuncStack.size()-1 ; vFuncStack[j] != 40 ;j--){
        sAns += vFuncStack[j];
        vFuncStack.pop_back();
      }
      vFuncStack.pop_back();
      i++;
    }
    else if( sFunc[i] == 42 ){// *
      vFuncStack.push_back(sFunc[i]);
      i++;
    }
    else if( sFunc[i] == 43 ){// +
      vFuncStack.push_back(sFunc[i]);;
      i++;
    }
    else if( sFunc[i] == 39){// '
      sAns += sFunc[i];
      i++;
    }
    else if( sFunc[i] == 94 ){// ^
      vFuncStack.push_back(sFunc[i]);
      i++;
    }
    else if( sFunc[i] == '1' && sFunc.length()==1 ){
      sAns += '1';
      i++;
    }
    else if( sFunc[i] == '0' && sFunc.length()==1 ){
      sAns += '0';
      i++;
    }
    else{
      string sTemp;
      sTemp.clear();
      while( sFunc[i] != 39 && sFunc[i] != 40 && sFunc[i] != 41 && sFunc[i] != 42
          && sFunc[i] != 43 && sFunc[i] != 94 && i<sFunc.length() ){
        sTemp += sFunc[i];
        i++;
      }

      if( sTemp == "ISO"){
        sTemp.clear();
        sAns += '1';
      }
      else if( sTemp == "ENB"){
        sTemp.clear();
        sAns += '1';
      }
      else if( sTemp == "D"){
        sTemp.clear();
        sAns += '0';
      }
      else if( sTemp == "SI"){
        sTemp.clear();
        sAns += '1';
      }
      else if( sTemp == "SE"){
        sTemp.clear();
        sAns += '2';
      }
      else if( sTemp == "A"){
        sTemp.clear();
        sAns += '0';
      }
      else if( sTemp == "A1"){
        sTemp.clear();
        sAns += '0';
      }
      else if( sTemp == "A2"){
        sTemp.clear();
        sAns += '1';
      }
      else if( sTemp == "A3"){
        sTemp.clear();
        sAns += '2';
      }
      else if( sTemp == "A4"){
        sTemp.clear();
        sAns += '3';
      }
      else if( sTemp == "A5"){
        sTemp.clear();
        sAns += '4';
      }
      else if( sTemp == "A6"){
        sTemp.clear();
        sAns += '5';
      }
      //sAns += sTemp;
    }
  }
  if( vFuncStack.size()>0 ){
    for(int j=vFuncStack.size()-1 ; j>=0 ;j--){
      sAns += vFuncStack[j];
      vFuncStack.pop_back();
    }
  }
  //cout<<sAns<<endl;

  return sAns;
}

void CnnctNetGate(cGate *GNode, string sNet, bool Outputflag){ //Outport flag=1, Inputport flag=0
  for(int i=0 ; i < vModule_list[vModule_list.size()-1].vWire_list.size() ; i++){
    if( !sNet.compare( vModule_list[vModule_list.size()-1].vWire_list[i].sName ) ){
      cWire *Wptr;
      Wptr = &vModule_list[vModule_list.size()-1].vWire_list[i];
      GNode->vWire_list.push_back(Wptr);
      if(!Outputflag){
        Wptr->vCnnctGate_list.push_back(GNode);
      }
      break;
    }
  }
}

string GetNetName(string sNet){
  size_t start, end;
  start = sNet.find("(")+1;
  end = sNet.find( ")" ) - start;
  sNet = sNet.substr( start, end );
  return sNet;
}

void AddGateList(cGate *GNode){
  if(vModule_list[vModule_list.size()-1].Gate_list_head.pNext == NULL){
    GTailptr = &vModule_list[vModule_list.size()-1].Gate_list_head;
    GTailptr->pNext = GNode;
    GTailptr = GNode;
  }
  else{
    GTailptr->pNext = GNode;
    GTailptr = GNode;
  }
}

void AddLSDN(){//寫死這兩個cell?????

  cLib_Cell *lptr = &LibHead;
  while( lptr->pNext != NULL){
    lptr = lptr->pNext;
  }

  cLib_Cell *LNode = new cLib_Cell;
  LNode->sName = "LSDNENX1";
  LNode->sFootprint = "LSDNEN";
  LNode->dArea = 7.116032;
  LNode->dPower = 2.145841e+05;
  LNode->sFunc = "1'0*";//alreadey postfix (!EN)*A
  LNode->iNumPort = 3;
  lptr->pNext = LNode;
  lptr = LNode;

  cLib_Cell *CNode = new cLib_Cell;
  CNode->sName = "LSDNX1";
  CNode->sFootprint = "LSDN";
  CNode->dArea = 7.116032;
  CNode->dPower = 3.460700e+05;
  CNode->sFunc = "0";
  CNode->iNumPort = 2;
  lptr->pNext = CNode;


}

void Read_Circuit(char* Filename){
	bool PIFlag = false, POFlag = false, WireFlag = false;
	char cIptData[1000], *tok;
  string sNet;

	fstream fin;
	fin.open(Filename, ios::in);
	if(!fin){
		cout<<"Open file fail."<<endl;
		exit(1);
	}
	while( fin.getline(cIptData, 1000) ){
		string sIptData = cIptData;
		//cout<<sIptData<<endl;

		if( sIptData.find("endmodule") != string::npos ){//？？？？？？尚未考慮多個module的情況, 目前只有考慮一個module的情況
			cout<<"# of Wire: "<<vModule_list[0].vWire_list.size()<<endl;
			//cout<<"Capacity of Wire: "<<vModule_list[0].vWire_list.capacity()<<endl;
			break;
		}

		if( sIptData.find("module") != string::npos ){
			//cout<<"find module"<<endl;
			cCircuit CNode;
  			CNode.sName = sIptData.substr( sIptData.find("module")+7, sIptData.find( " (" ) - (sIptData.find("module")+7) );//get module name
  			//cout<<CNode.sName<<endl;
  			vModule_list.push_back(CNode);//add module into module_list ????可能需要最後再加入
		}
		else if( sIptData.find("input") != string::npos ){
			PIFlag = true;
			size_t start=0, end=0;
			start = sIptData.find("input")+6;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire INode;
				INode.Type = PI;
  			INode.sName = sIptData.substr( start, end );

  			vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(INode);
				start =  start+ end + 2;
				end = sIptData.find(",",start,1)-start;
			}
		}
		else if( PIFlag ){
			if(sIptData.find(";") != string::npos ){
				PIFlag = false;
			}
			size_t start=0, end=0;
			start = 9;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire INode;
				INode.Type = PI;
  				INode.sName = sIptData.substr( start, end );
  				vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(INode);
				start =  start + end + 2;
				end = sIptData.find(",",start,1)-start;
			}
			//break;
		}
		else if( sIptData.find("output") != string::npos){
			POFlag = true;
			size_t start=0, end=0;
			start = sIptData.find("output")+7;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos  ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire ONode;
				ONode.Type = PO;
  				ONode.sName = sIptData.substr( start, end );
  				vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(ONode);
				start =  start+ end + 2;
				end = sIptData.find(",",start,1)-start;
			}
		}
		else if( POFlag ){
			if(sIptData.find(";") != string::npos ){
				POFlag = false;
			}
			size_t start=0, end=0;
			start = 9;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire ONode;
				ONode.Type = PO;
  				ONode.sName = sIptData.substr( start, end );
  				vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(ONode);
				start =  start + end + 2;
				end = sIptData.find(",",start,1)-start;
			}
		}
		else if( sIptData.find("wire") != string:: npos ){
			WireFlag = true;
			size_t start=0, end=0;
			start = sIptData.find("wire")+7;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos  ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire WNode;
				WNode.Type = Wire;
  				WNode.sName = sIptData.substr( start, end );
          //cout<<WNode.sName<<" "<<WNode.sName.length()<<endl;
  				vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(WNode);
				start =  start+ end + 2;
				end = sIptData.find(",",start,1)-start;

			}
		}
		else if( WireFlag ){
			if(sIptData.find(";") != string::npos ){
				WireFlag = false;
			}
			size_t start=0, end=0;
			start = 9;
			end = sIptData.find(",")-start;

			while( sIptData.find(",",start,1) != string::npos ){
				//cout<<sIptData.substr( start, end )<<endl;
				cWire WNode;
				WNode.Type = Wire;
  				WNode.sName = sIptData.substr( start, end );
  				vModule_list[ vModule_list.size()-1 ].vWire_list.push_back(WNode);
				start =  start + end + 2;
				end = sIptData.find(",",start,1)-start;
			}
		}
		else if( sIptData.find("assign") != string::npos ){//?????cant handle assign g123 = 1'b1';
			char cTo[100], cFrom[100];
			cWire *Optr1, *Optr2;
			sscanf(cIptData,"  assign %s = %s;",cTo, cFrom);
			cFrom[strlen(cFrom)-1] = '\0';
			//cout<<cTo<<" "<<cFrom<<endl;
			for( int i=0 ; i < vModule_list[ vModule_list.size()-1 ].vWire_list.size() ; i++){
				if( !vModule_list[ vModule_list.size()-1 ].vWire_list[i].sName.compare(cTo) ){
					Optr1 = &vModule_list[ vModule_list.size()-1 ].vWire_list[i];
					//cout<<i<<" "<<"TO: "<<cTo<<" "<<vModule_list[ vModule_list.size()-1 ].vPO_list[i].sName<<endl;
				}
			}
			for( int i=0 ; i < vModule_list[ vModule_list.size()-1 ].vWire_list.size() ; i++){
				if( !vModule_list[ vModule_list.size()-1 ].vWire_list[i].sName.compare(cFrom) ){
					Optr2 = &vModule_list[ vModule_list.size()-1 ].vWire_list[i];
					//cout<<i<<" "<<"From: "<<cFrom<<" "<<vModule_list[ vModule_list.size()-1 ].vPO_list[i].sName<<endl;
				}
			}
			Optr1->pFrom = Optr2;
			//cout<<vModule_list[ vModule_list.size()-1 ].vPO_list[108].pFrom->sName<<endl;
		}
    else if( sIptData.length()>0 ){
      bool tokflag = false;
      //cout<<"getline: "<<sIptData<<endl;
      cLib_Cell *Lptr = LibHead.pNext;
      tok = strtok(cIptData," ");//get gate type AND2X1

      while( Lptr != NULL ){
        if( !Lptr->sName.compare(tok) ){//match the  gate type in the cell library
          cGate *GNode = new cGate;//create a Gate node

          GNode->sName = strtok(NULL, " ");// get gate name Uxxxx;

          tok = strtok(NULL, " ");//get '(' useless
          //cout<<"NumPort: "<<Lptr->iNumPort<<endl;
          for(int i=0 ; i < Lptr->iNumPort ; ){//根據有幾個port 做幾次切割net

            if( tokflag == true ){
              tok = strtok(cIptData, " ");
            }
            else{
              tok = strtok(NULL," ");//.Ax
            }
            if( tok == NULL ){//original tok==NULL, 現在是有找到'(' 但沒有找到')'
              fin.getline(cIptData, 1000);
              tokflag = true;
            }
            else{
              tokflag = false;
              sNet = tok;
              if( sNet.find(")") != string::npos ){
                i++;
                sNet = GetNetName(sNet);
                if( Lptr->sName.find("SDFF") != string::npos ){ //sdff有兩個Output 個別處理
                  if( i >= Lptr->iNumPort-1 ){
                    if( !sNet.length()){
                      GNode->vWire_list.push_back(NULL);
                    }
                    else{
                      CnnctNetGate(GNode, sNet, true);//output port is true
                    }
                  }
                  else{
                    CnnctNetGate(GNode, sNet, false);//input port is false
                  }
                }
                else{//其餘的cell
                  if( i >= Lptr->iNumPort ){
                    CnnctNetGate(GNode, sNet, true);//output port is true
                  }
                  else{
                    CnnctNetGate(GNode, sNet, false);//input port is false
                  }
                }
              }
            }

          }//end for

          GNode->sFootprint = Lptr->sFootprint;
          GNode->dPower = Lptr->dPower;
          GNode->dArea = Lptr->dArea;
          GNode->sFunc = Lptr->sFunc;
          AddGateList(GNode);//gate count 不對 tok==NULL 有問題
          break;//找到且換下一個要被吃進來的gate
        }//end if
        Lptr = Lptr->pNext;//如果不是這個cell 換下一個cell
      }//end while
    }
	}
  fin.close();
}

void Read_Lib(){ //?????要讀兩個library 現在只有一個
	char cIptData[1000], *tok;
  int iNumPort;
	string sName, sFootprint, sFunc;
  double dArea, dPower;
	size_t start, end;
	bool cell_flag = false, ff_flag = false;
	cLib_Cell *lptr;

	fstream fin;
	fin.open("saed32rvt_tt1p05v25c.lib", ios::in);
	if(!fin){
		cout<<"Open file fail."<<endl;
		exit(1);
	}

  while( fin.getline(cIptData, 1000) ){
    string sIptData = cIptData;

    if( sIptData.find(" 	cell (") != string::npos ){
      cLib_Cell *LNode = new cLib_Cell;
      sName = sIptData;
      start = sName.find("(")+1;
      end = sName.find("_RVT") - start;//去掉RVT
      LNode->sName = sName.substr( start, end );//Get cell type AND2X1

      iNumPort = 0;
      ff_flag = false;
      cell_flag = true;

      if( LibHead.pNext == NULL ){
        LibHead.pNext = LNode;
        lptr = LNode;
      }
      else{
        lptr->pNext = LNode;
        lptr = LNode;
      }
    }
    else if(  sIptData.find("cell_footprint :") != string::npos && cell_flag ){
      sFootprint = cIptData;
      start = sFootprint.find(":")+3;
      end = sFootprint.find(";")-1 -start;
      lptr->sFootprint = sFootprint.substr( start, end);
    }
    else if(  sIptData.find("area :") != string::npos && cell_flag ){
      tok = strtok(cIptData," ");//get area
      tok = strtok(NULL, " ");//get :
      tok = strtok(NULL, " ");//get the target number
      lptr->dArea = strtod(tok, NULL);
    }
    else if(  sIptData.find("cell_leakage_power :") != string::npos && cell_flag ){
      tok = strtok(cIptData," ");//get cell_leakage_power
      tok = strtok(NULL, " ");//get :
      tok = strtok(NULL, ";");//get the target number
      lptr->dPower = strtod(tok, NULL);
    }
    else if( sIptData.find(" 		pin (") != string::npos && (cell_flag xor ff_flag) ) {//calculate number of ports of each cells
      iNumPort++;
      while( fin.getline(cIptData, 1000) ){
        string sDirection = cIptData;
        if( sDirection.find("direction") != string::npos ){
          if( sDirection.find("output") != string::npos ){
            lptr->iNumPort = iNumPort;//????? for direction = "INOUT" and just have "input" no output port cells cant save the iNumPort
            break;
          }
          else{
            //cout<<sDirection<<endl;
            break;
          }
        }
      }
    }
    else if( sIptData.find(" 		ff (") != string::npos && cell_flag ){//????? 有些flip flop會有多個 ff
      ff_flag = true;
      cell_flag = false;
    }
    else if( sIptData.find("next_state : ") != string::npos && ff_flag ){
      sFunc = sIptData;
      start = sFunc.find(":")+3;
      end = sFunc.find(";") - start-1;
      lptr->sFunc = BuildPostfix( sFunc.substr( start, end ) );
    }
    else if( sIptData.find(" 			function : ") != string::npos && cell_flag && !ff_flag ){//沒有func的不會被加進 Lib cell list
      sFunc = sIptData;
      start = sFunc.find(":")+3;
      end = sFunc.find(";") - start-1;
      lptr->sFunc = BuildPostfix( sFunc.substr( start, end ) );
      cell_flag = false;
    }
  }
  AddLSDN();//?????先寫死兩個cell LSDNEN LSDN
}
