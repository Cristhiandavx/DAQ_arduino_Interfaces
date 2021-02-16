# DAQ_arduino_Interfaces
Arduino based Data Acquisition Interfaces for various projects

Scripts para emplear arduino como herramienta de adquisición y procesamiento de datos mediante sensores y actuadores

Contenido:
Script de interpretación de señales de un sensor IMU 9250
- Los datos son filtrados ligeramente mediante algoritmos para el cálculo de los angulos Roll Pitch y Yaw
Script para adquisición de señales analógicas de un sensor de voltaje
- Los datos son anexados en la salida general
Script de adquisición de datos de corriente mediante un sensor de corriente
- Los datos son anexados en la salida general
Script que recibe datos mediante serial para la activación remota de actuadores

Salida general:
Se dispone de un orden igual para cada valor mostrado, de tal manera a cubrir 8bits por cada valor, por ejemplo
- Salida del acelerometro en X = 0.987212, se imprime como > "  0.9872"
Cada valor se muestra con una separacion mediante comas.
- AccX, AccY, AccZ, ...

Para realizar acciones se requiere conectar los sensores de la siguiente forma:
-IMU9250 > pines I2C
-INA219 > pines I2C 
-Standard Voltage sensor > A6
-LED > D2 /n
-BUZZER > D3 /n

Al ejecutar se observa la salida de los datos linea por linea.

Para enviar un comando se utiliza la consola del puerto Serial:
-"A" = Play melody. Toca una melodía y luego avisa por Serial
-"B" = Enciende el LED y luego avisa por Serial
-"b" = Apaga el LED y luego avisa por Serial

Durante el envio de comandos se detiene la adquisición de los datos del IMU. Al terminal el comando se reanuda la presentación.
