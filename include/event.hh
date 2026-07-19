#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include <map>

class G4Event;

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction();
    ~MyEventAction() override;
    
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;
    
private:
    G4int fEdepCollectionID = -1;
    std::map<G4int, G4ThreeVector> fDetectorPositions;
};

#endif
