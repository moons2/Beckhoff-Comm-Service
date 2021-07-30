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
		cout << "Beckhoff connected successfully! "<< endl;
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
bool Beckhoff_Communication_Service::read_digital_input(int pinAddr)
{   // work better to reduce this method
    // apply software pattern too

    //realiza a leitura de todos os pinos de entrada digitais
    wkc = ec_receive_processdata(EC_TIMEOUTRET);

	/*Aponta para o primeiro byte de uma palavra de 2 bytes
	que guarda o nível lógico de cada um dos 16 pinos de entrada lidos.
	O Pino 1 de Input digital é representado pelo bit menos significativo 
	do primeiro byte, portanto, seu endereço é igual a 0 */ 
    uint8 *inputPtr = ec_slave[remota.terminalPositionIn].inputs;

	/*Se o pino for maior que 8, reinicia a contagem dos pinos:
	atribui o valor 0 ao endereço do pino 9, 1 ao endereço do pino 10 e assim por diante..
	Pois o ponteiro é incrementado para ler o segundo byte e,
	no segundo byte, o pino 9 é representado pelo bit menos significativo, como
	O pino 9 tem inicialmente o endereço igual a 8, a verficação abaixo fica como "if (pin > 7)" para verificar se trata-se do pino 9*/
   
	// Pinagem das Inputs digitais da Beckhoff:  		   |1 ... 8|9 ...16|
	// Endereço atribuido aos pinos de Input: (pinAddr)    |0 ... 7|8 ...15|
	// Leitura dos níveis lógicos dos pinos:  (*inputPtr)  | byte1 | byte2 |
	// Bits, nos bytes lidos, refentes aos pinos: 		   |0 ... 7|0 ... 7|
	// Ponteiro:                                  			^inputPtr
	//																^inputPtr++
   
    if(pinAddr > 7){ //addrs 8 to 15 means pins 9 to 16 at beckhoff
		if(pinAddr == 8){
			pinAddr = 0;
		}
		else{
			pinAddr = (pinAddr % 9) + 1;
		}
        inputPtr++; // increments inputPtr to next byte
    }

	//o inteiro de 8 bits sem sinal beckhoffPinsValue recebe o conteúdo do ponteiro inputPtr
    unsigned char beckhoffPinsValue = (unsigned char) *inputPtr; //convert *inputPtr from int to unsigned char
	 
	/*cria máscara de bits que atribui o valor 1 no endereço do pino que se deseja ler 
	e o valor 0 nos demais endereços. Exemplo:
	Endereço do pino 1 = 0, então, máscara do pino 1 = b'00000001
    Endereço do pino 4 = 3, então, máscara do pino 4 = b'00001000
	*/
    unsigned char mask = pow(2,(pinAddr));
	
    /*Realiza a operação lógica AND entre o byte lido e a máscara de bits:
	Se o byte lido contiver o valor 1 na posição correspondente ao pino que se deseja ler,
	então, esta operação lógica irá resultar na própria máscara de bits. Nesse caso, retorna-se true.
	Caso o byte lido contiver o valor 0 na posição correspondente ao pino que se deseja ler,
	então, esta operação lógica irá resultar no valor 0. Nesse caso, retorna-se falso.
	*/
    if ((beckhoffPinsValue & mask) == mask){
    	return true;
    }
    
    return false;
}

//
// params: ? slaveIntAddress and value or int to be written?
// return: true if success
void Beckhoff_Communication_Service::write_digital_output(int pinAddr, bool signal)
{
    uint8 mask;

	//de 0 até 7: pino 1 ao pino 8 da el2809*/
	if(pinAddr < 8){
		mask = pow(2,(pinAddr)); //seta o bit referente ao pino e zera os demais

		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico

			//operação lógica AND
			remota.byteOutputA = remota.byteOutputA & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais
		
		}else {//deseja envial sinal lógico alto para um pino específico
			//operação lógica OR
			remota.byteOutputA = remota.byteOutputA | mask;
			
		}
	}
	else{//de 8 até 15: pino 9 ao pino 16 da el2809*/

	/*Se o pino for maior que 8, reinicia a contagem dos pinos:
	atribui o valor 0 ao endereço do pino 9, 1 ao endereço do pino 10 e assim por diante..
	Pois o ponteiro é incrementado para ler o segundo byte e,
	no segundo byte, o pino 9 é representado pelo bit menos significativo, como
	O pino 9 tem inicialmente o endereço igual a 8.*/
   
	// Pinagem das Outputs digitais da Beckhoff:  		   |1 ... 8|9 ...16|
	// Endereço atribuido aos pinos de Output: (pinAddr)   |0 ... 7|8 ...15|
	// Escrita dos níveis lógicos dos pinos:  (*data_ptr)  | byteA | byteB |
	// Bits, nos bytes escritos, refentes aos pinos:	   |0 ... 7|0 ... 7|
	// Ponteiro:                                  			^data_ptr
	//																^data_ptr++

		if(pinAddr == 8){ //Reinicia contagem dos pinos
			pinAddr = 0;
		}else{
			pinAddr = (pinAddr % 9) + 1;
		}

		mask = pow(2,(pinAddr));

		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico
			
			//operação lógica AND
			remota.byteOutputB = remota.byteOutputB & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais

		}else {//deseja envial sinal lógico alto para um pino específico

			//operação lógica OR
			remota.byteOutputB = remota.byteOutputB | mask;
			
		}
	}
	
	//Aponta para a estrutura de dados que irá enviar os valores de outputs desejados
	uint8 *data_ptr = ec_slave[remota.terminalPositionOut].outputs;
	*data_ptr = remota.byteOutputA;
	data_ptr++;
	*data_ptr = remota.byteOutputB;
	ec_send_processdata(); // enviando o dado para o IOmap (remota)
}

//Método para escrever em uma saída analógica por vez
/*
void Beckhoff_Communication_Service::write_analog_output(bool channel, int voltage){
//void Beckhoff_Communication_service::writes_motors_voltage(int voltageM1, int voltageM2){
	//garante tensão entre 0 e 24v
	if(voltage > 24)
		voltage = 24;
	else if (voltage < 0)
		voltage = 0;
	
	/*Se a resolução for de 12 bits:
	Valores variam de 0 a 4095
	 24[v] * x = 4095  => x = 170.625//
	uint16 voltage16 = voltage * 170.625;

	//cout << tensao16 << endl;
	/*Se a resolução for de 16 bits:
	Valores variam de 0 a 65535
	24[v] * x = 65535  => x = 2730.625//
   	//__uint16_t tensao16 = tensao * 2730.625;
	
	//aponta para estrutura de dados que diz respeito aos terminais de saída analógica
	uint8 *data_ptr = ec_slave[remota.terminalPositionAnalog].outputs;
	
	// | byte1 | byte2 | byte3 | byte4 | 
	// | M1 - Channel 0| M2 - Channel 1|

   	/* Move pointer to correct module index
	Motor do pino 1: canal 0
	Motor do pino 2: canal 1
	Como cada motor recebe dois bytes como referência para a tensão,
	Deve-se incrementar o ponteiro em dois bytes quando se desejar escrever no canal 2, já que os 4 bytes referentes aos valores de ambos motores são adjacentes
	//
	data_ptr += channel* 2;

	*data_ptr = voltage16; //escreve 8 bits menos significativos
	//cout << +*data_ptr << endl;
	data_ptr++;
	*data_ptr = (voltage16 >> 8); //shift à direita para escrever 8 bits mais significativos
	//cout << +*data_ptr << endl;
    ec_send_processdata(); // enviando o dado para o IOmap (remota)
	
}*/

void Beckhoff_Communication_Service::write_motors_voltage(int voltageM1, int voltageM2){
	//Verificar qual é o motor da direita e qual é o motor da esquerda e alterar nome do parâmetro. Ex.:
	//voltageM1 = voltageMotorLeft
	
	//garante tensão entre 0 e 24v
	if(voltageM1 > 24)
		voltageM1 = 24;
	else if (voltageM1 < 0)
		voltageM1 = 0;

	if(voltageM2 > 24)
		voltageM2 = 24;
	else if (voltageM2 < 0)
		voltageM2 = 0;

	// |   voltage M1  |   voltage M2  |
	// | byte1 | byte2 | byte3 | byte4 | 
	//  ^data_ptr
	
	//aponta para estrutura de dados que deve receber os sinais dos terminais de saída analógica
	uint8 *data_ptr = ec_slave[remota.terminalPositionAnalog].outputs; //Aponta para o byte 1
	
	//---------Escrevendo tensão do motor M1-------------------
	/*Se a resolução for de 12 bits:
	Valores variam de 0 a 4095
	 24[v] * x = 4095  => x = 170.625*/
	uint16 voltage16 = voltageM1 * 170.625; //converte tensão de 0 a 24v em um numero de 0 a 4095 correspondente

	/*Se a resolução for de 16 bits:
	Valores variam de 0 a 65535
	24[v] * x = 65535  => x = 2730.625*/
   	//__uint16_t tensao16 = tensao * 2730.625;

	*data_ptr = voltage16; //escreve 8 bits menos significativos no byte 1
	data_ptr++; //Aponta para o byte 2
	*data_ptr = (voltage16 >> 8); //shift à direita para escrever 8 bits mais significativos no byte 2

	/*
	Como cada motor recebe dois bytes como referência para a tensão,
	Deve-se incrementar o ponteiro em dois bytes quando se desejar escrever no motor M2, 
	já que os 4 bytes referentes aos valores de ambos motores são adjacentes
	*/
	//---------Escrevendo tensão do motor M2----------------------
	data_ptr ++; //Apontando para o byte 3
	voltage16 = voltageM2 * 170.625; //converte tensão de 0 a 24v em um numero de 0 a 4095 correspondente
	
	*data_ptr = voltage16; //escreve 8 bits menos significativos no byte 3
	data_ptr++; //Apontando para o byte 4
	*data_ptr = (voltage16 >> 8); //shift à direita para escrever 8 bits mais significativos no byte 4

    ec_send_processdata(); // enviando o dado para o IOmap (remota)
	
}