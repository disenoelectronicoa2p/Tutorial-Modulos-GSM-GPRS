
    #include <Separador.h> 
    
    Separador s;
    
    #define resetSim800l 8 // Pin 8: Resetear modulo Sim800l 

    String bodyMsm;

    String co0,co2,posStr;
    String countryCode,numMovil;
    String nameNumber;
    int lengthNumber,pos;   

    unsigned long measurementTime; //Contador para envio mensaje 
    unsigned long tiempoPrevioMSN;     //Contador para control envio mensaje   

    bool answerAtConectionRed;
    int errorAtConectionRed=0;    

    void setup() {

      pinMode(resetSim800l,OUTPUT);  
      digitalWrite(resetSim800l,HIGH);
            
      Serial.begin(115200);  

      pos = 3;          
      lengthNumber = 18;
      countryCode = "+57";
      measurementTime = 20000;
      
      sendAtConectionRed();      
      
      Serial.println("Start!...");           
    }

    void loop() {
    
      unsigned long tiempoActualMSN = millis(); //Inicio contador
      if ((unsigned long)(tiempoActualMSN - tiempoPrevioMSN) >= measurementTime){
                
        posStr = (String)(pos++);
        String number = readNumberSimCard("AT+CPBR="+posStr);
        if(number.length()>lengthNumber){
         separateMsm(number);    

          if(posStr.equals("3")){ // Comentar para enviar a todos los Numeros
           sendAtConectionRed();
           sendMsnNumbers();
          } // Comentar para enviar a todos los Numeros
         
        }

        if(pos==15)
          pos=3;     
        
       tiempoPrevioMSN = tiempoActualMSN;      
      }     
    }
    

    String readNumberSimCard(String comandoAT){

      while(Serial.available())
      Serial.read();                  // Limpie el buffer de entrada
    
      if (comandoAT.length()>0) {
       Serial.println(comandoAT);    // Enviar comando AT
       //Serial.println(comandoAT);  // Mostrar comando por serial
      }
      return readDataSIM800L();
    }


    String readDataSIM800L(){
     String texto;

      while(!Serial.available()){}
      texto = Serial.readString();
      Serial.println(texto);     

     return texto;
    }    


    void separateMsm(String bodyMsm){

      co0 = s.separa(bodyMsm, '"', 0); 
      numMovil = s.separa(bodyMsm, '"', 1); 
      co2 = s.separa(bodyMsm, '"', 2);
      nameNumber = s.separa(bodyMsm, '"', 3);

      Serial.println();
      Serial.print("Posicion:");
      Serial.println(posStr);        
      Serial.print("Numero:");
      Serial.println(numMovil);        
      Serial.print("Nombre:");
      Serial.println(nameNumber);    

      delay(1000);
    }

    void sendMsnNumbers(){
      Serial.println("Enviando mensaje...");
      delay(1000);
      Serial.println("AT+CMGS=\""+countryCode+numMovil+"\"");    // Numero para enviar mensaje  
      delay(500);                                                                     
      Serial.println("Mensaje enviado por: "+nameNumber);
      delay(1000);                                                          
      Serial.write(0x1A); //ctrl+z   
      delay(3000); 
      Serial.println("Mensaje enviado.");       
    }


    bool enviarComandoAT(char* comandoAT, char* respEsperada, unsigned int retardo){

     uint8_t x=0;
     bool responde=false;
     char respuesta[100];
     unsigned long anterior;

     memset(respuesta, '\0', 100);    // Borra el contenido del array
     delay(100);
    
     while( Serial.available() > 0)
     Serial.read();                  // Limpie el buffer de entrada
    
     if (comandoAT[0] != '\0') {
       Serial.println(comandoAT);    // Enviar comando AT
//      Serial.println(comandoAT);   // Mostrar comando por serial
     }

     x = 0;
     anterior = millis();

     // Este bucle espera la respuesta
     do{
       if(Serial.available() != 0){    // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
          respuesta[x] = Serial.read();
//          Serial.print(respuesta[x]);
          x++;
        if (strstr(respuesta, respEsperada) != NULL) {   //Verificar si la respuesta deseada (OK) 
          responde = true;                               //está en la respuesta del módulo
         }
       }
     }while((!responde) && ((millis() - anterior) < retardo));    // Espera la respuesta con tiempo de espera

     return responde;
    }


    void sendAtConectionRed(){
          
      while(!answerAtConectionRed){
        
        //Descomentar si no conoce respuesta a este comando: AT+CREG?
//        answerAtConectionRed = enviarComandoAT("AT", "OK", 1000); 

         //Comentar si no conoce respuesta a este comando: AT+CREG?
         answerAtConectionRed = enviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || enviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000);
         errorAtConectionRed++;
         if(errorAtConectionRed>8){
           errorAtConectionRed=0;
           delay(2000);        
           Serial.println("Reiniciando Sim800l...");    
           digitalWrite(resetSim800l,LOW); 
           delay(500); 
           digitalWrite(resetSim800l,HIGH); 
           delay(15000);
         }   

         if(answerAtConectionRed){
           errorAtConectionRed=0;            
           enviarComandoAT("AT", "OK", 1000);
           enviarComandoAT("AT+CLIP=1", "OK", 1000); 
           enviarComandoAT("AT+CNMI=2,2,0,0,0", "OK", 1000); /* Transmisión de mensaje directamente al puerto serial del dispositivo. */
           enviarComandoAT("AT+CMGD=1,4", "OK", 1000);      
           enviarComandoAT("AT+CMGF=1", "OK", 2000);              
         }
      }      
      answerAtConectionRed=false;
    }    
