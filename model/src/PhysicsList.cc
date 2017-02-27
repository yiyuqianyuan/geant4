#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{ 
	defaultCutValue=0.1*mm;
	G4int ver=0;
 	SetVerboseLevel(ver);
	SetCutsWithDefault();

	//Decays
	RegisterPhysics(new G4DecayPhysics("Decay"));
	//RadioactiveDecayPhysics
	RegisterPhysics(new G4RadioactiveDecayPhysics());
	//EM Physics
	//RegisterPhysics(new G4EmStandardPhysics(ver,"EM"));
	//RegisterPhysics(new G4EmLivermorePhysics(ver));
	//RegisterPhysics(new G4EmPenelopePhysics(ver));
	RegisterPhysics(new G4EmStandardPhysics_option4(ver));
	//Synchroton Radiation & GN Physics
	G4String fState="on";
	G4EmExtraPhysics *pEmPhy=new G4EmExtraPhysics(ver);
	pEmPhy->Synch(fState);
	pEmPhy->GammaNuclear(fState);
	pEmPhy->MuonNuclear(fState);	
	RegisterPhysics(pEmPhy);

 	//process for optical photon 
//	RegisterPhysics(new G4OpticalPhysics(ver,"Optical"));


 	//user limiter
	
}
PhysicsList::~PhysicsList()
{
	
}