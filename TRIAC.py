import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from threading import Thread
import time

# === CONFIGURACIÓN DEL PUERTO SERIAL ===
puerto = 'COM4'      # Cambia a tu puerto serial (Linux: '/dev/ttyUSB0')
baudios = 9600
arduino = serial.Serial(puerto, baudios, timeout=1)

# === VARIABLES PARA GRAFICACIÓN ===
tiempo = []
temperaturas = []
setpoints = []
potencias = []

start_time = time.time()

# === LECTURA Y PARSEO DE DATOS ===
def leer_datos():
    while True:
        try:
            linea = arduino.readline().decode('utf-8').strip()
            if 'Temp actual:' in linea:
                partes = linea.split('|')
                temp = float(partes[0].split(':')[1].replace('°C', '').strip())
                sp = float(partes[1].split(':')[1].replace('°C', '').strip())
                pot = float(partes[2].split(':')[1].replace('%', '').strip())

                t = time.time() - start_time

                tiempo.append(t)
                temperaturas.append(temp)
                setpoints.append(sp)
                potencias.append(pot)

                if len(tiempo) > 300:
                    tiempo.pop(0)
                    temperaturas.pop(0)
                    setpoints.pop(0)
                    potencias.pop(0)
        except Exception as e:
            print("Error leyendo datos:", e)

# === INGRESO DE SETPOINT DESDE CONSOLA ===
def entrada_usuario():
    while True:
        try:
            nuevo_sp = input("👉 Ingrese nuevo setpoint (°C): ")
            arduino.write((nuevo_sp + "\n").encode())
        except Exception as e:
            print("Error al enviar setpoint:", e)

# === ACTUALIZACIÓN DEL GRÁFICO ===
def actualizar(frame):
    plt.cla()
    plt.plot(tiempo, temperaturas, label='Temperatura actual (°C)', color='orange')
    plt.plot(tiempo, setpoints, label='Setpoint (°C)', linestyle='--', color='blue')
    plt.plot(tiempo, potencias, label='Potencia (%)', linestyle=':', color='green')
    plt.xlabel('Tiempo (s)')
    plt.ylabel('Valor')
    plt.title('Control de Temperatura con PID')
    plt.legend(loc='upper right')
    plt.grid(True)
    plt.tight_layout()

# === HILOS PARA LECTURA Y ENTRADA ===
hilo_lectura = Thread(target=leer_datos)
hilo_lectura.daemon = True
hilo_lectura.start()

hilo_setpoint = Thread(target=entrada_usuario)
hilo_setpoint.daemon = True
hilo_setpoint.start()

# === MOSTRAR GRÁFICO ===
fig = plt.figure()
ani = animation.FuncAnimation(fig, actualizar, interval=500)
plt.show()
