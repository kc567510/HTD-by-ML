
/*
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdbool.h>
*/
//using namespace std;
/*
enum eWireType {PI, PO, Wire};
/*enum eGateType {LSDNEN, NAND3, AND3, NAND2, INV, SDFF, AND2, OA222, AOI222, NBUFF, LSDN, AOI21, XNOR3, XNOR2,
				NOR2, XOR2, XOR3, OR2, OA221, OAI21, AO221, NOR4, NOR3, OA21, AO21, OA22, AOI221, NAND4,
				AND4, AO222, AO22, ISOLAND, OR3, OAI221, OR4, AOI22, OAI222, OAI22};
*/
void TestLIB();
void Read_Circuit(char* charsfilename);
void Read_Lib();
//PI->FF->Gate->Gate->FF->Gate->FF->PO
/*
class cWire;

class cGate{
	private:

	public:
		cGate() : sName(), sFootprint(), dPower(0), dArea(0), sFunc(), dSigDenominator(0), dSig0Nominator(0), dSig1Nominator(0), dSig0probability(0),
		value(false), pNext(NULL), pPre(NULL), TPLGVisit(false)
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
*/
