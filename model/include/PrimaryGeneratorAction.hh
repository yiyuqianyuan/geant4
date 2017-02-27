
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
#include "ybVPrimaryGeneratorAction.hh"

#include "G4EventManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4NuclideTable.hh"
#include "G4IsotopeProperty.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4ios.hh"

#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>
//===================ROOT==================//

#include "Math/SpecFunc.h"
#include "TF1.h"


//class G4ParticleGun;
//class G4Event;

class PrimaryGeneratorAction : public ybVPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* pEvent);

};

#endif


