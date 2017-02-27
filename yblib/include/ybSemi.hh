#ifndef ybSemi_H
#define ybSemi_H 1
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
#include "ybSD_Semi.hh"

class ybSemi : public ybVDetector
{
public:
	enum Geometry{kGeoBox,kGeoTube};
public:
	ybSemi(G4String name,G4VPhysicalVolume* world,G4double th=1000.*um,
			G4double widx=1.*mm,int numx=16,G4double widy=1.*mm,int numy=16  );
	ybSemi(G4String name,G4VPhysicalVolume *world,G4double rin,G4double rout,
			G4double th, G4double angle, int cirnum, int linenum);
	ybSemi();
	~ybSemi();

	inline void SetPosition(G4RotationMatrix *rotation,G4ThreeVector translation)
	{
		pRotation=rotation;
		fTranslation=translation;
	};
	inline ybSD_Semi* GetSD() {return pSD_Semi;}
	void Material();
	void DetectorConstruct();
	void Visualization();

private:
	//Shape
	enum Geometry fShape;
	//Logic World
	G4LogicalVolume *pLogicWorld;
	//Constant for Semi
	G4double fThickness;
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

	G4RotationMatrix *pRotation;
	G4ThreeVector fTranslation;
	//Materials
	G4Material *pSilicon;
	//Si Strips
	G4VSolid	 		*pSolidSemi;
	G4LogicalVolume		*pLogicSemi;
	G4VPhysicalVolume	*pPhysiSemi;	
	//SD 
	ybSD_Semi 		*pSD_Semi;
};

#endif

