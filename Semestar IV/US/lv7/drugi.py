import machine
import network
from machine import Pin, ADC, Timer, PWM
import time
from umqtt.robust import MQTTClient
import ujson

t1 = Pin(0)
potenciometar = ADC(Pin(26))
led0 = Pin(4, Pin.OUT)
led1 = Pin(5, Pin.OUT)
led2 = Pin(6, Pin.OUT)
led3 = PWM(Pin(7, Pin.OUT))
green = PWM(Pin(12, Pin.OUT))
blue = PWM(Pin(13, Pin.OUT))
red = PWM(Pin(14, Pin.OUT))
pot_value = 0.0

nic = network.WLAN(network.STA_IF)
nic.active(True)
nic.connect('Lab220', 'lab220lozinka')

while not nic.isconnected():
   print("Čekam konekciju ...")
   time.sleep(5)

print("WLAN konekcija uspostavljena")
ipaddr = nic.ifconfig()[0]
print("Mrežne postavke:")
print(nic.ifconfig())

def sub(topic, msg):
   print('Tema: ' + str(topic))
   print('Poruka: ' + str(msg))
   
   if topic == b'bakeroni/led0':
       if msg == b'1':
           led0.on()
       else:
           led0.off()
           
   if topic == b'bakeroni/led1':
       if msg == b'1':
           led1.on()
       else:
           led1.off()
           
   if topic == b'bakeroni/led2':
       if msg == b'1':
           led2.on()
       else:
           led2.off()
           
   if topic == b'bakeroni/led3':
       led3.duty_u16(int((float(msg)) * 65535))
       
   if topic == b'bakeroni/red':
       red.duty_u16(int((float(msg)) * 65535))
       
   if topic == b'bakeroni/green':
       green.duty_u16(int((float(msg)) * 65535))
       
   if topic == b'bakeroni/blue':
       blue.duty_u16(int((float(msg)) * 65535))

def t1_publish(p):
   mqtt_conn.publish(b'bakeroni/t1', b'1')

def t2_publish(p):
   mqtt_conn.publish(b'bakeroni/t2', b'1')

def t3_publish(p):
   mqtt_conn.publish(b'bakeroni/t3', b'1')

def t4_publish(p):
   mqtt_conn.publish(b'bakeroni/t4', b'1')

def pot_publish(t):
   global pot_value
   pot = potenciometar.read_u16() / 65535.
   if abs(pot - pot_value) > 0.05:
       mqtt_conn.publish(b'bakeroni/potenciometar', str(potenciometar.read_u16() / 65535.))
       pot_value = pot
   pass

mqtt_conn = MQTTClient(client_id='bakeroni', server='broker.hivemq.com', user='', password='', port=1883)
mqtt_conn.set_callback(sub)
mqtt_conn.connect()
mqtt_conn.subscribe(b"bakeroni/#")
print("Konekcija sa MQTT brokerom uspostavljena")

t1.irq(trigger=Pin.IRQ_RISING, handler=t1_publish)

tiky = Timer(mode=Timer.PERIODIC, period=200, callback=pot_publish)

while True:
   mqtt_conn.wait_msg()
   time.sleep(1) 
