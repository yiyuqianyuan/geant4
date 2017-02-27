
#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{

	//World
	fSizeWorld[0]=300*cm;
	fSizeWorld[1]=300*cm;
	fSizeWorld[2]=300*cm;
}

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction::WorldConstruct()
{
	//Vacuum
	G4NistManager* man = G4NistManager::Instance();
	G4Element *N = man->FindOrBuildElement("N");
	G4Element *O = man->FindOrBuildElement("O");
	G4Material* pVacuum = new G4Material("Vacuum",1.29e-10*mg/cm3,2,kStateGas);
	pVacuum->AddElement(N, 70.*perCent);
	pVacuum->AddElement(O, 30.*perCent);
	//----------
	const int fEntries=2;
	G4double fPhotonEnergy[fEntries]={1.*eV,10.*eV};
	double fVacuumRefractive[fEntries]={1.,1.};
	G4MaterialPropertiesTable *pVacuumMPT= new G4MaterialPropertiesTable(); 
	pVacuumMPT->AddProperty("RINDEX",fPhotonEnergy,fVacuumRefractive,fEntries);
	pVacuum->SetMaterialPropertiesTable(pVacuumMPT);
	
	//world 
  	pSolidWorld= new G4Box("solidWorld",0.5*fSizeWorld[0],0.5*fSizeWorld[1],0.5*fSizeWorld[2]);
  	pLogicWorld= new G4LogicalVolume(pSolidWorld,pVacuum, "logicWorld",0,0,0);
  	pPhysiWorld= new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicWorld,"physiWorld",0,false,0);
		

	//===========================Semi detector==============================//
	const G4double thickness=500.*um;
	
	ybSemi *d1=new ybSemi("D1",pPhysiWorld,thickness,1.*mm,40,1.*mm,40);
	G4RotationMatrix *ro_d1=new G4RotationMatrix();
//	ro_d1->rotateZ(90*deg);
	G4ThreeVector tr_d1(0.,-100.*mm,30.*mm);
	d1->SetPosition(ro_d1,tr_d1);
//	d1->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d1);
	
	ybSemi *d2=new ybSemi("D2",pPhysiWorld,thickness,1.*mm,40,1.*mm,40);
	G4RotationMatrix *ro_d2=new G4RotationMatrix();
//	ro_d2->rotateZ(90*deg);
	G4ThreeVector tr_d2(0.,-50.*mm,30.*mm);
	d2->SetPosition(ro_d2,tr_d2);
//	d2->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d2);	
	
	ybSemi *d3=new ybSemi("D3",pPhysiWorld,thickness,1.*mm,40,1.*mm,40);
	G4RotationMatrix *ro_d3=new G4RotationMatrix();
//	ro_d3->rotateZ(90*deg);
	G4ThreeVector tr_d3(0.,0.,30.*mm);
	d3->SetPosition(ro_d3,tr_d3);
//	d3->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d3);	

	ybSemi *d4=new ybSemi("D4",pPhysiWorld,thickness,1.*mm,40,1.*mm,40);
	G4RotationMatrix *ro_d4=new G4RotationMatrix();
//	ro_d4->rotateZ(90*deg);
	G4ThreeVector tr_d4(0.,50.*mm,30.*mm);
	d4->SetPosition(ro_d4,tr_d4);
//	d4->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d4);		
	
	ybSemi *d5=new ybSemi("D5",pPhysiWorld,thickness,1.*mm,40,1.*mm,40);
	G4RotationMatrix *ro_d5=new G4RotationMatrix();
//	ro_d5->rotateZ(90*deg);
	G4ThreeVector tr_d5(0.,100.*mm,30.*mm);
	d5->SetPosition(ro_d5,tr_d5);
//	d5->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d5);	

/*
	ybSemi *d2=new ybSemi("D2",pPhysiWorld,80.*mm,40,1.*mm,1000.0*um);
	G4RotationMatrix *ro_d2=new G4RotationMatrix();
//	ro_dssd->rotateZ(90*deg);
	G4ThreeVector tr_d2(0.,0.,40.*mm);
	d2->SetPosition(ro_d2,tr_d2);
//	dssd->GetSD()->SetResolution(0.12,3.6*eV,1.3*keV);//FanoFactor,IonizationEnergy,ElectronicNoise
	RegisterDetectors(d2);	
*/
/*	
	//===========================ExNaI detector==============================//
	ybExNaI *pNaI=new ybExNaI("ExNaI",pPhysiWorld,40.*mm,40.*mm,41.*mm);
	G4RotationMatrix *pRo_NaI=new G4RotationMatrix();
	G4ThreeVector fTr_NaI(0.,0.,80.*mm);
	pNaI->SetMylarThickness(2.0*um);
	pNaI->SetPosition(pRo_NaI,fTr_NaI);
	pNaI->GetSD()->SetAmpType(ybExSD_Scint::kPMT);
//	(ybPMT *)pNaI->GetSD()->GetAmplifier();
	RegisterDetectors(pNaI);
*/
/*
	//===========================NaI detector==============================//	
	ybScint *pNaI=new ybScint("NaI",pPhysiWorld,0.,0.5*25.*mm,25.*mm,0.,360.*deg);
	G4RotationMatrix *pRo_NaI=new G4RotationMatrix();
	G4ThreeVector fTr_NaI(0.,0.,40.*mm);
	pNaI->SetScintType(ybScint::kTypeCsI);	
	pNaI->SetPosition(pRo_NaI,fTr_NaI);
	pNaI->GetSD()->SetResolution(0.065);
	RegisterDetectors(pNaI);

	//===========================ExDSSD detector==============================//
	ybExDSSD *pExDSSD=new ybExDSSD("DSSD",pPhysiWorld,80.*mm,1,40.*mm,1000.0*um);
	G4RotationMatrix *ro_ExDSSD=new G4RotationMatrix();
//	ro_dssd->rotateZ(90*deg);
	G4ThreeVector tr_ExDSSD(0.,0.,0.);
	pExDSSD->SetPosition(ro_ExDSSD,tr_ExDSSD);
	RegisterDetectors(pExDSSD);		
*/

	
	return pPhysiWorld;
}




















