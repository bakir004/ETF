from machine import Pin, ADC, Timer, PWM
import network
import time
from umqtt.robust import MQTTClient
import ujson
import dht

t1 = Pin(0, Pin.IN)
potenciometar = ADC(Pin(26))
dht_sensor = dht.DHT11(Pin(28))

led0 = Pin(4, Pin.OUT)  
led1 = Pin(5, Pin.OUT)   
led2 = Pin(6, Pin.OUT) 
led3 = PWM(Pin(7))    
green = PWM(Pin(12)) 
blue = PWM(Pin(13)) 
red = PWM(Pin(14)) 

red.freq(1000)
green.freq(1000)
blue.freq(1000)

pot_value = 0.0

nic = network.WLAN(network.STA_IF)
nic.active(True)
nic.connect('Lab220', 'lab220lozinka')
while not nic.isconnected():
    time.sleep(5)
print(nic.ifconfig())

def sub(topic, msg):
    print('Tema:', topic)
    print('Poruka:', msg)

    if topic == b'bakeroni/led0':
        led0.value(int(msg))
    if topic == b'bakeroni/led1':
        led1.value(int(msg))
    if topic == b'bakeroni/led2':
        led2.value(int(msg))
    if topic == b'bakeroni/led3':
        led3.duty_u16(int(float(msg) * 65535))
   
    if topic == b'bakeroni/red':
        red.duty_u16(int(float(msg) * 65535))
    if topic == b'bakeroni/green':
        green.duty_u16(int(float(msg) * 65535))
    if topic == b'bakeroni/blue':
        blue.duty_u16(int(float(msg) * 65535))

def pot_publish(t):
    global pot_value
    pot = potenciometar.read_u16() / 65535.0
    if abs(pot - pot_value) > 0.05:
        mqtt_conn.publish(b'bakeroni/potenciometar', str(pot).encode())
        pot_value = pot

def dht_publish(t):
    try:
        dht_sensor.measure()
        temperature = dht_sensor.temperature()
        humidity = dht_sensor.humidity()
        mqtt_conn.publish(b'bakeroni/dht', ujson.dumps({
            "temperature": temperature,
            "humidity": humidity
        }).encode())
    except Exception as e:
        print("Greška u DHT:", e)

mqtt_conn = MQTTClient(client_id='bakeroni', server='broker.hivemq.com', port=1883)
mqtt_conn.set_callback(sub)
mqtt_conn.connect()
mqtt_conn.subscribe(b"bakeroni/#")

t1.irq(trigger=Pin.IRQ_RISING, handler=lambda p: mqtt_conn.publish(b'bakeroni/t1', b'1'))

tiky = Timer()
tiky.init(mode=Timer.PERIODIC, period=200, callback=pot_publish)

dht_timer = Timer()
dht_timer.init(mode=Timer.PERIODIC, period=2000, callback=dht_publish)

while True:
    mqtt_conn.wait_msg()  
    time.sleep(1)

