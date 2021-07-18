#ifndef FACADE_BEKHOFF_COMMUNICATION_SERVICE_H_INCLUDED
#define FACADE_BEKHOFF_COMMUNICATION_SERVICE_H_INCLUDED

#include <iostream>
#include "Beckhoff_Communication_Service.h"

using namespace std;

class Facade_Beckhoff_Communication_Service
{
    private:
        Beckhoff_Communication_Service* commService;

    public:
        Facade_Beckhoff_Communication_Service(); //constructor
        ~Facade_Beckhoff_Communication_Service(); //destructor

        bool verify_communication_status();
};

#endif