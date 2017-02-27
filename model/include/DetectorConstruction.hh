#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "ybVDetectorConstruction.hh"
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
//#include "ybExDSSD.hh"
#include "ybSemi.hh"
#include "ybScint.hh"
using namespace std;

class DetectorConstruction : public ybVDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

public:
    G4VPhysicalVolume* WorldConstruct();
private:
	//world
	G4double fSizeWorld[3];
	//Material
	G4Box* 				pSolidWorld;
	G4LogicalVolume*	pLogicWorld;
	G4VPhysicalVolume*	pPhysiWorld;
};

#endif

