#include "Beckhoff_Communication_Service.h"

/******************* beckhoff_connection ********************
*************************************************************
Operation: Perform connection between Toradex and Beckoff
Input Parameters: 
Output: 
*/

using namespace std;

char IOmap[4096];
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;
int expectedWKC;

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

    remota.connected = false; //flag to verify connection

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
        ec_config_map(&IOmap);
        ec_send_processdata();
        wkc = ec_receive_processdata(EC_TIMEOUTRET);
        ec_writestate(0);
        ec_statecheck(0, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);

        // ? this two next lines
        wkc = ec_receive_processdata(EC_TIMEOUTRET);
		expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;

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

int Beckhoff_Communication_Service::beckhoff_connect()
{
    // consider replace this if by a try-catch
    if(ec_init(remota.networkBoardName)){

        // wouldn't if (ec_config_init(FALSE)) produce de same result?
        if(ec_config_init(FALSE)){

            // Beckhoff found! not necessary to continue
            //if(ping)
            //    return 1;

            // verifying each module...
            if(strcmp(ec_slave[1].name, ACOPLADOR)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
				remota.connected = false;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionIn].name, DIGITAL_INPUT_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
					remota.connected = false;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionOut].name, DIGITAL_OUTPUT_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffOutupModuleName
                     << endl;
					remota.connected = false;
                return 4;
            }

            if(strcmp(ec_slave[remota.terminalPositionAnalog].name, ANALOG_MODULE)){
                cout << "error finding card "
                     << remota.beckhoffAnalogModuleName
                     << endl;
					remota.connected = false;
                return 4;
            }

            if(not set_slaves_up()){
                // something went wrong during set_slaves_up
                return 5;
            }
	    // SUCCESS
	    remota.connected = true; //conected
        cout << "Slave configured successfully! "<< endl;
	    //check the status of the slave and define it as operatiol, if it is non-operational 
	    ec_group[currentgroup].docheckstate = FALSE;
	    state_test();
            return 1;

        } else {
            //if(ping){
            //    return 0;
            //}
	    	remota.connected = false;
            return 3;
        }

    } else {
        // network board connection problem
        //if(ping){
        //    return 0;
        //}
		remota.connected = false;
        return 2;
    }

}

void Beckhoff_Communication_Service::state_test(){
	
	ec_readstate();
	for (int slave = 1; slave <= 4; slave++) //verify the four modules from beckhoff
	{
		switch(slave){
			case 1:
				cout << "EK1100: " << endl;
				break;
			case 2:
				cout << "EL1809: " << endl;
				break;
			case 3:
				cout << "EL2809: " << endl;
				break;
			case 4:
				cout << "EL4002: " << endl;
				break;
		}

		if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
			{
			ec_group[currentgroup].docheckstate = TRUE;
			if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
			{
				cout <<"ERROR : module " << slave << " is in state SAFE_OP + ERROR, attempting ack.\n";
				ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
				ec_writestate(slave);
			}
			else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
			{
				cout <<"WARNING : module " << slave << " is in state SAFE_OP, Changing to Operational.\n";
				ec_slave[slave].state = EC_STATE_OPERATIONAL;
				ec_writestate(slave);      
			}
			else if(ec_slave[slave].state > 0)
			{ 
				if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
				{
				ec_slave[slave].islost = FALSE;
				cout <<"MESSAGE : module " << slave << " reconfigured\n";   
				}
			} 
			else if(!ec_slave[slave].islost)
			{
			/* re-check state */
			ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
			if (!ec_slave[slave].state)
			{
				ec_slave[slave].islost = TRUE;
				cout<<"ERROR : module " << slave << " lost\n";   
			}
			}
		}
		if (ec_slave[slave].islost)
		{
			if(!ec_slave[slave].state)
			{
			if (ec_recover_slave(slave, EC_TIMEOUTMON))
			{
				ec_slave[slave].islost = FALSE;
				cout <<"MESSAGE : slave " << slave << " recovered\n";   
			}
			}
			else
			{
			ec_slave[slave].islost = FALSE;
			cout <<"MESSAGE : slave " << slave << " found\n";   
			}
			//return true;
		}
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
    // If connected return true. Connected: ready to read and write data
	state_test();
    return remota.connected;
}

//
//
//
bool Beckhoff_Communication_Service::beckhoff_disconnect()
{
    try {
        // close ethercat communication socket
        ec_close();
		remota.connected = false;
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
bool Beckhoff_Communication_Service::read_digital_input(int pin)
{   // work better to reduce this method
    // apply software pattern too

    //
    wkc = ec_receive_processdata(EC_TIMEOUTRET);

    uint8 *inputPtr = ec_slave[remota.terminalPositionIn].inputs;

    if(pin > 7){ //addrs 8 to 15 means pins 9 to 16 at beckhoff
	if(pin == 8){
		pin = 0;
	}
	else{
		pin = (pin % 9) + 1;
	}
        inputPtr++; // increments inputPtr to next byte
    }

    unsigned char beckhoffPinsValue = (unsigned char) *inputPtr; //convert *inputPtr from int to unsigned char

    unsigned char mask = pow(2,(pin));

    // b'00010100 = 5
    // b'00000100 = 1
    // b'00000100 = 4
	
    //verify logic operation and return true or false
    if ((beckhoffPinsValue & mask) == mask){
    	return true;
    }
    
    return false;
}

//
// params: ? slaveIntAddress and value or int to be written?
// return: true if success
void Beckhoff_Communication_Service::write_digital_output(int pin, bool signal)
{
    uint8 mask;

	//de 0 até 7: pino 1 ao pino 8 da el2809*/
	if(pin < 8){
		mask = pow(2,(pin)); //seta o bit referente ao pino e zera os demais

		//cout<< +mask << endl;
		//cout<< 255 - mask <<endl;
		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico

			//operação lógica AND
			remota.byteOutputA = remota.byteOutputA & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais
		
		}else {//deseja envial sinal lógico alto para um pino específico
			//operação lógica OR
			remota.byteOutputA = remota.byteOutputA | mask;
			
		}
	}
	else{//de 8 até 15: pino 9 ao pino 16 da el2809*/
		if(pin == 8){ //Reinicia contagem dos pinos
			pin = 0;
		}else{
			pin = (pin % 9) + 1;
		}

		mask = pow(2,(pin));

		//cout<< +mask << endl;
		//cout<< 255 - mask <<endl;
		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico
			
			//operação lógica AND
			remota.byteOutputB = remota.byteOutputB & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais

		}else {//deseja envial sinal lógico alto para um pino específico

			//operação lógica OR
			remota.byteOutputB = remota.byteOutputB | mask;
			
		}
	}
	
	uint8 *data_ptr = ec_slave[remota.terminalPositionOut].outputs;
	*data_ptr = remota.byteOutputA;
	data_ptr++;
	*data_ptr = remota.byteOutputB;
	ec_send_processdata(); // enviando o dado para o IOmap (remota)
}

