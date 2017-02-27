#include "ybSemi.hh"

ybSemi::ybSemi(G4String name,G4VPhysicalVolume *world,G4double th,
				G4double widx,int numx,G4double widy,int numy) : ybVDetector(name)
{
	G4cout<<"creating ybSemi "<<fDetectorName<<"..."<<G4endl;
	pLogicWorld=world->GetLogicalVolume();
	fShape=kGeoBox;
	
	fThickness=th;		//the thickness of Semi
	fNum_X=numx;		//number of strips
	fWidth_X=widx;		//width of strip 

	fNum_Y=numy;		//number of strips
	fWidth_Y=widy;		//width of strip 
	
	pSD_Semi=new ybSD_Semi(fDetectorName,fNum_X,fWidth_X,fNum_Y,fWidth_Y);
	G4SDManager *pSDMan=G4SDManager::GetSDMpointer();
	pSDMan->AddNewDetector(pSD_Semi);	
}


ybSemi::ybSemi(G4String name,G4VPhysicalVolume *world,G4double rin,G4double rout,
				G4double th, G4double angle, int cirnum, int linenum) : ybVDetector(name)
{
	G4cout<<"creating ybSemi "<<fDetectorName<<"..."<<G4endl;
	pLogicWorld=world->GetLogicalVolume();
	fShape=kGeoTube;
	
	fRin=rin;
	fRout=rout;	
	fThickness=th;
	fAngle=angle;
	fCirnum=cirnum;
	fLinenum=linenum;

	pSD_Semi=new ybSD_Semi(fDetectorName,fRin,fRout,fAngle,fCirnum,fLinenum);
	G4SDManager *pSDMan=G4SDManager::GetSDMpointer();
	pSDMan->AddNewDetector(pSD_Semi);	
}

ybSemi::ybSemi() : ybVDetector()
{
}

ybSemi::~ybSemi()
{
	G4cout<<"deleting ybSemi "<<fDetectorName<<"..."<<G4endl;
}

void ybSemi::Material()
{
	//G4NistManager *pNistmMan=G4NistManager::Instance();
	//Si
	pSilicon=G4Material::GetMaterial("Semi_Silicon",false);
	if(!pSilicon)
	{
		pSilicon=new G4Material("Semi_Silicon",14.,28.00*g/mole,2.33*g/cm3);
	}
}

void ybSemi::DetectorConstruct()
{
	Material();
	//==============Detector===========//
	if(fShape==kGeoBox)
	{
		pSolidSemi= new G4Box("Solid"+fDetectorName,0.5*fNum_X*fWidth_X,0.5*fNum_Y*fWidth_Y,0.5*fThickness);
		pLogicSemi= new G4LogicalVolume(pSolidSemi, pSilicon, "Logic"+fDetectorName,0,0,0);
		pPhysiSemi= new G4PVPlacement(pRotation,fTranslation,pLogicSemi,"Physi"+fDetectorName,pLogicWorld,false,0);		
	}else if(fShape==kGeoTube)
	{
		pSolidSemi= new G4Tubs("Solid"+fDetectorName,fRin,fRout,0.5*fThickness,0.,fAngle);
		pLogicSemi= new G4LogicalVolume(pSolidSemi, pSilicon, "Logic"+fDetectorName,0,0,0);
		pPhysiSemi= new G4PVPlacement(pRotation,fTranslation,pLogicSemi,"Physi"+fDetectorName,pLogicWorld,false,0);				
	}
	
	pLogicSemi->SetSensitiveDetector(pSD_Semi);

	Visualization();
}

void ybSemi::Visualization()
{
	//--------- Visualization attributes -------------------------------
	G4VisAttributes *pSemiVisAtt = new G4VisAttributes(G4Colour::Blue());
	//Si Strips
	pLogicSemi->SetVisAttributes(pSemiVisAtt);
}	

