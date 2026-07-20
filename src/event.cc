#include "event.hh"
#include "run.hh"

#include "G4Event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{}

void MyEventAction::EndOfEventAction(const G4Event*)
{}
// In exampleB3a, info like Edep is recorded in the main file via commnads like G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
