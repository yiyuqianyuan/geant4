
#include "ybVDetectorConstruction.hh"

ybVDetectorConstruction::ybVDetectorConstruction()
{
	G4cout<<"creating DetectorConstruction ..."<<G4endl;
}

ybVDetectorConstruction::~ybVDetectorConstruction()
{
	for(pIter=pDetectorVector.begin();pIter!=pDetectorVector.end();pIter++)
	{
		delete (*pIter);
		*pIter=NULL;
	}
	pDetectorVector.clear();
	G4cout<<"deleting DetectorConstruction ..."<<G4endl;
}

void ybVDetectorConstruction::RegisterDetectors(ybVDetector *pDetector)
{
	pDetectorVector.push_back(pDetector);
}

G4VPhysicalVolume* ybVDetectorConstruction::Construct()
{
	pPhysiWorld=WorldConstruct();
	for(pIter=pDetectorVector.begin();pIter!=pDetectorVector.end();pIter++)
	{
		(*pIter)->DetectorConstruct();
	}
	return pPhysiWorld;
}