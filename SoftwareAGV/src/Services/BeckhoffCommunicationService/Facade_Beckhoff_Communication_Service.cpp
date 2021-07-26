#include "Facade_Beckhoff_Communication_Service.h"

//constructor
Facade_Beckhoff_Communication_Service::Facade_Beckhoff_Communication_Service(){
    beckhoffService = new Beckhoff_Communication_Service();
}

//destructor
Facade_Beckhoff_Communication_Service::~Facade_Beckhoff_Communication_Service(){
}


int Facade_Beckhoff_Communication_Service::beckhoff_connect(){
    return beckhoffService->beckhoff_connect();
}

//
//
// return: true (connection open); false other

//void Facade_Beckhoff_Communication_Service::state_test(){
//    return beckhoffService->state_test();
//}

//verify communications status
bool Facade_Beckhoff_Communication_Service::verify_communication_status(){
    return beckhoffService->verify_communication_status();
}

//
//
//
bool Facade_Beckhoff_Communication_Service::beckhoff_disconnect(){
    return beckhoffService->beckhoff_disconnect();
}

//
//
//
bool Facade_Beckhoff_Communication_Service::read_digital_input(int pin){
    return beckhoffService->read_digital_input(pin);
}

//
//
//
void Facade_Beckhoff_Communication_Service::write_digital_output(int pin, bool signal){
    return beckhoffService->write_digital_output(pin, signal);
}