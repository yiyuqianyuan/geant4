#include "ybVDetector.hh"

ybVDetector::ybVDetector(G4String name)
{	
	size_t sLast = name.last('/');
	if(sLast==std::string::npos)
	{ // detector name only
		fDetectorName = name;
		fThePathName = "/";
	}
	else
	{ // name conatin the directory path
		fDetectorName = name;
		fDetectorName.remove(0,sLast+1);
		fThePathName = name;
		fThePathName.remove(sLast+1,name.length()-sLast);
		if(fThePathName(0)!='/') 
		{
			fThePathName.prepend("/");
		}
	}
	fFullPathName = fThePathName + fDetectorName;	
}

ybVDetector::ybVDetector()
{
}

ybVDetector::~ybVDetector()
{

}

ybVDetector::ybVDetector(const ybVDetector &right)
{
	fDetectorName = right.fDetectorName;
	fThePathName = right.fThePathName;
	fFullPathName = right.fFullPathName;
}
const ybVDetector & ybVDetector::operator=(const ybVDetector &right)
{
	fDetectorName = right.fDetectorName;
	fThePathName = right.fThePathName;
	fFullPathName = right.fFullPathName;
	return *this;
}

G4int ybVDetector::operator==(const ybVDetector &right) const
{
	return (this==&right);
}

G4int ybVDetector::operator!=(const ybVDetector &right) const
{
	return (this!=&right);
}




















