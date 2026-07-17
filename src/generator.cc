#include "generator.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

// commands in the constructor are read in when the run starts and can be overwritten
MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "kaon+";
    G4ParticleDefinition *particle = particleTable->FindParticle("kaon+");
    
    // Default only; macro can override this later.
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentum(339.25 *MeV);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

// called once per event
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // randomized position inside a cylinder
    G4double cosTheta_pos = 2.0*G4UniformRand() - 1.0; // uniform in solid angle
    G4double theta_pos = std::acos(cosTheta_pos);
    G4double phi_pos = twopi*G4UniformRand(); // uniform in azimuthal angle
    
    G4double R = 1.9*m;
    G4double h = 10.*m;
    
    G4double x = R * std::cos(phi_pos) * G4UniformRand();
    G4double y = R * std::sin(phi_pos) * G4UniformRand();
    G4double z = h * (G4UniformRand() - 0.5);

//    G4ThreeVector pos(x, y, z);
    G4ThreeVector pos(0.,0.,0.);

    // randomized momentum direction
    G4double cosTheta = 2.0*G4UniformRand() - 1.0;
    G4double theta = std::acos(cosTheta);
    G4double phi = twopi * G4UniformRand();

    G4ThreeVector direction(
        std::sin(theta)*std::cos(phi),
        std::sin(theta)*std::sin(phi),
        std::cos(theta)
    );
    G4ThreeVector mom(0.,0.,1.);
    
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
//    fParticleGun->SetParticleMomentumDirection(direction);
    
    fParticleGun->GeneratePrimaryVertex(anEvent); // insert particle into current event
}
