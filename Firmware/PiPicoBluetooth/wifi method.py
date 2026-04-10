import network
import socket
from machine import Pin
import time

# User added start (1)
import motor
# User added end (1)

# ========================
# WIFI SETUP
# ========================
SSID = "YOUR_WIFI_NAME"
PASSWORD = "YOUR_WIFI_PASSWORD"

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(SSID, PASSWORD)

print("Connecting to WiFi...")
while not wlan.isconnected():
    time.sleep(1)

ip = wlan.ifconfig()[0]
print("Connected! IP:", ip)

# ========================
# MOTOR SETUP
# ========================
in1 = Pin(2, Pin.OUT)
in2 = Pin(3, Pin.OUT)
in3 = Pin(4, Pin.OUT)
in4 = Pin(5, Pin.OUT)

def stop():
    in1.low()
    in2.low()
    in3.low()
    in4.low()

def forward():
    in1.high()
    in2.low()
    in3.high()
    in4.low()

def backward():
    in1.low()
    in2.high()
    in3.low()
    in4.high()

def left():
    in1.low()
    in2.high()
    in3.high()
    in4.low()

def right():
    in1.high()
    in2.low()
    in3.low()
    in4.high()

# ========================
# HEADLIGHT SETUP
# ========================
light = Pin(15, Pin.OUT)

def light_on():
    light.high()

def light_off():
    light.low()

# ========================
# WEBPAGE (optional UI)
# ========================
def webpage():
    return """<!DOCTYPE html>
<html>
<head>
<title>RC Car</title>
<style>
button { width:100px; height:50px; margin:5px; font-size:16px; }
</style>
</head>
<body>
<h2>RC Controller</h2>

<button onclick="fetch('/forward')">Forward</button><br>
<button onclick="fetch('/left')">Left</button>
<button onclick="fetch('/stop')">Stop</button>
<button onclick="fetch('/right')">Right</button><br>
<button onclick="fetch('/backward')">Backward</button><br><br>

<button onclick="fetch('/light_on')">Lights ON</button>
<button onclick="fetch('/light_off')">Lights OFF</button>

</body>
</html>
"""

# ========================
# SERVER SETUP
# ========================
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
server = socket.socket()
server.bind(addr)
server.listen(1)

print("Server running at http://" + ip)

# ========================
# MAIN LOOP
# ========================
while True:
    client, addr = server.accept()
    request = client.recv(1024).decode()

    # Debug print
    print(request)

    # ========================
    # CONTROL LOGIC
    # ========================
    if '/forward' in request:
        forward()

    elif '/backward' in request:
        backward()

    elif '/left' in request:
        left()

    elif '/right' in request:
        right()

    elif '/stop' in request:
        stop()

    elif '/light_on' in request:
        light_on()

    elif '/light_off' in request:
        light_off()

    # ========================
    # RESPONSE
    # ========================
    response = webpage()
    client.send("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n")
    client.send(response)
    client.close()