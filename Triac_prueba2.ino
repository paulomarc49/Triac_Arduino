#include <PID_v1.h>
#include <TimerOne.h>

#define TRIAC_PIN 3       // Pin digital para disparo del triac
#define LM35_PIN A0       // Pin analógico donde se conecta el LM35

// Variables PID
double setpoint = 10.0;   // Valor deseado de temperatura
double temperature;       // Temperatura actual
double output;            // Salida del PID (0 a 100%)
double Kp = 5, Ki = 0.1, Kd = 0.1;
PID pid(&temperature, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// Dimmer por cruce por cero
volatile int i = 0;
volatile boolean cruce_cero = false;
int dim = 0;
const int MAX_DELAY = 83; // Mayor retardo = menor potencia
const int T_int = 100;

// Tiempos
unsigned long ultimaLectura = 0;
unsigned long ultimaMuestra = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ingrese el setpoint de temperatura (°C):");

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(0, 100); // PID genera valores entre 0–100 %

  pinMode(TRIAC_PIN, OUTPUT);
  digitalWrite(TRIAC_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(2), deteccion_Cruce_cero, RISING); // Pin 2 a cruce por cero
  Timer1.initialize(T_int);          // Timer cada 100 us
  Timer1.attachInterrupt(Dimer);     // Llama a Dimer() en cada interrupción
}

void loop() {
  // Leer temperatura del LM35 cada 500 ms
  if (millis() - ultimaLectura >= 100) {
    int raw = analogRead(LM35_PIN);              // Lectura analógica
    float voltage = raw * (5.0 / 1023.0);         // Convertir a voltios
    temperature = voltage * 100.0;                // LM35: 10 mV/°C

    pid.Compute();                                // Calcular salida PID
    dim = map((int)output, 0, 100, MAX_DELAY, 0); // Convertir % a retardo

    ultimaLectura = millis();
  }

  // Mostrar info cada 500 ms
  if (millis() - ultimaMuestra >= 300) {
    Serial.print("Temp actual: ");
    Serial.print(temperature, 1);
    Serial.print(" °C | Setpoint: ");
    Serial.print(setpoint, 1);
    Serial.print(" °C | Potencia: ");
    Serial.print((int)output);
    Serial.println(" %");
    ultimaMuestra = millis();
  }

  // Leer nuevo setpoint desde Monitor Serial
  if (Serial.available()) {
    String entrada = Serial.readStringUntil('\n');
    entrada.trim(); // Quitar espacios/saltos
    if (entrada.length() > 0) {
      float nuevoSetpoint = entrada.toFloat();
      if (nuevoSetpoint > 0 && nuevoSetpoint < 150) {
        setpoint = nuevoSetpoint;
        Serial.print("Nuevo setpoint: ");
        Serial.print(setpoint, 1);
        Serial.println(" °C");
      } else {
        Serial.println("⚠️ Setpoint fuera de rango. Intente con un valor entre 0 y 150.");
      }
    }
  }
}

// Interrupción por cruce por cero (pin 2)
void deteccion_Cruce_cero() {
  cruce_cero = true;
  i = 0;
  digitalWrite(TRIAC_PIN, LOW);
}

// Función de disparo del triac
void Dimer() {
  if (cruce_cero) {
    if (i >= dim) {
      digitalWrite(TRIAC_PIN, HIGH);
      cruce_cero = false;
    } else {
      i++;
    }
  }
}
