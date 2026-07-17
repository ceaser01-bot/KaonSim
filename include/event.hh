#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"

class G4Event;
class MyRunAction;

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(MyRunAction*);
    ~MyEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif
