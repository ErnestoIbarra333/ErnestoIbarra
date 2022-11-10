import time
import board
import busio as io
import pyrebase
import sys

config = {
  "apiKey":"SqIGmPI4bYtQyoiWW6uEderrZRcZ4zxIXYgiQ7yb",
  "authDomain": "sdrpi-add4c.firebaseapp.com",
  "databaseURL": "https://sdrpi-add4c-default-rtdb.firebaseio.com/",
  "storageBucket": "sdrpi-add4c.appspot.com"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()

print("Send Data to Firebase Using Raspberry Pi")
print("————————————————————————————————————————")
print("")

while True:
    print("Enter Yes if you wish to send data to Firebase or No to stop the program: ")
    for line in sys.stdin:
        for var in line.split():
# while True
    if (var == "Yes"):
      ambientString = "gunshots"
      objectString = "X: 22, Y:25, Z: 10"
      print("Gun Type: ".format(ambientString))
      print("Location/Coordinates: ".format(objectString))
      print()

      data = {
        "Gun Type": ambientString,
        "Location": objectString,
      }
      db.child("mlx90614").child("1-set").set(data)
      db.child("mlx90614").child("2-push").push(data)
