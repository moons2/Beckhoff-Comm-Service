#ifndef MAINTENANCESOFTWARE_H
#define MAINTENANCESOFTWARE_H
#include <iostream>
#include <unistd.h>
#include "../../Services/BeckhoffCommunicationService/Facade_Beckhoff_Communication_Service.h"
#include "../../Services/BeckhoffCommunicationService/BeckhoffIOAddrs.h" //To use the input and output components correctly 
using namespace std;

class MaintenanceSoftware{
    private:
        Facade_Beckhoff_Communication_Service* beckhoffCommunication;

    public:
        MaintenanceSoftware();
        ~MaintenanceSoftware();
    
    void Run();
};

#endif