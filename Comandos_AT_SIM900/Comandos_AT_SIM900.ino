


/*
        Tutorial Módulos GSM/GPRS
--------------------------------------------------*
  Video 7: Puerto de comunicación en el Sim900.   *
    ARDUINO UNO     SIM900                        *
      D7              TX                          *
      D8              RX                          *
      RESET           RST                         *
      GND             GND                         *
                                                  *
  Autor: Diseño Electrónico A2P                   *
--------------------------------------------------*/

 #include <SoftwareSerial.h>  //Incluir Libreria SoftwareSerial

 SoftwareSerial MOD_SIM900(7, 8); // Se declaran los pines del arduino uno D7 y D8
                                  //(RX Del Modulo SIM900), (TX Del Modulo SIM900)

 void setup(){
  Serial.begin(19200);
  MOD_SIM900.begin(19200);
  Serial.println("Start!...");
  MOD_SIM900.println("AT");  
  
 }

  void loop(){
  
   if (MOD_SIM900.available())
     Serial.write(MOD_SIM900.read());    
    if(Serial.available()) 
    MOD_SIM900.write(Serial.read());
  }




  
