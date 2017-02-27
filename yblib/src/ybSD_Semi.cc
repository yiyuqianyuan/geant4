#include "ybSD_Semi.hh"

ybSD_Semi::ybSD_Semi(G4String name,int numx,G4double widx,int numy,G4double widy):G4VSensitiveDetector(name)
{
	G4cout<<"creating ybSD_Semi: "<<SensitiveDetectorName<<"..."<<G4endl;
	fShape=kGeoBox;
	//initialize the variables
	fNum_X=numx;		//number of strips
	fNum_Y=numy;
	fWidth_X=widx;		//width of strip 
	fWidth_Y=widy;	
	fTotNum=numx+numy;
	
	fHCID=-1;
	fDCID=-1;
	fResolution=0.01;
	fFano=0.12;
	fEnergyGap=3.6*eV;
	fNoise=0.1*keV;	//default fNoise
	fTimeInterval=0.001*ms;
	fTimeGate=0.004*ms;
	fThreshold=10.*keV;
	fResolutionType=kNull;
	fHitsMap=new ybSignalMap[fTotNum];
	fOutputMap=new ybSignalMap[fTotNum];
	
	collectionName.insert(SensitiveDetectorName);
	pDM_Amplifier=new ybDM_Amplifier(SensitiveDetectorName,fNum_X,fNum_Y);
	pDigiMan=G4DigiManager::GetDMpointer();
	pDigiMan->AddNewModule(pDM_Amplifier);
}

ybSD_Semi::ybSD_Semi(G4String name,G4double rin, G4double rout, G4double angle,
				int cirnum, int linenum):G4VSensitiveDetector(name)
{
	G4cout<<"creating ybSD_Semi: "<<SensitiveDetectorName<<"..."<<G4endl;
	fShape=kGeoTube;
	//initialize the variables
	fRin=rin;
	fRout=rout;	
	fAngle=angle;
	fCirnum=cirnum;
	fLinenum=linenum;
	fTotNum=cirnum+linenum;
	
	fHCID=-1;
	fDCID=-1;
	fResolution=0.01;
	fFano=0.12;
	fEnergyGap=3.6*eV;
	fNoise=0.1*keV;	//default fNoise
	fTimeInterval=0.001*ms;
	fTimeGate=0.004*ms;
	fThreshold=10.*keV;
	fResolutionType=kNull;
	fHitsMap=new ybSignalMap[fTotNum];
	fOutputMap=new ybSignalMap[fTotNum];
	
	collectionName.insert(SensitiveDetectorName);
	pDM_Amplifier=new ybDM_Amplifier(SensitiveDetectorName,fCirnum,fLinenum);
	pDigiMan=G4DigiManager::GetDMpointer();
	pDigiMan->AddNewModule(pDM_Amplifier);
}

ybSD_Semi::~ybSD_Semi()
{ 
	delete[] fHitsMap;
	fHitsMap=NULL;
	delete[] fOutputMap;
	fOutputMap=NULL;
	G4cout<<"deleting ybSD_Semi: "<<SensitiveDetectorName<<"..."<<G4endl;
}

void ybSD_Semi::Initialize(G4HCofThisEvent* pHCE)
{
	//Initialize map
	for(int i=0;i<fTotNum;i++)
	{
		fHitsMap[i].clear();
		fOutputMap[i].clear();
		multimap<G4double,G4double,less<G4double> >().swap(fHitsMap[i]); 
		multimap<G4double,G4double,less<G4double> >().swap(fOutputMap[i]); 
	}
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
		fDMname=pDM_Amplifier->GetName();
		fDCname=pDM_Amplifier->GetCollectionName(0);
		fDCID=pDigiMan->GetDigiCollectionID(fDCname);
	}
	pDigiCollection= new ybDataDigiCollection(fDMname,fDCname); 
	pDigiMan->SetDigiCollection(fDCID,pDigiCollection);
}

G4bool ybSD_Semi::ProcessHits(G4Step* pStep,G4TouchableHistory* )
{
	G4StepPoint* pPreStepPoint = pStep->GetPreStepPoint();
	G4StepPoint* pPostStepPoint = pStep->GetPostStepPoint();
	//energy
	fDeltaE=pStep->GetTotalEnergyDeposit();
	//fGlobaltime	
	fGlobaltime=pPostStepPoint->GetGlobalTime();
	//local position
	G4TouchableHandle theTouchable = pPreStepPoint->GetTouchableHandle();
	G4ThreeVector fTheGlobalPoint = pPreStepPoint->GetPosition();
	G4ThreeVector fTheLocalPoint = theTouchable->GetHistory()->GetTopTransform().TransformPoint(fTheGlobalPoint);

	G4double fPos_x=fTheLocalPoint.getX();
	G4double fPos_y=fTheLocalPoint.getY();
	//for Digi
	if(fShape==kGeoBox && fDeltaE>0.)
	{
		int id_x=int(fPos_x/fWidth_X+0.5*fNum_X);
		int id_y=int(fPos_y/fWidth_Y+0.5*fNum_Y);
		if(id_x>=fNum_X || id_x<0)	return false;
		if(id_y>=fNum_Y || id_y<0)	return false;
		fHitsMap[id_x].insert(make_pair(fGlobaltime,fDeltaE));
		fHitsMap[fNum_X+id_y].insert(make_pair(fGlobaltime,fDeltaE));		
	}else if(fShape==kGeoTube && fDeltaE>0.)
	{
		G4double fPos_r=sqrt(fPos_x*fPos_x+fPos_y*fPos_y);
		G4double fPhi=fTheLocalPoint.getPhi();
		
		int id_cir=int((fPos_r-fRin)*fCirnum/(fRout-fRin));
		int id_line=int(fPhi*fLinenum/fAngle);
		if(id_cir>=fCirnum || id_cir<0)		return false;
		if(id_line>=fLinenum || id_line<0)	return false;
		fHitsMap[id_cir].insert(make_pair(fGlobaltime,fDeltaE));		
		fHitsMap[fCirnum+id_line].insert(make_pair(fGlobaltime,fDeltaE));
		
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

void ybSD_Semi::EndOfEvent(G4HCofThisEvent* )
{
	CreatSignal();
	OutputDigiData();	
}

void ybSD_Semi::CreatSignal()
{
	//calculate outputs for every strips
	G4double fTimeStart=-1.;
	G4double fLastTime=-1.;
	G4double fEnergy=0.;
	G4double fSegma_intr=0.;
	G4double fSigma_reso=0.;
	
	ybSignalMap::iterator pIter=fHitsMap[0].begin();
	for(int i=0;i<fTotNum;i++)
	{
		if(fHitsMap[i].empty())
		{
			continue;
		}
		pIter=fHitsMap[i].begin();
		fLastTime=pIter->first;
		fTimeStart=pIter->first;
		fEnergy=0.;
		
		while(1)
		{
			if(pIter!=fHitsMap[i].end() && pIter->first<fLastTime+fTimeInterval)
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
				}else if(fResolutionType==kNoise)
				{
					fSegma_intr=sqrt(fFano*fEnergyGap*fEnergy);
					fSigma_reso=sqrt(pow(fSegma_intr,2)+pow(fNoise/2.355,2));
					G4double Erand=0.;
					while(1)
					{
						Erand=CLHEP::RandGauss::shoot(0.,fSigma_reso);
						if( fabs(Erand)<3.*fSigma_reso )
						{
							fEnergy += Erand;
							break;
						}
					}
				}	
				//end simulating noise
				if(fEnergy>fThreshold)
				{
					fOutputMap[i].insert(make_pair(fTimeStart,fEnergy));
				}
				if(pIter==fHitsMap[i].end())
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
}

void ybSD_Semi::OutputDigiData()
{
	G4double fMinTimeStart=-1.;
	G4double fEnergy=-1.;
	ybSignalMap::iterator pIter=fOutputMap[0].begin();
	do
	{
		fMinTimeStart=MinTime();
		if(fMinTimeStart<0)
		{
			break;
		}
		pNewDigi=new ybDataDigi();
		for(int i=0;i<fTotNum;i++)
		{	
			fEnergy=-1.;
			if(!fOutputMap[i].empty())
			{
				pIter=fOutputMap[i].begin();
				if(pIter->first<fMinTimeStart+fTimeGate)
				{
					fEnergy=pIter->second;
					fOutputMap[i].erase(pIter);
				}
			}	
			if(fEnergy>0.)
			{
				pNewDigi->AddTime(fMinTimeStart);
				pNewDigi->AddEnergy(fEnergy);
			}else
			{
				pNewDigi->AddTime(fMinTimeStart);
				pNewDigi->AddEnergy(0.);
			}
		}
		pDigiCollection->insert(pNewDigi);
	}while(fMinTimeStart>0.);
}

void ybSD_Semi::OutputHitsData()
{

}

G4double ybSD_Semi::MinTime()
{
	G4double fTime=-1.;
	ybSignalMap::iterator pIter;
	for(int i=0;i<fTotNum;i++)
	{
		if(!fOutputMap[i].empty())
		{
			pIter=fOutputMap[i].begin();
			if(fTime<0.)
			{
				fTime=pIter->first;
			}else if(fTime>pIter->first)
			{
				fTime=pIter->first;
			}
		}
	}
	return fTime;
}



