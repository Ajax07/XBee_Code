/* .................................................................
 *  This code is written as a part of IIT Mandi IoT testbed project.  I/We thank all the authors/organizations for providing libraries for different modules. 
*****************************************************************************
* last update: July 7, 2018 by Ajay Singh Chouhan (ajayc79134@gmail.com)
*/
#include <XBee.h>
//#include <SoftwareSerial.h>


uint8_t ssRX = 0;
uint8_t ssTX = 1;
int tmpval=0;
XBee xbee = XBee();


uint8_t idCmd[] = {'I','D'};
//uint8_t idValue[] = {0x01, 0x10}; // some made up value for testing purposes
uint8_t idValue[] = {0x02}; // some made up value for testing purposes

uint8_t niCmd[] = {'N','I'};
uint8_t niValue[] = {"END"}; // some made up value for testing purposes

uint8_t scCmd[] = {'S','C'};
uint8_t scValue[] = {0xFF};

uint8_t plCmd[] = {'P','L'};
uint8_t plValue[] = {0x1};

uint8_t idCmd2[] = {'W','R'}; // write to EEPROM


AtCommandRequest request = AtCommandRequest(idCmd, idValue, sizeof(idValue));
AtCommandResponse response = AtCommandResponse();



void setup() { 
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  sendAtCommand();
  request.clearCommandValue();
  request.setCommand(niCmd);
  request.setCommandValue(niValue);
  sendAtCommand();
  request.setCommandValueLength(sizeof(niValue));
  request.setCommand(scCmd);
  request.setCommandValue(scValue);
  request.setCommandValueLength(sizeof(scValue));
  sendAtCommand();
  request.setCommand(plCmd);
  request.setCommandValue(plValue);
  request.setCommandValueLength(sizeof(plValue));
  sendAtCommand();
  request.setCommand(idCmd2);
  sendAtCommand();
  request.clearCommandValue();
  Serial.println("Done---Going into an infinite loop...");
  Serial.print(tmpval);
  Serial.println("---");
  while (1) {};
}


void sendAtCommand() {
  xbee.send(request);
  if (xbee.readPacket(5000)) {
    tmpval+=1;
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(response);
      tmpval+=3;

      if (response.isOk()) {
        Serial.print("Command [");
        Serial.print(response.getCommand()[0]);
        Serial.print(response.getCommand()[1]);
        Serial.println("] was successful!");
        tmpval+=5;
        if (response.getValueLength() > 0) {
          Serial.print("Command value length is ");
          Serial.println(response.getValueLength(), DEC);
          Serial.print("Command value: ");
          tmpval+=7;
          for (int i = 0; i < response.getValueLength(); i++) {
            Serial.print(response.getValue()[i], HEX);
            Serial.print(" ");
          }
          Serial.println("");
        }
      } else {
        Serial.print("Command returned error code: ");
        Serial.println(response.getStatus(), HEX);
        tmpval+=11;
      }
    } else {
      Serial.print("Expected AT COMMAND response but got ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
      tmpval+=13;
    }    
  } else if (xbee.getResponse().isError()) {
    Serial.print("Error reading packet.  Error code: ");  
    Serial.println(xbee.getResponse().getErrorCode());
    tmpval+=17;
  } else {
    Serial.print("No response from radio");  
    tmpval+=19;
  }
}
