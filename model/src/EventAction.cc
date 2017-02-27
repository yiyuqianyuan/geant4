#include "EventAction.hh"

EventAction::EventAction()
{


}
 
EventAction::~EventAction()
{
}

ybLogicMap* EventAction::UserDigiTrigger()
{
	fEnergyUnit=MeV;
	fTimeUnit=ms;
	fDelayTime=0.;
	fGateWidth=0.004*ms;	
	
	
//	return LogicCoincidence("AND","D1[40,40]","D2[40,40]");
//	return BuildSequenceMap("D1[40,40]");

	G4String name[5]={"D1[40,40]","D2[40,40]","D3[40,40]","D4[40,40]","D5[40,40]"};
	G4double thr[5]={0.,0.,0.,0.,0.};
	return LogicMultiOR(name, thr, 5);


}

