#include <iostream>
#include <ctime>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4RunManager *runManager = new G4RunManager();
    
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    
    G4Random::setTheSeed(time(nullptr));
    
    runManager->Initialize();
    
    G4UIExecutive *ui = 0;
    
    if(argc == 1) // if number of command line arguments is one (i.e., ./sk)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
    
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    
    if(ui) // if ui exists, create graphics output
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else // if number of command line arguments is larger than 1, then execute our input script
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    
    delete visManager;
    delete runManager;
    
    return 0;
}
