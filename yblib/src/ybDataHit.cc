#include "ybDataHit.hh"

G4Allocator<ybDataHit> ybDataHitsAllocator;

ybDataHit::ybDataHit() 
{
}

ybDataHit::~ybDataHit()
 {
 }

ybDataHit::ybDataHit(const ybDataHit& ):G4VHit()
{
}

const ybDataHit& ybDataHit::operator=(const ybDataHit& )
{
	return *this;
}

G4int ybDataHit::operator==(const ybDataHit& right) const
{
	return (this==&right) ? 1 : 0;
}
