# CHANGELOG

Todos los cambios notables de este proyecto serán documentados en este archivo.

El formato está basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/),
y este proyecto adhiere a [Semantic Versioning](https://semver.org/lang/es/).

## [0.4.0] - 2026-01-12

### ✨ Añadido
- **Eventos Independientes con Variación Aleatoria**
  - Cada stat (hambre, limpieza, felicidad, educación, enfermedad) tiene su propio temporizador
  - Los eventos ocurren en tiempos impredecibles (±30 minutos de variación)
  - Comportamiento más similar al Tamagotchi original
  
- **Sistema de Próximos Eventos**
  - Variables `nextComida`, `nextLimpieza`, `nextMaldad`, `nextAburrimiento`, `nextEnfermedad`
  - Cada evento calcula el siguiente tiempo cuando ocurre

### 🗑️ Eliminado
- **Guardado persistente en EEPROM**
  - Funciones `guardarMascota()` y `cargarMascota()` eliminadas
  - Ya no se realiza guardado automático
  - Eliminado `#include <EEPROM.h>`
  - Razón: Evitar desgaste innecesario del Arduino Nano (EEPROM tiene ciclos limitados)

### 🔄 Cambiado
- La mascota siempre reinicia desde cero al encender el Arduino
- Inicialización más simple sin manejo de EEPROM
- Variable `tiempoUltimoGuardado` eliminada

### 🐛 Corregido
- Mejorada la estabilidad de la inicialización del display OLED
- Agregado flag `displayOK` para prevenir intentos de dibujo si el display falla

---

## [0.3.0] - 2025-12-24

### ✨ Añadido
- Animación del bichejo/mascota en pantalla según estado.
- Iconos visuales para cada sección del menú (navegación con iconos).
- Estados de la mascota: dormido, despierto, muerto.
- Guardado persistente de datos en EEPROM (con protección para evitar fallos).
- Eventos de comida y condición de muerte.
- Cálculo de tiempo de vida y degradación automática de atributos (hambre, limpieza, felicidad).
- Textos y mensajes adicionales en el menú y acciones.
- Navegación mejorada en el menú con feedback visual.

### 🐛 Corregido
- Mejoras de estabilidad y protección contra crashes al guardar en EEPROM.
- Correcciones menores en la navegación y visualización de atributos.

### 🔄 Cambiado
- Refactorización de la estructura de menús y eventos.
- Mejoras en la visualización y feedback de acciones.

---
## [0.2.0] - 2024-12-21

### ✨ Añadido
- **Sistema de Mascota Virtual Completo**
  - Estructura de datos `Mascota` con 10 atributos
  - Fase de crecimiento (inicialmente "huevo")
  - Atributos de cuidado: salud, felicidad, saciado, limpieza, educación
  - Sistema de enfermedad (0-5)
  - Control de tiempo vivo (milisegundos)
  - Estados booleanos: isDead y despierto
  - Función `inicializarMascota()` con valores por defecto

- **Sistema de Menú Interactivo (8 secciones)**
  - Enumeración `SeccionMenu` para navegación clara
  - 7 secciones accesibles + 1 sección de alerta automática
  - Navegación cíclica con Botón 1 (0→6→0)
  - Sistema de feedback sonoro diferenciado por acción

- **Funciones de Control Diferenciadas para los 3 Botones**
  - **Botón 1**: `navegarMenu()` - Avanza por las secciones del menú
  - **Botón 2**: `ejecutarAccion()` - Ejecuta la acción de la sección actual
  - **Botón 3**: `cancelarAccion()` - Cancela y vuelve al menú principal
  - Cada botón tiene un tono de feedback único

- **6 Acciones de Cuidado Implementadas**
  - `darDeComer()`: Incrementa saciedad (máx. 5)
  - `acariciar()`: Incrementa felicidad (máx. 5)
  - `limpiar()`: Incrementa limpieza (máx. 5)
  - `disciplinar()`: Incrementa educación (máx. 5)
  - `curar()`: Reduce enfermedad (mín. 0)
  - `toggleLuz()`: Alterna entre despierto/dormido
  - Todas las acciones incluyen validación de límites

- **Sistema de Visualización**
  - `mostrarMenu()`: Muestra la sección actual del menú
  - `mostrarInformacion()`: Despliega todos los atributos de la mascota
  - Preparado para futuras mejoras con iconos gráficos

### 🔧 Cambiado
- **Reorganización completa del código**
  - Código dividido en secciones lógicas con comentarios separadores
  - Todas las funciones documentadas con comentarios explicativos
  - Nombres de variables y funciones más descriptivos
  - Estructura modular para facilitar futuras expansiones

- **Mejora del sistema de botones**
  - Los botones ahora tienen funciones específicas y diferenciadas
  - Sistema de debounce mejorado
  - Espera activa hasta que el botón sea liberado

- **Feedback sonoro mejorado**
  - Botón 1 (navegar): 800Hz, 50ms
  - Botón 2 (acción): 1200Hz, 100ms
  - Botón 3 (cancelar): 600Hz, 100ms
  - Tonos únicos permiten identificar acciones sin mirar la pantalla

### 📝 Documentación
- README actualizado con:
  - Descripción del sistema de menú de 8 secciones
  - Tabla de atributos de la mascota con valores iniciales
  - Tabla de secciones del menú con iconos y efectos
  - Instrucciones de navegación detalladas
  - Funcionalidades actuales vs futuras actualizadas
  - Referencia a Tamagotchi como inspiración

### 🐛 Corregido
- Prevención de valores fuera de rango en todos los atributos
- Las acciones ahora informan cuando ya no pueden ejecutarse (ej: "ya está llena")

### 📋 Notas de Desarrollo
- La sección 8 (MENU_ALERTA) está reservada para alertas automáticas futuras
- El menú salta de la sección 6 directamente a la 0 (no incluye sección 7 en navegación por error - a corregir)
- Los atributos usan escala 1-5 excepto:
  - Enfermedad: 0-5 (0 = sano)
  - Tiempo vivo: milisegundos
  - Estados booleanos: true/false
- El uso de `enum` facilita la lectura y mantenimiento del código
- Preparado para implementar sistema de degradación temporal

### 🔧 Configuración Técnica Actualizada
- **Estructura de datos:**
  - `struct Mascota`: 49 bytes aproximadamente
  - `enum SeccionMenu`: valores 0-7
  - Variable global `miMascota`
  - Variable global `menuActual`

- **Feedback Sonoro por Acción:**
  - Navegar menú: 800Hz
  - Ejecutar acción: 1200Hz  
  - Cancelar: 600Hz

### 🐛 Bugs Conocidos
- La navegación del menú incluye solo secciones 0-6, pero debería incluir sección 7 (Luz)
- La función `mostrarMenu()` actualmente solo muestra texto, pendiente implementar iconos
- No hay degradación temporal de atributos aún (la mascota no tiene hambre automáticamente)

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
| 0.2.0 | 2024-12-21 | Sistema de mascota virtual y menú interactivo |
| 0.1.0 | 2024-12-21 | Release inicial - Funcionalidad básica de botones y display |

---

## Próximas Versiones Planeadas

### [0.3.0] - Por definir
- Dibujo de iconos para las 8 secciones del menú
- Resaltado visual del icono seleccionado
- Animación básica de la mascota en pantalla central
- Corrección del bug de navegación del menú

### [0.4.0] - Por definir
- Sistema de degradación temporal (hambre/felicidad bajan cada X minutos)
- Detección automática de necesidades críticas
- Activación automática del icono de alerta (sección 8)
- Evolución de huevo a bebé después de X tiempo

### [0.5.0] - Por definir
- Primer minijuego (adivinar secuencia de botones)
- Diferentes animaciones según estado emocional
- Cálculo dinámico de salud
- Sistema de muerte (isDead = true cuando salud = 0)

### [1.0.0] - Por definir
- Mascota virtual completa y funcional
- Múltiples minijuegos
- Sistema completo de evolución (3+ fases)
- Animaciones avanzadas suaves
- Melodías variadas por acción
- Persistencia de datos robusta en EEPROM
- Sistema de muerte y reinicio con estadísticas
- Múltiples tipos de mascotas

---

## Contribuciones

Para sugerir cambios o reportar bugs, por favor documenta:
1. Versión actual del proyecto
2. Descripción detallada del cambio/bug
3. Pasos para reproducir (en caso de bugs)
4. Comportamiento esperado vs actual

---

**Última actualización**: 21 de diciembre de 2024
### ✨ Añadido
- Navegación con iconos visuales para cada sección del menú.
- Degradación automática de atributos (hambre, limpieza, felicidad bajan con el tiempo).
- Animaciones dinámicas y bitmaps para las fases huevo y adulto.
- Estructura lista para animaciones diferentes por fase (arrays por fase en dibujarPantalla).

### 🔧 Cambiado
- Eliminada toda la funcionalidad de "educar" (botón, variable, lógica y visualización) para optimizar memoria y simplificar el código.
- Limpieza de variables y funciones no utilizadas.
- Optimización del uso de memoria RAM en pantalla y gráficos.

### 🐛 Corregido
- Solucionado bug de error OLED relacionado con uso de memoria y textos.
- Mejorada la robustez del sistema tras eliminar funciones innecesarias.

### 📝 Documentación
- README y comentarios del código actualizados para reflejar la nueva estructura de animaciones, navegación y degradación automática.

### 🚧 Pendiente / Próximos pasos
- Añadir/definir bitmaps y animaciones para las fases bebé y anciano.
- Implementar el sistema de alerta (sección 8) para necesidades críticas.
- Añadir minijuegos y efectos visuales/sonoros avanzados.
- Mejorar el resaltado visual del icono seleccionado y feedback visual/sonoro.
