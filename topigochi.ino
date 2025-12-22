/*
 * Topigochi - Arduino UNO R4
 * Control de pantalla OLED 0.96" con 3 botones
 */

 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #include <EEPROM.h>
 
 // Configuración del display OLED
 #define SCREEN_WIDTH 128
 #define SCREEN_HEIGHT 64
 #define OLED_RESET -1
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
 // Pines de los botones
 #define BOTON_1 2  // Moverse por el menú
 #define BOTON_2 3  // Ejecutar acción
 #define BOTON_3 4  // Cancelar
 
 // Tiempo interno
 unsigned long tiempoInicioSesion = 0;  // Momento en que se enciende el Arduino
 unsigned long tiempoAcumulado = 0;     // Tiempo total acumulado de sesiones anteriores
 unsigned long tiempoActual = 0;
 unsigned long tiempoTranscurrido = 0;  // Tiempo total (acumulado + sesión actual)
 unsigned long tiempoUltimoGuardado = 0;
 
 // Intervalos de tiempo para eventos
 const unsigned long intervaloComida = 28800000; //cada 8 horas
 const unsigned long intervaloLimpieza = 43200000;  //cada 12 horas
 const unsigned long intervaloMaldad = 43200000; //cada 12 horas
 const unsigned long intervaloAburrimiento = 28800000; //cada 8 horas
 const unsigned long intervaloEnfermedad = 86400000;  //cada 24 horas
 
 //variacion en los eventos
 const unsigned long variacionMaxima = 1800000; //30 minutos
 
 //ultima vez de la accion
 unsigned long lastComida = 0; //cada 8 horas
 unsigned long lastLimpieza = 0;  //cada 12 horas
 unsigned long lastMaldad = 0; //cada 12 horas
 unsigned long lastAburrimiento = 0; //cada 8 horas
 unsigned long lastEnfermedad = 0;  //cada 24 horas
 
 // Pin del buzzer
 #define BUZZER 8
 
 // Variables para debounce
 unsigned long lastDebounceTime = 0;
 unsigned long debounceDelay = 50;
 
 // Variables para timeout del menú
 unsigned long ultimaInteraccion = 0;
 const unsigned long TIMEOUT_MENU = 5000; // 5 segundos en milisegundos
 bool menuActivo = false; // Indica si estamos en el menú o en pantalla principal
 
 // Marca de validación para EEPROM
 #define EEPROM_MAGIC 0xABCD
 #define EEPROM_MAGIC_ADDR 0
 
 // ========================================
 // ESTRUCTURA DE LA MASCOTA
 // ========================================
 struct Mascota {
   // Fase de crecimiento del bicho
   char fase[10];           // String convertido a array de chars
   
   // Atributos principales (escala del 1 al 5)
   int salud;            // Salud general (depende de otros valores)
   int felicidad;        // Nivel de felicidad
   int saciado;          // Nivel de hambre (5 = lleno, 1 = hambriento)
   int limpieza;         // Nivel de limpieza
   int educacion;        // Nivel de educación/disciplina
   int enfermedad;       // Nivel de enfermedad (0 = sano, 5 = muy enfermo)
   
   // Control de tiempo
   unsigned long tiempoVivo;  // Tiempo en milisegundos desde el inicio
   
   // Estados booleanos
   bool isDead;          // ¿Está muerto?
   bool despierto;       // ¿Está despierto o durmiendo?
 };
 
 // Instancia global de la mascota
 Mascota miMascota;
 
 // ========================================
 // SISTEMA DE MENÚ
 // ========================================
 // El menú tiene 8 secciones, navegables con los botones
 
 enum SeccionMenu {
   MENU_INFO = 0,        // Información de la mascota
   MENU_COMER = 1,       // Dar de comer
   MENU_ACARICIAR = 2,   // Acariciar (aumenta felicidad)
   MENU_LIMPIAR = 3,     // Limpiar
   MENU_DISCIPLINAR = 4, // Disciplinar (aumenta educación)
   MENU_CURAR = 5,       // Curar enfermedad
   MENU_LUZ = 6,         // Encender/apagar luz
   MENU_ALERTA = 7       // Icono de alerta (no accesible directamente)
 };
 
 // Variable que indica la sección actual del menú
 int menuActual = MENU_INFO;
 
 // ========================================
 // FUNCIONES DE EEPROM
 // ========================================
 void guardarMascota() {
   int dir = 2; // Empezar después del magic number
   
   // Actualizar tiempo total antes de guardar
   tiempoTranscurrido = tiempoAcumulado + (millis() - tiempoInicioSesion);
   
   // Guardar magic number
   EEPROM.put(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
 
   // Guardar fase
   for (int i = 0; i < 10; i++) {
     EEPROM.write(dir++, miMascota.fase[i]);
   }
 
   // Guardar enteros
   EEPROM.put(dir, miMascota.salud); dir += sizeof(miMascota.salud);
   EEPROM.put(dir, miMascota.felicidad); dir += sizeof(miMascota.felicidad);
   EEPROM.put(dir, miMascota.saciado); dir += sizeof(miMascota.saciado);
   EEPROM.put(dir, miMascota.limpieza); dir += sizeof(miMascota.limpieza);
   EEPROM.put(dir, miMascota.educacion); dir += sizeof(miMascota.educacion);
   EEPROM.put(dir, miMascota.enfermedad); dir += sizeof(miMascota.enfermedad);
   EEPROM.put(dir, miMascota.tiempoVivo); dir += sizeof(miMascota.tiempoVivo);
   EEPROM.put(dir, miMascota.isDead); dir += sizeof(miMascota.isDead);
   EEPROM.put(dir, miMascota.despierto); dir += sizeof(miMascota.despierto);
 
   // Guardar tiempos de eventos
   EEPROM.put(dir, lastComida); dir += sizeof(lastComida);
   EEPROM.put(dir, lastLimpieza); dir += sizeof(lastLimpieza);
   EEPROM.put(dir, lastMaldad); dir += sizeof(lastMaldad);
   EEPROM.put(dir, lastAburrimiento); dir += sizeof(lastAburrimiento);
   EEPROM.put(dir, lastEnfermedad); dir += sizeof(lastEnfermedad);
   EEPROM.put(dir, tiempoActual); dir += sizeof(tiempoActual);
   EEPROM.put(dir, tiempoTranscurrido); dir += sizeof(tiempoTranscurrido);
 
   // Actualizar tiempo del último guardado
   tiempoUltimoGuardado = millis();
   EEPROM.put(dir, tiempoUltimoGuardado); dir += sizeof(tiempoUltimoGuardado); 
 
   Serial.print(F("Mascota guardada - Tiempo total: "));
   Serial.print(tiempoTranscurrido / 60000);
   Serial.println(F(" minutos"));
 }
 
 bool cargarMascota() {
   int dir = 2; // Empezar después del magic number
   
   // Verificar magic number
   uint16_t magic;
   EEPROM.get(EEPROM_MAGIC_ADDR, magic);
   
   if (magic != EEPROM_MAGIC) {
     Serial.println(F("No hay datos guardados en EEPROM, inicializando mascota"));
     return false;
   }
 
   // Cargar fase
   for (int i = 0; i < 10; i++) {
     miMascota.fase[i] = EEPROM.read(dir++);
   }
 
   // Cargar enteros
   EEPROM.get(dir, miMascota.salud); dir += sizeof(miMascota.salud);
   EEPROM.get(dir, miMascota.felicidad); dir += sizeof(miMascota.felicidad);
   EEPROM.get(dir, miMascota.saciado); dir += sizeof(miMascota.saciado);
   EEPROM.get(dir, miMascota.limpieza); dir += sizeof(miMascota.limpieza);
   EEPROM.get(dir, miMascota.educacion); dir += sizeof(miMascota.educacion);
   EEPROM.get(dir, miMascota.enfermedad); dir += sizeof(miMascota.enfermedad);
   EEPROM.get(dir, miMascota.tiempoVivo); dir += sizeof(miMascota.tiempoVivo);
   EEPROM.get(dir, miMascota.isDead); dir += sizeof(miMascota.isDead);
   EEPROM.get(dir, miMascota.despierto); dir += sizeof(miMascota.despierto);
 
   // Cargar tiempos de eventos
   EEPROM.get(dir, lastComida); dir += sizeof(lastComida);
   EEPROM.get(dir, lastLimpieza); dir += sizeof(lastLimpieza);
   EEPROM.get(dir, lastMaldad); dir += sizeof(lastMaldad);
   EEPROM.get(dir, lastAburrimiento); dir += sizeof(lastAburrimiento);
   EEPROM.get(dir, lastEnfermedad); dir += sizeof(lastEnfermedad);
   EEPROM.get(dir, tiempoActual); dir += sizeof(tiempoActual);
   EEPROM.get(dir, tiempoTranscurrido); dir += sizeof(tiempoTranscurrido);
   EEPROM.get(dir, tiempoUltimoGuardado); dir += sizeof(tiempoUltimoGuardado);
 
   Serial.println(F("Mascota cargada desde EEPROM"));
   return true;
 }
 
 // ========================================
 // FUNCIONES DE INICIALIZACIÓN
 // ========================================
 
 // Inicializa los valores de la mascota a su estado inicial
 void inicializarMascota() {
   strcpy(miMascota.fase, "huevo");
   miMascota.salud = 5;
   miMascota.felicidad = 5;
   miMascota.saciado = 5;
   miMascota.limpieza = 5;
   miMascota.educacion = 2;
   miMascota.enfermedad = 0;
   miMascota.tiempoVivo = 0;
   miMascota.isDead = false;
   miMascota.despierto = true;
   
   Serial.println(F("Mascota inicializada:"));
   Serial.println(F("  Fase: huevo"));
   Serial.println(F("  Todos los valores en estado inicial"));
 }
 
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
       
       // NO hacer bucle infinito, solo continuar sin display
       delay(2000);
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
 
   // Inicializar o cargar la mascota
   if (!cargarMascota()) {
     // Mascota nueva
     inicializarMascota();
     tiempoInicioSesion = millis();
     tiempoAcumulado = 0;
     tiempoTranscurrido = 0;
     lastComida = millis();
     lastLimpieza = millis();
     lastMaldad = millis();
     lastAburrimiento = millis();
     lastEnfermedad = millis();
     //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
     //guardarMascota();
     Serial.println(F("Nueva mascota creada"));
   } else {
     // Mascota cargada - continuar desde donde se quedó
     tiempoInicioSesion = millis();
     tiempoAcumulado = tiempoTranscurrido; // El tiempo que ya tenía guardado
     Serial.print(F("Mascota cargada - Tiempo previo: "));
     Serial.print(tiempoAcumulado / 60000);
     Serial.println(F(" minutos"));
   }
   
   // Limpiar pantalla
   display.clearDisplay();
   display.setCursor(0, 0);
   display.println(F("Presiona un boton"));
   display.display();
   Serial.println(F("Sistema listo!"));
 }
 
 // Variables para sistema de guardado inteligente
 bool datosModificados = false;
 const unsigned long INTERVALO_GUARDADO_NORMAL = 300000; // 5 minutos
 const unsigned long INTERVALO_GUARDADO_CRITICO = 60000;  // 1 minuto si hay cambios
 
 void loop() {
   // ========================================
   // Reloj interno - Calcular tiempo total
   // ========================================
   tiempoActual = millis();
   // Tiempo total = tiempo acumulado de antes + tiempo de esta sesión
   tiempoTranscurrido = tiempoAcumulado + (tiempoActual - tiempoInicioSesion);
 
   // Sistema de guardado inteligente
   unsigned long tiempoDesdeUltimoGuardado = tiempoActual - tiempoUltimoGuardado;
   
   if (datosModificados && tiempoDesdeUltimoGuardado > INTERVALO_GUARDADO_CRITICO) {
     // Hay cambios importantes, guardar en 1 minuto
     //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
     //guardarMascota();
     datosModificados = false;
     Serial.println(F("Guardado critico realizado"));
   } else if (tiempoDesdeUltimoGuardado > INTERVALO_GUARDADO_NORMAL) {
     // Guardado de rutina cada 5 minutos
     //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
     //guardarMascota();
     Serial.println(F("Auto-guardado realizado"));
   }
 
   // ========================================
   // Eventos
   // ========================================
   cheackearEventos();
 
   // ========================================
   // VERIFICAR TIMEOUT DEL MENÚ
   // ========================================
   if (menuActivo && (millis() - ultimaInteraccion > TIMEOUT_MENU)) {
     // Han pasado 5 segundos sin interacción, volver a pantalla principal
     menuActivo = false;
     mostrarPantallaPrincipal();
     Serial.println(F("Timeout del menu - Volviendo a pantalla principal"));
   }
   
   // ========================================
   // LECTURA DE BOTONES
   // ========================================
   
   // BOTÓN 1: Moverse por el menú
   if (digitalRead(BOTON_1) == LOW) {
     delay(debounceDelay); // Anti-rebote
     if (digitalRead(BOTON_1) == LOW) {
       navegarMenu();
       while(digitalRead(BOTON_1) == LOW); // Esperar a que se suelte
     }
   }
   
   // BOTÓN 2: Ejecutar acción del menú actual
   if (digitalRead(BOTON_2) == LOW) {
     delay(debounceDelay);
     if (digitalRead(BOTON_2) == LOW) {
       ejecutarAccion();
       while(digitalRead(BOTON_2) == LOW);
     }
   }
   
   // BOTÓN 3: Cancelar (volver al menú principal)
   if (digitalRead(BOTON_3) == LOW) {
     delay(debounceDelay);
     if (digitalRead(BOTON_3) == LOW) {
       cancelarAccion();
       while(digitalRead(BOTON_3) == LOW);
     }
   }
 }
 
 // ========================================
 // FUNCIONES DE EVENTOS
 // ========================================
 void cheackearEventos() {
   if(miMascota.isDead==false){
     //comida
     if (tiempoActual - lastComida >= intervaloComida) {
       if (miMascota.saciado > 1) {
         miMascota.saciado--;
         Serial.println(F("Evento: Hambre - Saciado -1"));
         lastComida = tiempoActual;
         datosModificados = true; // Marcar que hay cambios
         //TODO: Mostrar mensaje en pantalla
       }else{
         miMascota.isDead = true;
         datosModificados = true;
       }
     }
     //limpieza
     if (tiempoActual - lastLimpieza >= intervaloLimpieza) {
       if (miMascota.limpieza > 1) {
         miMascota.limpieza--;
         Serial.println(F("Evento: Suciedad - Limpieza -1"));
         lastLimpieza = tiempoActual;
         datosModificados = true;
         //TODO: Mostrar mensaje en pantalla
       }else{
         miMascota.isDead = true;
         datosModificados = true;
       }
     }
     //maldad
     if (tiempoActual - lastMaldad >= intervaloMaldad) {
       if (miMascota.educacion > 1) {
         miMascota.educacion--;
         Serial.println(F("Evento: Maldad - Educacion -1"));
         lastMaldad = tiempoActual;
         datosModificados = true;
         //TODO: Mostrar mensaje en pantalla   
       }else{
         miMascota.isDead = true;
         datosModificados = true;
       }
     }
     //aburrimiento
     if (tiempoActual - lastAburrimiento >= intervaloAburrimiento) {
       if (miMascota.felicidad > 1) {
         miMascota.felicidad--;
         Serial.println(F("Evento: Aburrimiento - Felicidad -1"));
         lastAburrimiento = tiempoActual;
         datosModificados = true;
         //TODO: Mostrar mensaje en pantalla 
       }else{
         miMascota.isDead = true;
         datosModificados = true;
       }
     }
     //enfermedad
     if (tiempoActual - lastEnfermedad >= intervaloEnfermedad) {
       if (miMascota.enfermedad < 5) {
         miMascota.enfermedad++;
         Serial.println(F("Evento: Enfermedad - Enfermedad +1"));
         lastEnfermedad = tiempoActual;
         datosModificados = true;
         //TODO: Mostrar mensaje en pantalla
       }else{
         miMascota.isDead = true;
         datosModificados = true;
       }
     } 
   }else{
     // La mascota está muerta
     mostrarMensaje("Tu mascota ha muerto");
     Serial.println(F("La mascota ha muerto"));
     // Aquí podríamos agregar lógica para reiniciar el juego o similar
   }
 }
 
 // ========================================
 // FUNCIONES DEL SISTEMA DE MENÚ
 // ========================================
 
 // Navega a la siguiente sección del menú (Botón 1)
 void navegarMenu() {
   // Activar menú y actualizar tiempo de interacción
   menuActivo = true;
   ultimaInteraccion = millis();
   
   // Avanzar al siguiente menú (0-6, saltamos el MENU_ALERTA)
   menuActual++;
   if (menuActual >= MENU_ALERTA) {
     menuActual = MENU_INFO; // Volver al inicio
   }
   
   // Feedback sonoro
   tone(BUZZER, 800, 50);
   
   // Mostrar el menú actualizado
   mostrarMenu();
   
   Serial.print(F("Menu actual: "));
   Serial.println(menuActual);
 }
 
 // Ejecuta la acción correspondiente al menú actual (Botón 2)
 void ejecutarAccion() {
   // Actualizar tiempo de interacción
   ultimaInteraccion = millis();
   
   // Feedback sonoro
   tone(BUZZER, 1200, 100);
   
   // Ejecutar según la sección actual
   switch(menuActual) {
     case MENU_INFO:
       mostrarInformacion();
       break;
     case MENU_COMER:
       darDeComer();
       break;
     case MENU_ACARICIAR:
       acariciar();
       break;
     case MENU_LIMPIAR:
       limpiar();
       break;
     case MENU_DISCIPLINAR:
       disciplinar();
       break;
     case MENU_CURAR:
       curar();
       break;
     case MENU_LUZ:
       toggleLuz();
       break;
   }
   
   Serial.print(F("Accion ejecutada: "));
   Serial.println(menuActual);
 }
 
 // Cancela la acción actual y vuelve a la pantalla principal (Botón 3)
 void cancelarAccion() {
   // Feedback sonoro diferente
   tone(BUZZER, 600, 100);
   
   // Desactivar menú y volver a pantalla principal
   menuActivo = false;
   mostrarPantallaPrincipal();
   
   Serial.println(F("Cancelado - Volviendo a pantalla principal"));
 }
 
 // ========================================
 // FUNCIONES DE VISUALIZACIÓN
 // ========================================
 
 // Muestra el menú con los 8 iconos
 void mostrarMenu() {
   display.clearDisplay();
   
   // TODO: Aquí dibujaremos los 8 iconos del menú
   // 4 iconos arriba, 4 iconos abajo
   // El icono del menuActual estará resaltado
   
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   display.println(F("MENU:"));
   display.print(F("Seccion: "));
   switch (menuActual) {
     case 0:
       display.println("Información");
       break;
     case 1:
       display.println("Dar de comer");
       break;
     case 2:
       display.println("Acariciar");
       break;
     case 3:
       display.println("Limpiar");
       break;
     case 4:
       display.println("Disciplinar");
       break;
     case 5:
       display.println("Curar");
       break;
     case 6:
       display.println("Luz");
       break;
     default:
       display.println(menuActual);
       break;
     }
   display.display();
 }
 
 // Muestra la información detallada de la mascota
 void mostrarInformacion() {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   
   display.print(F("Fase: "));
   display.println(miMascota.fase);
   display.print(F("Salud: "));
   display.println(miMascota.salud);
   display.print(F("Feliz: "));
   display.println(miMascota.felicidad);
   display.print(F("Hambre: "));
   display.println(miMascota.saciado);
   display.print(F("Limpio: "));
   display.println(miMascota.limpieza);
   display.print(F("Tiempo: "));
   // Convertir milisegundos a horas y minutos
   int horas = (tiempoTranscurrido / 3600000) % 24;
   int minutos = (tiempoTranscurrido / 60000) % 60;
   if (horas < 10){
     display.print("0");
   } 
   display.print(horas);
   display.print(":");
   if (minutos < 10){
     display.print("0");
   } 
   display.println(minutos);
   display.display();
 }
 
 // Muestra la pantalla principal con la mascota
 void mostrarPantallaPrincipal() {
   display.clearDisplay();
   
   // TODO: Aquí se dibujará la mascota animada
   // Por ahora mostramos un mensaje simple
   
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(20, 20);
   display.println(F("TOPIGOCHI"));
   
   display.setTextSize(1);
   display.setCursor(10, 45);
   display.print(F("Fase: "));
   display.println(miMascota.fase);
   
   display.display();
   
   Serial.println(F("Mostrando pantalla principal"));
 }
 
 // ========================================
 // FUNCIONES DE ACCIONES
 // ========================================
 
 // Dar de comer: aumenta la saciedad en 1 (máximo 5)
 void darDeComer() {
   if (miMascota.saciado < 5) {
     miMascota.saciado++;
     Serial.println(F("Dando de comer... Saciado +1"));
     mostrarMensaje("Comiendo!");
   } else {
     Serial.println(F("La mascota ya esta llena!"));
     mostrarMensaje("Lleno!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
 }
 
 // Acariciar: aumenta la felicidad en 1 (máximo 5)
 void acariciar() {
   if (miMascota.felicidad < 5) {
     miMascota.felicidad++;
     Serial.println(F("Acariciando... Felicidad +1"));
     mostrarMensaje("Feliz!");
   } else {
     Serial.println(F("La mascota ya esta muy feliz!"));
     mostrarMensaje("Muy feliz!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
 }
 
 // Limpiar: aumenta la limpieza en 1 (máximo 5)
 void limpiar() {
   if (miMascota.limpieza < 5) {
     miMascota.limpieza++;
     Serial.println(F("Limpiando... Limpieza +1"));
     mostrarMensaje("Limpio!");
   } else {
     Serial.println(F("La mascota ya esta muy limpia!"));
     mostrarMensaje("Muy limpio!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
 }
 
 // Disciplinar: aumenta la educación en 1 (máximo 5)
 void disciplinar() {
   if (miMascota.educacion < 5) {
     miMascota.educacion++;
     Serial.println(F("Disciplinando... Educacion +1"));
     mostrarMensaje("Educado!");
   } else {
     Serial.println(F("La mascota ya esta bien educada!"));
     mostrarMensaje("Bien educado!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
 }
 
 // Curar: disminuye la enfermedad en 1 (mínimo 0)
 void curar() {
   if (miMascota.enfermedad > 0) {
     miMascota.enfermedad--;
     Serial.println(F("Curando... Enfermedad -1"));
     mostrarMensaje("Curado!");
   } else {
     Serial.println(F("La mascota ya esta sana!"));
     mostrarMensaje("Ya sano!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
 }
 
 // Encender/Apagar luz: cambia el estado de despierto
 void toggleLuz() {
   miMascota.despierto = !miMascota.despierto;
   
   if (miMascota.despierto) {
     Serial.println(F("Luz encendida - Mascota despierta"));
     mostrarMensaje("Despierto!");
   } else {
     Serial.println(F("Luz apagada - Mascota durmiendo"));
     mostrarMensaje("Durmiendo!");
   }
   //COMENTADO GUARDAR MASCOTA PARA NO DAñar EEPROM
   //guardarMascota(); // Guardar inmediatamente después de acción
   datosModificados = false; // Ya guardamos
   delay(2000);
   menuActivo = false;
   mostrarPantallaPrincipal();
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