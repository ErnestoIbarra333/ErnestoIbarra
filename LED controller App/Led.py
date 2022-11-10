import RPi.GPIO as GPIO
import pyrebase
from time import sleep

config = {     
    "apiKey": "AIzaSyAO4-DKzmEFZTLMK5tvuQpLMgkL2Ym-tjE",
    "authDomain": "sdrpi-add4c.firebaseapp.com",
    "databaseURL": "https://sdrpi-add4c-default-rtdb.firebaseio.com/",
    "storageBucket": "sdrpi-add4c.appspot.com"
}

firebase = pyrebase.initialize_app(config)  

redLED = 12          
blueLED = 19
greenLED = 18

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(redLED,GPIO.OUT)
GPIO.setup(blueLED,GPIO.OUT)
GPIO.setup(greenLED,GPIO.OUT)

red_pwm = GPIO.PWM(redLED,1000)
blue_pwm = GPIO.PWM(blueLED,1000)
green_pwm = GPIO.PWM(greenLED,1000)

red_pwm.start(0)
blue_pwm.start(0)
green_pwm.start(0)

print("AH Shit! Here we go again! Press CTRL+C to exit")

def mode6 (redIntensity, greenIntensity, blueIntensity):
    for duty in range (0, redIntensity, 1):
        red_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.3)
    for duty in range (redIntensity - 1, -1, -1):
        red_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.3)
    for duty in range (0, blueIntensity, 1):
        blue_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.3)
    for duty in range (blueIntensity - 1, -1, -1):
        blue_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.3)
    for duty in range (0, greenIntensity, 1):
        green_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.3)
    for duty in range (greenIntensity - 1, -1 ,-1):
       green_pwm.ChangeDutyCycle(duty)
       sleep(0.01)
    sleep(0.3)

def mode5(redIntensity, greenIntensity, blueIntensity):
    for duty in range (0, redIntensity, 1):
        red_pwm.ChangeDutyCycle(duty)
        sleep(0.001)
    sleep(0.3)
    for duty in range (redIntensity - 1, -1, -1):
        red_pwm.ChangeDutyCycle(duty)
        sleep(0.001)
    sleep(0.3)
    for duty in range (0, blueIntensity, 1):
        blue_pwm.ChangeDutyCycle(duty)
        sleep(0.001)
    sleep(0.3)
    for duty in range (blueIntensity - 1 , -1, -1):
        blue_pwm.ChangeDutyCycle(duty)
        sleep(0.001)
    sleep(0.3)
    for duty in range (0, greenIntensity, 1):
        green_pwm.ChangeDutyCycle(duty)
        sleep(0.001)
    sleep(0.3)
    for duty in range (greenIntensity - 1 , -1, -1):
       green_pwm.ChangeDutyCycle(duty)
       sleep(0.001)
    sleep(0.3)

def mode4(redIntensity, greenIntensity, blueIntensity):
    for duty in range (0, redIntensity, 1):
        red_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.5)
    green_pwm.ChangeDutyCycle(0)
    sleep(0.2)
    green_pwm.ChangeDutyCycle(greenIntensity - 1)

    for duty in range (0, blueIntensity, 1):
        blue_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.5)
    blue_pwm.ChangeDutyCycle(0)
    sleep(0.2)
    blue_pwm.ChangeDutyCycle(blueIntensity - 1)

    for duty in range (0,greenIntensity,1):
        green_pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    sleep(0.5)
    red_pwm.ChangeDutyCycle(0)
    sleep(0.2)
    red_pwm.ChangeDutyCycle(redIntensity - 1)

    red_pwm.ChangeDutyCycle(0)
    green_pwm.ChangeDutyCycle(0)
    blue_pwm.ChangeDutyCycle(0)

def mode3(redIntensity, greenIntensity, blueIntensity):
    red_pwm.ChangeDutyCycle(0)
    green_pwm.ChangeDutyCycle(0)
    blue_pwm.ChangeDutyCycle(0)

    red_pwm.ChangeDutyCycle(redIntensity - 1)
    blue_pwm.ChangeDutyCycle(blueIntensity - 1)
    green_pwm.ChangeDutyCycle(greenIntensity - 1)

def mode2(redIntensity, greenIntensity, blueIntensity):
    red_pwm.ChangeDutyCycle(0)
    green_pwm.ChangeDutyCycle(0)
    blue_pwm.ChangeDutyCycle(0)

    blue_pwm.ChangeDutyCycle(blueIntensity - 1)
    sleep(0.1)
    blue_pwm.ChangeDutyCycle(0)
    sleep(0.1)
    green_pwm.ChangeDutyCycle(greenIntensity - 1)
    sleep(0.1)
    green_pwm.ChangeDutyCycle(0)
    sleep(0.1)
    red_pwm.ChangeDutyCycle(redIntensity - 1)
    sleep(0.1)
    red_pwm.ChangeDutyCycle(0)
    sleep(0.1)

def mode1(redIntensity, greenIntensity, blueIntensity):
    red_pwm.ChangeDutyCycle(redIntensity - 1)
    sleep(0.02)
    red_pwm.ChangeDutyCycle(0)
    sleep(0.02)
    red_pwm.ChangeDutyCycle(redIntensity - 1)
    sleep(0.02)
    red_pwm.ChangeDutyCycle(0)
    sleep(0.02)

    green_pwm.ChangeDutyCycle(greenIntensity - 1)
    sleep(0.02)
    green_pwm.ChangeDutyCycle(0)
    sleep(0.02)
    green_pwm.ChangeDutyCycle(greenIntensity - 1)
    sleep(0.02)
    green_pwm.ChangeDutyCycle(0)
    sleep(0.02)

    blue_pwm.ChangeDutyCycle(blueIntensity - 1)
    sleep(0.02)
    blue_pwm.ChangeDutyCycle(0)
    sleep(0.02)
    blue_pwm.ChangeDutyCycle(blueIntensity - 1)
    sleep(0.02)
    blue_pwm.ChangeDutyCycle(0)
    sleep(0.02)

try:
    while True:

        database = firebase.database()
        RGBControlBucket = database.child("RGBControl")
        powerState = RGBControlBucket.child("powerState").get().val()

        if "true" in powerState.lower():
            database = firebase.database()
            RGBControlBucket = database.child("RGBControl")
            redLightIntensity = RGBControlBucket.child("redLightIntensity").get().val()

            database = firebase.database()
            RGBControlBucket = database.child("RGBControl")
            blueLightIntensity = RGBControlBucket.child("blueLightIntensity").get().val()

            database = firebase.database()
            RGBControlBucket = database.child("RGBControl")
            greenLightIntensity = RGBControlBucket.child("greenLightIntensity").get().val()

            database = firebase.database()
            RGBControlBucket = database.child("RGBControl")
            lightPresetMode = RGBControlBucket.child("lightMode").get().val()

            if "mode6" in lightPresetMode.lower():
                mode6(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
            elif "mode5" in lightPresetMode.lower():
                mode5(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
            elif "mode4" in lightPresetMode.lower():
                mode4(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
            elif "mode3" in lightPresetMode.lower():
                mode3(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
            elif "mode2" in lightPresetMode.lower():
                mode2(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
            elif "mode1" in lightPresetMode.lower():
                mode1(int(redLightIntensity), int(greenLightIntensity), int(blueLightIntensity))
        else:
            print("DAMN, the power state is: " + powerState)


except KeyboardInterrupt:
    red_pwm.stop()
    blue_pwm.stop()
    green_pwm.stop()
    GPIO.cleanup()