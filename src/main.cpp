// Quelle https://arduino-projekte.info/blogs/projekte/zisterne-fuellstandsanzeige?srsltid=AfmBOooX92uzrs9_x-6LpC01pBRju-faEiXiYulyaeVUsV0zWtvoDlNZ
#include <ESP8266WiFi.h>
  
const char* ssid = "GLT1";
const char* password = "glt123456";
  
WiFiServer server(80);
  
// PINverwaltung
int trigger = D6;
int echo = D7;

// Variablen
long dauer = 0;
long distanz = 0;

// ###################################################### SETUP
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
}

// ###################################################### LOOP
void loop() {
digitalWrite(trigger, LOW);
delayMicroseconds(2);
digitalWrite(trigger, HIGH);
delayMicroseconds(10);
digitalWrite(trigger, LOW);

dauer = pulseIn(echo, HIGH);
//Calculate the distance (in cm) based on the speed of sound.
distanz = dauer/58.2;
//Serial.println(distanz);

//Delay 50ms before next reading.
delay(50);



  String cmd;
  cmd += "";
  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head><meta charset='UTF-8'>");
  client.println("<title>Wasserstand Regenwasserzisterne</title>");
  client.println("</head><body>");
  client.println("<H2>Wasserstand Regenwasserzisterne</H2>");
  client.println("<h3>");

  if (entfernung >= 200 || entfernung <= 0)
  {
    client.println("Kein Messwerte vorhanden!");
  }
  else
  {
    entfernung = 192 - entfernung;
    entfernung = entfernung * 84.7457;
    client.print(entfernung);
    client.println(" Liter");
  }
// ################################################### 1
if (entfernung >= 0 && entfernung <= 1833)
{
  client.print("<p> Füllstand  &#x1F534;</p>");
}

// ################################################### 2
if (entfernung >= 1833 && entfernung <= 3666)
{
  client.print("<p> Füllstand 2</p>");
}

// ################################################### 3
if (entfernung >= 3666 && entfernung <= 5500)
{
  client.print("<p> Füllstand 3</p>");
}
// ################################################### 4  
if (entfernung >= 5500 && entfernung <= 7332)
{
  client.print("<p> Füllstand 4</p>");
}
  
// ################################################### 5
if (entfernung >= 7332 && entfernung <= 9165)
{
  client.print("<p> Füllstand 5</p>");
}

// ################################################### 6
if (entfernung >= 9165 && entfernung <= 16000)
{
    client.print("<p> Füllstand 6</p>");
}

client.println("</h3> ");
  client.print("</body></html>");
  delay(500);
}