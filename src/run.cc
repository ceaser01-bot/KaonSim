#include "run.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    // following commands are in the constructor so they don't create Ntuple everytime run starts

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent"); // event number
    man->CreateNtupleIColumn("fCopyNo"); // detector ID
    man->CreateNtupleDColumn("fEdep"); // total energy deposition
    man->CreateNtupleDColumn("fX"); // x position of detector
    man->CreateNtupleDColumn("fY"); // y position of detector
    man->CreateNtupleDColumn("fZ"); // z position of detector
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

// create output root file for every new run (so we're not overwriting old files)
void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();
    
    std::stringstream strRunID;
    strRunID << runID;
    
    man->OpenFile("output"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
}
