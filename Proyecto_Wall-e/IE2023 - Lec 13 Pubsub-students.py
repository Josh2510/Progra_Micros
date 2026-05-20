# Import standard python modules.
import sys
import time
import serial

# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient
from Adafruit_IO import Client, Feed

# holds the count for the feed
run_count = 0

# Set to your Adafruit IO username and key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_AIO_USERNAME = "user"
ADAFRUIT_AIO_KEY      = "password"

# Set to the ID of the feed to subscribe to for updates.
# FEED DE LOS QUE SE DEBE DE RECIBIR ALGUN VALOR DESDE ADA FRUIT
FEED_ID_motor1 = 'motor-1'
FEED_ID_motor2 = 'motor-2'
FEED_ID_motor3 = 'motor-3'
FEED_ID_motor4 = 'motor-4'
FEED_ID_motor5 = 'motor-5'
FEED_ID_motor6 = 'motor-6'
FEED_ID_estado = 'estado'
FEED_ID_eeprom = 'posicion-eeprom'

# FUNCIONES ADICIONALES
def cadena_datos(d1, d2):
    return bytes([ord(d1), ord(d2) if isinstance(d2, str) else d2])

def velocidad(valor_vel):
    valor_vel = max(-50, min(50, valor_vel))
    duty = int((valor_vel + 50)/(100) * 255)
    return max(0, min(255, duty))

def angulo(valor_ang):
    valor_ang = max(0, min(180, valor_ang))
    duty = int(valor_ang * 255/180)
    return max(0, min(255, duty))

def mandar_mensaje(dato):
    if dato == b'M\n':
        client.publish('estado', 'M')
    elif dato == b'E\n':
        client.publish('estado', 'E')
    elif dato == b'S\n':
        client.publish('estado', 'S')
    elif dato == b'G\n':
        print("Guardando en EEPROM")

# Define "callback" functions which will be called when certain events 
# happen (connected, disconnected, message arrived).
def connected(client):
    # Subscribe to changes on a feed named Counter.
    print('Subscribing to Feed {0}'.format(FEED_ID_motor1))
    client.subscribe(FEED_ID_motor1)
    client.subscribe(FEED_ID_motor2)
    client.subscribe(FEED_ID_motor3)
    client.subscribe(FEED_ID_motor4)
    client.subscribe(FEED_ID_motor5)
    client.subscribe(FEED_ID_motor6)
    client.subscribe(FEED_ID_estado)
    client.subscribe(FEED_ID_eeprom)
    print('Waiting for feed data...')

def disconnected(client):
    sys.exit(1)

def message(client, feed_id, payload):
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    
    # Publish or "send" message to corresponding feed
    match feed_id:
         # FEED PARA INDICARLE AL NANO EN QUE ESTADO ESTA Y QUE VALORES DEBE DE RECIBIR
        case "estado":
            dato = cadena_datos("E", payload)

        # FEED PARA INDICARLE AL NANO QUE POSICION DE LA EEPROM DEBE DE MOSTRAR
        case "posicion-eeprom": 
            dato = cadena_datos("P", int(payload))
        
        # FEED DE MOTOR SERVO PARA BRAZO DERECHO
        case "motor-1": 
            payload = angulo(float(payload))
            dato = cadena_datos("U", payload)

        # FEED DE MOTOR SERVO PARA BRAZO IZQUIERDO
        case "motor-2": 
            payload = angulo(float(payload))
            dato = cadena_datos("V", payload)

        # FEED DE MOTOR SERVO PARA OJO DERECHO
        case "motor-3": 
            payload = angulo(float(payload))
            dato = cadena_datos("W", payload)

        # FEED DE MOTOR SERVO PARA OJO IZQUIERDO
        case "motor-4": 
            payload = angulo(float(payload))
            dato = cadena_datos("X", payload)

        # FEED DE MOTOR DC PARA LA RUEDA DERECHA
        case "motor-5": 
            payload = velocidad(float(payload))
            dato = cadena_datos("Y", payload)

        # FEED DE MOTOR DC PARA LA RUEDA IZQUIERDA
        case "motor-6": 
            payload = velocidad(float(payload))
            dato = cadena_datos("Z", payload)

    print('Feed {0} manda a arduino: {1}'.format(feed_id, dato))
    myArduino.write(dato)

try:
    # Create an MQTT client instance.
    client = MQTTClient(ADAFRUIT_AIO_USERNAME, ADAFRUIT_AIO_KEY)
    myArduino = serial.Serial(port='COM6', baudrate=9600, timeout=0.1)

    # Setup the callback functions defined above.
    client.on_connect = connected
    client.on_disconnect = disconnected
    client.on_message = message

    # Connect to the Adafruit IO server.
    client.connect()

    # The first option is to run a thread in the background so you can continue
    # doing things in your program.
    client.loop_background()

    while True:
        dato = myArduino.readline()
        mandar_mensaje(dato)
        print('Running "main loop" ')
        time.sleep(3)
        
except KeyboardInterrupt:
    print("Adios")
    if myArduino.is_open:
        myArduino.close()
    sys.exit(1)