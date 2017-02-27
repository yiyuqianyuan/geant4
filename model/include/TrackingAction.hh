

#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

using namespace std;

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction();
    virtual ~TrackingAction();
    virtual void PreUserTrackingAction(const G4Track *);
	virtual void PostUserTrackingAction(const G4Track *);
	
private:
	G4String fParticleName;
	G4double fKEnergy;
	const G4VProcess *pProcess;
	G4String fProcessName;
	int fTrackID;
	int fParentID;
	G4double fTime;
};

#endif
