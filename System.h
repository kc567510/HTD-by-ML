#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <stdbool.h>
#include <math.h>
#include <time.h>

using namespace std;

enum eWireType {PI, PO, Wire};

class cWire;

class cGate{
	private:

	public:
		cGate() : sName(), sFootprint(), dPower(0), dArea(0), sFunc(), dSigDenominator(0), dSig0Nominator(0), dSig1Nominator(0), dSig0probability(0),
		dSig1probability(0), value(false), pNext(NULL), pPre(NULL), TPLGVisit(false)
		{ vNext_list.clear(), vWire_list.clear(); }

		string sName;
		string sFootprint;
		double dPower;
		double dArea;
		string sFunc;
		//eGateType Type;
		vector <cGate*> vNext_list;
		cGate *pNext;
		cGate *pPre;
		vector <cWire*> vWire_list;
		bool value;
		double dSigDenominator;//分母
		double dSig0Nominator;//紀錄訊號0的次數 分子 算singal probability
		double dSig1Nominator;//紀錄訊號0的次數 分子 算singal probability
		double dSig0probability;//存signal probability
    double dSig1probability;
		bool TPLGVisit;//在找TPLG order時看有沒有被走過
};

class cWire{
	private:

	public:
		cWire() : sName(), value(false), Type(), pFrom(NULL)  { vCnnctGate_list.clear(); }
		string sName;
		bool value;
		vector<cGate*> vCnnctGate_list;//remember the fanout of the wire
		cWire *pFrom;
		eWireType Type;
};

class cLib_Cell{

	private:

	public:
		cLib_Cell() : sName(), sFootprint(), sFunc(), dPower(0), dArea(0), iNumPort(0), pNext(NULL){}

		string sName;
		string sFootprint;
		double dPower;
		double dArea;
		string sFunc;
		int iNumPort;
		cLib_Cell *pNext = NULL;
};

class cCircuit{
	private:

	public:
		cCircuit(): sName() { vWire_list.clear(); }

		string sName;
		vector <cWire> vWire_list;
		cGate Gate_list_head;
};
