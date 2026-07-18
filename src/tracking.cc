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
{}

void MyTrackingAction::PostUserTrackingAction(const G4Track* track)
{}
