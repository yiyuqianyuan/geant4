#ifndef ybDataDigi_h
#define ybDataDigi_h 1
//===================C++==================//
#include <vector>
//=================GEANT4=================//
#include "globals.hh"
#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
using namespace std;

class ybDataDigi : public G4VDigi
{
public:
  
	ybDataDigi();
	virtual ~ybDataDigi();
	ybDataDigi(const ybDataDigi&);
	const ybDataDigi& operator=(const ybDataDigi&);
	int operator==(const ybDataDigi&) const;
  
	inline void* operator new(size_t);
	inline void  operator delete(void*);
	
	inline G4double GetTime(int i);
	inline G4double GetEnergy(int i);
	inline void AddTime(G4double time) {vTime.push_back(time);}
	inline void AddEnergy(G4double energy) {vEnergy.push_back(energy);}

private:
	typedef vector<G4double> ybDigiVector;
	ybDigiVector vEnergy;
	ybDigiVector vTime;
};

//////////////////////////////////////////////////
typedef G4TDigiCollection<ybDataDigi> ybDataDigiCollection;
extern G4Allocator<ybDataDigi> ybDataDigiAllocator;

inline void* ybDataDigi::operator new(size_t)
{
	void *pDigi;
	pDigi = (void *) ybDataDigiAllocator.MallocSingle();
	return pDigi;
}

inline void ybDataDigi::operator delete(void* pDigi)
{
	ybDataDigiAllocator.FreeSingle((ybDataDigi*) pDigi);

}

inline G4double ybDataDigi::GetEnergy(int i) 
{	
	if(i<(int)vEnergy.size())
	{
		return vEnergy[i];
	}else
	{
		G4cout<<"Error::ybDataDigi!!  i > vEnergy.size() !!"<<G4endl;
		exit(0);
	}
}

inline G4double ybDataDigi::GetTime(int i) 
{	
	if(i<(int)vTime.size())
	{
		return vTime[i];
	}else
	{
		G4cout<<"Error::ybDataDigi!!  i > vTime.size() !!"<<G4endl;
		exit(0);
	}
}
#endif









