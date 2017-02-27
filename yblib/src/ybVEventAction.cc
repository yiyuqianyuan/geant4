#include "ybVEventAction.hh"

ybVEventAction::ybVEventAction():fCTime(-10.*ns)
{
	fDataType=kDigi;
	fTotalEventNumber=0;
	fPercentEvent=0;
	
	fDelayTime=0.;
	fGateWidth=0.004*ms;
	fEnergyUnit=MeV;
	fTimeUnit=ms;
	fDelayTime=0.;
	
	pDataMan=ybDataManager::GetInstance();
}
 
ybVEventAction::~ybVEventAction()
{
}
 
void ybVEventAction::BeginOfEventAction(const G4Event *pEvent)
{	
	fTotalEventNumber=G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
	fPercentEvent=fTotalEventNumber/10;
	if(fTotalEventNumber<100000)
	{
		if(pEvent->GetEventID()%10000==0)
		{
			G4cout<<"eventid:  "<<pEvent->GetEventID()<<G4endl;
		}
	}else
	{
		if((pEvent->GetEventID()+1)%fPercentEvent==0)
		{
			G4cout<<"progress:  "<<10*(pEvent->GetEventID()+1)/fPercentEvent<<"%"<<G4endl;
		}
	}
	vDigiTrigger.clear();
	vector<G4double>().swap(vDigiTrigger); 
	vHitsTrigger.clear();
	vector<G4double>().swap(vHitsTrigger);
}

void ybVEventAction::EndOfEventAction(const G4Event *pEvent)
{
	CreateTrigger();
	SaveDigiData(pEvent);
	SaveHitsData(pEvent);		
}

//=============================================================================================//
//=============================================================================================//
//=============================================================================================//

void ybVEventAction::CreateTrigger()
{
	fDataType=kDigi;
	ybLogicMap *pDigiLogicMap=UserDigiTrigger();	
	fDataType=kHits;
	ybLogicMap *pHitsLogicMap=UserHitsTrigger();
	
	ybLogicMap::iterator pIter;
	if(pDigiLogicMap)
	{
		pIter=pDigiLogicMap->begin();
		while(pIter!=pDigiLogicMap->end())
		{
			vDigiTrigger.push_back( pIter->first );
			pIter++;
		}
	}
	if(pHitsLogicMap)
	{
		pIter=pHitsLogicMap->begin();
		while(pIter!=pHitsLogicMap->end())
		{
			vHitsTrigger.push_back( pIter->first );
			pIter++;
		}
	}	
	for(size_t i=0;i<vLogicMapMan.size();i++)
	{
		delete vLogicMapMan[i];
		vLogicMapMan[i]=NULL;
	}
	vLogicMapMan.clear();
	vector<ybLogicMap*>().swap(vLogicMapMan);
}

ybLogicMap*  ybVEventAction::UserDigiTrigger()
{
	return NULL;
}

ybLogicMap*  ybVEventAction::UserHitsTrigger()
{
	return NULL;
}

void ybVEventAction::SaveDigiData(const G4Event *pEvent)
{
	if(vDigiTrigger.empty())
	{
		return;
	}
	size_t fNumOfTrigs=vDigiTrigger.size();		// number of triggers
	size_t fNumOfDC=pEvent->GetDCofThisEvent()->GetNumberOfCollections();		// numbers of detectors
	size_t fNumOfSignals=0;					//numbers of signals one detector gives
	size_t fNumOfChannels=0;					//numbers of one detector
	G4double fRealTime=-1.;
	G4double fStartTime=0.,fStopTime=0.;
	int fTheSignal=-1;
	ofstream *pFile_e=NULL;
	ofstream *pFile_t=NULL;
	ybDataDigiCollection *pDigiC=NULL;
	G4String fDCname="";

	for(size_t i=0;i<fNumOfTrigs;i++)		// loop for trigers
	{
		fStartTime=vDigiTrigger[i]+fDelayTime+fCTime;
		fStopTime=vDigiTrigger[i]+fDelayTime+fGateWidth;
		for(size_t j=0;j<fNumOfDC;j++)			// loop for detectors
		{
			pDigiC=(ybDataDigiCollection*)pEvent->GetDCofThisEvent()->GetDC(j);
			fDCname=pDigiC->GetName();	
			pFile_e=pDataMan->GetFile(fDCname+"(D)_e.txt");
			pFile_t=pDataMan->GetFile(fDCname+"(D)_t.txt");
			fNumOfSignals=pDigiC->entries();
			fNumOfChannels=GetNumOfChannels(fDCname);
			fTheSignal=-1;
			for(size_t k=0;k<fNumOfSignals;k++)		// loop for signals one detector gives
			{
				fRealTime=(*pDigiC)[k]->GetTime(0);
				if(fRealTime>fStartTime && fRealTime<fStopTime )
				{
					fTheSignal=k;
				}
			}
			if(fTheSignal>=0)
			{
				for(size_t w=0;w<fNumOfChannels;w++)
				{
					(*pFile_e)<<fixed<<setprecision(3)<<(*pDigiC)[fTheSignal]->GetEnergy(w)/fEnergyUnit<<" ";
					(*pFile_t)<<fixed<<setprecision(3)<<(*pDigiC)[fTheSignal]->GetTime(w)/fTimeUnit<<" ";
				}
			}else
			{
				for(size_t w=0;w<fNumOfChannels;w++)
				{

					(*pFile_e)<<fixed<<setprecision(3)<<0.<<" ";
					(*pFile_t)<<fixed<<setprecision(3)<<vDigiTrigger[i]/fTimeUnit<<" ";
				}
			}
			(*pFile_e)<<endl;
			(*pFile_t)<<endl;
		}
		SaveUserData();
	}
	
}

void ybVEventAction::SaveHitsData(const G4Event *pEvent)
{
	if(vHitsTrigger.empty())
	{
		return;
	}
}

void ybVEventAction::SaveUserData()
{
}
//=============================================================================================//
//=============================================================================================//
//=============================================================================================//

size_t ybVEventAction::GetNumOfChannels(G4String name,G4String type)
{
	size_t num=1,numx=0,numy=0;
	size_t leftpos = name.first('[');
	if(leftpos != std::string::npos)
	{	
		size_t midpos=name.first(',');
		size_t rightpos = name.first(']');
		G4String namex=name, namey=name;
		namex.remove(midpos,name.length()-midpos);
		namex.remove(0,leftpos+1);
		namey.remove(rightpos,name.length()-rightpos);
		namey.remove(0,midpos+1);
		stringstream ssx(namex);
		ssx>>numx;
		stringstream ssy(namey);
		ssy>>numy;
		num=numx+numy;
	}
	if(type=="F")
	{
		return numx;
	}else if(type=="B")
	{
		return numy;
	}else
	{
		return num;
	}
}

//Logic Calculation 
ybLogicMap* ybVEventAction::BuildSequenceMap(G4String name,G4double thr, size_t multi)
{	
	ybLogicMap* pLogicMap=new ybLogicMap;
	vLogicMapMan.push_back(pLogicMap);
	size_t fMultiTrigger=0;
	if(fDataType==kDigi)
	{
		G4DCofThisEvent *pDCofThisEvent=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetDCofThisEvent(); 
		int fDCID = G4DigiManager::GetDMpointer()->GetDigiCollectionID(name);
		if(fDCID<0)
		{
			cout<<"Error::ybVEventAction!! No DigiCollection named "<<name<<"  exists!"<<endl;
			exit(0);
		}
		ybDataDigiCollection *pDC=(ybDataDigiCollection*)pDCofThisEvent->GetDC(fDCID);
		size_t fNumOfSignals=pDC->entries();
		size_t fChannels=GetNumOfChannels(name,"F");
		for(size_t i=0;i<fNumOfSignals;i++)
		{
			fMultiTrigger=0;
			for(size_t j=0;j<fChannels;j++)
			{
				if( fabs( (*pDC)[i]->GetEnergy(j) )>fabs(thr) )
				{
					fMultiTrigger++;			
				}
			}
			if(fMultiTrigger>=multi)
			{
				pLogicMap->insert(make_pair((*pDC)[i]->GetTime(0),fMultiTrigger));
			}	
		}
	}else if(fDataType==kHits)
	{
		G4HCofThisEvent *pHCofThisEvent=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetHCofThisEvent(); 	
		int fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(name);
		if(fHCID<0)
		{
			cout<<"Error::ybVEventAction!! No HitsCollection named "<<name<<"  exists!"<<endl;
			exit(0);
		}
		ybDataHitsCollection *pHC=(ybDataHitsCollection*)pHCofThisEvent->GetHC(fHCID);
		size_t fNumOfSignals=pHC->entries();
		for(size_t i=0;i<fNumOfSignals;i++)
		{
			pLogicMap->insert(make_pair((*pHC)[i]->GetTime(),1));
		}
	}
	return pLogicMap;
}

ybLogicMap* ybVEventAction::LogicAND(ybLogicMap *pLogicMap_A, ybLogicMap *pLogicMap_B)
{
	if(pLogicMap_A->empty())
	{
		return pLogicMap_A;
	}
	if(pLogicMap_B->empty())
	{
		return pLogicMap_B;
	}
	ybLogicMap* pLogicMap=new ybLogicMap;
	vLogicMapMan.push_back(pLogicMap);
	
	ybLogicMap::iterator pIter_A=pLogicMap_A->begin();
	ybLogicMap::iterator pIter_B=pLogicMap_B->begin();
	
	G4bool fSignal_A=false;
	G4bool fSignal_B=false;
	G4double fTheMinTime=0.;
	while(pIter_A!=pLogicMap_A->end() && pIter_B!=pLogicMap_B->end() )
	{
		fSignal_A=false;
		fSignal_B=false;
		if(pIter_A->first < pIter_B->first)
		{
			fTheMinTime=pIter_A->first;
		}else
		{
			fTheMinTime=pIter_B->first;
		}
		if(fTheMinTime+fCTime<pIter_A->first && fTheMinTime+fCTime+fGateWidth>pIter_A->first)
		{
			pIter_A++;
			fSignal_A=true;
		}
		if(fTheMinTime+fCTime<pIter_B->first && fTheMinTime+fCTime+fGateWidth>pIter_B->first)
		{
			pIter_B++;
			fSignal_B=true;
		}
		
		if(fSignal_A && fSignal_B)
		{
			pLogicMap->insert(make_pair(fTheMinTime,1));
		}
	}
	
	return pLogicMap;
}

ybLogicMap* ybVEventAction::LogicOR(ybLogicMap *pLogicMap_A, ybLogicMap *pLogicMap_B)
{
	if(pLogicMap_A->empty())
	{
		return pLogicMap_B;
	}
	if(pLogicMap_B->empty())
	{
		return pLogicMap_A;
	}
	ybLogicMap* pLogicMap=new ybLogicMap;
	vLogicMapMan.push_back(pLogicMap);
	
	ybLogicMap::iterator pIter_A=pLogicMap_A->begin();
	ybLogicMap::iterator pIter_B=pLogicMap_B->begin();
	
	G4bool fSignal_A=false;
	G4bool fSignal_B=false;
	G4double fTheMinTime=0.;
	while(pIter_A!=pLogicMap_A->end() && pIter_B!=pLogicMap_B->end() )
	{
		fSignal_A=false;
		fSignal_B=false;
		if(pIter_A->first < pIter_B->first)
		{
			fTheMinTime=pIter_A->first;
		}else
		{
			fTheMinTime=pIter_B->first;
		}
		if(fTheMinTime+fCTime<pIter_A->first && fTheMinTime+fCTime+fGateWidth>pIter_A->first)
		{
			pIter_A++;
			fSignal_A=true;
		}
		if(fTheMinTime+fCTime<pIter_B->first && fTheMinTime+fCTime+fGateWidth>pIter_B->first)
		{
			pIter_B++;
			fSignal_B=true;
		}
		
		if(fSignal_A || fSignal_B)
		{
			pLogicMap->insert(make_pair(fTheMinTime,1));
		}
	}
	
	for(pIter_A=pLogicMap_A->begin();pIter_A!=pLogicMap_A->end();pIter_A++)
	{
		pLogicMap->insert(make_pair(pIter_A->first,1));
	}
	for(pIter_B=pLogicMap_B->begin();pIter_B!=pLogicMap_B->end();pIter_B++)
	{
		pLogicMap->insert(make_pair(pIter_B->first,1));
	}
	
	return pLogicMap;  
}

ybLogicMap* ybVEventAction::LogicMulties(G4String *name, G4double *thr, size_t num, size_t multies)
{	
	ybLogicMap** pLogicMapMulti=new ybLogicMap*[num];
	for(size_t i=0;i<num;i++)
	{
		pLogicMapMulti[i]=BuildSequenceMap(name[i],thr[i],1);
	}
	ybLogicMap* pLogicMap=LogicMulties(pLogicMapMulti, num, multies);
	delete[] pLogicMapMulti;
	return pLogicMap;		
}

ybLogicMap* ybVEventAction::LogicMulties(ybLogicMap **pLogicMapMulti, size_t num, size_t multies)
{
	ybLogicMap* pLogicMap=new ybLogicMap;
	vLogicMapMan.push_back(pLogicMap);
	G4double fTheMinTime=0.;
	size_t fMultiSignal=0;
	ybLogicMap::iterator pIter;
	while( !MultiEmpty(pLogicMapMulti,num))
	{
		fMultiSignal=0;
		fTheMinTime=MinTime(pLogicMapMulti,num);
		for(size_t i=0;i<num;i++)
		{
			pIter=pLogicMapMulti[i]->begin();
			if(pIter==pLogicMapMulti[i]->end())
			{
				continue;
			}else if(fTheMinTime+fCTime<pIter->first && fTheMinTime+fCTime+fGateWidth>pIter->first)
			{
				pLogicMapMulti[i]->erase(pIter);
				fMultiSignal += pIter->second;
			}
		}
		if(fMultiSignal>=multies)
		{
			pLogicMap->insert(make_pair(fTheMinTime,fMultiSignal));
		}
	}
	return pLogicMap;
}

ybLogicMap* ybVEventAction::LogicMultiAND(G4String *name, G4double *thr, size_t num)
{
	return LogicMulties(name, thr, num, num);
}

ybLogicMap* ybVEventAction::LogicMultiOR(G4String *name, G4double *thr, size_t num)
{
	return LogicMulties(name, thr, num, 1);	 
}

G4double ybVEventAction::MinTime(ybLogicMap **pLogicMapMulti,size_t num)
{
	G4double fTime=1.e15;
	ybLogicMap::iterator pIter;
	for(size_t i=0;i<num;i++)
	{
		pIter=pLogicMapMulti[i]->begin();
		if(pIter==pLogicMapMulti[i]->end())
		{
			continue;
		}else if(fTime>pIter->first)
		{
			fTime=pIter->first;
		}
	}
	return fTime;
}

bool ybVEventAction::MultiEmpty(ybLogicMap **pLogicMapMulti,size_t num)
{
	size_t fMulti=0;
	for(size_t i=0;i<num;i++)
	{
		if(pLogicMapMulti[i]->empty())
		{
			fMulti++;
		}
	}
	if(fMulti==num)
	{
		return true;
	}else
	{
		return false;
	}
}
