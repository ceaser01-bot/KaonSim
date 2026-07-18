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
}
