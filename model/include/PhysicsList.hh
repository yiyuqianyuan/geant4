#ifndef PhysicsList_h
#define PhysicsList_h 1

#include <iomanip> 
#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"  
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4OpticalPhysics.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
	PhysicsList();
	~PhysicsList();

};


#endif



