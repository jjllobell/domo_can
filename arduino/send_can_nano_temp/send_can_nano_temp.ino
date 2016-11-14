// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>
#include "DHT.h" //cargamos la librería DHT

#define DHTPIN 4 //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22 //Se selecciona el DHT22(hay otros DHT)

DHT sen_dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

byte data[8] = {0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte altotemp;
byte bajotemp;
byte altohum;
byte bajohum;
int tint;
int hint;
unsigned long ultimEnvio = 0;
unsigned long delayEnvio = 20000;


MCP_CAN CAN0(10);     // Set CS to pin 10

void setup()
{
  Serial.begin(115200);
  sen_dht.begin();       //Se inicia el sensor temp y hum DHT22
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  // Ajustar adress modulo CAN
  data[0] = 0x10;
  data[1] = 0x01;
  
 
}

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

void loop() {
    float h = sen_dht.readHumidity(); //Se lee la humedad
    float t = sen_dht.readTemperature(); //Se lee la temperatura
  
      // Calculamos las ints de humedad y temp
    tint = int(t*100);
    hint = int(h*100);
  
    //Pasamos a descomponer las ints en bytes
    altotemp = highByte(tint);
    bajotemp = lowByte(tint);
    altohum = highByte(hint);
    bajohum = lowByte(hint);
    
    // Configuramos la trama con los bytes calculados
    data[7] = bajohum;
    data[6] = altohum;
    data[5] = bajotemp;
    data[4] = altotemp;

    

  if ((millis() > ultimEnvio + delayEnvio)){
      enviar_datos();
      for ( int x=0;x<8;x++){
          Serial.print(data[x],HEX);
          Serial.print("-");
        }
        Serial.println("");
      delay(100);
      ultimEnvio= millis();     
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
  
  
