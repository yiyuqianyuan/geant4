#include "ybDataDigi.hh"
G4Allocator<ybDataDigi> ybDataDigiAllocator;

ybDataDigi::ybDataDigi()
{
}

ybDataDigi::~ybDataDigi()
{

}

ybDataDigi::ybDataDigi(const ybDataDigi& ):G4VDigi()
{
}

const ybDataDigi& ybDataDigi::operator=(const ybDataDigi& )
{
	return *this;
}


int ybDataDigi::operator==(const ybDataDigi& right) const
{ 
	return (this==&right) ? 1 : 0;
}












