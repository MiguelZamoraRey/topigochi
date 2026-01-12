# Instrucciones para el Jugador

Bienvenido a Topigochi, tu mascota virtual estilo Tamagotchi. Aquí encontrarás toda la información necesaria para jugar y cuidar de tu Topigochi.

## Objetivo del Juego
Mantén a tu Topigochi feliz, saludable y entretenido. Cuida sus necesidades básicas y observa cómo crece y evoluciona.

## Controles Básicos

### Botones
- **Botón 1 (Verde):** Navega por el menú
- **Botón 2 (Azul):** Ejecuta la acción seleccionada
- **Botón 3 (Rojo):** Cancelar/Control de luz

### Secciones del Menú
1. **📋 Información** - Muestra el estado de tu mascota
2. **🍽️ Comer** - Alimenta tu Topigochi
3. **❤️ Acariciar** - Aumenta su felicidad
4. **🛁 Limpiar** - Mejora su higiene
5. **📚 Disciplinar** - Aumenta su educación
6. **💊 Curar** - Reduce su enfermedad
7. **💡 Luz** - Pone a dormir/despierta

## Indicadores de Estado

| Atributo | Rango | Qué significa |
|----------|-------|--------------|
| **Salud** | 0-10 | Estado físico general |
| **Felicidad** | 0-10 | Nivel de alegría |
| **Hambre** (Saciado) | 0-10 | Cuánto necesita comer |
| **Limpieza** | 0-10 | Cuánta higiene tiene |
| **Educación** | 0-10 | Comportamiento |
| **Enfermedad** | 0-5 | Nivel de infección |

## Consejos Básicos

- ✅ **Atiende las necesidades regularmente** - Los eventos ocurren en intervalos aleatorios
- ✅ **Si algún indicador baja, actúa rápido** - La mascota puede morir si descuidas algo
- ✅ **Balancea el cuidado** - No solo alimentes, también juega y mantén limpio
- ✅ **Observa los patrones** - Los eventos son impredecibles, ¡como un Tamagotchi real!

## ⚠️ Importante

**No hay guardado persistente**
- Tu Topigochi se reinicia completamente cada vez que apagas el Arduino
- Esto es intencional para evitar desgaste del hardware
- Cada partida es una experiencia nueva

---

## Instrucciones Avanzadas

### Fases de Topigochi
Topigochi pasa por diferentes fases según el tiempo acumulado:

- **🥚 Fase Huevo:** Dura 6 segundos (modo demo). No puede realizar acciones.
- **👶 Fase Bebé:** Dura 24 horas. Necesita alimentación cada ~2 horas.
- **👦 Fase Adulto:** Dura 72 horas. Más independiente, intervalos más largos.
- **👴 Fase Anciano:** Dura 24 horas. Necesidades más frecuentes nuevamente.

### Sistema de Eventos Independientes

A diferencia de versiones anteriores, **cada necesidad tiene su propio cronómetro**:

- **Hambre** → Cada 2 horas (±30min) aproximadamente
- **Limpieza** → Cada 2 horas (±30min) aproximadamente
- **Aburrimiento** → Cada 2 horas (±30min) aproximadamente
- **Maldad/Educación** → Cada 24 horas (±30min)
- **Enfermedad** → Cada 24 horas (±30min)

La variación aleatoria (±30 minutos) hace que **nunca sepas exactamente cuándo tendrá hambre tu mascota**, creando tensión y sorpresas como en el Tamagotchi original.

### Apagar la Luz
Cuando presionas "Luz":
- Topigochi entra en modo descanso (pantalla se apaga)
- Vuelve a presionar para despertar
- El tiempo sigue corriendo en el fondo

### Curación
Si tu Topigochi se enferma:
- Entra en el menú "Curar"
- Cada curación reduce la enfermedad en 1 punto
- Si llega a 5 puntos, se muere
- ¡Cura antes de que sea tarde!

### Monitoreo Serial (Debug)
Si conectas el Arduino a una computadora:
- Abre el Serial Monitor (9600 baud)
- Verás logs de eventos, cambios de fase y estado del sistema
- Útil para entender qué está pasando con tu mascota

---

## Estrategias para Ganar

1. **Comprueba regularmente** - Abre el menú de información frecuentemente
2. **Mantén balance** - No dejes ningún atributo muy bajo
3. **Aprende los patrones** - Los eventos son aleatorios pero dentro de rangos predecibles
4. **Reacciona rápido** - Cuando veas un atributo bajo, actúa inmediatamente
5. **Duerme cuando baje mucho** - A veces descansar ayuda

---

¡Ahora tienes toda la información para convertirte en un experto cuidando a tu Topigochi!
