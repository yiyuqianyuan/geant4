#include "ybDM_Amplifier.hh"

ybDM_Amplifier::ybDM_Amplifier(G4String name) : ybVDigitizerModule(name)
{
	collectionName.push_back(moduleName);
}

ybDM_Amplifier::ybDM_Amplifier(G4String name,int ch_x, int ch_y) : ybVDigitizerModule(name)
{
	std::stringstream ss;
	ss.clear();
	ss.str("");
	ss<<ch_x<<","<<ch_y;
	G4String fDCName=moduleName+"["+ss.str()+"]";	
	collectionName.push_back(fDCName);
}

ybDM_Amplifier::~ybDM_Amplifier()
{

}

void ybDM_Amplifier::Digitize()
{
}

















