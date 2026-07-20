#include "construction.hh"
//#include "detector.hh"

#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"

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
    
//     LAr->SetMaterialPropertiesTable(mptLAr);
    
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    vacuum = nist->FindOrBuildMaterial("G4_Galactic");
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
    // Rectangular shell (really here to help me see that all the detectors are sitting in the right spots)
    //
    G4double xTube = 1.25*m;
    G4double yTube = 1.15*m;
    G4double zTube = 5.2*m;
    G4double thick = 0.1*m;
    
    // Outer box dimensions
    G4Box *outerBox = new G4Box("OuterBox", xTube+thick, yTube+thick, zTube+thick);
    
    // Inner cavity must be smaller than the outer box
    G4Box *innerBox = new G4Box("InnerBox", xTube, yTube, zTube);
    
    // Subtract the inner box from the outer box
    auto shellSolid = new G4SubtractionSolid("RectangularShell", outerBox, innerBox);
    
    auto shellLogical = new G4LogicalVolume(shellSolid, vacuum, "RectangularShellLogical"); // currently made of nothing (vacuum)

    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), shellLogical, "RectangularShellPhysical", logicWorld, false, 0, true);
    
    //
    // Nx*Ny*Nz Detectors (rectangular prisms)
    //
    // half lengths of the detectors along each axis
    auto Nx = 20;
    G4double lx = xTube / Nx;
    auto Ny = 20;
    G4double ly = yTube / Ny;
    auto Nz = 50;
    G4double lz = zTube / Nz;
    
    
    G4Box *solidDetector = new G4Box("solidDetector", lx, ly, lz);
    
    logicDetector = new G4LogicalVolume(solidDetector, LAr, "logicDetector");
        
    fDetectorPositions.clear();
    
    for(G4int k = 0; k < Nz; k++)
    {
        for(G4int i = 0; i < Nx; i++)
        {
            for(G4int j = 0; j < Ny; j++)
            {
                const G4ThreeVector pos = G4ThreeVector(-xTube+(2*i+1)*lx, -yTube+(2*j+1)*ly, -zTube+(2*k+1)*lz);
                fDetectorPositions.push_back(pos); // adds the computed detector position to the end of our vector
                
                G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(pos.x(), pos.y(), pos.z()), logicDetector, "physDetector", logicWorld, false, j+i*Ny+k*(Nx*Ny), false); // check for overlaps is false since there are so many detectors (and it already checked for when there were not as many detectors)
            }
        }
    }
   
    return physWorld;
}

const G4ThreeVector& MyDetectorConstruction::GetDetectorPosition(G4int detectorID) const
{
    return fDetectorPositions.at(detectorID); // .at(detectorID) gives you bounds checking
}

void MyDetectorConstruction::ConstructSDandField()
{
    auto* detectorSD = new G4MultiFunctionalDetector("DetectorSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    G4VPrimitiveScorer* energyScorer = new G4PSEnergyDeposit("Edep");
    detectorSD->RegisterPrimitive(energyScorer);
    SetSensitiveDetector("logicDetector", detectorSD);
}
