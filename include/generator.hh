#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();
    
    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun *fParticleGun;
};

#endif
