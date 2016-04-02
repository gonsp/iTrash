#include "ServerComm.h"
#include "Arduino.h"

void copyStr(char* from, char* to) {
  int i = 0;
  while (from[i] != '\0'){
      to[i] = from[i];
      ++i;
  }
  to[i] = '\0';
}

ServerComm::ServerComm(char* server, char* page) {
  copyStr(server, serverName);
  copyStr(page, pageName);
  Ethernet.begin(MAC);
  delay(1000);
}

bool ServerComm::post(char* postData) {
  return postPage(serverName, serverPort, pageName, postData);
}

void ServerComm::close() {
  Serial.println("This does nothing, actually");
}

//http://playground.arduino.cc/Code/WebClient
byte ServerComm::postPage(char* domainBuffer, int thisPort, char* page, char* thisData)
{
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  }
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}