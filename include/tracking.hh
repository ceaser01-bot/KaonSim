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
};

#endif
