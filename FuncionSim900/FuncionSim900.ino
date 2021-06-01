
    #define onSim900 9
    
    bool responde,o,p;
    String dato,result;

    void setup(){
      pinMode(onSim900,OUTPUT);
      Serial.begin(19200);  

      powerSim900();  // Encendido automático del módulo SIM900.
      Serial.println("Se ha iniciado el proceso!..."); 
      delay(2000);
      clearBuffer();
    }

    void loop(){

       while(!Serial.available()){}
       dato = Serial.readString();
//       Serial.println(dato);

       if(dato.length()>2){
        
          while(!p){ result = readAnswer(dato); p=true; }
          if(result.length()>0 && (result.lastIndexOf("+CUSD: 2") || result.lastIndexOf("+CMTI"))<=0){
            clearBuffer();
            o=false; p=false; dato="1";          
          }else{
            clearBuffer();
            o=false; p=false; dato="1"; responde=false; 
            enviarComandoAT("", "", 2000);  
          }
       }
    }

    void powerSim900(){   
       Serial.println("Se esta iniciando el modulo Sim900...");
       responde = false;                         
       delay(1000);                                     
       digitalWrite(onSim900,LOW);                      
       delay(1000);                                     
       digitalWrite(onSim900,HIGH);                      
       delay(3000);                                      
       digitalWrite(onSim900,LOW);
       delay(4000);
  
       while(!responde){
          /*Respuesta: "+CGREG: 0,1" solo para Operador movil Claro-Colombia.*/                            
          responde = enviarComandoAT("AT+CGREG?", "+CGREG: 0,1", 1000); 
          //responde = enviarComandoAT("AT", "OK", 1000);
       }                                                                            
    }                                                   

    String readAnswer(String at){
       String result,atAdj;
       if(at.length()>3){//*611#
         atAdj = at.substring(0,5);
       }else{
         atAdj = at.substring(0,1);//2
       }
        delay(1000);
        while(!o){
         Serial.println("AT+CUSD=1,\""+atAdj+"\",15");  
         clearBuffer();
         o = true;         
        }
    
        while(!Serial.available()){}
        result = Serial.readString();
        Serial.println(result);
      return result;    
    }

    void clearBuffer(){
      delay(1000);  
      while(Serial.available()>0)
      Serial.read();              // Limpie el buffer de entrada       
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
