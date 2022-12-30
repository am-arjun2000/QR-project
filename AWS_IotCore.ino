#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
//#include <secrets.h>
#include "DHT.h"
//-----------------------------------------------

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//-----------------------------------------------
#include <pgmspace.h>
 
//#define SECRET
char text;
 
const char WIFI_SSID[] = "arjun";               //TAMIM2.4G
const char WIFI_PASSWORD[] = "qwertyuiop";           //0544287380
 
#define THINGNAME "ESP8266"
 
int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
 
const char MQTT_HOST[] = "a2z7ny7f5lo476-ats.iot.us-east-1.amazonaws.com";
 
 
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAOYET//c1Qt7/z6zNZtM69OWUoSHMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMjEyMTcwODUz
NTdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDFdMzg5BjVPqHBP3ad
Hlu5thneIZVrxbfSPgDOlSERkx1SmXXpsY1U2Er6pe5O6m5pet2wFD4G+//O4nKS
PHmFA50MzM9VIoey7Ei9D98D6Deac9r2UwDS7CWNU05zwtmbW7tW7JHTlNKIqso2
GZ8PLRCJTeHDYV8/JbXjimM5rWjLaljEH74SITLtNlnT7zLUN5i5528RF8TumCOZ
v3wKmk3wend1IoYFGCkSIsLdV9RxSbV+PjnEhT0EbRvb2hwCBso+5Cgm2sNR+gEs
283KdYvNiF36JCKKke45poFOtod5CnWnrAgVBU7nf7dx5+SDa8xD1pB3m4SRojxs
EAg9AgMBAAGjYDBeMB8GA1UdIwQYMBaAFHPksZnRHDj1a+M+yAuvgskO8uJLMB0G
A1UdDgQWBBRNgPibT33zxvlE3lUf7P6iCnzKmDAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAxFtHsRD6p8QVGn+W+ggyAdwW
7pm1PmKcsnqRMmbNajyd4VlN4hyLKQxVzbpBtJFpMAdG8pNo4i0ciM1SD2UF1EzF
ZRAhW69ZUokWwYQhzGqc4lmy6vLfrS8ZfZ3xrskfTb0/TNCmDE+fI4AOG8CDK4Dv
eyc7GOO51PZ3YXq64C1adJusMxW54ERh12okdsGSuqTv8JmZwskJaGZlDF1TyIGB
8YPQR1II98sEMHhp97rI9eYiOWNjpPzxD+DX7BC4XWY9O0J4lo0yErbSD7NC03PT
gcie85cDmOZlOHTDQ3Rl+2nJlAsmX5q7VwoHaVnZUhI07ENUhGOfOnpiaqcBIQ==
-----END CERTIFICATE-----
 
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAxXTM4OQY1T6hwT92nR5bubYZ3iGVa8W30j4AzpUhEZMdUpl1
6bGNVNhK+qXuTupuaXrdsBQ+Bvv/zuJykjx5hQOdDMzPVSKHsuxIvQ/fA+g3mnPa
9lMA0uwljVNOc8LZm1u7VuyR05TSiKrKNhmfDy0QiU3hw2FfPyW144pjOa1oy2pY
xB++EiEy7TZZ0+8y1DeYuedvERfE7pgjmb98CppN8Hp3dSKGBRgpEiLC3VfUcUm1
fj45xIU9BG0b29ocAgbKPuQoJtrDUfoBLNvNynWLzYhd+iQiipHuOaaBTraHeQp1
p6wIFQVO53+3cefkg2vMQ9aQd5uEkaI8bBAIPQIDAQABAoIBAAEEszHNu1sOMaNf
gmsZBvwUNlnw7mZZ5LLO4w4tsrf2fEY6yGLmKYZwfyJ/zJTXnK4qlg4cbJ14mz8j
59ddPHs1I5A3VjJoNS71DQL8m/fXMMI5Kr3ue6ia0y7BaTwrmxe0B2lRWwcuU0+l
IdnZk/puDxHGNw3c6LLYrPF5ez2G5xgkFoRlX4qG8he0hBY6GehMru1IVA1cNu7M
Ue1VfCeYtPWeFyQ1P/Urtm1fzQiR+iMs8qIBDlCHKrC5eNPlr/yA5LwAmWriqdV/
tsrDgUlcXEhDeCftWhkTf/hNeFvE6/I6I0k5CObeZ51bl+g5QVXJMuZhVhKr/2jz
7S+9KXkCgYEA+xXGmapeOKoqfw+nHzQyF2CJDOMLj/uD4UycUoLLZ87JlxKT2Det
KvOAkNARH4SSJ6scFPP+T3AjuUHTxh866x8Eh/ZA7PtN7NeBI7fz8SehpImTHHA1
wWnI33QIkI/ZOci5HHV2Y4QESwCj3QJJSe8nKbRDIEuJFexuvL5jp4sCgYEAyVJI
XEXEmSyEn64j1hXACPiJQye0/Z20/iHKy0rm1U7a5ZRlSi+HaGIgOwhVfrXshggB
193MjLpDz5UIQQHpgQayjVhKTDkfOlTx1AGOc3Wmut0j5Zfv4AviBFFm6DufaZmI
P+3NBxncS0bj++ZoZ81tA9OQELvsgBpXv42ZSFcCgYEAx1o4vtT3zkG9T/jblfjZ
NJpe9i5mhcEKqp0rGI/kc91JTXtgizFZelqvX0rz7XRSntBKkZBQAD5m/rX3MLyn
CPstm823KvE0o7FPRU7OjlALFRpuBQxKgfA9nrz/99def3SRFLlmcAjnZ61wbQ9d
pSgOjhwjl8ahZxi7q3v2n5UCgYBYJPSVod5gCkPO/hmmpEM+KkM+6eWunRwfoH//
3j2O60Wf+wcWIhUxOLVP42g7YD+8t0jHUM88VllWS7QfckzgNyR6CkwNb30itwpi
foP2PW7Qpf4PiiR2ff1pUpKx31j+jUBAbUe1cjcnplQveEQCHsk6Qaj0CrOTqvT8
Y2zXZQKBgDtH2bnUt/Y2t0vUHk9IgKseg0pNPHrm8ojQr/fWLTF6YPJraUu79JPj
cYlvGxzUi4F4uXcZELiuRHIsDMeBkokSRILOr0/OzhdK6buQhea6V/ldTxSl0s6U
h9c4J5Xn3oSAnvzJrrxijEy5Ai9RfyD7cJkfBxchGrsSWcnn+cgt
-----END RSA PRIVATE KEY-----
 
)KEY";



//------------------------------------------ 
#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);
 
float h ;
float t;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient client(net);
 
time_t now;
time_t nowish = 1510592825;
 
 
void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  // Display the payload on the OLED display
  display.clearDisplay();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Convert the payload to a string
  String payloadString = "";
  for (int i = 0; i < length; i++)
  {
    payloadString += (char)payload[i];
  }

  // Print the payload to the serial port and OLED display
  Serial.println(payloadString);
  display.println(payloadString);
  display.display();

  display.clearDisplay();
}

 
 
void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
 
  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
 
void setup()
{
  Serial.begin(115200);
  connectAWS();
  dht.begin();

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds
  
}
 
 
void loop()
{

//Serial.println(text);
delay(1000);
  
//---------------------------------------
  now = time(nullptr);
 
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}
