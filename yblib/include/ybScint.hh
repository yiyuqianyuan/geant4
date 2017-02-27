#ifndef ybScint_H
#define ybScint_H 1
//=================GEANT4=================//
#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh" 
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
//===================yb===================//
#include "ybVDetector.hh"
#include "ybSD_Scint.hh"

class ybScint : public ybVDetector
{
public:
	enum Geometry{kGeoBox,kGeoTube};
	enum ScintType{kTypeNull,kTypeNaI,kTypeCsI,kTypeBGO,kTypeBC408};
	
public:
	ybScint(G4String name,G4VPhysicalVolume* world,G4double x,G4double y,G4double z);
	ybScint(G4String name,G4VPhysicalVolume* world,G4double r1,G4double r2,G4double h,G4double phi1,G4double phi2);   
	ybScint();
	~ybScint();

	inline void SetScintType(enum ScintType scint) {fScintillator=scint;};
	inline ybSD_Scint* GetSD() {return pSD_Scint;}
	inline void SetPosition(G4RotationMatrix *rotation,G4ThreeVector translation)
	{
		pRotation=rotation;
		fTranslation=translation;
	};	
	void Material();
    void DetectorConstruct();
	void Visualization();

private:
	//Shape
	enum Geometry fShape;
	enum ScintType fScintillator;
	//Logic World
	G4LogicalVolume		*pLogicWorld;
	//Constant for Scintillator
	G4double fSize_x;
	G4double fSize_y;
	G4double fSize_z;
	
	G4double fRmin;
	G4double fRmax;	
	G4double fDz;	
	G4double fSphi;
	G4double fDphi;
	
	G4RotationMatrix *pRotation;
	G4ThreeVector fTranslation;	
	//Material for Scintillator	
	G4Material *pMaterial;
	//Detector
	G4CSGSolid			*pSolidScint;
	G4LogicalVolume		*pLogicScint;
	G4VPhysicalVolume	*pPhysiScint;	
	//SD
	ybSD_Scint *pSD_Scint;

};

#endif

