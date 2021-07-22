#include <iostream>
#include <cmath>

using namespace std;

//Mapeamento dos pinos de input e output digitais Deve ser separado em um arquivo "BeckhoffInputOutputAddrs.h" 
//para ser usado por todos os softwares que fazem uso deste serviço

//EL1809 - módulo de Inputs digitais
//Atribui pino do módulo EL1809 à um endereço correspondente no vetor ethercat->sinaisInput[]
#define INPUT_PIN_1 0
#define INPUT_PIN_2 1
#define INPUT_PIN_3 2
#define INPUT_PIN_4 3
#define INPUT_PIN_5 4
#define INPUT_PIN_6 5
#define INPUT_PIN_7 6
#define INPUT_PIN_8 7
#define INPUT_PIN_9 8
#define INPUT_PIN_10 9
#define INPUT_PIN_11 10
#define INPUT_PIN_12 11
#define INPUT_PIN_13 12
#define INPUT_PIN_14 13
#define INPUT_PIN_15 14
#define INPUT_PIN_16 15


//EL2809 - módulo de Outputs digitais
//Atribui pino do módulo EL2809 à um endereço correspondente no vetor ethercat->sinaisOutput[]
#define OUTPUT_PIN_1 0
#define OUTPUT_PIN_2 1
#define OUTPUT_PIN_3 2
#define OUTPUT_PIN_4 3
#define OUTPUT_PIN_5 4
#define OUTPUT_PIN_6 5
#define OUTPUT_PIN_7 6
#define OUTPUT_PIN_8 7
#define OUTPUT_PIN_9 8
#define OUTPUT_PIN_10 9
#define OUTPUT_PIN_11 10
#define OUTPUT_PIN_12 11
#define OUTPUT_PIN_13 12
#define OUTPUT_PIN_14 13
#define OUTPUT_PIN_15 14
#define OUTPUT_PIN_16 15

#define TAM 16 //tamanho do vetor de entradas e saídas digitais
bool sinaisInput[TAM]; //simula ethercat->sinaisInput[i];
bool sinaisOutput[TAM]; //simula ethercat->sinaisOutput[i];

//para ilustar valor lido dos terminais da EL1809
int inputsBeckhoff = 20;

//para inicializar o valor de envio para os terminais da EL2809
int outputsBeckhoff = 0;

//void  lerTerminal(Ethercat *ethercat)
void  lerTodasInputs()
{ 

	//wkc = ec_receive_processdata(EC_TIMEOUTRET);
	
	int *data_ptr; //int no lugar de uint8 pois uint8 pertence a ethercat.h

	data_ptr = &inputsBeckhoff;//inputsBeckhoff simula ec_slave[ethercat->posTerminalInput].inputs;
	
	int valorLido = *data_ptr;
	// *data_ptr é um número inteiro decimal de 8 bits, vai de 0 a 255.
	
	//Conversão de data_ptr para binário e preenche vetor sinaisInput - pino 1 ao 8 de input do módulo EL1809
	for(int i=0; i<8; i++){
		if(valorLido > 1){ //divisões sucessivas por 2
			int bit = valorLido%2; //bit igual ao resto da divisao
			sinaisInput[i] = bit; //simula ethercat->sinaisInput[i];
			valorLido = valorLido/2;
			
		}
		else if(valorLido == 1){ //se o valor recebido for igual a 1 ou seu a divisão tiver chegado ao fim
			sinaisInput[i] = 1;
			valorLido = 0; //fim da divisão, o bit lido é zerado
		}
		else if(valorLido == 0){ //se o valor recebido for igual a 0 ou se houver bits a serem preenchidos no vetor depois do fim da divisão
			sinaisInput[i] = 0;
		}
	}
	
	data_ptr++; //incrementa endereço do ponteiro para ler o próximo byte

	//Conversão de data_ptr para binário e preenche vetor sinaisInput - pino 9 ao 16 de input do módulo EL1809
	for(int i=8; i<16; i++){
		if(valorLido > 1){ //divisões sucessivas por 2
			int bit = valorLido%2; //bit igual ao resto da divisao
			sinaisInput[i] = bit; //simula ethercat->sinaisInput[i];
			valorLido = valorLido/2;
			
		}
		else if(valorLido == 1){ //se o valor recebido for igual a 1 ou seu a divisão tiver chegado ao fim
			sinaisInput[i] = 1;
			valorLido = 0; //fim da divisão, o byte lido é zerado
		}
		else if(valorLido == 0){ //se o valor recebido for igual a 0 ou se houver bits a serem preenchidos no vetor depois do fim da divisão
			sinaisInput[i] = 0;
		}
	}
}

bool lerPinoDigital(int pino){// retorna leitura de um pino específico
	//lerTodasInputs(Ethercat *ethercat);
	lerTodasInputs();
	return sinaisInput[pino]; //ethercat->sinaisInput[pino];
}

//void enviaSinal(Ethercat *ethercat, int pino, int sinal)
void enviaSinal(int pino, bool sinal)
{ 

/* Para enviar ou receber os sinais temos algumas observações importantes quando fazemos uso da SOEM. Nossos módulos EL1809 e EL2809 possuem 16 canais, sendo duas fileiras de 8 canais cada.
Acontece que cada canal é na verdade uma potência de 2, por exemplo, digamos que queiramos enviar um sinal para o canal 3 da primeira fileira do módulo EL2809, teremos de enviar o valor 4 na variável que estará apontando para a saída, neste caso é a variável data_ptr, isso pois a contagem dos canais internamente se inicia no 0(zero) e o terceiro canal está na posição 2, que elevado a 2 é 4. */
	
	bool mascaraDeEnvio[16];
	
	//construção do vetor de máscara e operação lógica com o vetor de dados de envio
	if (sinal == 0){ //deseja envial sinal lógico baixo para um pino específico
		for(int i=0; i<16; i++){ //escreve 1 em todas as posições do vetor de máscara
			mascaraDeEnvio[i] = 1;		
		}
		mascaraDeEnvio[pino] = 0; //zera a penas o pino que se deseja enviar sinal lógico baixo
		
		//operação lógica AND
		for(int i=0; i<16; i++){
			sinaisOutput[i] = sinaisOutput[i] and mascaraDeEnvio[i];		
		}
	
	}else if (sinal == 1){//deseja envial sinal lógico alto para um pino específico
		for(int i=0; i<16; i++){ //escreve 0 em todas as posições do vetor de máscara
			mascaraDeEnvio[i] = 0;		
		}
		mascaraDeEnvio[pino] = 1; //atribui sinal lógico alto apenas ao pino que se deseja
		
		//operação lógica OR
		for(int i=0; i<16; i++){
			sinaisOutput[i] = sinaisOutput[i] or mascaraDeEnvio[i];		
		}
	}


	int *data_ptr;

	data_ptr = &outputsBeckhoff;//outputsBeckhoff simula ec_slave[ethercat->posTerminalInput].outputs;

	int byte1=0,byte2=0;


	for(int i=0;i<16;i++){
		if(i<8) //CONVERSÃO DE BINÁRIO PARA DECIMAL byte a byte
			//POTENCIA pow(base, expoente)
			byte1=byte1+sinaisOutput[i]*pow(2, i); //byte1 é a conversão de binário para decimal das posições 0 a 7 do vetor sinaisOutput
		else
			byte2=byte2+sinaisOutput[i]*pow(2, i-8); ////byte2 é a conversão de binário para decimal das posições 8 a 15 do vetor sinaisOutput
	}

	cout << "byte 1 = " << byte1 << endl;
	cout << "byte 2 = " << byte2 << endl;

	*data_ptr = byte1; //*DATA_PTR ENTÃO É UM NÚMERO INTEIRO DECIMAL
	data_ptr++; //INCREMENTA ENDEREÇO DE MEMÓRIA
	*data_ptr= byte2; //CONTEÚDO DO ENDEREÇO DE MEMÓRIA APONTADO POR DATA_PTR ATUALIZADO (ec_slave[ethercat->posTerminalOutput].outputs)

	
	//ec_send_processdata(); // enviando o dado para o IOmap (remota)

}

int main(){
	//inicializando vetores de input e output com 0, apenas para ilustrar construtor
	for(int i=0; i<16; i++){
		sinaisInput[i]=0;
	}
	for(int i=0; i<16; i++){
		sinaisOutput[i]=0;
	}


	lerTodasInputs();
	cout << "Leitura dos pinos de Input da EL1809: \n";
	for(int i=0; i<16; i++){
		cout << "Nível lógico do pino " << i+1 << " :";
		cout << sinaisInput[i] << endl;
	}
	
	int pinoLido = lerPinoDigital(INPUT_PIN_3);
	cout << "Pino 3 tem nível lógico: " << pinoLido << endl;

	cout << "enviando 1 para o pino de output 1...\n";
	enviaSinal(OUTPUT_PIN_1, 1);

	cout << "Leitura dos pinos de Output da EL2809: \n";
	for(int i=0; i<16; i++){
		cout << "Nível lógico do pino " << i+1 << " :";
		cout << sinaisOutput[i] << endl;
	}

	cout << "enviando 1 para o pino de output 3...\n";
	enviaSinal(OUTPUT_PIN_3, 1);

	cout << "Leitura dos pinos de Output da EL2809: \n";
	for(int i=0; i<16; i++){
		cout << "Nível lógico do pino " << i+1 << " :";
		cout << sinaisOutput[i] << endl;
	}

	cout << "enviando 0 para o pino de output 1...\n";
	enviaSinal(OUTPUT_PIN_1, 0);

	cout << "Leitura dos pinos de Output da EL2809: \n";
	for(int i=0; i<16; i++){
		cout << "Nível lógico do pino " << i+1 << " :";
		cout << sinaisOutput[i] << endl;
	}


	return 0;
}