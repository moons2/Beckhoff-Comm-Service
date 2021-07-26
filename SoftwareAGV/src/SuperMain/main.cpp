#include <iostream>
#include "../Softwares/MaintenanceSoftware/MaintenanceSoftware.h"

//global variables for enable or disable softwares
bool maintenance = false;
//bool motionControl = false;
//bool security = false;
//bool interlock = false;
//bool humanMachineInterface = false;
//bool navigation = false;


using namespace std;
int main(){

    //create maintenance software object
    MaintenanceSoftware* maintenanceSoftware = new MaintenanceSoftware;
    
    //definir rotinas para habilitar e ou desabilitar softwares - escalonamento
    maintenance = true; //enable maintenance software
    
    while(maintenance){
        maintenanceSoftware->Run();
    }
    
    return 0;
}
