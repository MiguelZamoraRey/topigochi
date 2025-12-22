# Topigochi - Proyecto Arduino UNO R4

## 📋 Descripción
Proyecto de mascota virtual (Topigochi) estilo Tamagotchi controlada por Arduino UNO R4, con pantalla OLED, botones de interacción y feedback sonoro mediante buzzer. Cuida a tu mascota virtual alimentándola, jugando con ella, limpiándola y manteniéndola sana y feliz.

## 🔧 Hardware Necesario

### Componentes
- **Arduino UNO R4** (1x)
- **Display OLED 0.96"** I2C SSD1306 (1x)
- **Buzzer Pasivo** (1x)
- **Botones Pulsadores** (3x)
- **Resistencias** (opcional, si los botones no tienen pull-up)
- **Protoboard**
- **Cables Dupont**

### Especificaciones del Display OLED
- Tamaño: 0.96 pulgadas
- Resolución: 128x64 píxeles
- Driver: SSD1306
- Interfaz: I2C
- Voltaje: 3.3V - 5V
- Dirección I2C: 0x3C (por defecto)

## 🔌 Electrónica y Conexiones

### Diagrama de Pines

#### Arduino UNO R4 - Pinout
```
                  Arduino UNO R4
                ┌─────────────────┐
                │                 │
    RESET ──────┤ RESET       VIN ├───── 
                │                 │
       A0 ──────┤ A0          GND ├───── GND (común)
       A1 ──────┤ A1          5V  ├───── 
       A2 ──────┤ A2          3.3V├───── 3.3V (común)
       A3 ──────┤ A3          REF ├───── 
  SDA  A4 ──────┤ A4 (SDA)    A5  ├───── SCL (Display)
  SCL  A5 ──────┤ A5 (SCL)        │
                │                 │
      GND ──────┤ GND         D13 ├───── 
                │                 │
   BOTON_1 D2 ──┤ D2          D12 ├───── 
   BOTON_2 D3 ──┤ D3          D11 ├───── 
   BOTON_3 D4 ──┤ D4          D10 ├───── 
       D5 ──────┤ D5           D9 ├───── 
       D6 ──────┤ D6           D8 ├───── BUZZER
       D7 ──────┤ D7              │
                └─────────────────┘
```

#### Arduino Nano - Pinout

                   Arduino Nano
             ┌───────────────────────┐
             │                       │
 RESET ──────┤ RESET           VIN   ├─────
             │                       │
    A0 ──────┤ A0              GND   ├───── GND (común)
    A1 ──────┤ A1              5V    ├───── 
    A2 ──────┤ A2              3.3V  ├───── 3.3V (común)
    A3 ──────┤ A3              REF   ├─────
 SDA A4 ─────┤ A4 (SDA)        A5    ├───── SCL (Display)
 SCL A5 ─────┤ A5 (SCL)        A6    ├─────
             │                       │
   GND ──────┤ GND             D13   ├─────
             │                       │
BOTON_1 D2 ──┤ D2              D12   ├─────
BOTON_2 D3 ──┤ D3              D11   ├─────
BOTON_3 D4 ──┤ D4              D10   ├─────
     D5 ─────┤ D5               D9   ├─────
     D6 ─────┤ D6               D8   ├───── BUZZER
     D7 ─────┤ D7               D7   ├─────
             └───────────────────────┘


### Conexiones Detalladas

#### 1. Display OLED (I2C)
| Pin OLED | Pin Arduino | Descripción |
|----------|-------------|-------------|
| VCC      | 3.3V        | Alimentación |
| GND      | GND         | Tierra |
| SCL      | A5          | Reloj I2C |
| SDA      | A4          | Datos I2C |

**Nota**: El Arduino UNO R4 tiene pines I2C dedicados en A4 (SDA) y A5 (SCL).

#### 2. Botones Pulsadores
| Botón   | Pin Arduino | Conexión |
|---------|-------------|----------|
| Botón 1 | D2          | Un terminal al pin D2, el otro a GND |
| Botón 2 | D3          | Un terminal al pin D3, el otro a GND |
| Botón 3 | D4          | Un terminal al pin D4, el otro a GND |

**Configuración**: Los botones utilizan las resistencias pull-up internas del Arduino (configuradas en el código con `INPUT_PULLUP`). Cuando el botón se presiona, conecta el pin a GND (estado LOW).

```
    Arduino Pin Dx ────┬──── Botón ──── GND
                       │
                 (Pull-up interno)
                      ▲
                     10kΩ
                      │
                     5V
```

#### 3. Buzzer
| Pin Buzzer | Pin Arduino | Descripción |
|------------|-------------|-------------|
| Positivo (+) | D8        | Señal PWM |
| Negativo (-) | GND       | Tierra |

**Tipo**: Buzzer pasivo (permite diferentes tonos mediante PWM)

### Esquema de Conexión Completo

```
                     ┌──────────────────┐
                     │  Arduino UNO R4  │
                     │                  │
    ┌────────────────┤ A4 (SDA)         │
    │    ┌───────────┤ A5 (SCL)         │
    │    │           │                  │
    │    │     ┌─────┤ D2               │
    │    │     │ ┌───┤ D3               │
    │    │     │ │ ┌─┤ D4               │
    │    │     │ │ │ │                  │
    │    │     │ │ │ ├─────┐            │
    │    │     │ │ │ │ D8  │            │
    │    │     │ │ │ │     │            │
    │    │     │ │ │ ├─────┼────────────┤
    │    │     │ │ │ │ 5V  │            │
    │    │     │ │ │ │     │            │
    │    │     │ │ │ └─────┼────────────┤
    │    │     │ │ │  GND  │            │
    │    │     │ │ │       │            │
    │    │     │ │ │       └────────────┘
    │    │     │ │ │
    │    │     │ │ │
┌───┴────┴───┐ │ │ │    ┌─────────┐
│ OLED 0.96" │ │ │ │    │ BUZZER  │
│            │ │ │ │    │         │
│ SDA  SCL   │ │ │ │    │  +   -  │
│  │    │    │ │ │ │    └──┬───┬──┘
│  │ VCC│GND │ │ │ │       │   │
└──┼────┼────┘ │ │ │       │   │
   │    │      │ │ │       │   │
   │    └──────┘ │ │       │   │
   │             │ │       │   │
   │    ┌────────┘ │       │   │
   │    │  ┌───────┘       │   │
   │    │  │               │   │
   │    │  │   ┌──────┐    │   │
   │    │  └───┤ BTN1 ├────┤   │
   │    │      └──────┘    │   │
   │    │      ┌──────┐    │   │
   │    └──────┤ BTN2 ├────┤   │
   │           └──────┘    │   │
   │           ┌──────┐    │   │
   └───────────┤ BTN3 ├────┼───┤
               └──────┘    │   │
                           │   │
    ┌──────────────────────┴───┴───┐
    │          PROTOBOARD          │
    │ 3.3V Rail          GND Rail  │
    └──────────────────────────────┘
```

## 💻 Software y Librerías

### IDE
- **Arduino IDE** 2.x o superior
- Compatible con PlatformIO

### Librerías Requeridas
1. **Adafruit SSD1306** (versión 2.5.0 o superior)
   - Para control del display OLED
   - Instalación: `Herramientas > Administrar Bibliotecas > Buscar "Adafruit SSD1306"`

2. **Adafruit GFX Library** (versión 1.11.0 o superior)
   - Librería gráfica base
   - Instalación: `Herramientas > Administrar Bibliotecas > Buscar "Adafruit GFX"`

3. **Wire.h** (incluida en Arduino IDE)
   - Para comunicación I2C

### Configuración del Arduino IDE
1. Seleccionar placa: `Herramientas > Placa > Arduino UNO R4`
2. Seleccionar puerto: `Herramientas > Puerto > [Tu puerto USB]`
3. Velocidad de baudios: `9600` (para monitor serial)

## 🚀 Instalación y Uso

### Pasos de Instalación
1. Clonar o descargar este repositorio
2. Abrir el archivo `topigochiv1.ino` en Arduino IDE
3. Instalar las librerías necesarias (ver sección anterior)
4. Realizar las conexiones según el diagrama de pines
5. Conectar el Arduino UNO R4 al computador
6. Verificar y subir el código al Arduino

### Uso
1. Una vez cargado el programa, el display mostrará "Topigochi - Iniciando..."
2. El sistema inicializará tu mascota en fase "huevo" con todos sus atributos
3. Usa los 3 botones para interactuar:
   - **Botón 1**: Navegar por el menú (8 secciones)
   - **Botón 2**: Ejecutar la acción seleccionada
   - **Botón 3**: Cancelar y volver al menú

### Sistema de Menú
El juego cuenta con 8 secciones accesibles mediante los botones:

| Sección | Icono | Función | Efecto |
|---------|-------|---------|--------|
| 1. Info | ℹ️ | Información | Muestra los atributos de la mascota |
| 2. Comer | 🍔 | Alimentar | Aumenta saciedad +1 (máx. 5) |
| 3. Acariciar | ❤️ | Jugar/Acariciar | Aumenta felicidad +1 (máx. 5) |
| 4. Limpiar | 🧹 | Limpiar | Aumenta limpieza +1 (máx. 5) |
| 5. Disciplinar | 📚 | Educar | Aumenta educación +1 (máx. 5) |
| 6. Curar | 💊 | Medicina | Disminuye enfermedad -1 (mín. 0) |
| 7. Luz | 💡 | Dormir/Despertar | Alterna estado despierto/dormido |
| 8. Alerta | ⚠️ | Sistema | Icono automático de advertencias |

**Navegación**:
- Presiona **Botón 1** repetidamente para moverte entre secciones 1-7
- Presiona **Botón 2** para ejecutar la acción de la sección actual
- Presiona **Botón 3** para cancelar y regresar al menú

### Atributos de la Mascota
Tu mascota tiene los siguientes atributos (escala 1-5, excepto indicados):

| Atributo | Inicial | Descripción |
|----------|---------|-------------|
| **Fase** | "huevo" | Etapa de crecimiento (huevo → bebé → adulto) |
| **Salud** | 5 | Salud general (depende de otros atributos) |
| **Felicidad** | 5 | Nivel de felicidad |
| **Saciado** | 5 | Nivel de hambre (5 = lleno, 1 = hambriento) |
| **Limpieza** | 5 | Nivel de higiene |
| **Educación** | 2 | Nivel de disciplina/educación |
| **Enfermedad** | 0 | Nivel de enfermedad (0 = sano, 5 = muy enfermo) |
| **Tiempo Vivo** | 0 | Tiempo desde el inicio (milisegundos) |
| **Estado** | Vivo | Vivo o muerto (isDead: false/true) |
| **Despierto** | Sí | Despierto o durmiendo (true/false) |

### Monitor Serial
Para debugging, abre el monitor serial (`Herramientas > Monitor Serie`) a 9600 baudios. Verás:
- Estado de inicialización
- Escaneo de dispositivos I2C
- Mensajes de navegación del menú
- Acciones ejecutadas
- Cambios en los atributos de la mascota

## 📝 Funcionalidades Actuales

- ✅ Sistema de mascota virtual con 10 atributos
- ✅ Menú interactivo de 8 secciones navegable
- ✅ 3 botones con funciones diferenciadas (navegar, acción, cancelar)
- ✅ 6 acciones para cuidar la mascota
- ✅ Sistema de información de estado
- ✅ Control de luz (dormir/despertar)
- ✅ Feedback sonoro diferenciado por acción
- ✅ Detección de pulsación de botones
- ✅ Visualización de mensajes en display OLED
- ✅ Anti-rebote (debounce) en botones
- ✅ Comunicación serial para debugging
- ✅ Límites en los atributos (evita valores fuera de rango)

## 🔮 Futuras Funcionalidades

- [ ] Iconos visuales para cada sección del menú
- [ ] Animaciones de la mascota según su estado
- [ ] Sistema de degradación automática (hambre, limpieza, felicidad bajan con el tiempo)
- [ ] Evolución de fases (huevo → bebé → adulto)
- [ ] Cálculo dinámico de salud según otros atributos
- [ ] Sistema de alerta (sección 8) que detecta necesidades
- [ ] Minijuegos para aumentar felicidad
- [ ] Efectos visuales al ejecutar acciones
- [ ] Diferentes tonos/melodías según la acción
- [ ] Sistema de muerte y reinicio
- [ ] Guardado de estado en EEPROM
- [ ] Múltiples tipos de comida
- [ ] Sistema de recompensas

## 🛠️ Solución de Problemas

### El display no enciende
- Verificar las conexiones I2C (SDA en A4, SCL en A5)
- Verificar alimentación (VCC a 5V, GND a GND)
- Probar cambiar la dirección I2C en el código (0x3C o 0x3D)
- Ejecutar un scanner I2C para detectar la dirección correcta

### Los botones no responden
- Verificar que los botones estén conectados entre el pin digital y GND
- Verificar que el código usa `INPUT_PULLUP`
- Probar con el monitor serial para ver si se detectan las pulsaciones

### El buzzer no suena
- Verificar que es un buzzer pasivo (no activo)
- Verificar conexión: positivo a D8, negativo a GND
- Verificar que el pin 8 está configurado como OUTPUT

### Error al compilar
- Verificar que las librerías Adafruit estén instaladas
- Verificar la versión del Arduino IDE
- Limpiar y volver a compilar

## 📄 Licencia

Este proyecto es de código abierto y está disponible para uso educativo y personal.

## 👨‍💻 Autor

Miguel Zamora - Diciembre 2025

## 📚 Referencias

- [Arduino UNO R4 Documentation](https://docs.arduino.cc/hardware/uno-r4-minima/)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [I2C Communication Protocol](https://learn.sparkfun.com/tutorials/i2c)
- [Tamagotchi - Wikipedia](https://es.wikipedia.org/wiki/Tamagotchi)
