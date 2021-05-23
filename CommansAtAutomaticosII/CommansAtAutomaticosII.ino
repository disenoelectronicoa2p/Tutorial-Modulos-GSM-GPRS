
    bool respuesta;

    void setup() {
      Serial.begin(115200);       
      enviarComandoAT("AT+CLIP=1", "OK", 1000);
      enviarComandoAT("AT+CNMI=2,2,0,0,0", "OK", 1000);
      Serial.println("Start!...");      
    }

    void loop() { 

       // LLamadas
/*//       respuesta = enviarComandoAT("","RING",1000);
       respuesta = enviarComandoAT("","+CLIP",1000);
       if(respuesta){
        Serial.println("Se ha detectado una llamada");
        enviarComandoAT("", "", 2000);
        enviarComandoAT("ATH", "OK", 1000);
       } */

       respuesta = enviarComandoAT("","Prueba2",1000);
       if(respuesta){
        Serial.println("Se ha detectado un mensaje");
        Serial.println("AT+CMGS=\"+57313XXXXXXX\"");/*<<<--!Colocar número al cual vas a enviar el mensaje!!!*/
        delay(500);
        Serial.println("Usted ha enviando un mensaje.");
        delay(1000);
        Serial.write(0x1A); //ctrl+z
        delay(2000);
       }      
 
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
