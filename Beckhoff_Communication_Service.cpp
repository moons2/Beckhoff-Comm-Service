#include "Beckhoff_Communication_Service.h"

/******************* beckhoff_connection ********************
*************************************************************
Operation: Perform connection between Toradex and Beckoff
Input Parameters: 
Output: 
*/

using namespace std;

volatile int wck;
char IOMap[4096];
int expectedWck;

// constructor method that performs the initial configurations before apply
// the connections between toradex - beckhoff.
// in the future, we can make the module positions dinamic not fixed
//
//
Beckhoff_Communication_Service::Beckhoff_Communication_Service()
{
    strcpy(remota.networkBoardName, NETW_BOARD_NAME);
    
    remota.terminalPositionIn = 2;
    strcpy(remota.beckhoffInputModuleName, DIGITAL_INPUT_MODULE);

    remota.terminalPositionOut = 3;
    strcpy(remota.beckhoffOutupModuleName, DIGITAL_OUTPUT_MODULE);

    remota.terminalPositionAnalog = 4;
    strcpy(remota.beckhoffAnalogModuleName, ANALOG_MODULE);

    cout << "SUCCESSFULLY BUILT!" << endl;
};

//
//
//
Beckhoff_Communication_Service::~Beckhoff_Communication_Service()
{
    // ? free variables
};

//
// params:
// return: true if slaves are operational
bool Beckhoff_Communication_Service::set_slaves_up()
{
    try {
        ec_config_map(&IOMap);
        ec_send_processdata();
        wck = ec_receive_processdata(EC_TIMEOUTRET);
        ec_writestate(0);
        ec_statecheck(0, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);

        // ? this two next lines
        wck = ec_receive_processdata(EC_TIMEOUTRET);
		expectedWck = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;

        return true;

    } catch (int err) {
        // cout << err << endl;
        return false;
    }
    
}


// return:  1 (success)
//          2 (network card problems)
//          3 (slaves problems)
//          4 (cards problems)
int Beckhoff_Communication_Service::beckhoff_connect(bool ping = false)
{
    // consider replace this if by a try-catch
    if(ec_init(remota.networkBoardName)){

        // wouldn't if (ec_config_init(FALSE)) produce de same result?
        if(ec_config_init(FALSE)){

            // Beckhoff found! not necessary to continue
            if(ping)
                return 1;

            // verifying each module...
            if(strcmp(ec_slave[1].name, ACOPLADOR)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionIn].name, DIGITAL_INPUT_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionOut].name, DIGITAL_OUTPUT_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionAnalog].name, ANALOG_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffAnalogModuleName
                     << endl;
                return 4;
            }

            if(not set_slaves_up()){
                // something went wrong during set_slaves_up
                return 5;
            }
        
            // SUCCESS
            return 1;

        } else {
            if(ping){
                return 0;
            }
            return 3;
        }

    } else {
        // network board connection problem
        if(ping){
            return 0;
        }
        return 2;
    }

}


// melhor se verificar se a conexão já está estabelecida
// ao invés de conectar e desconectar
// 
// conexao aberta, fechada, ausencia de cards
// ? teste de estado
//      se correto -> 1
//      otherwise ->  codigo do erro para depuração negativos
bool Beckhoff_Communication_Service::verify_communication_status()
{
    // If I got connected and disconnected successfullly, return true
    return beckhoff_connect(true) and beckhoff_disconnect();
}

//
//
//
bool Beckhoff_Communication_Service::beckhoff_disconnect()
{
    try {
        // close ethercat communication socket
        ec_close();
        // call ~Beckhoff_Communication_Service
        return true;

    } catch (int err) {
        cout << err << endl;
        return false;
    }
    
}

// this method read all digital inputs and
// saves
//
//
bool Beckhoff_Communication_Service::read_digital_inputs(int pin)
{   // work better to reduce this method
    // apply software pattern too

    //
    wck = ec_receive_processdata(EC_TIMEOUTRET);

    uint8* inputPtr = ec_slave[remota.terminalPositionIn].inputs;

    if(pin > 8){
		pin = (pin % 9) + 1;
        inputPtr++;
		// increments valor_lido para o proimo byte
	}

	unsigned char beckhofPinsValue = (unsigned char)* inputPtr;
	
	unsigned char mask = pow(2,(pin - 1));
	
	// b'00010100 = 5
	// b'00000100 = 1
	// b'00000100 = 4
	return (beckhofPinsValue & mask);
}

//
// params: ? slaveIntAddress and value or int to be written?
// return: true if success
bool write_digital_outputs()
{
    // SUCCESS
    return true;
}

