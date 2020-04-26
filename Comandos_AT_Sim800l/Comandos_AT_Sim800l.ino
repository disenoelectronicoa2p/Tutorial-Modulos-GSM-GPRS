
/*
        Tutorial Módulos GSM/GPRS
--------------------------------------------------*
  Video 6: Comandos AT en el Sim800l. Video 6.    *
                                                  *
  Se utiliza la tarjetade Arduino Mega 2560       *
  El módulo sim800l y 2 baterías de litho de 3.7V *
                                                  *
  Autor: Diseño Electrónico A2P                   *
--------------------------------------------------*
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
