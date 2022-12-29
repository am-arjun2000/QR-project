#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

const int OLED_ADDRESS = 0x3C; // I2C address of the OLED display
const int OLED_WIDTH = 128; // width of the OLED display in pixels
const int OLED_HEIGHT = 64; // height of the OLED display in pixels

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
QRCode qrcode;

void setup() {

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000); // Pause for 1 seconds
  
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Generate QR code data
  const char *data = "upi://pay?pn=arjun&pa=arjunb22@fbl&cu=INR&mode=01&am=340&orgid=000000&purpose=00";
  uint8_t qrcodeData[qrcode_getBufferSize(5)];

qrcode_initText(&qrcode, qrcodeData, 4, 0, data);
Serial.print("QR code size: ");
Serial.println(qrcode.size);


  // Display QR code on OLED
  display.clearDisplay();
  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.drawPixel(x, y, WHITE);
      } else {
        display.drawPixel(x, y, BLACK);
      }
    }
  }
  display.display();

  // Wait 5 seconds before generating and displaying a new QR code
  delay(5000);
}
