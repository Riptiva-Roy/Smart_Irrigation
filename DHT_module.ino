
//Code revisions: 
//temp and humid are now passed directly to the updateTS() function instead of storing them in separate variables.
// variables t and h eliminated (unnecessary)
//can use float for temp & humid instead of strings

#include <dht.h>
#include <SoftwareSerial.h>

dht DHT;
SoftwareSerial wifi (11, 12);

String ssid = "jvh85";
String password = "riyanandpari";
String api_key = "1M2EYQ75G8NHCY2J";

//Function to connect and set up the wifi data configuration

String sendATCommand(String command, const int timeout) {
  String response = "";
  wifi.print(command);
  long int time = millis();
  
  while ((time + timeout) > millis()) {
    while (wifi.available()) {
      char c = wifi.read();
      response += c;
    }
  }

  Serial.print(response);
  return response;
}

//function to connect directly to the API key//Thingspeak channel

void updateTS(float temp, float humid) {
  Serial.println("");
  sendATCommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 1000);
  delay(2000);

  String cmd = "GET /update?key=" + api_key + "&field1=" + String(temp) + "&field2=" + String(humid) + "\r\n";
  int cmdlen = cmd.length();
  sendATCommand("AT+CIPSEND=" + String(cmdlen) + "\r\n", 2000);
  wifi.print(cmd);
  Serial.println("");
  sendATCommand("AT+CIPCLOSE\r\n", 2000);
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  wifi.begin(115200);
  sendATCommand("AT+RST\r\n", 1000);
  sendATCommand("AT+CWMODE=1\r\n", 2000);
  sendATCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"\r\n", 2000);
  sendATCommand("AT+CIPMUX=1\r\n", 1000);
  sendATCommand("AT+CIPSERVER=1,80\r\n", 5000);
  sendATCommand("AT+CIFSR\r\n", 2000);
}

void loop() {
  int value = DHT.read(4);
  float temp = DHT.temperature;
  float humid = DHT.humidity;
  
  Serial.print("temperature=");
  Serial.print(temp);
  Serial.print("\thumidity=");
  Serial.println(humid);

  updateTS(temp, humid);
  delay(3000);
}

