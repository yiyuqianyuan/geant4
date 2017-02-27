

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);
	
private:


};

#endif
