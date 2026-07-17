#include "physics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4OpticalPhysics.hh"

MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics());
//    RegisterPhysics(new G4RadioactiveDecayPhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
