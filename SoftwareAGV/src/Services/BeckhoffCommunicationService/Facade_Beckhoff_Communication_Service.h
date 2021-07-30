#ifndef FACADE_BEKHOFF_COMMUNICATION_SERVICE_H_INCLUDED
#define FACADE_BEKHOFF_COMMUNICATION_SERVICE_H_INCLUDED

#include <iostream>
#include "Beckhoff_Communication_Service.h"

using namespace std;

class Facade_Beckhoff_Communication_Service
{
    private:
        Beckhoff_Communication_Service* beckhoffService;

    public:
        Facade_Beckhoff_Communication_Service(); //constructor
        ~Facade_Beckhoff_Communication_Service(); //destructor

        int beckhoff_connect();

        //
        //
        // return: true (connection open); false other
	
	    //void state_test();

        bool verify_communication_status();

        //
        //
        //
        bool beckhoff_disconnect();

        //
        //
        //
        bool read_digital_input(int pin);

        //
        //
        //
        void write_digital_output(int pin, bool signal);
        
        //
        //
        //
        void write_motors_voltage(int voltageM1, int voltageM2);
        //
        //
        //
        //bool read_analog_inputs();

        //
        //
        //
        //void write_analog_outputs();
};

#endif