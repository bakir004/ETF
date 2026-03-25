from machine import Pin, PWM, ADC
import time

ldr = ADC(26)

led_pins = [4, 5, 6, 7, 8, 9, 10, 11]
leds = [PWM(Pin(pin)) for pin in led_pins]

for led in leds:
    led.freq(1000)

adc_min =  57591  
adc_max = 65535   

TOTAL_STEPS = 80
MAX_PWM = 65535

led_max_intensity = [
    int(MAX_PWM * 0.03),  
    int(MAX_PWM * 0.07),  
    int(MAX_PWM * 0.1), 
    int(MAX_PWM * 0.2),  
    int(MAX_PWM * 0.3), 
    int(MAX_PWM * 0.4), 
    int(MAX_PWM * 0.6), 
    int(MAX_PWM * 1.0),  
]

def map_adc_to_step(adc_value):
    return max(0, min(int((adc_value - adc_min) * TOTAL_STEPS / (adc_max - adc_min)), TOTAL_STEPS))
def get_led_intensities(step):
    intensities = [0] * 8
    step_per_led = TOTAL_STEPS // 8  

    for i in range(8):
        led_start = i* step_per_led
        led_end = (i + 1) * step_per_led

        if step >= led_end:
            intensities[i] = led_max_intensity[i]
        elif step >= led_start:
            local_step = step - led_start
            scale = local_step / step_per_led
            intensities[i] = int(scale * led_max_intensity[i])
        else:
            intensities[i] = 0

    return intensities

while True:
    adc_val = ldr.read_u16()
    step = map_adc_to_step(adc_val)
    led_values = get_led_intensities(step)

    for i in range(8):
        leds[i].duty_u16(led_values[i])

    voltage = ((adc_val - adc_min) / (adc_max - adc_min)) * 3.3
    voltage = max(0.0, min(voltage, 3.3))  
    print(voltage)

    time.sleep(0.05)
