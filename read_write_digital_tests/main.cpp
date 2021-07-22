#include <iostream>
#include <cmath>

using namespace std;

//int inputsBeckhoff = 20;

bool read_digital(int pin, int valor_lido_simulado)
{
    //wck = ec_receive_processdata(EC_TIMEOUTRET);
    //int *data_ptr;
	//data_ptr = &valor_lido_simulado;

	//unsigned char valor_lido = (unsigned char) *data_ptr;
	unsigned char valor_lido = (unsigned char) *(&valor_lido_simulado);
	
	unsigned char mask = pow(2,(pin - 1)); // 1
	
	// b'00010100 = 5
	// b'00000100 = 1
	// b'00000100 = 4
	return (valor_lido & mask);
}


bool write_digital(int pin, bool set)
{
    //wck = ec_receive_processdata(EC_TIMEOUTRET);

    //int *data_ptr;
    
	//data_ptr = &valor_lido_simulado;

	//unsigned char valor_lido = (unsigned char) *data_ptr;
	unsigned char valor_lido = (unsigned char) *(&valor_lido_simulado);
	
	unsigned char mask = pow(2,(pin - 1)); // 1
	
	// b'00010100 = 5
	// b'00000001 = 1
	// b'00010101 = 4
	return (valor_lido & mask);
}

int main()
{
    unsigned char a = 4;
	// bit [8, 7, 6, 5, 4, 3, 2, 1]
	//bech 
	// 1d = primeiro prino aceso da beckhoff
    cout << "5 bit de d'20  (b'00010100) "  << read_digital(5, 20) << endl;
	cout << "4 bit de d'20  (b'00010100) "  << read_digital(4, 20) << endl;
	cout << "3 bit de d'20  (b'00010100) "  << read_digital(3, 20) << endl;
	cout << "4 bit de d'23  (b'00010111) "  << read_digital(4, 23) << endl;
	cout << "8 bit de d'151 (b'10010111) "  << read_digital(8, 151) << endl;

}
