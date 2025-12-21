# Topigochi - Proyecto Arduino UNO R4

## 📋 Descripción
Proyecto de mascota virtual (Topigochi) controlada por Arduino UNO R4, con pantalla OLED, botones de interacción y feedback sonoro mediante buzzer.

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
       A1 ──────┤ A1          5V  ├───── 5V (común)
       A2 ──────┤ A2          3.3V├───── 
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
       D7 ──────┤ D7               │
                 └─────────────────┘
```

### Conexiones Detalladas

#### 1. Display OLED (I2C)
| Pin OLED | Pin Arduino | Descripción |
|----------|-------------|-------------|
| VCC      | 5V          | Alimentación |
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
    ┌──────────────────────┴───┴──┐
    │          PROTOBOARD          │
    │   5V Rail          GND Rail  │
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
2. Después de 2 segundos aparecerá "Presiona un boton"
3. Al presionar cada botón:
   - **Botón 1**: Muestra "Boton 1" en pantalla
   - **Botón 2**: Muestra "Boton 2" en pantalla
   - **Botón 3**: Muestra "Boton 3" en pantalla
4. Cada pulsación genera un tono de 1000Hz por 100ms en el buzzer

### Monitor Serial
Para debugging, abre el monitor serial (`Herramientas > Monitor Serie`) a 9600 baudios. Verás mensajes cada vez que presiones un botón.

## 📝 Funcionalidades Actuales

- ✅ Detección de pulsación de 3 botones
- ✅ Visualización de mensajes en display OLED
- ✅ Feedback sonoro mediante buzzer
- ✅ Anti-rebote (debounce) en botones
- ✅ Comunicación serial para debugging

## 🔮 Futuras Funcionalidades

- [ ] Sistema de menú interactivo
- [ ] Mascota virtual con estados (feliz, triste, hambriento)
- [ ] Minijuegos
- [ ] Sistema de puntuación
- [ ] Animaciones en OLED
- [ ] Diferentes tonos/melodías en el buzzer
- [ ] Guardado de estado (EEPROM)

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
