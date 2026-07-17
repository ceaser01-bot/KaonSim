#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    
    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
    
    virtual G4VPhysicalVolume *Construct();

private:
    G4Material *H2O, *worldMat; // declrations moved into definition of class so they are only created once (not whenever we change something with messengers)

    void DefineMaterials(); // must define this new function so materials are only defined when the object of our detector construction is created
    virtual void ConstructSDandField(); // SD (sensitive detector) and field (B-field and E-field)
    
    G4LogicalVolume *logicDetector; // sensitive volume has to refer to this logical volume (since we need to access it outside of the construction function)
    
    G4LogicalVolume *fScoringVolume;

};

#endif
