#include "Facade_Beckhoff_Communication_Service.h"

//constructor
Facade_Beckhoff_Communication_Service::Facade_Beckhoff_Communication_Service(){
    commService = new Beckhoff_Communication_Service();
}

//destructor
Facade_Beckhoff_Communication_Service::~Facade_Beckhoff_Communication_Service(){
}

//verify communications status
bool Facade_Beckhoff_Communication_Service::verify_communication_status(){
    return commService->verify_communication_status();
}