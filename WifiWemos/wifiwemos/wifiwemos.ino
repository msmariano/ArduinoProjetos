#include <ESP8266WiFi.h>

 
//Projeto : IOT para Controle de Automação Portão Carro	
//Inicio : 14/06/2017
 

const char* ssid = "Escritorio";
const char* password = "80818283";
 
int outAct = D5;
int warningLed = D4;
int warningState = 0;
WiFiServer server(81);
String mens = "";
 
void setup() {
	
  mens.reserve(200);
  Serial.begin(115200);
  delay(10); 
 
  pinMode(outAct, OUTPUT);
  pinMode(warningLed, OUTPUT);  
  
  digitalWrite(outAct, LOW);
  digitalWrite(warningLed, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
     warningChange(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
   digitalWrite(warningLed, HIGH);
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(outAct, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(outAct, LOW);
    value = LOW;
  }
 
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}


void AcionarPortao(){
   digitalWrite(outAct, HIGH);
   delay(1000);
   digitalWrite(outAct, LOW);
}

void warning(int pulses)
{  
  for (int i=0;i<pulses;i++)
  {
   digitalWrite(warningLed, HIGH);
   delay(50);
   digitalWrite(warningLed, LOW);
   delay(50);
  }  
}

void warningChange(int tempo){
	delay(tempo);
 if(warningState) {	 	
 	   digitalWrite(warningLed, LOW);
 	   warningState = 0;
 	}
 	else {
 			digitalWrite(warningLed, HIGH);
 			warningState = 1;
 	}
 }
