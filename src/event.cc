#include "event.hh"
#include "construction.hh"

#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"

MyEventAction::MyEventAction()
{}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    // Nothing needs to be reset here.
    // Geant4 creates a new hits collection for every event.
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    if (!HCE) return;
    
    // Look up the collection ID once.
    if (fEdepCollectionID < 0)
    {
        G4SDManager* SDMan = G4SDManager::GetSDMpointer();
        fEdepCollectionID = SDMan->GetCollectionID("DetectorSD/Edep");
    }
    
    auto* evtMap = static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fEdepCollectionID));
    
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    const G4int eventID = event->GetEventID();
    
    for (const auto& mapElement : (*evtMap->GetMap()))
    {
        // The underlying map is essentially std::map<G4int, G4double*>
        // mapElement.first is just the copy number
        // mapElement.second is a pointer and must be dereferenced to get the actual G4double
        const G4int detectorID = mapElement.first;
        auto totalEdep = *(mapElement.second);
        
        const auto* detector = static_cast<const MyDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        const G4ThreeVector position = detector->GetDetectorPosition(detectorID);

        man->FillNtupleIColumn(0, eventID);
        man->FillNtupleIColumn(1, detectorID);
        man->FillNtupleDColumn(2, totalEdep / MeV);
        man->FillNtupleDColumn(3, position.x() / cm);
        man->FillNtupleDColumn(4, position.y() / cm);
        man->FillNtupleDColumn(5, position.z() / cm);

        man->AddNtupleRow(0);
    }
}
// In exampleB3a, info like Edep is recorded in the main file via commnads like G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
