#include "construction.hh"
#include "detector.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineMaterials(); // Otherwise geant4 does not know when do this function
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    LAr = new G4Material("Liquid Argon", 1.390*g/cm3, 1);
    LAr->AddElement(nist->FindOrBuildElement("Ar"), 1);
    
//    G4double energy[2] = {*eV/, *eV/};
//    G4double rindexLAr[2] = {,};
//    
//    G4MaterialPropertiesTable *mptLAr = new G4MaterialPropertiesTable();
//    mptLAr->AddProperty("RINDEX", energy, rindexLAr, 2);
    
    // LAr->SetMaterialPropertiesTable(mptLAr);
    
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    //
    // World
    //
    G4double xWorld = 5.5*m;
    G4double yWorld = 5.5*m;
    G4double zWorld = 5.5*m;
    
    G4Box *solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    //
    // Box (detector)
    //
    G4double xTube = 1.25*m;
    G4double yTube = 1.15*m;
    G4double zTube = 5.2*m;
 
    G4Box *solidTube = new G4Box("solidTube", xTube, yTube, zTube);
    
//    G4LogicalVolume *logicTube = new G4LogicalVolume(solidTube, H2O, "logicalTube");
    logicDetector = new G4LogicalVolume(solidTube, LAr, "logicDetector");
    
    fScoringVolume = logicDetector; // define what our scoring volume is
    
//    G4VPhysicalVolume *physTube = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTube, "physTube", logicWorld, false, 0, true);
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicDetector, "physDetector", logicWorld, false, 0, true);
   
    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    // example B3a has
        // auto cryst = new G4MultiFunctionalDetector("crystal")
        // G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
    // instead of a user-written detector class
    // info like Edep is recorded in its DetectorConstruction source file (not stepping action)
    
    logicDetector->SetSensitiveDetector(sensDet); // tells logic detector this is our sensitive detector
    // could also use 'SetSensitiveDetector("name of logical detector volume", sensDet);'
}
