#ifndef ybVPrimaryGeneratorAction_h
#define ybVPrimaryGeneratorAction_h 1
//===================C++==================//
#include <cmath>
#include <iostream>
#include <cstdio>
//=================GEANT4=================//
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4NuclideTable.hh"
#include "G4IsotopeProperty.hh"
#include "G4ParticleDefinition.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
using namespace std;

class ybVPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    ybVPrimaryGeneratorAction();
    ~ybVPrimaryGeneratorAction();
	
protected:
	//for single particle
	void SetParticleDefinition(G4int z,G4int a,G4double e);
	void SetParticleDefinition(G4String particlename);
	void SetParticleEnergy(G4double e, G4double sig);
	void SetParticlePosition(G4String type, G4double x0,G4double y0,G4double r,G4double z0,G4double z1);
	void SetParticleMomentumDirection(G4String type);
	void SetParticleTime(G4String type,G4double t);
	//for beam
	void SetBeamEnergy(G4double e,G4double sigma);
	void SetReactionPosition(G4double x,G4double y,G4double z);
	void SetBeamIntensity(G4double t);
	void SetBeamIntensity(G4String type,G4double t);
	void SetTargetReaction(G4double beamA, G4double targetA,G4double emergeZ, G4double emergeA,G4double recoilZ, G4double recoilA, G4double QValue);
	void SetEmergeEx(G4double e);
	void SetRecoilEx(G4double e);
	virtual void ReactionAngleInCM(G4double &theta,G4double &phi);
	
	void SetEmergeBreakUp(G4double z1,G4double a1,G4double z2,G4double a2,G4double QValue);
	void SetEmergeDaughterEx(G4double e1,G4double e2);
	virtual void EmergeBreakUpAngleInCM(G4double &theta,G4double &phi);
	void SetRecoilBreakUp(G4double z1,G4double a1,G4double z2,G4double a2,G4double QValue);	
	void SetRecoilDaughterEx(G4double e1,G4double e2);
	virtual void RecoilBreakUpAngleInCM(G4double &theta,G4double &phi);
	
	void EmitParticle(G4Event* pEvent);
	void NucleiReaction();

	G4bool EmergeBreakUp(G4double QValue, G4ThreeVector vMother);
	G4bool RecoilBreakUp(G4double QValue, G4ThreeVector vMother);
	G4ThreeVector LorentzSpeed(const G4ThreeVector &vFrame, const G4ThreeVector &vcm);
	
protected:
	G4double mass_u;
	G4ParticleGun* pParticleGun;
	G4double fParticleTime;

	G4double fBeamE,fBeamSig;
	G4double A_b,A_t,Q_bt;
	G4double A_e,Z_e,Ex_e,J_e,Q_e,Ek_e;
	G4double A_r,Z_r,Ex_r,J_r,Q_r,Ek_r;
	G4ThreeVector Vel_r,Vel_e;
	
	G4double Z_EmerDau[2],A_EmerDau[2],Ex_EmerDau[2],Ek_EmerDau[2];
	G4double Z_RecDau[2],A_RecDau[2],Ex_RecDau[2],Ek_RecDau[2];
	G4ThreeVector Vel_EmerDau[2];
	G4ThreeVector Vel_RecDau[2];	

	G4bool EmergeReaction,RecoilReaction;	
	G4bool EmergeBreak,RecoilBreak;
	G4ThreeVector RePos;

};

#endif


