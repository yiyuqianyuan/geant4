#ifndef ybVEventAction_H
#define ybVEventAction_H 1
//===================C++==================//
#include <fstream>
#include <sstream>
#include <map>
//=================GEANT4=================//
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4DCofThisEvent.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

//===================yb===================//
#include "ybDataHit.hh"
#include "ybDataDigi.hh"
#include "ybDataManager.hh"
#include "G4DigiManager.hh"
using namespace std;
typedef map<G4double,G4int,less<G4double> > ybLogicMap;	
class ybVEventAction : public G4UserEventAction
{
protected:

public:
	enum DataType{kHits,kDigi};
public:
	ybVEventAction();
	~ybVEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);
	void CreateTrigger();
	virtual ybLogicMap* UserDigiTrigger()=0;
	virtual ybLogicMap* UserHitsTrigger();
	virtual void SaveHitsData(const G4Event *pEvent);
	virtual void SaveDigiData(const G4Event *pEvent);
	virtual void SaveUserData();
	//============================Logic Section===========================//
	size_t GetNumOfChannels(G4String name,G4String type="");
	ybLogicMap* BuildSequenceMap(G4String name,G4double thr=0, size_t multies=1);
	ybLogicMap* LogicAND(ybLogicMap *pLogicMap_A, ybLogicMap *pLogicMap_B);
	ybLogicMap* LogicOR(ybLogicMap *pLogicMap_A, ybLogicMap *pLogicMap_B);
	ybLogicMap* LogicMulties(G4String *name, G4double *thr, size_t num, size_t multies);
	ybLogicMap* LogicMulties(ybLogicMap **pLogicMapMulti, size_t num, size_t multies);
	ybLogicMap* LogicMultiAND(G4String *name, G4double *thr, size_t num);
	ybLogicMap* LogicMultiOR(G4String *name, G4double *thr, size_t num);
	G4double MinTime(ybLogicMap **pLogicMapMulti,size_t num);
	bool MultiEmpty(ybLogicMap **pLogicMapMulti,size_t num);
	
protected:
	const G4double fCTime;
	G4double fDelayTime;
	G4double fGateWidth;
	G4double fEnergyUnit;
	G4double fTimeUnit;
	vector<G4double> vDigiTrigger;
	vector<G4double> vHitsTrigger;
	ybDataManager *pDataMan;
private:
	enum DataType fDataType;
	//at 	BeginOfEventAction
	int fTotalEventNumber;
	int fPercentEvent;	
	//at 	EndOfEventAction
	vector<ybLogicMap*> vLogicMapMan; 
};
#endif

    
