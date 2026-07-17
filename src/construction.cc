#include "construction.hh"
#include "detector.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineMaterials(); // Otherwise geant4 does not know when do this function
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);
    
    G4double energy[2] = {1.239841939*eV/0.55, 1.239841939*eV/0.3};
    G4double rindexH2O[2] = {1.33, 1.33};
    
    G4MaterialPropertiesTable *mptH2O = new G4MaterialPropertiesTable();
    mptH2O->AddProperty("RINDEX", energy, rindexH2O, 2);
    
    // H2O->SetMaterialPropertiesTable(mptH2O);
    
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    //
    // World
    //
    G4double xWorld = 12.*m;
    G4double yWorld = 12.*m;
    G4double zWorld = 12.*m;
    
    G4Box *solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    //
    // Cylinder (detector)
    //
    G4double r = 1.9*m;
    G4double h = 5.*m;
 
    G4Tubs *solidTube = new G4Tubs("solidTube", 0.*m, r, h, 0.*deg, 360.*deg);
    
//    G4LogicalVolume *logicTube = new G4LogicalVolume(solidTube, H2O, "logicalTube");
    logicDetector = new G4LogicalVolume(solidTube, H2O, "logicDetector");
    
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
