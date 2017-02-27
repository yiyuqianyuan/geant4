#ifndef ybVDigitizerModule_H
#define ybVDigitizerModule_H 1
//=================GEANT4=================//
#include "G4ThreeVector.hh"
#include "G4VDigitizerModule.hh"


class ybVDigitizerModule : public G4VDigitizerModule
{
public:
	ybVDigitizerModule(G4String name);
	virtual ~ybVDigitizerModule();
	virtual void Digitize()=0;

};

#endif

