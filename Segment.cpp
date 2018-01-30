#include "System.h"
//#include "Segment.h"

using namespace std;
extern int PathCount;
extern int FrPathCount;
extern int TjPathCount;
extern vector <cCircuit> vModule_list;

double StdDeviation(vector <cGate*> vPathStack, double Avg, int n, int Option){ // Option 0 is area, 1 is fin, 2 is fout, 3 is signal probability
  double dAns=0;
  double dSum=0;

  for(int i=0 ; i<vPathStack.size() ; i++){
    if(Option == 0){
      dSum += ( vPathStack[i]->dArea - Avg ) * ( vPathStack[i]->dArea - Avg );
    }
    else if(Option == 1){
      dSum += ( (vPathStack[i]->vWire_list.size()-1) - Avg ) * ( (vPathStack[i]->vWire_list.size()-1) - Avg );
    }
    else if(Option == 2){
      int index = vPathStack[i]->vWire_list.size()-1;
      dSum += ( vPathStack[i]->vWire_list[index]->vCnnctGate_list.size() - Avg ) * ( vPathStack[i]->vWire_list[index]->vCnnctGate_list.size() - Avg );
    }
    else if(Option == 3){
      dSum += ( vPathStack[i]->dSig0probability - Avg ) * ( vPathStack[i]->dSig0probability - Avg );
    }
    else if(Option == 4){
      dSum += ( vPathStack[i]->dSig1probability - Avg ) * ( vPathStack[i]->dSig1probability - Avg );
    }
  }
  dAns = sqrt( dSum/(n-1) );
  return dAns;
}

void OutputtoFile(string OutFileName, vector <cGate*> vPathStack, int iPathGateCount, double dMaxArea, double dMinArea, double dSumArea, int iMaxFin, int iMinFin, int iSumFin,//??? AVG 標準差還沒
                int iMaxFout, int iMinFout, int iSumFout, double dMaxSig0Prop, double dMinSig0Prop, double dSumSig0Prop,
                double dMaxSig1Prop, double dMinSig1Prop, double dSumSig1Prop ){
  //write data output to the File
  bool TjFlag = false;

  double dAvgArea, dStdArea, dAvgFin, dStdFin, dAvgFout, dStdFout, dAvgSumSig0Prop, dStdSumSig0Prop, dAvgSumSig1Prop, dStdSumSig1Prop;
  if( iPathGateCount==0 ){
    return;
  }
  PathCount++;
  dAvgArea = (double) dSumArea / iPathGateCount;
  dAvgFin = (double) iSumFin / iPathGateCount;
  dAvgFout = (double) iSumFout / iPathGateCount;
  dAvgSumSig0Prop = (double) dSumSig0Prop / iPathGateCount;
  dAvgSumSig1Prop = (double) dSumSig1Prop / iPathGateCount;

  //這邊使用樣本標準差的算法 還有另一種是母體標準差
  dStdArea = StdDeviation(vPathStack, dAvgArea, iPathGateCount, 0);
  dStdFin = StdDeviation(vPathStack, dAvgFin, iPathGateCount, 1);
  dStdFout = StdDeviation(vPathStack, dAvgFout, iPathGateCount, 2);
  dStdSumSig0Prop = StdDeviation(vPathStack, dAvgSumSig0Prop, iPathGateCount, 3);
  dStdSumSig1Prop = StdDeviation(vPathStack, dAvgSumSig1Prop, iPathGateCount, 4);


  char FileName[100];
  strcpy(FileName, OutFileName.c_str() );
  FILE *fptr = fopen(FileName, "a");
  if(!fptr){
    cout<<"Open PathFeature.txt Fail"<<endl;
    exit(1);
  }

  for(int i=0 ; i<vPathStack.size() ; i++){
    char cName[100];
    strcpy(cName, vPathStack[i]->sName.c_str());
    fprintf(fptr,"%s", cName);
    if( i<vPathStack.size()-1 ){
      fprintf(fptr,"->");
    }
    if( vPathStack[i]->sName.find("Trojan") != string::npos && TjFlag == false ){
      TjFlag = true;
      TjPathCount++;
    }
  }

  if( TjFlag == false ){
    FrPathCount++;
  }
  fprintf(fptr," ");//space
  fprintf(fptr,"%d %d %1.5f %1.5f %5.5f %2.5f %1.5f %d %d %d %2.5f %1.5f %d %d %d %2.5f %1.5f %1.5f %1.5f %5.5f %2.5f %1.5f %1.5f %1.5f %5.5f %2.5f %1.5f\n",
    iPathGateCount, TjFlag, dMaxArea, dMinArea, dSumArea, dAvgArea, dStdArea, iMaxFin, iMinFin, iSumFin, dAvgFin, dStdFin,
    iMaxFout, iMinFout, iSumFout, dAvgFout, dStdFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dAvgSumSig0Prop, dStdSumSig0Prop,
    dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop, dAvgSumSig1Prop, dStdSumSig1Prop);

  fclose(fptr);
}

void CalPathFeature(int *iPathGateCount, double dCurArea, double *dMaxArea, double *dMinArea, double *dSumArea,
                                         int iCurFin, int *iMaxFin, int *iMinFin, int *iSumFin,
                                         int iCurFout, int *iMaxFout, int *iMinFout, int *iSumFout,
                                         double dCurSig0Prop, double *dMaxSig0Prop, double *dMinSig0Prop, double *dSumSig0Prop,
                                         double dCurSig1Prop, double *dMaxSig1Prop, double *dMinSig1Prop, double *dSumSig1Prop){
  //get the Feature for the each Path
  //path count
  *iPathGateCount += 1;

  //Area
  *dSumArea += dCurArea;
  if( dCurArea > *dMaxArea ){
    *dMaxArea = dCurArea;
  }
  if( dCurArea < *dMinArea ){
    *dMinArea = dCurArea;
  }
  //Fanin
  *iSumFin += iCurFin;
  if( iCurFin > *iMaxFin ){
    *iMaxFin = iCurFin;
  }
  if( iCurFin < *iMinFin ){
    *iMinFin = iCurFin;
  }
  //Fanout
  *iSumFout += iCurFout;
  if( iCurFout > *iMaxFout ){
    *iMaxFout = iCurFout;
  }
  if( iCurFout < *iMinFout ){
    *iMinFout = iCurFout;
  }
  //Signal0 probability
  *dSumSig0Prop += dCurSig0Prop;
  if( dCurSig0Prop > *dMaxSig0Prop ){
    *dMaxSig0Prop = dCurSig0Prop;
  }
  if( dCurSig0Prop < *dMinSig0Prop ){
    *dMinSig0Prop = dCurSig0Prop;
  }
  //Signal1 probability
  *dSumSig1Prop += dCurSig1Prop;
  if( dCurSig1Prop > *dMaxSig1Prop ){
    *dMaxSig1Prop = dCurSig1Prop;
  }
  if( dCurSig1Prop < *dMinSig1Prop ){
    *dMinSig1Prop = dCurSig1Prop;
  }
}

void RecurrGetPath(string OutFileName, vector <cGate*> vPathStack, cGate *CurGptr, cWire *Wptr, int iPathGateCount, double dMaxArea, double dMinArea, double dSumArea,
                    int iMaxFin, int iMinFin, int iSumFin, int iMaxFout, int iMinFout, int iSumFout,
                    double dMaxSig0Prop, double dMinSig0Prop, double dSumSig0Prop, double dMaxSig1Prop, double dMinSig1Prop, double dSumSig1Prop){
  cGate *NextGptr;
  if(CurGptr != NULL){
    if(CurGptr->sFootprint != "SDFF"){
      vPathStack.push_back(CurGptr);
    }
    CalPathFeature( &iPathGateCount, CurGptr->dArea, &dMaxArea, &dMinArea, &dSumArea,
                    CurGptr->vWire_list.size()-1, &iMaxFin, &iMinFin, &iSumFin,
                    CurGptr->vWire_list[ CurGptr->vWire_list.size()-1 ]->vCnnctGate_list.size(), &iMaxFout, &iMinFout, &iSumFout,
                    CurGptr->dSig0probability, &dMaxSig0Prop, &dMinSig0Prop, &dSumSig0Prop,
                    CurGptr->dSig1probability, &dMaxSig1Prop, &dMinSig1Prop, &dSumSig1Prop );
  }

  if( Wptr->Type == PO && Wptr->vCnnctGate_list.size() == 0 ){ //表示net is PO
    OutputtoFile(OutFileName, vPathStack, iPathGateCount, dMaxArea, dMinArea, dSumArea, iMaxFin, iMinFin, iSumFin,
                  iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop );
    vPathStack.pop_back();
    return;
  }
  else{
    for(int i=0 ; i < Wptr->vCnnctGate_list.size() ; i++){
      NextGptr = Wptr->vCnnctGate_list[i];
      if( NextGptr->sFootprint == "SDFF"){ //PI to FF
        OutputtoFile(OutFileName, vPathStack, iPathGateCount, dMaxArea, dMinArea, dSumArea, iMaxFin, iMinFin, iSumFin,
                      iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop );//feature 不包含FF
      }
      else{ //for other gates with only one output
        //cout<<"Gate: "<<NextGptr->sName<<endl;

        int OptNum = NextGptr->vWire_list.size()-1; //get output net of the gate
        if( NextGptr->vWire_list[OptNum] != NULL ){
          RecurrGetPath(OutFileName, vPathStack, NextGptr, NextGptr->vWire_list[OptNum], iPathGateCount, dMaxArea, dMinArea, dSumArea, iMaxFin, iMinFin, iSumFin,
                        iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop) ;

        }
      }
    } //end for
    vPathStack.pop_back();
    return ;
  }// end else
}

void FindPath(string OutFileName){
  /*
  1. from PI to FF
  2. from PI to PO
  3. from FF to FF
  4. from FF to PO
  */
  int iPathGateCount=0;
  double dMaxArea=0, dMinArea=100, dSumArea=0;
  int iMaxFin=0, iMinFin=100, iSumFin=0;
  int iMaxFout=0, iMinFout=100, iSumFout=0;
  double dMaxSig0Prop=0, dMinSig0Prop=1, dSumSig0Prop=0;
  double dMaxSig1Prop=0, dMinSig1Prop=1, dSumSig1Prop=0;

  for(int i=0 ; i<vModule_list.size() ;i++){
    for(int j=0 ; j<vModule_list[i].vWire_list.size() ; j++){ //from PI
      if(vModule_list[i].vWire_list[j].sName == "CK" || vModule_list[i].vWire_list[j].sName == "test_se" ){
        continue; //Pass net: CK
      }
      if(vModule_list[i].vWire_list[j].Type == PI){ //from PI to FF or PO
        iPathGateCount=0;
        dMaxArea=0, dMinArea=100, dSumArea=0;
        iMaxFin=0, iMinFin=100, iSumFin=0;
        iMaxFout=0, iMinFout=100, iSumFout=0;
        dMaxSig0Prop=0, dMinSig0Prop=1, dSumSig0Prop=0;
        dMaxSig1Prop=0, dMinSig1Prop=1, dSumSig1Prop=0;
        vector <cGate*> vPathStack;
        vPathStack.clear();

        RecurrGetPath(OutFileName, vPathStack, NULL, &vModule_list[i].vWire_list[j], iPathGateCount, dMaxArea, dMinArea, dSumArea, iMaxFin, iMinFin, iSumFin,
                        iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop );
      }
    }
    //cout<<"PI to FF total: "<<PathCount<<endl;

    cGate *Gptr = vModule_list[i].Gate_list_head.pNext;
    while( Gptr != NULL ){ //from FF to PO
      if( Gptr->sFootprint == "SDFF"){
        iPathGateCount=0;
        dMaxArea=0, dMinArea=100, dSumArea=0;
        iMaxFin=0, iMinFin=100, iSumFin=0;
        iMaxFout=0, iMinFout=100, iSumFout=0;
        dMaxSig0Prop=0, dMinSig0Prop=1, dSumSig0Prop=0;
        dMaxSig1Prop=0, dMinSig1Prop=1, dSumSig1Prop=0;
        vector <cGate*> vPathStack;
        vPathStack.clear();
        if( Gptr->vWire_list[ Gptr->vWire_list.size()-1 ] != NULL ) { //for QN
          RecurrGetPath(OutFileName, vPathStack, NULL, Gptr->vWire_list[ Gptr->vWire_list.size()-1 ], iPathGateCount, dMaxArea, dMinArea, dSumArea,
                        iMaxFin, iMinFin, iSumFin, iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop );
        }
        if( Gptr->vWire_list[ Gptr->vWire_list.size()-2 ] != NULL ){ //for Q
          RecurrGetPath(OutFileName, vPathStack, NULL, Gptr->vWire_list[ Gptr->vWire_list.size()-2 ], iPathGateCount, dMaxArea, dMinArea, dSumArea,
                        iMaxFin, iMinFin, iSumFin, iMaxFout, iMinFout, iSumFout, dMaxSig0Prop, dMinSig0Prop, dSumSig0Prop, dMaxSig1Prop, dMinSig1Prop, dSumSig1Prop );
        }
      }
      Gptr = Gptr->pNext;
    }
    //cout<<"FF to FF total: "<<PathCount<<endl;

  }

}
