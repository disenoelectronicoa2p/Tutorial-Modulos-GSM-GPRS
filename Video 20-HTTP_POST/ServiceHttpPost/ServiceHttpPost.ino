
#include <DHT11.h>

#define dhtPin 2       //Pin 2: Sensor DHT11
#define resetSim800l 8 //Pin 8: Resetear modulo Sim800l

DHT11 dht11(dhtPin);

bool validateConfig;

unsigned long measurementTime = 0; //Contador para envio mensaje
unsigned long tiempoPrevioMSN;     //Contador para control envio mensaje

float temp,hum,currentTemp,currentHum;   
int err;
String data;

void setup() {
  Serial.begin(115200);
  pinMode(resetSim800l, OUTPUT);
  digitalWrite(resetSim800l, HIGH);

  measurementTime = 20000; // 20 segundos

  /**
  **Valida si el sim800l se concecto correctamente a la red movil
  **Descomentar la siguiente linea de codigo "sendAtConectionRed()" si se requiere validar esta accion.
  **/
  //sendAtConectionRed(); 

  //Cierra la sesión GPRS si esta activa.
  enviarComandoAT("AT+SAPBR=0,1", "OK", 3000);  
  
  // Configura el tipo de conexión a GPRS
  enviarComandoAT("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 3000); 
  
  // Configura la APN para la conexión GPRS
  enviarComandoAT("AT+SAPBR=3,1,\"APN\",\"internet.comcel.com.co\"", "OK", 3000); 

  Serial.println("Start!...");
}

void loop() {

  while (!validateConfig) {
    bool answer = false;
    answer = enviarComandoAT("AT+SAPBR=1,1", "OK", 2000); //Activa la conexión GPRS
    if (answer) {
      answer = enviarComandoAT("AT+HTTPINIT", "OK", 2000); //Inicia el servicio HTTP en el SIM800L
      if (answer) {
        answer = enviarComandoAT("AT+HTTPPARA=\"CID\",1", "OK", 4000); //Configura el Context Identifier (CID) para la conexión HTTP
        if (answer) {
          enviarComandoAT("AT+HTTPPARA=\"URL\",\"http://prueba-sim800l-post.atwebpages.com/test-hum-temp.php\"", "OK", 3000);
          enviarComandoAT("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"", "OK", 3000);
          validateConfig = true;
        }
      } else {
        enviarComandoAT("AT+HTTPTERM", "OK", 2000);
      }
    } else {
      enviarComandoAT("AT+SAPBR=0,1", "OK", 2000);
    }
  }

  unsigned long tiempoActualMSN = millis(); //Inicio contador
  if ((unsigned long)(tiempoActualMSN - tiempoPrevioMSN) >= measurementTime) {
    
    if((err=dht11.read(hum, temp))==0){
      currentHum = hum,2;
      currentTemp = temp,2;
      data = "data="+String(currentTemp)+";"+String(currentHum);
    }else{
      Serial.println();
      Serial.print("Error Num :");
      Serial.print(err);
      Serial.println();
    }

    enviarComandoAT("AT+HTTPDATA=192,5000", "", 2000);
    enviarComandoAT("", "", 4000);
    Serial.println(data);
    enviarComandoAT("", "", 2000);

    bool answerOk = false;
    int countAnswerOk = 0;
    while (!answerOk) {
      answerOk = enviarComandoAT("AT+HTTPACTION=1", "OK", 2000); //Se realizará una solicitud HTTP POST
      if (answerOk) {
        currentHum = 0; currentTemp = 0;
        Serial.println("\nDatos guardados.");
        enviarComandoAT("", "", 3000);
      }

      countAnswerOk++;
      if (countAnswerOk > 5) {
        countAnswerOk = 0; answerOk = true;
        validateConfig = false;
        Serial.println("Error guardando datos.");
        enviarComandoAT("AT+HTTPTERM", "OK", 2000);  //Cierra la solicitud HTTP
        enviarComandoAT("AT+SAPBR=0,1", "OK", 2000); //Cierra la sesión GPRS si esta activa.
        currentHum = 0; currentTemp = 0;
      }
    }

    tiempoPrevioMSN = tiempoActualMSN;
  }

}

//================================
//CONFIG SIM800L
void sendAtConectionRed() {

  bool answerAtConectionRed = false;
  int errorAtConectionRed = 0;
  while ( !answerAtConectionRed ) {
    //La respuesta al comando AT+CREG? puede variar, segun el operador movil.
    answerAtConectionRed = enviarComandoAT("AT+CREG?", "+CREG: 0,1", 1000) || enviarComandoAT("AT+CREG?", "+CREG: 0,5", 1000);
    errorAtConectionRed++;
    if (errorAtConectionRed > 8) {
      errorAtConectionRed = 0;
      delay(2000);
      Serial.println("Reiniciando Sim800l...");
      digitalWrite(resetSim800l, LOW);
      delay(500);
      digitalWrite(resetSim800l, HIGH);
      delay(15000);
    }

    if (answerAtConectionRed) {
      errorAtConectionRed = 0;
      enviarComandoAT("AT", "OK", 1000);
    }
  }
}

bool enviarComandoAT(char* comandoAT, char* respEsperada, unsigned int retardo) {

  uint8_t x = 0;
  bool responde = false;
  char respuesta[100];
  unsigned long anterior;

  memset(respuesta, '\0', 100);    // Borra el contenido del array
  delay(100);

  while ( Serial.available() > 0)
    Serial.read();                  // Limpie el buffer de entrada

  if (comandoAT[0] != '\0') {
    Serial.println(comandoAT);    // Enviar comando AT
    //      Serial.println(comandoAT);   // Mostrar comando por serial
  }

  x = 0;
  anterior = millis();

  // Este bucle espera la respuesta
  do {
    if (Serial.available() != 0) {  // Si hay datos en el búfer de entrada de UART, léalo y verifica la respuesta
      respuesta[x] = Serial.read();
      //Serial.print(respuesta[x]);
      x++;
      if (strstr(respuesta, respEsperada) != NULL) {   //Verificar si la respuesta deseada (OK)
        responde = true;                               //está en la respuesta del módulo
      }
    }
  } while ((!responde) && ((millis() - anterior) < retardo));   // Espera la respuesta con tiempo de espera

  return responde;
}
