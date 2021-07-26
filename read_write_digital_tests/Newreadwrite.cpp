#include <iostream>
#include <cmath>
#include <cstdint>
using namespace std;

//Mapeamento dos pinos de input e output digitais Deve ser separado em um arquivo "BeckhoffInputOutputAddrs.h" 
//para ser usado por todos os softwares que fazem uso deste serviço

//EL1809 - módulo de Inputs digitais
//Atribui pino do módulo EL1809 à um endereço correspondente no vetor ethercat->sinaisInput[]
#define INPUT_PIN_1 0 //Sensor Fotoeletrico 1
#define INPUT_PIN_2 1 //Sensor Fotoeletrico 2
#define INPUT_PIN_3 2 //Pistão
#define INPUT_PIN_4 3 //Botão Stop
#define INPUT_PIN_5 4 //Sensor de pressao 1
#define INPUT_PIN_6 5 //Sensor de pressao 1
#define INPUT_PIN_7 6 //livre
#define INPUT_PIN_8 7 //livre
#define INPUT_PIN_9 8 //livre
#define INPUT_PIN_10 9 //livre
#define INPUT_PIN_11 10 //livre
#define INPUT_PIN_12 11 //livre
#define INPUT_PIN_13 12 //livre
#define INPUT_PIN_14 13 //livre
#define INPUT_PIN_15 14 //livre
#define INPUT_PIN_16 15 //livre


//EL2809 - módulo de Outputs digitais
//Atribui pino do módulo EL2809 à um endereço correspondente no vetor ethercat->sinaisOutput[]
#define OUTPUT_PIN_1 0 //livre
#define OUTPUT_PIN_2 1 //Dig In motor 1 motor da direita ou da esquerda?
#define OUTPUT_PIN_3 2 //Dig In motor 1
#define OUTPUT_PIN_4 3 //Dig IN/Out motor 1
#define OUTPUT_PIN_5 4 //Dig In motor 2
#define OUTPUT_PIN_6 5	//Dig In motor 2
#define OUTPUT_PIN_7 6 //Dig In/Out motor 2
#define OUTPUT_PIN_8 7 //Pistão
#define OUTPUT_PIN_9 8 //Iluminação da Camera
#define OUTPUT_PIN_10 9 //livre
#define OUTPUT_PIN_11 10 //Aviso_sonoro
#define OUTPUT_PIN_12 11 //Led canal red
#define OUTPUT_PIN_13 12 //Led canal green
#define OUTPUT_PIN_14 13 //Led canal blue
#define OUTPUT_PIN_15 14 //livre
#define OUTPUT_PIN_16 15 //livre

//para ilustar valor lido dos terminais da EL1809
__uint8_t inputsBeckhoff;

//para inicializar o valor de envio para os terminais da EL2809
__uint8_t outputsBeckhoff;
//unsigned char sinaisOutput; //uint8
int byteOutputA = 0;
int byteOutputB = 0;

bool read_digital_inputs(int pin)
{   // work better to reduce this method
    // apply software pattern too

    //
    //wck = ec_receive_processdata(EC_TIMEOUTRET);

	__uint8_t *inputPtr = &inputsBeckhoff;//inputsBeckhoff simula ec_slave[ethercat->posTerminalInput].inputs;
    if(pin > 7){ //addrs 0 to 7 means pins 1 to 8 at beckhoff
		if(pin == 8){
			pin = 0;
		}
		else{
			pin = (pin % 9) + 1;
		}
        inputPtr++;
		// increments valor_lido para o proimo byte
    }

    unsigned char beckhoffPinsValue = (unsigned char) *inputPtr; //convert *inputPtr from int to unsigned char

	cout << "Sinal recebido da Beckhoff: " << +beckhoffPinsValue << endl;
    unsigned char mask = pow(2,(pin));

    // b'00010100 = 5
    // b'00000100 = 1
    // b'00000100 = 4
	
    //verify logic operation and return true or false
    if ((beckhoffPinsValue & mask) == mask){
    	return true;
    }
    
    return false;
    
    //return (beckhofPinsValue & mask);
}


void enviaSinal(int pin, bool signal)
{ 

/* Para enviar ou receber os sinais temos algumas observações importantes quando fazemos uso da SOEM. Nossos módulos EL1809 e EL2809 possuem 16 canais, sendo duas fileiras de 8 canais cada.
Acontece que cada canal é na verdade uma potência de 2, por exemplo, digamos que queiramos enviar um sinal para o canal 3 da primeira fileira do módulo EL2809, teremos de enviar o valor 4 na variável que estará apontando para a saída, neste caso é a variável data_ptr, isso pois a contagem dos canais internamente se inicia no 0(zero) e o terceiro canal está na posição 2, que elevado a 2 é 4. */
	
	__uint8_t mask;

	//de 0 até 7: pino 1 ao pino 8 da el2809*/
	if(pin < 8){
		mask = pow(2,(pin)); //seta o bit referente ao pino e zera os demais

		//cout<< +mask << endl;
		//cout<< 255 - mask <<endl;
		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico

			//operação lógica AND
			byteOutputA = byteOutputA & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais
		
		}else {//deseja envial sinal lógico alto para um pino específico
			//operação lógica OR
			byteOutputA = byteOutputA | mask;
			
		}
	}
	else{//de 8 até 15: pino 9 ao pino 16 da el2809*/
		if(pin == 8){
			pin = 0;
		}else{
			pin = (pin % 9) + 1;
		}

		mask = pow(2,(pin));

		//cout<< +mask << endl;
		//cout<< 255 - mask <<endl;
		if (signal == 0){ //deseja envial sinal lógico baixo para um pino específico
			
			//operação lógica AND
			byteOutputB = byteOutputB & (255 - mask); //inversão dos bits da máscara - zera o bit que se deseja resetar e seta os demais

		}else {//deseja envial sinal lógico alto para um pino específico

			//operação lógica OR
			byteOutputB = byteOutputB | mask;
			
		}
	}
	
	__uint8_t *data_ptr = &outputsBeckhoff;//outputsBeckhoff simula ec_slave[ethercat->posTerminalInput].outputs;
	*data_ptr = byteOutputA;
	data_ptr++;
	*data_ptr = byteOutputB;
	//ec_send_processdata(); // enviando o dado para o IOmap (remota)

}

/*
void xpin(){ //verificar se a numeração dos pinos reinicia a partir do pino 8 - teste ok

	for(int i=0; i<16; i++){
		int pin = i;
		if(pin > 7){
			if(pin == 8){
				pin = 0;
			}else{
				pin = (pin % 9) + 1;
			}
			cout << pin << endl;
			//data_ptr++; //posiciona ponteiro para escrever segundo byte
		}else
			cout << pin << endl;
	}
}*/

int main(){

	//--------------INPUTS-----------------------------
	inputsBeckhoff = 20; //simula valor lido da beckhoff

	cout << "Leitura dos pinos de Input da EL1809: \n";
	int pino3 = read_digital_inputs(INPUT_PIN_3);
	cout << "Nível lógico do pino 3: " << pino3 << endl;

	inputsBeckhoff = 1;	//set 0 to pin 3 and 1 to pin 1

	pino3 = read_digital_inputs(INPUT_PIN_3);
	cout << "Nível lógico do pino 3: " << pino3 << endl;

	int pino1 = read_digital_inputs(INPUT_PIN_1);
	cout << "Nível lógico do pino 1: " << pino1 << endl;
	cout << " \n";
	//--------------TESTE NUMERACAO ENDERECOS-----------
	//cout << "endereços de 0 a 7: \n";
	//xpin();//testa se a contagem dos pinos reinicia do 0 a partir do pino 8 - teste ok
	//--------------OUTPUTS-----------------------------
	cout << "Escrita nos pinos de Output da EL2809: \n";
	enviaSinal(OUTPUT_PIN_1, 1);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;
	enviaSinal(OUTPUT_PIN_3, 1);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;
	enviaSinal(OUTPUT_PIN_10, 1);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;
	enviaSinal(OUTPUT_PIN_1, 0);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;
	enviaSinal(OUTPUT_PIN_3, 0);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;
	enviaSinal(OUTPUT_PIN_10, 0);
	cout << "sinal enviado no byte 1: " << +byteOutputA<< endl;
	cout << "sinal enviado no byte 2: " << +byteOutputB<< endl;

	return 0;
}