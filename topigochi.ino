/*
 * Topigochi - Arduino UNO R4
 * Control de pantalla OLED 0.96" con 3 botones
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración del display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pines de los botones
#define BOTON_1 2
#define BOTON_2 3
#define BOTON_3 4

// Pin del buzzer
#define BUZZER 8

// Variables para debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  delay(1000); // Esperar a que el serial esté listo
  Serial.println(F("=== Topigochi Iniciando ==="));
  
  // Configurar pines de botones como entrada con pull-up
  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  pinMode(BOTON_3, INPUT_PULLUP);
  
  // Configurar pin del buzzer como salida
  pinMode(BUZZER, OUTPUT);
  
  // Escanear dispositivos I2C
  Serial.println(F("Escaneando dispositivos I2C..."));
  Wire.begin();
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print(F("Dispositivo I2C encontrado en 0x"));
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println(F("No se encontraron dispositivos I2C!"));
    Serial.println(F("Verifica las conexiones SDA y SCL"));
  } else {
    Serial.print(F("Total dispositivos encontrados: "));
    Serial.println(nDevices);
  }
  
  // Intentar inicializar display OLED con dirección 0x3C
  Serial.println(F("Intentando inicializar display en 0x3C..."));
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo con 0x3C, intentando 0x3D..."));
    
    // Intentar con dirección alternativa 0x3D
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
      Serial.println(F("ERROR: No se pudo inicializar el display OLED"));
      Serial.println(F("Verifica:"));
      Serial.println(F("1. Conexiones: SDA->A4, SCL->A5, VCC->5V, GND->GND"));
      Serial.println(F("2. Display encendido"));
      Serial.println(F("3. Librerias Adafruit instaladas"));
      
      // Parpadear LED integrado para indicar error
      pinMode(LED_BUILTIN, OUTPUT);
      while(1) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
      }
    } else {
      Serial.println(F("Display inicializado exitosamente en 0x3D!"));
    }
  } else {
    Serial.println(F("Display inicializado exitosamente en 0x3C!"));
  }
  
  // Limpiar el buffer
  display.clearDisplay();
  
  // Mostrar mensaje de bienvenida
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Topigochi"));
  display.println(F("Iniciando..."));
  display.display();
  Serial.println(F("Mensaje de bienvenida mostrado"));
  delay(2000);
  
  // Limpiar pantalla
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Presiona un boton"));
  display.display();
  Serial.println(F("Sistema listo!"));
}

void loop() {
  // Leer estado de los botones (LOW cuando están presionados por el pull-up)
  if (digitalRead(BOTON_1) == LOW) {
    delay(debounceDelay); // Anti-rebote
    if (digitalRead(BOTON_1) == LOW) {
      mostrarMensaje("Boton 1");
      while(digitalRead(BOTON_1) == LOW); // Esperar a que se suelte
    }
  }
  
  if (digitalRead(BOTON_2) == LOW) {
    delay(debounceDelay);
    if (digitalRead(BOTON_2) == LOW) {
      mostrarMensaje("Boton 2");
      while(digitalRead(BOTON_2) == LOW);
    }
  }
  
  if (digitalRead(BOTON_3) == LOW) {
    delay(debounceDelay);
    if (digitalRead(BOTON_3) == LOW) {
      mostrarMensaje("Boton 3");
      while(digitalRead(BOTON_3) == LOW);
    }
  }
}

void mostrarMensaje(const char* mensaje) {
  // Limpiar display
  display.clearDisplay();
  
  // Configurar texto
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  
  // Mostrar mensaje
  display.println(mensaje);
  display.display();
  
  // Feedback de audio (opcional)
  tone(BUZZER, 1000, 100); // Tono de 1000Hz por 100ms
  
  // Mostrar en serial para debug
  Serial.println(mensaje);
}
