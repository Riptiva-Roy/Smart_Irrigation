/*

This code is an Arduino program that uses an ESP8266 Wi-Fi module to connect to the internet
it send data to the ThingSpeak platform. 
The program reads data from a soil moisture sensor connected to an analog pin of the Arduino board, 
then maps the sensor value to a pump status value, and sends both values to ThingSpeak.

The ESP8266 module is connected to the Arduino board using a software serial interface on pins 3 and 4. 
The program sends AT commands to the ESP8266 module using the sendAT function and receives responses from the module. 
The program uses the AT commands to configure the ESP8266 module to connect to the Wi-Fi network, 
obtain an IP address, and establish a TCP connection to the ThingSpeak server.

The program also defines a function called updateTS that takes two arguments, 
the soil moisture sensor value and the pump status value, 
and sends them to ThingSpeak using an HTTP GET request. 
The function constructs the GET request URL using the ThingSpeak API key, 
the field numbers for the sensor and pump status values, and the values themselves.

The program then waits for 15 seconds before looping back to read the soil moisture sensor value again
and sending it to ThingSpeak.

*/
#include <SoftwareSerial.h>
int srdata;
int prdata;
int pump_status;
String temp;
String humid;
SoftwareSerial  esp8266(11, 12);
String ssid = "jvh85";
String password = "riyanandpari";

String sendAT(String command, const int timeout)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }

  Serial.print(response);

  return response;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
  sendAT("AT+RST\r\n", 2000);
  sendAT("AT+CWMODE=1\r\n", 1000);
  sendAT("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"\r\n", 2000);
  while (!esp8266.find("OK"))
  {

  }

  sendAT("AT+CIFSR\r\n", 1000);//You will get the IP Address of the ESP8266 from thissendAT.
  sendAT("AT+CIPMUX=0\r\n", 1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  srdata=analogRead(A0);
  prdata=map(srdata,0,1023,100,0);//low vale in soil sensor means high
                                    //max water present so 100
Serial.print("sensor data:");
  Serial.println(prdata);
  String sensor_value= String(prdata);
  if(prdata<50)      // low value means max water and pump should be off and its status is max=100 as per mapping
 
  {
    digitalWrite(8,LOW);
    pump_status=100;  
   
    }
   else
  {
    digitalWrite(8,HIGH);   //soil dry,pump on
    pump_status=0;  
   
    }
    String pump= String(pump_status);
    updateTS(sensor_value, pump);
  delay(2000);
}
void updateTS(String T, String P)
{
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=1T73D6HH0E1WL506&field1=" + T + "&field2=" + P + "\r\n";
  cmdlen = cmd.length();     //Used by portal to accept a data packet of that length and close connection after that
  sendAT("AT+CIPSEND=" + cmdlen + "\r\n", 2000); // length should be less than 2024 bytes to avoid error data loss
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n", 2000);
  Serial.println("");
  delay(15000);
}
