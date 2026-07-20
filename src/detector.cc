#include "detector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step*, G4TouchableHistory*)
{
    return true;
}
