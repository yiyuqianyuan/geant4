#ifndef ybVDetectorConstruction_H
#define ybVDetectorConstruction_H 1
//=================GEANT4=================//
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh" 
#include "G4PVPlacement.hh"
//===================yb===================//
#include "ybVDetector.hh"
using namespace std;

class ybVDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    ybVDetectorConstruction();
    virtual ~ybVDetectorConstruction();

public:
	void RegisterDetectors(ybVDetector *pDetector);
	virtual G4VPhysicalVolume* WorldConstruct()=0;
    G4VPhysicalVolume* Construct();

private:
	G4VPhysicalVolume*	pPhysiWorld;	
	typedef vector<ybVDetector*> ybDetConstVector;
	ybDetConstVector pDetectorVector;
	ybDetConstVector::iterator pIter;
};

#endif

