#ifndef ybSD_Scint_H
#define ybSD_Scint_H 1
//===================C++==================//
#include <stdio.h>
#include <cmath>
//=================GEANT4=================//
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "Randomize.hh"
//===================yb===================//
#include "G4DigiManager.hh"
#include "ybDataHit.hh"
#include "ybDataDigi.hh"
#include "ybDM_Amplifier.hh"

class ybSD_Scint : public G4VSensitiveDetector
{
public:
	enum ResoType{kNull,kReso};
public:
	ybSD_Scint(G4String thisname);
	~ybSD_Scint();
	
	inline void SetResolution(double reso) {fResolutionType=kReso;fResolution=reso;};
	inline void SetTimeInterval(G4double time) {fTimeInterval=time;}
	inline void SetThreshold(G4double energy) {fThreshold=energy;}
	
	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);
	
private:
	void CreatSignal();
	void OutputHitsData();
	void OutputDigiData();
	
private:
	enum ResoType fResolutionType;
	int fHCID;
	int fDCID;
	G4double fTimeInterval;
	G4double fThreshold;
	double fResolution;
	G4double fDeltaE;
	G4double fGlobaltime;
	ybDM_Amplifier *pDM_Amplifier;
	
	
	typedef multimap<G4double,G4double,less<G4double> > ybSignalMap;
	ybSignalMap fHitsMap;
	ybSignalMap fOutputMap;	

	G4DigiManager *pDigiMan;
	ybDataHitsCollection* pHitsCollection;	
	ybDataDigiCollection* pDigiCollection;	
	ybDataHit *pNewHit;
	ybDataDigi *pNewDigi;
};

#endif

