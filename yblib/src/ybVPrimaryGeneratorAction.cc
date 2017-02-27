#include "ybVPrimaryGeneratorAction.hh"

ybVPrimaryGeneratorAction::ybVPrimaryGeneratorAction()
{
	mass_u=937.*MeV;
	Q_bt=0.,Q_e=0.,Q_r=0.;
	A_b=0.,A_t=0.,A_e=0.,A_r=0.;
	Z_e=0.,Z_r=0.;
	Ex_e=0.,Ex_r=0.;
	memset(Z_EmerDau,0.,sizeof(Z_EmerDau));
	memset(A_EmerDau,0.,sizeof(A_EmerDau));
	memset(Ek_EmerDau,0.,sizeof(Ek_EmerDau));
	memset(Z_RecDau,0.,sizeof(Z_RecDau));
	memset(A_RecDau,0.,sizeof(A_RecDau));
	memset(Ek_RecDau,0.,sizeof(Ek_RecDau));	
	EmergeReaction=RecoilReaction=false;
	EmergeBreak=RecoilBreak=false;
	
	fParticleTime=0.;
	//particle number
	G4int n_particle = 1;
	pParticleGun = new G4ParticleGun(n_particle);
}

ybVPrimaryGeneratorAction::~ybVPrimaryGeneratorAction()
{
	delete pParticleGun;
}

void ybVPrimaryGeneratorAction::SetParticleDefinition(G4int z,G4int a,G4double e)
{
	G4ParticleDefinition* particle= G4IonTable::GetIonTable()->GetIon(z,a,e);
	pParticleGun->SetParticleDefinition(particle);
}

void ybVPrimaryGeneratorAction::SetParticleDefinition(G4String particlename)
{
	G4ParticleTable* particleTable=G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle=particleTable->FindParticle(particlename); 
	pParticleGun->SetParticleDefinition(particle);
}

void ybVPrimaryGeneratorAction::SetParticleEnergy(G4double energy,G4double sigma)
{
	pParticleGun->SetParticleEnergy(CLHEP::RandGauss::shoot(energy,sigma));
}

void ybVPrimaryGeneratorAction::SetParticlePosition(G4String type, G4double x0,G4double y0,G4double radius,G4double z0,G4double z1)
{
	G4double pos_x=0.,pos_y=0.,pos_z=0.,pos_r=0.;
	G4double theta=0.;
	
	if(type=="uniform")
	{
		G4double rr=CLHEP::RandFlat::shoot(0.,1.);
		pos_r=pos_r*sqrt(rr);
	}else if(type=="gaus")
	{
		pos_r=CLHEP::RandGauss::shoot(0.,radius);
		pos_r=fabs(pos_r);
	}else
	{
		G4cout<<"Error::ybVPrimaryGeneratorAction!!  Unknown type '"<<type<<"' in SetParticlePosition() method !"<<G4endl;
		exit(0);
	}
	theta=CLHEP::RandFlat::shoot(0.,2*3.14159);
	pos_x=x0+pos_r*cos(theta);
	pos_y=y0+pos_r*sin(theta);
	pos_z=CLHEP::RandFlat::shoot(z0,z1);

	pParticleGun->SetParticlePosition(G4ThreeVector(pos_x,pos_y,pos_z));
}
	
void ybVPrimaryGeneratorAction::SetParticleMomentumDirection(G4String type)
{
	G4double dir_x=0.,dir_y=0.,dir_z=1.;
	if(type=="uniform")
	{
		G4double costheta,sintheta,phi;
		phi=CLHEP::RandFlat::shoot(0.,2.*3.14159);
		costheta=CLHEP::RandFlat::shoot(-1.,1.);
		sintheta=sqrt(1.0-costheta*costheta);
		dir_x=sintheta*cos(phi);
		dir_y=sintheta*sin(phi);
		dir_z=costheta;
	}else
	{
		G4cout<<"Error::ybVPrimaryGeneratorAction!!  Unknown type '"<<type<<"' in SetParticleMomentumDirection() method !"<<G4endl;
		exit(0);
	}
	
	pParticleGun->SetParticleMomentumDirection(G4ThreeVector(dir_x,dir_y,dir_z));
}

void ybVPrimaryGeneratorAction::SetParticleTime(G4String type,G4double time)
{
	if(type=="exp")
	{
		fParticleTime+=CLHEP::RandExponential::shoot(time);
	}else if(type=="const")
	{
		fParticleTime+=time;
	}else
	{
		G4cout<<"Error::ybVPrimaryGeneratorAction!!  Unknown type '"<<type<<"' in SetParticleTime() method !"<<G4endl;
		exit(0);
	}		
	pParticleGun->SetParticleTime(fParticleTime);
}


//================for beam==================//
void ybVPrimaryGeneratorAction::SetBeamEnergy(G4double e,G4double sigma)
{
	fBeamE=e;
	fBeamSig=sigma;
}

void ybVPrimaryGeneratorAction::SetReactionPosition(G4double x,G4double y,G4double z)
{
	RePos.setX(x);
	RePos.setY(y);
	RePos.setZ(z);
}

void ybVPrimaryGeneratorAction::SetBeamIntensity(G4double time)
{
	fParticleTime=time;
}

void ybVPrimaryGeneratorAction::SetBeamIntensity(G4String type, G4double time)
{
	if(type=="exp")
	{
		fParticleTime+=CLHEP::RandExponential::shoot(time);
	}else if(type=="const")
	{
		fParticleTime+=time;
	}else
	{
		G4cout<<"Error::ybVPrimaryGeneratorAction!!  Unknown type '"<<type<<"' in SetParticleTime() method !"<<G4endl;
		exit(0);
	}	
}

void ybVPrimaryGeneratorAction::SetTargetReaction(G4double beamA,G4double targetA,G4double emergeZ,G4double emergeA,G4double recoilZ,G4double recoilA,G4double QValue)
{
	A_b=beamA;
	A_t=targetA;
	Z_e=emergeZ;
	A_e=emergeA;
	Z_r=recoilZ;
	A_r=recoilA;
	Q_bt=QValue;	//it should be the value for geound states
}

void ybVPrimaryGeneratorAction::SetEmergeBreakUp(G4double z1,G4double a1,G4double z2,G4double a2,G4double QValue)
{
	Z_EmerDau[0]=z1;
	Z_EmerDau[1]=z2;
	A_EmerDau[0]=a1;
	A_EmerDau[1]=a2;
	Q_e=QValue;
	EmergeReaction=true;
}

void ybVPrimaryGeneratorAction::SetRecoilBreakUp(G4double z1,G4double a1,G4double z2,G4double a2,G4double QValue)
{
	Z_RecDau[0]=z1;
	Z_RecDau[1]=z2;
	A_RecDau[0]=a1;
	A_RecDau[1]=a2;
	Q_r=QValue;
	RecoilReaction=true;
}

void ybVPrimaryGeneratorAction::SetEmergeEx(G4double e)
{
	Ex_e=e;
}

void ybVPrimaryGeneratorAction::SetRecoilEx(G4double e)
{
	Ex_r=e;
}

void ybVPrimaryGeneratorAction::SetEmergeDaughterEx(G4double e1,G4double e2)
{
	Ex_EmerDau[0]=e1;
	Ex_EmerDau[1]=e2;
}

void ybVPrimaryGeneratorAction::SetRecoilDaughterEx(G4double e1,G4double e2)
{
	Ex_RecDau[0]=e1;
	Ex_RecDau[1]=e2;
}

void ybVPrimaryGeneratorAction::NucleiReaction()
{
	G4double Mb=A_b*mass_u,Mt=A_t*mass_u;
	G4double Me=A_e*mass_u+Ex_e,Mr=A_r*mass_u+Ex_r;
	
	G4double Ek_b=CLHEP::RandGauss::shoot(fBeamE,fBeamSig);
	while(fabs(Ek_b-fBeamE)>3.*fBeamSig)
	{
		Ek_b=CLHEP::RandGauss::shoot(fBeamE,fBeamSig);
	}
	//the total Energy in c.m. 
	G4double Pb=sqrt(Ek_b*Ek_b+2*Mb*Ek_b);
	G4double betaC=Pb/(Mt+sqrt(Pb*Pb+Mb*Mb));
	G4double Etot=(Mt+sqrt(Pb*Pb+Mb*Mb))*sqrt(1-betaC*betaC);
	G4double Ecm=Etot-Mt-Mb;
	if(Ecm+Q_bt<Ex_e+Ex_r)
	{
		G4cout<<"Error::ybVPrimaryGeneratorAction !!  Ecm<Ex!  Ecm+Q_bt="<<Ecm+Q_bt<<G4endl;
		exit(0);			
	}	
	//the speed of emerge particle in c.m.
	G4double par[4]={0.,0.,0.,0.};
	par[0]=Etot+Q_bt+Me+Mr;
	par[1]=Etot+Q_bt+Me-Mr;
	par[2]=Etot+Q_bt-Me+Mr;
	par[3]=Etot+Q_bt-Me-Mr;
	G4double tmp=par[0]*par[1]*par[2]*par[3]/(4*Me*Me*(Etot+Q_bt)*(Etot+Q_bt));
	G4double betaE=sqrt(tmp/(1+tmp));
	G4double Pec=Me*betaE/sqrt(1-betaE*betaE);
	G4double Eec=sqrt(Pec*Pec+Me*Me);
	//calculate the angle in lab.
	G4double thetaC=0.,phiC=0.,thetaL=0.,phiL=0.;
	ReactionAngleInCM(thetaC,phiC);
	G4double tant=sqrt(1-betaC*betaC)*sin(thetaC)/(cos(thetaC)+betaC/betaE);
	if(tant>0.)
	{
		thetaL=atan(tant)*rad;
	}else
	{
		thetaL=atan(tant)*rad+180*deg;
	}
	phiL=phiC;	

	//Ek_e
	double uMb=Mb/GeV,uMe=Me/GeV,uMr=Mr/GeV,uEk_b=Ek_b/GeV,uQ=(Q_bt-Ex_e-Ex_r)/GeV;
	par[0]=uMe+uMr+uEk_b+uQ;
	par[1]=uEk_b*(uMb-uMr)-uQ*(uEk_b+uMr+0.5*uQ);
	par[2]=par[0]*par[0]-(uEk_b*uEk_b+2*uEk_b*uMb)*cos(thetaL)*cos(thetaL);
	par[3]=par[0]*par[1]-uMe*(uEk_b*uEk_b+2*uEk_b*uMb)*cos(thetaL)*cos(thetaL);
	G4double delta=par[3]*par[3]-par[2]*par[1]*par[1];
	G4double sqrtdelta=0.;
	if(fabs(delta)*1e5<fabs(par[3]))
	{
		delta=0.;
	}else
	{
		if(delta<0)
		{
			G4cout<<"Error::ybVPrimaryGeneratorAction !!  delta="<<delta<<"  < 0 ! "<<"  par[3]="<<par[3]<<G4endl;
			exit(0);		
		}
		sqrtdelta=sqrt(delta);
	}

	Ek_e=((sqrtdelta-par[3])/par[2])*GeV;
	if(betaE/betaC<1)
	{
		G4double costhetamax=betaC*Eec/Pec-Pec/(betaC*Eec)-Eec*betaC/Pec;
		G4double thetaCmax=acos(costhetamax)*rad;
		if(thetaC>thetaCmax)
		{
			Ek_e=((-sqrtdelta-par[3])/par[2])*GeV;
		}
	}

	//Vel_e
	G4double Pe=0.,Pr=0.,Ve=0.,Vr=0.;
	Pe=sqrt(Ek_e*Ek_e+2*Me*Ek_e);
	Ve=Pe/sqrt((Pe*Pe+Me*Me));
	Vel_e.setRThetaPhi(Ve,thetaL,phiL);	

	//Calculate recoil particle
	Ek_r=Ek_b+Q_bt-Ek_e-Ex_e-Ex_r;
	if(fabs(Ek_r-1.*keV)<1.*keV)
	{
		Ek_r=0.;
		Vel_r.setRThetaPhi(1.*m/s,180.*deg,0.);
	}else
	{
		Pr=sqrt(Ek_r*Ek_r+2*Mr*Ek_r);
		Vr=Pr/sqrt((Pr*Pr+Mr*Mr));
		G4double costheta=(Pr*Pr+Pb*Pb-Pe*Pe)/(2*Pr*Pb);
		if(costheta>1.-1e-10)	costheta=1.-1e-10;
		Vel_r.setRThetaPhi(Vr,acos(costheta),180.*deg+phiL);
	}
	
	//Calculate EmergeDaughter
	if(EmergeReaction)
	{
		EmergeBreak=EmergeBreakUp(Ex_e+Q_e-Ex_EmerDau[0]-Ex_EmerDau[1],Vel_e);
	}
	//Calculate RecoilDaughter
	if(RecoilReaction)
	{
		RecoilBreak=RecoilBreakUp(Ex_r+Q_r-Ex_RecDau[0]-Ex_RecDau[1],Vel_r);	
	}

}

void ybVPrimaryGeneratorAction::EmitParticle(G4Event* pEvent)
{
	NucleiReaction();
	G4ParticleTable* pParticleTable=G4ParticleTable::GetParticleTable();
	G4IonTable* pIonTable=G4IonTable::GetIonTable();
	G4ParticleDefinition *particle=NULL;
	if(EmergeBreak)
	{
		for(int i=0;i<2;i++)
		{
			if(Z_EmerDau[i]>0.5)
			{
				particle= pIonTable->GetIon(Z_EmerDau[i],A_EmerDau[i],Ex_EmerDau[i]);
			}else
			{
				particle= pParticleTable->FindParticle("neutron");
			}
			pParticleGun->SetParticleDefinition(particle);
			pParticleGun->SetParticleEnergy(Ek_EmerDau[i]);
			pParticleGun->SetParticleMomentumDirection(Vel_EmerDau[i].unit());
			pParticleGun->SetParticlePosition(RePos);
			pParticleGun->SetParticleTime(fParticleTime);
			pParticleGun->GeneratePrimaryVertex(pEvent);
		}
	}else
	{
		if(Z_e>0.5)
		{
			particle= pIonTable->GetIon(Z_e,A_e,Ex_e);
		}else
		{
			particle= pParticleTable->FindParticle("neutron");
		}
		pParticleGun->SetParticleDefinition(particle);
		pParticleGun->SetParticleEnergy(Ek_e);
		pParticleGun->SetParticleMomentumDirection(Vel_e.unit());
		pParticleGun->SetParticlePosition(RePos);
		pParticleGun->SetParticleTime(fParticleTime);
		pParticleGun->GeneratePrimaryVertex(pEvent);		
	}
	if(RecoilBreak)
	{
		for(int i=0;i<2;i++)
		{
			if(Z_RecDau[i]>0.5)
			{
				particle= pIonTable->GetIon(Z_RecDau[i],A_RecDau[i],Ex_RecDau[i]);
			}else
			{
				particle= pParticleTable->FindParticle("neutron");
			}
			pParticleGun->SetParticleDefinition(particle);
			pParticleGun->SetParticleEnergy(Ek_RecDau[i]);
			pParticleGun->SetParticleMomentumDirection(Vel_RecDau[i].unit());
			pParticleGun->SetParticlePosition(RePos);
			pParticleGun->SetParticleTime(fParticleTime);
			pParticleGun->GeneratePrimaryVertex(pEvent);
		}
	}else
	{
		if(Z_r>0.5)
		{
			particle= pIonTable->GetIon(Z_r,A_r,Ex_r);
		}else
		{
			particle= pParticleTable->FindParticle("neutron");
		}
		pParticleGun->SetParticleDefinition(particle);
		pParticleGun->SetParticleEnergy(Ek_r);
		pParticleGun->SetParticleMomentumDirection(Vel_r.unit());
		pParticleGun->SetParticlePosition(RePos);
		pParticleGun->SetParticleTime(fParticleTime);
		pParticleGun->GeneratePrimaryVertex(pEvent);	
	}
}

G4bool ybVPrimaryGeneratorAction::EmergeBreakUp(G4double QValue, G4ThreeVector vMother)
{
	if(QValue<0.)
	{
		G4cout<<"Warning::ybVPrimaryGeneratorAction !! QValue<0 ! So the emerge particle can not break up !"<<G4endl;
		return false;
	}
	G4double mass1=A_EmerDau[0]*mass_u,mass2=A_EmerDau[1]*mass_u;
	G4double par[3]={0.,0.,0.};
	G4double beta=0.;
	G4double gamma=0.;
	G4double theta=0.,phi=0.;
	
	par[0]=2*(QValue+mass1+mass2);
	par[1]=pow(QValue+mass1+mass2,2)+mass1*mass1-mass2*mass2;
	par[2]=1-pow(par[0]*mass1/par[1],2);
	beta=sqrt(par[2]);
	EmergeBreakUpAngleInCM(theta,phi);

	Vel_EmerDau[0].setRThetaPhi(beta,theta,phi);
	Vel_EmerDau[0]=LorentzSpeed(vMother,Vel_EmerDau[0]);
	beta=Vel_EmerDau[0].getR();
	gamma=1/sqrt(1-beta*beta);
	Ek_EmerDau[0]=(gamma-1)*mass1;

	par[1]=pow(QValue+mass1+mass2,2)-mass1*mass1+mass2*mass2;
	par[2]=1-pow(par[0]*mass2/par[1],2);
	beta=sqrt(par[2]);
	Vel_EmerDau[1].setRThetaPhi(beta,180.*deg-theta,180.*deg+phi);
	Vel_EmerDau[1]=LorentzSpeed(vMother,Vel_EmerDau[1]);
	beta=Vel_EmerDau[1].getR();
	gamma=1/sqrt(1-beta*beta);
	Ek_EmerDau[1]=(gamma-1)*mass2;

	return true;
}

G4bool ybVPrimaryGeneratorAction::RecoilBreakUp(G4double QValue, G4ThreeVector vMother)
{
	if(QValue<0.)
	{
		G4cout<<"Warning::ybVPrimaryGeneratorAction !! QValue<0 ! So the recoil particle can not break up !"<<G4endl;
		return false;
	}
	G4double mass1=A_RecDau[0]*mass_u,mass2=A_RecDau[1]*mass_u;
	G4double par[3]={0.,0.,0.};
	G4double beta=0.;
	G4double gamma=0.;
	G4double theta=0.*deg,phi=0.;
	
	par[0]=2*(QValue+mass1+mass2);
	par[1]=pow(QValue+mass1+mass2,2)+mass1*mass1-mass2*mass2;
	par[2]=1-pow(par[0]*mass1/par[1],2);
	beta=sqrt(par[2]);
	RecoilBreakUpAngleInCM(theta,phi);
	Vel_RecDau[0].setRThetaPhi(beta,theta,phi);
	Vel_RecDau[0]=LorentzSpeed(vMother,Vel_RecDau[0]);
	beta=Vel_RecDau[0].getR();
	gamma=1/sqrt(1-beta*beta);
	Ek_RecDau[0]=(gamma-1)*mass1;
	
	par[1]=pow(QValue+mass1+mass2,2)-mass1*mass1+mass2*mass2;
	par[2]=1-pow(par[0]*mass2/par[1],2);
	beta=sqrt(par[2]);
	Vel_RecDau[1].setRThetaPhi(beta,180.*deg-theta,180.*deg+phi);
	Vel_RecDau[1]=LorentzSpeed(vMother,Vel_RecDau[1]);
	beta=Vel_RecDau[1].getR();
	gamma=1/sqrt(1-beta*beta);
	Ek_RecDau[1]=(gamma-1)*mass2;
	
	return true;
}

//vFrame : speed of mother particle ; vcm : speed of daughter particle in center of mass system 
//the unit of vFrame and vcm is 1 (meaning if vFrame is 0.1, then its real speed is 0.1c, where c is the speed of light) 
G4ThreeVector ybVPrimaryGeneratorAction::LorentzSpeed(const G4ThreeVector &vFrame, const G4ThreeVector &vcm)
{
	G4double v=vFrame.getR();
	G4double r=1/sqrt(1-v*v);
	G4double par=(r-1)/(v*v);
	G4double vx=vFrame.getX(),vy=vFrame.getY(),vz=vFrame.getZ();
	G4double vx_=vcm.getX(),vy_=vcm.getY(),vz_=vcm.getZ();
	
	G4double dtdt_=r*(1+vx*vx_+vy*vy_+vz*vz_);
	G4double dxdt_=r*vx+( 1+vx*vx*par )*vx_+( vx*vy*par )*vy_+( vx*vz*par )*vz_;
	G4double dydt_=r*vy+( vy*vx*par )*vx_+( 1+vy*vy*par )*vy_+( vy*vz*par )*vz_;
	G4double dzdt_=r*vz+( vz*vx*par )*vx_+( vz*vy*par )*vy_+( 1+vz*vz*par )*vz_;
	
	G4ThreeVector vlab(0,0,0);
	vlab.setX(dxdt_/dtdt_);
	vlab.setY(dydt_/dtdt_);
	vlab.setZ(dzdt_/dtdt_);
	return vlab;
}

void ybVPrimaryGeneratorAction::ReactionAngleInCM(G4double &theta,G4double &phi)
{
	phi=CLHEP::RandFlat::shoot(0.,360.)*deg;
	G4double costheta=CLHEP::RandFlat::shoot(-1.,1.);
	theta=acos(costheta)*rad;
}

void ybVPrimaryGeneratorAction::EmergeBreakUpAngleInCM(G4double &theta,G4double &phi)
{
	phi=CLHEP::RandFlat::shoot(0.,360.)*deg;
	G4double costheta=CLHEP::RandFlat::shoot(-1.,1.);
	theta=acos(costheta)*rad;
}

void ybVPrimaryGeneratorAction::RecoilBreakUpAngleInCM(G4double &theta,G4double &phi)
{
	phi=CLHEP::RandFlat::shoot(0.,360.)*deg;
	G4double costheta=CLHEP::RandFlat::shoot(-1.,1.);
	theta=acos(costheta)*rad;
}
