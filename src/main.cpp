/* 
Quelle https://arduino-projekte.info/blogs/projekte/zisterne-fuellstandsanzeige?srsltid=AfmBOooX92uzrs9_x-6LpC01pBRju-faEiXiYulyaeVUsV0zWtvoDlNZ
Ultraschall Sensor: HC-SR04
*/

#include <ESP8266WiFi.h>
  
const char* ssid = "GLT1";
const char* password = "glt123456";
  
WiFiServer server(80);

/* Kabelbelegung
vcc     rot
trig    gelb D6
echo    weiss D7
gnd     schwarz
*/
  
// PINverwaltung
int trigger = D6;
int echo = D7;

// Variablen
int dauer = 0;
int distanz = 0;
int liter_zisterne = 0;
int liter_luft =0;
int liter_fuellstand =0;

// Variablen Zisterne
int zisterne_r = 100;
int zisterne_h = 220;

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
Serial.print("Distanz Schall------------------------ > in cm : "); Serial.println(distanz);

//Delay 50ms before next reading.
delay(300);

  String cmd;
  cmd += "";
  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head><meta charset='UTF-8'>");
  client.println("<title>Zisterne</title>");
  client.println("<meta http-equiv='refresh' content='5'>");
  client.println("</head><body>");
  client.println("<H2>Wasserstand Zisterne</H2>");
  client.println("<h3>");
  
    // Volumen Zisterne
    liter_zisterne = ((zisterne_r * zisterne_r) * zisterne_h * 3.1415926535)/1000;
    Serial.print("Liter Zisterne: "); Serial.println(liter_zisterne);
    liter_luft = ((zisterne_r * zisterne_r) * distanz * 3.1415926535)/1000;
    Serial.print("Liter Luft: "); Serial.println(liter_luft);
    liter_fuellstand = liter_zisterne - liter_luft;
    Serial.print("Liter Füllstand: "); Serial.println(liter_fuellstand);
    //client.print(distanz);
    Serial.println("");
    Serial.println("");
    client.print("<p> Füllstand ");
    client.print(liter_fuellstand);
    client.print(" Liter </p>");

    /*
// ################################################### 1
if (liter_fuellstand <= 200)
{
  client.print("<p> Füllstand unter 200 Liter &#x1F534;</p>");
}

// ################################################### 2
if (liter_fuellstand >= 201 && liter_fuellstand <= 2000)
{
  client.print("<p> Füllstand unter 2000 Liter</p>");
}

// ################################################### 2
if (liter_fuellstand >= 2001 && liter_fuellstand <= 4000)
{
  client.print("<p> Füllstand unter 4000 Liter</p>");
}

// ################################################### 2
if (liter_fuellstand >= 4001 && liter_fuellstand <= 7500)
{
  client.print("<p> Füllstand unter 7500 Liter</p>");
}

// ################################################### 2
if (liter_fuellstand >= 7501 && liter_fuellstand <= 8500)
{
  client.print("<p> Füllstand unter 8500 Liter</p>");
}

// ################################################### 2
if (liter_fuellstand >= 8501 && liter_fuellstand <= 9600)
{
  client.print("<p> Füllstand VOLL</p>");
}
*/
client.println("</h3> ");
client.println("<p>Seite aktualisiert sich alle 5 Sekunden!</p>");
client.println("<p>IP: ");client.println(WiFi.localIP());client.println("</p>");
client.println("<p>Kennung Sensor - MAC 44:17:93:0d:73:de - Hostname ESP-0D73DE</p>");
client.println("<p>coder 017620288404 - code@guggbyte.com</p>");

client.print("</body></html>");
delay(600);
}