#ifndef TRACKING_HH
#define TRACKING_HH

#include "G4UserTrackingAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class MyRunAction;

class MyTrackingAction : public G4UserTrackingAction
{
public:
    MyTrackingAction(MyRunAction* runAction);
    virtual ~MyTrackingAction();
    
    virtual void PreUserTrackingAction(const G4Track*) override;
    virtual void PostUserTrackingAction(const G4Track*) override;

private:
    MyRunAction* fRunAction;
    
    G4ThreeVector fKaonInitialPosition;
    G4ThreeVector fKaonDecayDirection;
    G4bool fKaonDecayedInFlight = false;
    G4bool fIsSelectedKaon = false;
    
    G4ThreeVector fMuonInitialPosition;
    G4bool fMuonDecayedInFlight = false;
    G4bool fIsSelectedMuon = false;
    // Track ID of the mu+ whose decay positron we want
    G4int fSelectedMuonTrackID = -1;
    
    G4ThreeVector zAxis = G4ThreeVector(0., 0., 1.);
};

#endif
