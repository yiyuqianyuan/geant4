#ifndef ybSD_Semi_H
#define ybSD_Semi_H 1
//===================C++==================//
#include <stdio.h>
#include <cmath>
//=================GEANT4=================//
#include "globals.hh"
#include "G4EventManager.hh"
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

class ybSD_Semi : public G4VSensitiveDetector
{
public:
	enum Geometry{kGeoBox,kGeoTube};
	enum ResoType{kNull,kReso,kNoise};

public:
	ybSD_Semi(G4String name,int numx,G4double widx,int numy,G4double widy);
	ybSD_Semi(G4String name,G4double rin,G4double rout, G4double angle, int cirnum, int linenum);
	~ybSD_Semi();

	inline void SetResolution(double reso) {fResolutionType=kReso;fResolution=reso;};
	inline void SetResolution(double fano,G4double gap,G4double noise)
	{fResolutionType=kNoise;fFano=fano;fEnergyGap=gap;fNoise=noise;};
	inline void SetTimeGate(G4double time) {fTimeGate=time;}
	inline void SetTimeInterval(G4double time) {fTimeInterval=time;}
	inline void SetThreshold(G4double energy) {fThreshold=energy;}
	
	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);
	
private:
	void CreatSignal();
	G4double MinTime();
	void OutputHitsData();
	void OutputDigiData();
	
private:
	enum Geometry fShape;
	enum ResoType fResolutionType;
	//for Box
	int fNum_X;
	int fNum_Y;
	G4double fWidth_X;
	G4double fWidth_Y;
	//for Tube
	G4double fRin;
	G4double fRout;
	G4double fAngle;
	int fCirnum;
	int fLinenum;
	
	int fTotNum;
	
	int fHCID;
	int fDCID;
	G4double fTimeInterval;
	G4double fTimeGate;
	G4double fThreshold;
	double fResolution;
	double fFano;
	G4double fEnergyGap;
	double fNoise;
	G4double fDeltaE;
	G4double fGlobaltime;
	ybDM_Amplifier *pDM_Amplifier;
	G4String fDMname;
	G4String fDCname;
	
	typedef multimap<G4double,G4double,less<G4double> > ybSignalMap;
	ybSignalMap *fHitsMap;
	ybSignalMap *fOutputMap;

	G4DigiManager *pDigiMan;
	ybDataHitsCollection* pHitsCollection;	
	ybDataDigiCollection* pDigiCollection;	
	ybDataHit *pNewHit;
	ybDataDigi *pNewDigi;
};

#endif

