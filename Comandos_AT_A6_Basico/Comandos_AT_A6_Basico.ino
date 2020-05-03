
/*
        Tutorial Módulos GSM/GPRS
----------------------------------------------------*
  Video 10: Comandos AT en el A6 Básico.            *
                                                    *
  Se utiliza la tarjetade Arduino Mega 2560         *
  El módulo A6 Bàsico y una fuente de 5.0V          *
                                                    *
  Autor: Diseño Electrónico A2P                     *
----------------------------------------------------*
*/

 void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Start!...");
  Serial1.println("AT");    
 }

  void loop(){  
   if (Serial1.available())
     Serial.write(Serial1.read());    
    if(Serial.available()) 
    Serial1.write(Serial.read());
  }

  
