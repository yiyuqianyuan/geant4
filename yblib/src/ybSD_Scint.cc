#include "ybSD_Scint.hh"

ybSD_Scint::ybSD_Scint(G4String name):G4VSensitiveDetector(name)
{	
	G4cout<<"creating ybSD_Scint: "<<SensitiveDetectorName<<"..."<<G4endl;
	//initialize the variables
	fResolution=0.08;
	fHCID=-1;
	fDCID=-1;
	fTimeInterval=0.001*ms;
	fThreshold=10.*keV;
	fResolutionType=kNull;
	collectionName.insert(SensitiveDetectorName);
	
	pDM_Amplifier=new ybDM_Amplifier(name);
	pDigiMan=G4DigiManager::GetDMpointer();
	pDigiMan->AddNewModule(pDM_Amplifier);
}

ybSD_Scint::~ybSD_Scint()
{ 
	G4cout<<"deleting ybSD_Scint: "<<SensitiveDetectorName<<"..."<<G4endl;
}

void ybSD_Scint::Initialize(G4HCofThisEvent* pHCE)
{
	//Initialize map
	fHitsMap.clear();
	fOutputMap.clear();
	multimap<G4double,G4double,less<G4double> >().swap(fHitsMap); 
	multimap<G4double,G4double,less<G4double> >().swap(fOutputMap);
	//Initialize hitscollection	
	if(fHCID<0)
	{
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
	}
	pHitsCollection= new ybDataHitsCollection(SensitiveDetectorName,collectionName[0]); 
	pHCE->AddHitsCollection(fHCID ,pHitsCollection);
	//Initialize digicollection		
	if(fDCID<0)
	{
		fDCID=pDigiMan->GetDigiCollectionID(pDM_Amplifier->GetCollectionName(0));
	}
	pDigiCollection= new ybDataDigiCollection(SensitiveDetectorName,collectionName[0]); 
	pDigiMan->SetDigiCollection(fDCID,pDigiCollection);
}

G4bool ybSD_Scint::ProcessHits(G4Step* pStep,G4TouchableHistory* )
{
	G4StepPoint* pPreStepPoint = pStep->GetPreStepPoint();
	//energy
	fDeltaE=pStep->GetTotalEnergyDeposit();
	//globaltime	
	fGlobaltime=pPreStepPoint->GetGlobalTime();
	//globalposition
	G4ThreeVector fTheGlobalPoint = pPreStepPoint->GetPosition();
	
	//for Digi
	if(fDeltaE>0.)
	{
		fHitsMap.insert(make_pair(fGlobaltime,fDeltaE));
	}
/*	
	//for hits
	if(pPreStepPoint->GetStepStatus() == fGeomBoundary && pStep->GetTrack()->GetParentID()==0)
	{

		ybDataHit *newhit= new ybDataHit();
		newhit->SetDetectorName(SensitiveDetectorName);
		newhit->SetParticleName(pStep->GetTrack()->GetDefinition()->GetParticleName());
		newhit->SetTime(fGlobaltime);
		newhit->SetPosition(fTheGlobalPoint);
		newhit->SetKineticEnergy(pPreStepPoint->GetKineticEnergy());
		pHitsCollection->insert(newhit);
	}
*/	
	return true;
}

void ybSD_Scint::EndOfEvent(G4HCofThisEvent* )
{
	CreatSignal();
	OutputDigiData();
	OutputHitsData();
}

void ybSD_Scint::CreatSignal()
{
	if(fHitsMap.empty())
	{
		return;
	}
	G4double fTimeStart=-1.;
	G4double fLastTime=-1.;
	G4double fEnergy=0.;
	ybSignalMap::iterator pIter;
	
	pIter=fHitsMap.begin();
	fLastTime=pIter->first;
	fTimeStart=pIter->first;
	fEnergy=0.;
	
	while(1)
	{
		if(pIter!=fHitsMap.end() && pIter->first<fLastTime+fTimeInterval)
		{
			fEnergy+=pIter->second;	
		}else
		{
			//simulating noise
			if(fResolutionType==kReso)
			{
				G4double Esigma=fEnergy*fResolution/2.355;
				G4double Erand=0.;
				while(1)
				{
					Erand=CLHEP::RandGauss::shoot(0.,Esigma);
					if( fabs(Erand)<3.*Esigma )
					{
						fEnergy += Erand;
						break;
					}
				}
			}
			//end simulating noise
			if(fEnergy>fThreshold)
			{
				fOutputMap.insert(make_pair(fTimeStart,fEnergy));
			}
			if(pIter==fHitsMap.end())
			{
					break;
			}
			fEnergy=pIter->second;
			fTimeStart=pIter->first;			
		}
		fLastTime=pIter->first;
		pIter++;
	}
}

void ybSD_Scint::OutputDigiData()
{
	ybSignalMap::iterator pIter=fOutputMap.begin();
	while( pIter!=fOutputMap.end() )
	{
		pNewDigi=new ybDataDigi();
		pNewDigi->AddTime(pIter->first);
		pNewDigi->AddEnergy(pIter->second);
		pDigiCollection->insert(pNewDigi);		
		pIter++;
	}

}

void ybSD_Scint::OutputHitsData()
{
}