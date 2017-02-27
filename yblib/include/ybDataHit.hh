#ifndef ybDataHit_H
#define ybDataHit_H 1
//=================GEANT4=================//
#include "globals.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

using namespace std;
class ybDataHit : public G4VHit
{
public:

	ybDataHit();
	virtual ~ybDataHit();
	ybDataHit(const ybDataHit&);
	const ybDataHit& operator=(const ybDataHit&);
	G4int operator==(const ybDataHit&) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);
	inline G4double GetTime() {return fTime;}
	inline G4double GetKineticEnergy() {return fEnergy;}
	inline G4ThreeVector GetPosition() {return fPosition;}
	inline G4String GetParticleName() {return fParticleName;}
	inline G4String GetDetectorName() {return fDetectorName;}
	inline void SetTime(G4double time) {fTime=time;}
	inline void SetKineticEnergy(G4double energy) {fEnergy=energy;}
	inline void SetPosition(G4ThreeVector pos) {fPosition=pos;}
	inline void SetParticleName(G4String name) {fParticleName=name;}
	inline void SetDetectorName(G4String name) {fDetectorName=name;}
	
private:
	G4double fTime;
	G4double fEnergy;
	G4ThreeVector fPosition;
	G4String fParticleName;
	G4String fDetectorName;
};

//////////////////////////////////////////////////

typedef G4THitsCollection<ybDataHit> ybDataHitsCollection;
extern G4Allocator<ybDataHit> ybDataHitsAllocator;

inline void* ybDataHit::operator new(size_t)
{
	void *pDigi;
	pDigi = (void *) ybDataHitsAllocator.MallocSingle();
	return pDigi;
}

inline void ybDataHit::operator delete(void *pDigi)
{
	ybDataHitsAllocator.FreeSingle((ybDataHit*) pDigi);
}

#endif
