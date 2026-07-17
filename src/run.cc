#include "run.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    // following commands are in the constructor so they don't create Ntuple everytime run starts
    // What kind of info do we want to store?
    
    // Ntuple 0: primary kaon scoring
    man->CreateNtuple("KaonScoring", "Primary Kaon Scoring"); // create columns
    man->CreateNtupleDColumn("fKaondEdx");
    man->FinishNtuple(0);
    
    // Ntuple 1: secondary muon scoring
    man->CreateNtuple("MuonScoring", "Secondary Muon Scoring");
    man->CreateNtupleDColumn("fMuondEdx");
    man->FinishNtuple(1);
    
    man->CreateNtuple("StoppedKaonRange", "Stopped Primary Kaon Range");
    man->CreateNtupleDColumn("fKaonRange");
    man->FinishNtuple(2);
    
    man->CreateNtuple("StoppedMuonRange", "Stopped Secondary Muon Range");
    man->CreateNtupleDColumn("fMuonRange");
    man->FinishNtuple(3);
    
    man->CreateNtuple("KaonMuonDecayAngle", "K+ mu+ angle");
    man->CreateNtupleDColumn("fCosTheta");
    man->CreateNtupleDColumn("Tag");
    man->FinishNtuple(4);
    
    man->CreateNtuple("KaonDecayEnergy", "Kaon Decay Energy");
    man->CreateNtupleDColumn("fEKaon");
    man->FinishNtuple(5);
   
    man->CreateNtuple("MuonDecayPositron", "Muon Decay Positron");
    man->CreateNtupleDColumn("fEpositron");
    man->CreateNtupleDColumn("Tag");
    man->FinishNtuple(6);
    
    man->CreateNtuple("TrackKaonSteps", "Primary Kaon Scoring");
    man->CreateNtupleDColumn("fKaondEdx");
    man->CreateNtupleDColumn("fKaonTrackLength");
    man->FinishNtuple(7);
    
    man->CreateNtuple("TrackMuonSteps", "Secondary Muon Scoring");
    man->CreateNtupleDColumn("fMuondEdx");
    man->CreateNtupleDColumn("fMuonTrackLength");
    man->FinishNtuple(8);
    
    man->CreateNtuple("TrackKaonEdep", "Primary Kaon Scoring");
    man->CreateNtupleDColumn("fKaonEdep");
    man->CreateNtupleDColumn("fKaonStepLength");
    man->FinishNtuple(9);
    
    man->CreateNtuple("TrackMuonEdep", "Primary Muon Scoring");
    man->CreateNtupleDColumn("fMuonEdep");
    man->CreateNtupleDColumn("fMuonStepLength");
    man->FinishNtuple(10);
}

MyRunAction::~MyRunAction()
{}

// create output root file for every new run (so we're not overwriting old files)
void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    fNumInFlightKaons = 0;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();
    
    std::stringstream strRunID;
    strRunID << runID;
    
    man->OpenFile("output"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4cout << fNumInFlightKaons << " Kaons decayed in-flight" << G4endl;
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
}
