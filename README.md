# 🔥 Control de Temperatura con TRIAC y PID usando Arduino

Este proyecto permite controlar la temperatura de un sistema eléctrico (como una resistencia calefactora) utilizando un TRIAC, un sensor de temperatura LM35 y un controlador PID implementado en Arduino.

El sistema regula la potencia del TRIAC mediante control de ángulo de fase (con cruce por cero) para mantener la temperatura deseada (setpoint).

## 🧰 Materiales Necesarios

### Hardware:
- Arduino UNO (u otro compatible)
- TRIAC (como el BTA16) + optoacoplador MOC3021 o MOC3041
- Sensor de temperatura LM35
- Diodo para detección de cruce por cero (o módulo ZCD)
- Resistencias y cables
- Fuente de alimentación adecuada

### Software:
- Arduino IDE
- Librerías:
  - `PID_v1`
  - `TimerOne`

---

## 🔧 Instalación de Librerías (para usuarios amateur)

### Paso 1: Abrir el Administrador de Librerías
1. Abre el **Arduino IDE**.
2. Ve al menú: **Programa > Incluir Librería > Administrar bibliotecas…**

### Paso 2: Buscar e instalar librerías
En la ventana del gestor de librerías:

- Escribe `PID` en el cuadro de búsqueda. Busca **PID by Brett Beauregard** y haz clic en **Instalar**.
- Luego busca `TimerOne` y selecciona **TimerOne by Paul Stoffregen**. Haz clic en **Instalar**.

¡Listo! Ya tienes lo necesario para compilar el programa.

---

## ⚙️ Cómo usar el programa

1. **Conecta el LM35** al pin analógico A0.
2. **Conecta el pin de control del TRIAC** al pin digital 3.
3. **Conecta el detector de cruce por cero** al pin digital 2.
4. Sube el programa a tu Arduino desde el IDE.
5. Abre el **Monitor Serial** a 9600 baudios.
6. Ingresa el setpoint deseado en °C (por ejemplo, `50`) y presiona Enter.
7. El sistema comenzará a ajustar la potencia del TRIAC para mantener la temperatura.

---

## 📈 ¿Cómo funciona?

- El sensor **LM35** mide la temperatura ambiente.
- El controlador **PID** calcula cuánta potencia necesita aplicar al TRIAC para alcanzar el setpoint.
- El sistema dispara el TRIAC en el momento adecuado tras el **cruce por cero**, regulando el ángulo de conducción.

---

## 📋 Notas Importantes

⚠️ **Este proyecto involucra corriente alterna (AC)**. Si no tienes experiencia trabajando con AC, por favor solicita ayuda a un experto. ¡La seguridad es lo primero!

---

## 🧪 Créditos y Agradecimientos

Desarrollado por Luis Paolo Marcial Sánchez. Basado en librerías de:
- [Brett Beauregard (PID Library)](https://github.com/br3ttb/Arduino-PID-Library)
- [Paul Stoffregen (TimerOne)](https://github.com/PaulStoffregen/TimerOne)

---

## 📸 Imagen de ejemplo (opcional)
Puedes agregar un diagrama de conexión o una foto del montaje real si lo deseas.

---

## ✅ Licencia

Este proyecto está licenciado bajo la [MIT License](LICENSE).
