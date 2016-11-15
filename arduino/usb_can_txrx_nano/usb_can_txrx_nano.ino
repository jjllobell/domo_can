// USB Gateway CAN-PC
// Use MCP2515 - TJA1050 - NiRen Module
//

#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
SoftwareSerial usb_pc(4, 3);                // RX, TX


void setup()
{
  Serial.begin(19200);
  usb_pc.begin(19200);
  CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ);
  delay(1);
  
  // Initialize MCP2515 running at 8MHz with a baudrate of 250kb/s and the masks and filters disabled, 
  // special config. to MCP2515 - TJA1050 - NiRen Module
 // if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK){
 if (CAN_OK) {
    Serial.println("MCP2515 - TJA1050 Initialized Successfully!");
    usb_pc.println("MCP2515 - TJA1050 Initialized Successfully!");
  }
  else {
    Serial.println("Error Initializing MCP2515...");
    usb_pc.println("Error Initializing MCP2515...");
  }
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515-TJA1050 - Ready to receive!!");
  usb_pc.println("MCP2515-TJA1050 - Ready to receive!!");

}

void loop() {
  
   if(!digitalRead(2))  {                  // If pin 2 is low, read receive buffer
      CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)
      for(int i = 0; i<len; i++)           // Print on serial port each byte of the data
      {
        if(rxBuf[i] < 0x10)                // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
          usb_pc.print("0");
        }
        Serial.print(rxBuf[i], HEX);        // Print data of index i
        usb_pc.print(rxBuf[i], HEX);
        Serial.print(" ");
        usb_pc.print(" ");
      }
      Serial.println();
      usb_pc.println();
    }

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
