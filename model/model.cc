//===================C++==================//
#include <ctime>// initialize random seed
#include <string>
//=================GEANT4=================//
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
//==================User==================//
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
int main(int argc,char** argv)
{

	//===========================Set the Seed of Random===========================//
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	CLHEP::HepRandom::setTheSeed(time(NULL));
	
	//================================Main Codes==================================//
	G4RunManager* runManager = new G4RunManager();
	//Detector
	DetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);
	//PhysicsList
	PhysicsList* physics= new PhysicsList(); 
	runManager->SetUserInitialization(physics);

	//Action
	PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction;
	runManager->SetUserAction(gen_action);
	if(argc==1)
	{
		G4UserSteppingAction* stepping_action =new SteppingAction;
		runManager->SetUserAction(stepping_action);
		G4UserTrackingAction* tracking_action =new TrackingAction;
		runManager->SetUserAction(tracking_action);
		G4UserEventAction* event_action = new EventAction;
		runManager->SetUserAction(event_action);
		G4UserRunAction* run_action = new RunAction;
		runManager->SetUserAction(run_action);
	}

	//============================Initialize G4 kernel============================// 	
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
	runManager->Initialize();
	
	//==========================Control the Verbose Level=========================//
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/run/verbose 0");
	UImanager->ApplyCommand("/event/verbose 0");
	UImanager->ApplyCommand("/tracking/verbose 0");
	
	//===================================BeamOn==================================//
	if(argc==1)
	{
		for(int runid=0;runid<1;runid++)
		{
			runManager->BeamOn(1);
		}
	}else if(string(argv[1])=="vis")
	{
		G4UIExecutive *ui=new G4UIExecutive(argc,argv);
		UImanager->ApplyCommand("/control/execute vis.mac"); 	
		ui->SessionStart();
		delete ui;
		ui=NULL;
	}
	
	delete visManager;
	visManager=NULL;	
	delete runManager;
	runManager=NULL;
	
	return 0;
}
