#ifndef BECKHOFF_COMMUNICATION_SERVICE_H_INCLUDED
#define BECKHOFF_COMMUNICATION_SERVICE_H_INCLUDED

#include <iostream>
#include <cstring>
#include <cinttypes>
#include <unistd.h>
#include <ctime>
#include <cmath>


extern "C"
{ //lib escrita em linguagem C
	#include "../../../libethercat/ethercat.h" //interface da lib ethercat codificada em C
}


#define EC_TIMEOUTMON               500
#define NUM_BECKHOFF_COMPONENTS     16
#define TAM_BECKHOFF_NAME_MODULE    15 // number of chars max of the Beckhoff module
#define CURR_GROUP                  0


#define NETW_BOARD_NAME             "eth0"
#define ACOPLADOR                   "EK1100"
#define DIGITAL_INPUT_MODULE        "EL1809"
#define DIGITAL_OUTPUT_MODULE       "EL2809"
#define ANALOG_MODULE               "EL4002"

#define ANALOG_PROPORTIONAL_CONST   0.000305625

typedef struct
{
    int terminalPositionIn;
    int terminalPositionOut;
    int terminalPositionAnalog;

    bool outputSignals[NUM_BECKHOFF_COMPONENTS];
    int inputSignals[NUM_BECKHOFF_COMPONENTS];

    // the below values were inserted manually, now they
    // they will be consts in the final value
    char beckhoffInputModuleName[TAM_BECKHOFF_NAME_MODULE];
    char beckhoffOutupModuleName[TAM_BECKHOFF_NAME_MODULE];
    char beckhoffAnalogModuleName[TAM_BECKHOFF_NAME_MODULE];
    char networkBoardName[TAM_BECKHOFF_NAME_MODULE];

} Ethercat;

class Beckhoff_Communication_Service
{
    private:
        Ethercat remota;

	public:
		Beckhoff_Communication_Service(); //constructor
		~Beckhoff_Communication_Service(); //destructor

        //
        // return: 1 for success; 2 for  ; 3 for 
        int beckhoff_connect(bool);

        //
        //
        // return: true (connection open); false other
        bool verify_communication_status();

        //
        //
        //
        bool beckhoff_disconnect();

        //
        //
        //
        bool set_slaves_up();

        //
        //
        //
        bool read_digital_inputs(int pin);

        //
        //
        //
        void write_digital_outputs();

        //
        //
        //
        bool read_analog_inputs();

        //
        //
        //
        void write_analog_outputs();

        //
        //
        //
        bool read_pin_digital_input(int pin);
};


// CONTANTS ERRORS

#endif
