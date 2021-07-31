
// Mapping of Input Pins and Remote Digital Output
// to be used by all software that make use of communication service with Beckhoff

//Digital signal reference
#define LOW 0
#define HIGH 1

// EL1809 - Digital Inputs Module
// assigns pin from the EL1809 module to the position of the corresponding bit 
#define IN_PHOTOELTRIC_SENSOR_1 0 //Sensor Fotoeletrico 1 - PINO 1
#define IN_PHOTOELTRIC_SENSOR_2 1 //Sensor Fotoeletrico 2 - PINO 2
#define IN_PISTON_SENSOR 2 //Sensor Pistão - PINO 3
#define IN_STOP_BTN 3 //Botão Stop - PINO 4
#define IN_PRESSURE_SENSOR_1 4 //Sensor de pressao 1 - PINO 5
#define IN_PRESSURE_SENSOR_2 5 //Sensor de pressao 2 - PINO 6
#define IN_PIN_7 6 //livre
#define IN_PIN_8 7 //livre
#define IN_PIN_9 8 //livre
#define IN_PIN_10 9 //livre
#define IN_PIN_11 10 //livre
#define IN_PIN_12 11 //livre
#define IN_PIN_13 12 //livre
#define IN_PIN_14 13 //livre
#define IN_PIN_15 14 //livre
#define IN_PIN_16 15 //livre


// EL2809 - Digital Outputs Module
// assigns pin from the EL2809 module to the position of the corresponding bit 
#define OUT_PIN_1 0 //livre
#define OUT_STOP_M1 1 //Dig In motor 1 motor da direita ou da esquerda? - PINO 2
#define OUT_ENABLE_M1 2 //Dig In motor 1 - PINO 3
#define OUT_DIRECTION_M1 3 //Dig IN/Out motor 1 - PINO 4
#define OUT_STOP_M2 4 //Dig In motor 2 - PINO 5
#define OUT_ENABLE_M2 5	//Dig In motor 2 - PINO 6
#define OUT_DIRECTION_M2 6 //Dig In/Out motor 2 - PINO 7
#define OUT_PISTON 7 //Pistão - PINO 8
#define OUT_CAMERA_LAMP 8 //Iluminação da Camera - PINO 9
#define OUT_PIN_10 9 //livre
#define OUT_WARNING_SOUND 10 //Aviso_sonoro - PINO 11
#define OUT_LED_RED 11 //Led canal red - PINO 12
#define OUT_LED_GREEN 12 //Led canal green - PINO 13
#define OUT_LED_BLUE 13 //Led canal blue - PINO 14
#define OUT_PIN_15 14 //livre
#define OUT_PIN_16 15 //livre

// EL4002 - Analog Outputs Module
// assigns pin from the EL4002 module to the corresponding channel
//#define ANALOG_MOTOR_M1 0 //left or right? ANALOG_MOTOR_LEFT
//#define ANALOG_MOTOR_M2 1