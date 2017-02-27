#include "ybScint.hh"

ybScint::ybScint(G4String name,G4VPhysicalVolume* world,G4double x,G4double y,G4double z) : ybVDetector(name)
{
	G4cout<<"creating ybScint "<<fDetectorName<<"..."<<G4endl;
	pLogicWorld=world->GetLogicalVolume();
	fShape=kGeoBox;
	
	fSize_x=x;
	fSize_y=y;
	fSize_z=z;

	pSD_Scint=new ybSD_Scint(fDetectorName);
	G4SDManager *pSDman=G4SDManager::GetSDMpointer();
	pSDman->AddNewDetector(pSD_Scint);
}

ybScint::ybScint(G4String name,G4VPhysicalVolume* world,G4double r1,G4double r2,G4double h,G4double phi1,G4double phi2) : ybVDetector(name)
{
	G4cout<<"creating ybScint "<<fDetectorName<<"..."<<G4endl;
	pLogicWorld=world->GetLogicalVolume();
	fShape=kGeoTube;
	
	fRmin=r1;
	fRmax=r2;	
	fDz=h;	
	fSphi=phi1;
	fDphi=phi2;
	
	pSD_Scint=new ybSD_Scint(fDetectorName);
	G4SDManager *pSDman=G4SDManager::GetSDMpointer();
	pSDman->AddNewDetector(pSD_Scint);
}

ybScint::ybScint() : ybVDetector()
{
}

ybScint::~ybScint()
{
	G4cout<<"deleting ybScint "<<fDetectorName<<"..."<<G4endl;
}

void ybScint::Material()
{
	G4NistManager *pNistMan=G4NistManager::Instance();
	//NaI
	if(fScintillator==kTypeNaI)
	{
		pMaterial=G4Material::GetMaterial("Scint_NaI",false);
		if(!pMaterial)
		{	
			G4Element *pNa=pNistMan->FindOrBuildElement("Na");
			G4Element *pI=pNistMan->FindOrBuildElement("I");
			G4Element *pTl=pNistMan->FindOrBuildElement("Tl");
			pMaterial=new G4Material("Scint_NaI", 3.67*g/cm3, 3, kStateSolid);
			pMaterial->AddElement(pNa, 15.29*perCent);
			pMaterial->AddElement(pI, 84.41*perCent);
			pMaterial->AddElement(pTl, 0.3*perCent);
		}
	}else if(fScintillator==kTypeCsI)
	{
		pMaterial=G4Material::GetMaterial("Scint_CsI",false);
		if(!pMaterial)
		{
			G4Element *pCs=pNistMan->FindOrBuildElement("Cs");
			G4Element *pI=pNistMan->FindOrBuildElement("I");
			G4Element *pTl=pNistMan->FindOrBuildElement("Tl");
			pMaterial=new G4Material("Scint_CsI", 4.51*g/cm3, 3, kStateSolid);
			pMaterial->AddElement(pCs, 51.08*perCent);
			pMaterial->AddElement(pI, 48.77*perCent);
			pMaterial->AddElement(pTl, 0.15*perCent);
		}
	}else if(fScintillator==kTypeBGO)
	{
		pMaterial=G4Material::GetMaterial("Scint_BGO",false);
		if(!pMaterial)
		{
			G4Element *pBi=pNistMan->FindOrBuildElement("Bi");
			G4Element *pGe=pNistMan->FindOrBuildElement("Ge");
			G4Element *pO=pNistMan->FindOrBuildElement("O");
			pMaterial=new G4Material("Scint_BGO", 7.13*g/cm3, 3, kStateSolid);
			pMaterial->AddElement(pBi,4);
			pMaterial->AddElement(pGe,3);
			pMaterial->AddElement(pO,12);
		}		
	}else if(fScintillator==kTypeBC408)
	{
		pMaterial=G4Material::GetMaterial("Scint_BC408",false);
		if(!pMaterial)
		{
			G4Element *pC=pNistMan->FindOrBuildElement("C");
			G4Element *pH=pNistMan->FindOrBuildElement("H");
			pMaterial=new G4Material("Scint_BC408", 7.13*g/cm3, 2, kStateSolid);
			pMaterial->AddElement(pC,10);
			pMaterial->AddElement(pH,11);
		}		
	}
	
}

void ybScint::DetectorConstruct()
{
	if(fScintillator==kTypeNull)
	{
		G4cout<<"Error::ybScint!!  No 'ScintType' has been set in ybScint!!"<<G4endl;
		exit(0);
	}
	Material();
	if(fShape==kGeoBox)
	{
		pSolidScint= new G4Box("Solid"+fDetectorName,0.5*fSize_x,0.5*fSize_y,0.5*fSize_z);
		pLogicScint= new G4LogicalVolume(pSolidScint, pMaterial, "Logic"+fDetectorName,0,0,0);
		pPhysiScint= new G4PVPlacement(pRotation,fTranslation,pLogicScint,"Physi"+fDetectorName,pLogicWorld,false,0);
	}else if(fShape==kGeoTube)
	{
		pSolidScint= new G4Tubs("Solid"+fDetectorName,fRmin,fRmax,0.5*fDz,fSphi,fDphi);
		pLogicScint= new G4LogicalVolume(pSolidScint, pMaterial, "Logic"+fDetectorName,0,0,0);
		pPhysiScint= new G4PVPlacement(pRotation,fTranslation,pLogicScint,"Physi"+fDetectorName,pLogicWorld,false,0);
	}
	pLogicScint->SetSensitiveDetector(pSD_Scint);
	
	Visualization();
}

void ybScint::Visualization()
{
	G4VisAttributes* pScintVisAtt = new G4VisAttributes(G4Colour::Yellow());
	pLogicScint->SetVisAttributes(pScintVisAtt);
}	

