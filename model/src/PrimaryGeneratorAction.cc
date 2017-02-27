#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	SetBeamEnergy(90.*MeV,0.01*keV);
	//SetTargetReaction(beamA,targetA,emergeZ,emergeA,recoilZ,recoilA,QValue);
	//emergeA should be bigger than recoilA
	SetTargetReaction(18.,9.,8.,18.,4.,9.,0.);
	//SetEmergeBreakUp(z1,a1,z2,a2,QValue);
	SetEmergeBreakUp(2.,4.,6.,14.,-6.26*MeV);
	SetEmergeDaughterEx(0.,0.);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{

}

/*
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* pEvent)
{
	SetParticleDefinition("e-");
	SetParticleEnergy(10*MeV,0.1*MeV);
	SetParticlePosition("uniform",0.,0.,5.*mm,-1.*mm,1.*mm);//type,x0,y0,r,z0,z1
	SetParticleMomentumDirection("uniform");
	SetParticleTime("const",1.*ns);
	pParticleGun->GeneratePrimaryVertex(pEvent);
}
*/

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* pEvent)
{
	SetEmergeEx(15.*MeV);
	SetReactionPosition(0.,0.,0.);
	SetBeamIntensity("const",0.1*ms);
	EmitParticle(pEvent);
/*	
		if(pEvent->GetEventID()%100==0)
		{
			G4cout<<"eventid:  "<<pEvent->GetEventID()<<G4endl;
		}
*/
}




//===================not used====================/

/*
double LaguerreAMP(double *x,double *p)
{
	return pow(ROOT::Math::assoc_laguerre(0,p[0],cos(x[0])),2.);
}

G4double ybVPrimaryGeneratorAction::GetLaguerreTheta(G4double j)
{
	TF1 ff("ff",LaguerreAMP,0.,3.1419,1);
	ff.SetParameter(0,j);	
	return ff.GetRandom();
}


double ybVPrimaryGeneratorAction::LaguerreAMP(double *x,double *p)
{
	return pow(ROOT::Math::assoc_laguerre(0,p[0],cos(x[0])),2.);
}

G4double ybVPrimaryGeneratorAction::GetLaguerreTheta(G4double j)
{
	TF1 ff("ff",this,&ybVPrimaryGeneratorAction::LaguerreAMP,0.,3.1419,1);
	ff.SetParameter(0,j);	
	return ff.GetRandom();
}

*/