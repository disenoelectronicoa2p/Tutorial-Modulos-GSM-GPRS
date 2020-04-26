
/*
        Tutorial Módulos GSM/GPRS
--------------------------------------------------*
  Video 8: Encendido automático Sim900.           *
    ARDUINO UNO     SIM900                        *
      D7              TX                          *
      D8              RX                          *
      RESET           RST                         *
      GND             GND                         *
                                                  *
  Autor: Diseño Electrónico A2P                   *
--------------------------------------------------*/
  
    int Encender   = 9;  // Pin Para encender SIM900  

    void setup(){
     Serial.begin(19200);
     pinMode(Encender, OUTPUT);
  
     Encender_SIM900();  
     
     while( (EnviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || EnviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );
     Serial.println("Modulo encendido correctamente...");     
    }
    void loop(){  

    }
        
    void Encender_SIM900(){                            
     delay(2000);                                      
     digitalWrite(Encender,LOW);                       
     delay(1000);                                      
     digitalWrite(Encender,HIGH);                      
     delay(3000);                                      
     digitalWrite(Encender,LOW);    
     delay(2000);                         
    }


   int8_t EnviarComandoAT(char* ComandoAT, char* Resp_Esperada, unsigned int Retardo){

    uint8_t x=0,  Responde=0;
    char Respuesta[100];
    unsigned long Anterior;

    memset(Respuesta, '\0', 100);    // Borra el contenido del array
    delay(100);
    
    while( Serial.available() > 0)
    Serial.read();                  // Limpie el buffer de entrada
    
    if (ComandoAT[0] != '\0') {
      Serial.println(ComandoAT);    // Enviar comando AT
//      Serial.println(ComandoAT);  // Mostrar comando por serial
    }


    x = 0;
    Anterior = millis();

    // Este bucle espera la respuesta
    do{
       if(Serial.available() != 0){    // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
          Respuesta[x] = Serial.read();
          //Serial.print(response[x]);
          x++;
        if (strstr(Respuesta, Resp_Esperada) != NULL) {   // Verificar si la respuesta deseada (OK) está en la respuesta del módulo
          Responde = 1;
         }
       }
    }while((Responde == 0) && ((millis() - Anterior) < Retardo));    // Espera la respuesta con tiempo de espera

    return Responde;
 } 


 
