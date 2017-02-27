#ifndef ybRunAction_H
#define ybRunAction_H 1
//==================Linux=================//
#include "unistd.h"
//===================C++==================//
#include <fstream>
//=================GEANT4=================//
#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4Timer.hh"
//==================ROOT==================//
#include "TFile.h"
#include "TTree.h"
//===================yb===================//
#include "ybDataManager.hh"
using namespace std;

class ybRunAction : public G4UserRunAction
{
public:
	ybRunAction();
	virtual ~ybRunAction();

public:
	void BeginOfRunAction(const G4Run* aRun);
	void EndOfRunAction(const G4Run* aRun);
	virtual void SaveData();

protected:
	ybDataManager* pDataMan;	
	int fRunID;
private:
	G4Timer* pTimer;
};

#endif
