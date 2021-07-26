#include "MaintenanceSoftware.h"
using namespace std;

MaintenanceSoftware::MaintenanceSoftware(){  //constructor
    beckhoffCommunication = new Facade_Beckhoff_Communication_Service;

    //Setup
    switch(beckhoffCommunication->beckhoff_connect()){ //connect and depuring
        case 1:
            cout << "Remota connected successfully \n";
            break;
        case 2:
            cout << "ERROR: Network Card Configuration Error \n";
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

void MaintenanceSoftware::Run(){
    bool stopBtn;
    bool cameraLamp;
    
    bool isConected = beckhoffCommunication->verify_communication_status();

    if(isConected){
        cout << "Toradex - Beckhoff conected successfully! \n";
        stopBtn = beckhoffCommunication->read_digital_input(IN_STOP_BTN);
        
        while(stopBtn){ //Floor Camera Lamp 
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 1);
            sleep(1); //delay 1s
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 0);
            sleep(1); //delay 1s
        }
        if(!stopBtn){
            beckhoffCommunication->write_digital_output(OUT_CAMERA_LAMP, 0);
        }
    }else{
        cout << "ERROR: Toradex - Beckhoff is not conected! \n";
        cout << "Trying connect again ... \n";
        
        switch(beckhoffCommunication->beckhoff_connect()){ //connect and depuring
        case 1:
            cout << "Remota connected successfully \n";
            break;
        case 2:
            cout << "ERROR: Network Card Configuration Error \n";
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