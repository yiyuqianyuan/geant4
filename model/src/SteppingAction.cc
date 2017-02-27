#include "SteppingAction.hh"

SteppingAction::SteppingAction(): G4UserSteppingAction()
{ 


}

SteppingAction::~SteppingAction()
{ 

}

void SteppingAction::UserSteppingAction(const G4Step* pStep) 
{
	G4Track* pTrack = pStep->GetTrack();
	//for G4 time cut
	if(pTrack->GetGlobalTime()>1.e15*ns)
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);

	}
	//for decay time cut
	if(pTrack->GetGlobalTime()>360000*s)
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);

	}	
	
	G4String fParticleName=pTrack->GetDefinition()->GetParticleName();
/*
	if(fParticleName=="Be9")
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}else if(fParticleName=="C14")
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}
*/

/*
if(fParticleName=="Co51")
{
G4StepPoint *pPrePoint=pStep->GetPreStepPoint();		
G4StepPoint *pPostPoint=pStep->GetPostStepPoint();	
G4cout<<"fParticleName: "<<fParticleName<<"  precharge="<<pPrePoint->GetCharge()<<"  postcharge="<<pPostPoint->GetCharge()
<<"  Ek="<<pTrack->GetKineticEnergy()<<"  z1="<<pPrePoint->GetPosition().getZ()<<"  z2="<<pPostPoint->GetPosition().getZ()
<<"   ProName: "<<pPostPoint->GetProcessDefinedStep()->GetProcessName()<<G4endl;
}
*/	
/*		
	if(fParticleName=="Fe51")
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}
		

	

	G4StepPoint *pPrePoint=pStep->GetPreStepPoint();
	G4Track* pTrack = pStep->GetTrack();
	G4String fParticleName=pTrack->GetDefinition()->GetParticleName();
	E_pos=0.;
	E_neg=0.;	

//G4cout<<" fParticleName="<<fParticleName<<G4endl;	
	if(fParticleName=="Po210" || fParticleName=="TI206")//Fe51
	{
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}else if(fParticleName=="e+")
	{
		G4String fProName=pTrack->GetCreatorProcess()->GetProcessName();

		if(fProName=="RadioactiveDecay")
		{
			P_pos=pPrePoint->GetMomentum().getR()/(MeV/c);	
			E_pos=pStep->GetPreStepPoint()->GetKineticEnergy()/MeV;
			newtree->Fill();
		}
		
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}else if(fParticleName=="e-")
	{
		G4String fProName=pTrack->GetCreatorProcess()->GetProcessName();
//G4cout<<" fProName="<<fProName<<G4endl;	
		if(fProName=="RadioactiveDecay")
		{
			P_neg=pPrePoint->GetMomentum().getR()/(MeV/c);
			E_neg=pStep->GetPreStepPoint()->GetKineticEnergy()/MeV;
			newtree->Fill();
		}
		
		pTrack->SetTrackStatus(fKillTrackAndSecondaries);
	}
*/	


}



