#ifndef ybDM_Amplifier_H
#define ybDM_Amplifier_H 1
//===================yb===================//
#include "G4String.hh"
#include "ybVDigitizerModule.hh"

class ybDM_Amplifier : public ybVDigitizerModule
{
public:
	ybDM_Amplifier(G4String name);
	ybDM_Amplifier(G4String name, int ch_x, int ch_y);
	virtual ~ybDM_Amplifier();
	virtual void Digitize();

};

#endif

