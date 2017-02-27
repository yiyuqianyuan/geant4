#include "ybRunAction.hh"

ybRunAction::ybRunAction()
{
	pTimer = new G4Timer;
	pDataMan=ybDataManager::GetInstance();
}

ybRunAction::~ybRunAction()
{
	delete pTimer;
	pTimer=NULL;
}

void ybRunAction::BeginOfRunAction(const G4Run* pRun)
{
	fRunID=pRun->GetRunID();
	pDataMan->SetRunNum(fRunID);
	pTimer->Start();
	G4cout<<"======================	  RunID = "<<fRunID<<"  ======================"<<G4endl;
}

void ybRunAction::EndOfRunAction(const G4Run* pRun)
{
	pDataMan->CloseAllFiles();
	SaveData();

	pDataMan->ResetFileMap();
	pTimer->Stop();
	G4cout<<"number of event = "<<pRun->GetNumberOfEvent()<<G4endl;
	G4cout<<" time:  "<<*pTimer<<G4endl;
}

void ybRunAction::SaveData()
{
	pDataMan->CreateRootFile();
}

