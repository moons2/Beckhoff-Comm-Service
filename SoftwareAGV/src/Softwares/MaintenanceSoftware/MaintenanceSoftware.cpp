#include "MaintenanceSoftware.h"
using namespace std;

MaintenanceSoftware::MaintenanceSoftware(){  //constructor
    beckhoffCommunication = new Facade_Beckhoff_Communication_Service;

    //---------------------SETUP----------------------------------------
    //solicita conexão com a Beckhoff
    switch(beckhoffCommunication->beckhoff_connect()){ //connect and depuring
        case 1:
            cout << "Remota connected successfully \n";
            break;
        case 2:
            cout << "ERROR: Network Board Configuration Error \n";
            break;
        case 3:
            cout << "ERROR: configuring slaves error\n";
            break;
        case 4:
            cout << "ERROR: Not all entry and exit cards were found in the correct positions! \n";
            break;
        /*case 5:
            cout << "ERROR: slave not operational\n";
            break;*/
    }
}
MaintenanceSoftware::~MaintenanceSoftware(){ //destructor

}

void MaintenanceSoftware::Run(){ //Run in loop at Super Main

    int speedPercentageM1  = 90, //90% da velocidade dos motores
        speedPercentageM2 = 90; //90% da velocidade dos motores
    
    bool isConnected = beckhoffCommunication->verify_communication_status();

    if(isConnected){

        beckhoffCommunication->write_digital_output(OUT_ENABLE_M1, 1); //habilita M1
        beckhoffCommunication->write_digital_output(OUT_ENABLE_M2, 1); //habilita M2
        
        if(beckhoffCommunication->read_digital_input(IN_STOP_BTN)){ //lê estado do botão de emergência
            cout << "Motores devem girar em sentido horário...\n";
            cout << "Lâmpadas devem piscar... \n";
            //----------ACIONAENTO DOS MOTORES------------------------------------------------
            //girar ambos os motores no sentido horário*, *verificar se a rotação será horária ou anti horária
            beckhoffCommunication->write_digital_output(OUT_DIRECTION_M1, 0);
            beckhoffCommunication->write_digital_output(OUT_DIRECTION_M2, 0);
            beckhoffCommunication->write_engines_voltage(speedPercentageM1, speedPercentageM2);

            //----------PISCA LAMPADA DA CAMERA------------------------------------------------
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 1);
            sleep(1); //delay 1s
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 0);
            sleep(1); //delay 1s
            
        }
        else{ 
            cout << "Motores devem girar em sentido anti horário... \n";
            cout << "Lâmpadas devem estar desligadas... \n";

            //----------ACIONAENTO DOS MOTORES-------------------------------------------------
            //girar ambos os motores no sentido anti horário*, *verificar se a rotação será horária ou anti horária
            beckhoffCommunication->write_digital_output(OUT_DIRECTION_M1, 1);
            beckhoffCommunication->write_digital_output(OUT_DIRECTION_M2, 1);
            beckhoffCommunication->write_engines_voltage(speedPercentageM1, speedPercentageM2);

            //turn off camera Lamp
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 0);
            sleep(1); //delay 1s
        }
    }else{
        cout << "ERROR: Toradex - Beckhoff is not connected! \n";
        cout << "Trying connect again ... \n";
        
        switch(beckhoffCommunication->beckhoff_connect()){ //connect and depuring
        case 1:
            cout << "Remota connected successfully \n";
            break;
        case 2:
            cout << "ERROR: Network Board Configuration Error \n";
            break;
        case 3:
            cout << "ERROR: configuring slaves error\n";
            break;
        case 4:
            cout << "ERROR: Not all entry and exit cards were found in the correct positions! \n";
            break;
        /*case 5:
            cout << "ERROR: slave not operational\n";
            break;*/
        }
    }
}