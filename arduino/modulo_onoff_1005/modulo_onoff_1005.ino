// MODULO CAN 2IN-2OUT ON/OFF
//

#include <mcp_can.h>
#include <SPI.h>
long unsigned int rxId; // Variables para recepcion de mensaje CAN
unsigned char len = 0;
unsigned char rxBuf[8];

byte adress1 = 0x10; // para ajustar direccion de modulo
byte adress2 = 0x05;

byte led1 = 5; // declaracion de entradas y salidas
byte led2 = 6;
byte pul1 = 7;
byte pul2 = 8;

boolean transmitir = false; // Para saber cuando hay datos para transmitir
unsigned long  lastpress = 0;
unsigned long delaypress = 400;

byte data[8] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define CAN0_INT 2    // Set INT to pin2 
MCP_CAN CAN0(10);     // Set CS to pin 10

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 8MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
   pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
    // Ajustar adress modulo CAN
  data[0] = adress1;
  data[1] = adress2;

  // Configurando entradas y salidas
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(pul1, INPUT);
  pinMode(pul2, INPUT);
  
}

void loop() {
 // Leer pulsadores y actuar si han cambiado de estado 
  if (digitalRead(pul1) == LOW) {
      encender_apagar (led1,7);
    }    

  if (digitalRead(pul2) == LOW) {
      encender_apagar (led2,6);
    }

// Leer estado de recepcion del buffer CAN
  if(!digitalRead(2)) {                   // If pin 2 is low, read receive buffer
        CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)
        if (rxBuf[0]== adress1 && rxBuf[1]== adress2){ // si el mensaje es para este modulo
          // hacemos equivalentes los bytes de recepcion y transmision
          data[6] = rxBuf[6];  
          data[7] = rxBuf[7];
          // Leer bytes 6 y 7 y actuar segun valor
           if (rxBuf[7] == 0 ){  
              digitalWrite(led1, LOW);
              transmitir = true;
            }
           else {
              digitalWrite(led1, HIGH);
              transmitir = true;
            }
            if (rxBuf[6] == 0 ){
              digitalWrite(led2, LOW);
              transmitir = true;
            }
           else {
              digitalWrite(led2, HIGH);
              transmitir = true;
            }
          }
        
  }
  if (transmitir) {enviar_datos(); transmitir = false; }  // enviamos datas si esta activo transmitir y desactivamos

}

// Funcion para transmitir datos al BUS
void enviar_datos(){
    // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } 
  else {
    Serial.println("Error Sending Message...");
  }
  delay(1);   // send data per 5ms
  }

// Funcion para encender/apagar y odenar transmision
void encender_apagar (int led, int i){
  if (millis()>lastpress+delaypress){
           if (data[i] == 0) {
              data[i] = 0xff;
              digitalWrite(led, HIGH);
              lastpress = millis();
              transmitir = true;
           }
           else {
            data[i] = 0x00;
            digitalWrite(led, LOW);
            lastpress = millis();
            transmitir = true;
              }     
      }
  }
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
  
  
