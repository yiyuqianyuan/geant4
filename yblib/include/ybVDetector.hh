#ifndef ybVDetector_H
#define ybVDetector_H 1
//=================GEANT4=================//
#include "globals.hh"

class ybVDetector
{
public:
	ybVDetector(G4String name);
	ybVDetector();
	virtual ~ybVDetector();
	ybVDetector(const ybVDetector &right);
	const ybVDetector & operator=(const ybVDetector &right);
	G4int operator==(const ybVDetector &right) const;
	G4int operator!=(const ybVDetector &right) const;	
	
    virtual void DetectorConstruct()=0;

protected:
	G4String fDetectorName;
	G4String fThePathName;
	G4String fFullPathName;
};

#endif

