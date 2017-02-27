#ifndef EventAction_h
#define EventAction_h 1

#include "ybVEventAction.hh"
#include "G4DigiManager.hh"
class EventAction : public ybVEventAction
{
public:
	EventAction();
	~EventAction();

	ybLogicMap* UserDigiTrigger();
	
private:

};
#endif

    
