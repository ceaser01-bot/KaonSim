#include "tracking.hh"
#include "run.hh"

#include "G4Track.hh"
#include "G4KaonPlus.hh"
#include "G4MuonPlus.hh"
#include "G4Positron.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

MyTrackingAction::MyTrackingAction(MyRunAction* runAction)
{
    fRunAction = runAction;
}

MyTrackingAction::~MyTrackingAction()
{}

void MyTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    fIsSelectedKaon = false;
    fIsSelectedMuon = false;
    
    G4ParticleDefinition* particle = track->GetDefinition();
    
    if (particle == G4KaonPlus::Definition() && track->GetParentID() == 0)
    {
        fIsSelectedKaon = true;
        fKaonInitialPosition = track->GetPosition(); // for each event, the first particle/track that's followed is the primary Kaon
    }
    
    // If the program is about to follow a mu+, its track could only have originated from the decay of the primary K+
    if (particle == G4MuonPlus::Definition() && track->GetParentID() == 1)
    {
        fIsSelectedMuon = true;
        fSelectedMuonTrackID = track->GetTrackID();
        fMuonInitialPosition = track->GetPosition();
        
        G4ThreeVector muonDirection = track->GetMomentumDirection();
        G4double cosTheta = zAxis.dot(muonDirection);
        
        G4AnalysisManager* man = G4AnalysisManager::Instance();
        
        if (fKaonDecayedInFlight)
        {
            man->FillNtupleDColumn(4, 0, cosTheta);
            man->FillNtupleDColumn(4, 1, 1);
            man->AddNtupleRow(4);
            
            fKaonDecayedInFlight = false; // note fKaonDecayedInFlight will be true until the K+ decays into a mu+ which is most of the time
        }
        
        else
        {
            man->FillNtupleDColumn(4, 0, cosTheta);
            man->FillNtupleDColumn(4, 1, 0);
            man->AddNtupleRow(4);
        }
    }
    
    // if the program is getting ready to look at an e+ track, make sure it came from the mu+
    if (particle == G4Positron::Definition() && track->GetParentID() == fSelectedMuonTrackID)
    {
        const G4VProcess* creatorProcess = track->GetCreatorProcess();
        
        if (creatorProcess && creatorProcess->GetProcessName() == "Decay")
        {
            // This is the positron kinetic energy at creation
            const G4double positronKE = track->GetKineticEnergy();
            
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            
            if (fMuonDecayedInFlight)
            {
                man->FillNtupleDColumn(6, 0, positronKE / MeV);
                man->FillNtupleDColumn(6, 1, 1);
                man->AddNtupleRow(6);
                
                fMuonDecayedInFlight = false; // note fMuonDecayedInFlight will be true until the mu+ decays into a e+
            }
            
            else
            {
                man->FillNtupleDColumn(6, 0, positronKE / MeV);
                man->FillNtupleDColumn(6, 1, 0);
                man->AddNtupleRow(6);
            }
        }
    }
}

//    Once a track reaches fStopButAlive, Geant4 stops transporting the particle. There are no more ordinary spatial steps (stepLength is typically 0).
//    Geant4 asks "Are there any at-rest processes registered for this particle?"
// when the track status becomes fStopAndKill, the tracking manager exits the loop and calls PostUserTrackingAction(track)
// Often the track will terminate becuase the particle decayed and PostUserTrackingAction is called. Whether or not it was at rest or moving, the process responsible for ending the last step will be "Decay"
void MyTrackingAction::PostUserTrackingAction(const G4Track* track)
{
    G4double threshold = 1.0 * keV;
    G4double finalKE = track->GetKineticEnergy();
    
    G4String processName = "Unknown";
    // Last step that ended the track
    if (track->GetStep())
    {
        // track->GetStep() returns the last G4Step taken by the track
        // track->GetStep()->GetPostStepPoint() returns the end point of that step
        // track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep() returns a pointer to the G4VProcess responsible for ending that step
        const G4VProcess* process = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep();
        
        if (process)
            processName = process->GetProcessName();
    }

    // Only examine the primary K+
    if (fIsSelectedKaon)
    {
        if (processName == "Decay" && finalKE > threshold)
        {
            // decays aren't simply limited to mu+ here
            fKaonDecayedInFlight = true;
            
            fRunAction->IncrementInFlightKaons();
            
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            
            man->FillNtupleDColumn(5, 0, finalKE / MeV);
            man->AddNtupleRow(5);
        }
        
        else // process responsible for ending the track wasn't a decay (unlikely I think), Kaon was at rest when the track ended (likely due to a decay process), or both
        {
            G4ThreeVector finalPosition = track->GetPosition(); // position at the current stage of tracking, typically the end of the current step
            
            G4double distance = (finalPosition - fKaonInitialPosition).mag();
            
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            
            man->FillNtupleDColumn(2, 0, distance / cm);
            man->AddNtupleRow(2);
        }
    }
        
    if (fIsSelectedMuon)
    {
        if (processName == "Decay" && finalKE > threshold)
        {
            fMuonDecayedInFlight = true;
        }
        
        else
        {
            G4ThreeVector finalPosition = track->GetPosition();
            
            G4double distance = (finalPosition - fMuonInitialPosition).mag();
            
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            
            man->FillNtupleDColumn(3, 0, distance / cm);
            man->AddNtupleRow(3);
        }
    }

//    This assumes one primary K+ per event.

//    K+ track starts
//        ↓
//    PreUserTrackingAction(K+)
//        ↓
//    ... many steps ...
//        ↓
//    K+ decays
//        ↓
//    PostUserTrackingAction(K+)
//        ↓
//    PreUserTrackingAction(mu+)
//        ↓
//    ... mu+ is tracked ...
//        ↓
//    PostUserTrackingAction(mu+)
//    
//    In PostUserTrackingAction(K+), you can save the kaon's TrackID, its momentum direction just before decay, and whether it decayed in flight.
//    Then, in PreUserTrackingAction(mu+), that information is already available.
//

//    record the positron in PreUserTrackingAction(). At that point, the positron track has just been created
}
