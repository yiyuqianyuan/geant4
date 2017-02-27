#include "TrackingAction.hh"

TrackingAction::TrackingAction(): G4UserTrackingAction()
{ 
}

TrackingAction::~TrackingAction()
{ 

}


void TrackingAction::PreUserTrackingAction(const G4Track*) 
{
/*	
	fParticleName=pTrack->GetDefinition()->GetParticleName();
	fTrackID=pTrack->GetTrackID();
	fParentID=pTrack->GetParentID();
	fKEnergy=pTrack->GetKineticEnergy()/MeV;
	pProcess=pTrack->GetCreatorProcess();
	fTime=pTrack->GetGlobalTime()/ms;


	if(pProcess != 0)
	{
		fProcessName=pProcess->GetProcessName();
		G4cout<<"ID: "<<fTrackID<<"  ParentID: "<<fParentID<<"  particle: "<<fParticleName<<" KEnergy: "<<fKEnergy<<"  ProcessName: "<<fProcessName<<fixed<<setprecision(8)<<"  time: "<<fTime<<G4endl;
	}else
	{
		G4cout<<"ID: "<<fTrackID<<"  ParentID: "<<fParentID<<"  particle: "<<fParticleName<<" KEnergy: "<<fKEnergy<<fixed<<setprecision(8)<<"  time: "<<fTime<<G4endl;
	}
*/
}

void TrackingAction::PostUserTrackingAction(const G4Track*) 
{
	
}

