#include "stepping.hh"
#include "construction.hh"
#include "event.hh"

#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4KaonPlus.hh"
#include "G4MuonPlus.hh"
#include "G4Positron.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // logical volume is where the particle currently is
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    // scoring volume that we defined
    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
    
    if(volume != fScoringVolume) // if the volume is not the scoring volume, close the function right here
        return;
    
    G4Track *track = step->GetTrack();
    
    G4ParticleDefinition* particleName = track->GetDefinition();
    
    G4double edep = step->GetTotalEnergyDeposit();
    G4double stepLength = step->GetStepLength();
    G4double dEdx =  edep/stepLength;
    
    G4ThreeVector currentPosition = track->GetPosition(); // position at the current stage of tracking, typically the end of the current step
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    // Primary kaon
    if (track->GetParentID() == 0 && particleName == G4KaonPlus::Definition())
    {
        // The first step
        if (track->GetCurrentStepNumber() == 1)
        {
            fKaonInitialPosition = step->GetPreStepPoint()->GetPosition();
            
            man->FillNtupleDColumn(0, 0, dEdx / (MeV/cm));
            man->AddNtupleRow(0);
        }
        
        // will also record the first step
        if (stepLength > 0.) // for stepLength == 0, the track status is most likely fStopButAlive because the incident particle has nearly slowed to a stop. The particle may still give off whatever little energy it has left but the stepLength is 0 (dE/dx = nan)
        {
            G4double length = (currentPosition - fKaonInitialPosition).mag();

            man->FillNtupleDColumn(7, 0, dEdx / (MeV/cm));
            man->FillNtupleDColumn(7, 1, length / cm);
            man->AddNtupleRow(7); // each row has the penetration depth matches and corresponding energy deposition across the last track
        }
        
        man->FillNtupleDColumn(9, 0, edep / MeV);
        man->FillNtupleDColumn(9, 1, stepLength / cm);
        man->AddNtupleRow(9);
    }
    
    // Secondary muons from kaon
    if (track->GetParentID() == 1 && particleName == G4MuonPlus::Definition())
    {
        // since this is the first step I'm not worried about the stepLength being 0 (I think)
        if (track->GetCurrentStepNumber() == 1)
        {
            fMuonInitialPosition = step->GetPreStepPoint()->GetPosition();
            
            man->FillNtupleDColumn(1, 0, dEdx / (MeV/cm));
            man->AddNtupleRow(1);
        }
        
        if (stepLength > 0.)
        {
            G4double length = (currentPosition - fMuonInitialPosition).mag();

            man->FillNtupleDColumn(8, 0, dEdx / (MeV/cm));
            man->FillNtupleDColumn(8, 1, length / cm);
            man->AddNtupleRow(8);
        }
        
        man->FillNtupleDColumn(10, 0, edep / MeV);
        man->FillNtupleDColumn(10, 1, stepLength / cm);
        man->AddNtupleRow(10);
    }
}
