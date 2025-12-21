# CHANGELOG

Todos los cambios notables de este proyecto serán documentados en este archivo.

El formato está basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/),
y este proyecto adhiere a [Semantic Versioning](https://semver.org/lang/es/).

## [Unreleased]

### Planeado
- Sistema de menú navegable con los 3 botones
- Mascota virtual con diferentes estados emocionales
- Sistema de hambre y felicidad
- Minijuegos interactivos
- Animaciones en el display OLED
- Melodías y efectos de sonido variados
- Guardado persistente de datos en EEPROM
- Sistema de logros o achievements

---

## [0.1.0] - 2024-12-21

### ✨ Añadido
- **Configuración inicial del proyecto**
  - Estructura base del código Arduino
  - Archivo principal `topigochiv1.ino`
  - Documentación completa en README.md
  - Archivo CHANGELOG.md para seguimiento de versiones

- **Hardware configurado**
  - Soporte para Arduino UNO R4
  - Integración de Display OLED 0.96" (SSD1306) vía I2C
  - Configuración de 3 botones pulsadores con pull-up interno
  - Integración de buzzer pasivo para feedback sonoro

- **Funcionalidades básicas**
  - Detección de pulsación de 3 botones independientes
  - Sistema anti-rebote (debounce) para los botones
  - Visualización de mensajes en display OLED según botón presionado
  - Feedback sonoro al presionar botones (tono de 1000Hz, 100ms)
  - Comunicación serial para debugging (9600 baudios)
  - Pantalla de bienvenida "Topigochi - Iniciando..."
  - Mensaje de espera "Presiona un boton"

- **Librerías implementadas**
  - Adafruit SSD1306 para control del display
  - Adafruit GFX para gráficos
  - Wire.h para comunicación I2C

- **Documentación**
  - Diagrama de pines del Arduino UNO R4
  - Esquema de conexiones detallado
  - Tabla de conexiones para cada componente
  - Guía de instalación y configuración
  - Sección de solución de problemas
  - Referencias y recursos útiles

### 🔧 Configuración Técnica
- **Pines asignados:**
  - D2: Botón 1
  - D3: Botón 2
  - D4: Botón 3
  - D8: Buzzer
  - A4 (SDA): Display OLED
  - A5 (SCL): Display OLED

- **Display OLED:**
  - Resolución: 128x64 píxeles
  - Dirección I2C: 0x3C
  - Tamaño de texto: 2 (para mensajes de botones)
  - Tamaño de texto: 1 (para mensajes del sistema)

- **Buzzer:**
  - Frecuencia: 1000Hz
  - Duración: 100ms por pulsación

- **Serial:**
  - Velocidad: 9600 baudios

### 📋 Notas de Desarrollo
- El código utiliza resistencias pull-up internas para los botones, simplificando el circuito
- Se implementó un sistema de debounce simple con delay de 50ms
- El estado LOW indica botón presionado (lógica invertida por pull-up)
- Los botones esperan a ser liberados antes de detectar nueva pulsación

### 🐛 Bugs Conocidos
- Ninguno reportado en esta versión

### 📚 Dependencias
- Arduino IDE 2.x o superior
- Adafruit SSD1306 >= 2.5.0
- Adafruit GFX Library >= 1.11.0

---

## Leyenda de Tipos de Cambios

- **✨ Añadido**: Para nuevas funcionalidades
- **🔧 Cambiado**: Para cambios en funcionalidades existentes
- **⚠️ Deprecado**: Para funcionalidades que serán removidas
- **🗑️ Removido**: Para funcionalidades removidas
- **🐛 Corregido**: Para corrección de bugs
- **🔒 Seguridad**: Para vulnerabilidades de seguridad
- **⚡ Rendimiento**: Para mejoras de rendimiento
- **📝 Documentación**: Para cambios solo en documentación
- **🎨 Estilo**: Para cambios de formato que no afectan el código

---

## Versionado

Este proyecto usa [Semantic Versioning](https://semver.org/lang/es/):

- **MAJOR**: Cambios incompatibles en la API
- **MINOR**: Nuevas funcionalidades compatibles hacia atrás
- **PATCH**: Correcciones de bugs compatibles hacia atrás

Formato: `MAJOR.MINOR.PATCH` (ej: 1.2.3)

---

## Historial de Releases

| Versión | Fecha | Descripción |
|---------|-------|-------------|
| 0.1.0 | 2024-12-21 | Release inicial - Funcionalidad básica de botones y display |

---

## Próximas Versiones Planeadas

### [0.2.0] - Por definir
- Sistema de menú básico
- Navegación entre opciones con botones
- Primeras animaciones simples

### [0.3.0] - Por definir
- Implementación de mascota virtual básica
- Estados emocionales (feliz, triste, neutral)
- Sistema de hambre

### [0.4.0] - Por definir
- Primer minijuego
- Sistema de puntuación
- Guardado de estado básico

### [1.0.0] - Por definir
- Mascota virtual completa y funcional
- Múltiples minijuegos
- Sistema completo de estados y necesidades
- Animaciones avanzadas
- Melodías variadas
- Persistencia de datos robusta

---

## Contribuciones

Para sugerir cambios o reportar bugs, por favor documenta:
1. Versión actual del proyecto
2. Descripción detallada del cambio/bug
3. Pasos para reproducir (en caso de bugs)
4. Comportamiento esperado vs actual

---

**Última actualización**: 21 de diciembre de 2024
