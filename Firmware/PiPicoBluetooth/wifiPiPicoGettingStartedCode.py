import network
import socket
from time import sleep
import machine

# on board led
led_pin = machine.Pin('LED', machine.Pin.OUT)

# ------------------------
# Access point setup
# ------------------------
def start_ap():
    access_point = network.WLAN(network.AP_IF)
    access_point.active(True)

    access_point.config(essid='Team37Network', password='Team37UNOS')

    while not access_point.active(): #do we need???
        pass

    print("Access Point Started")
    print("IP:", access_point.ifconfig()[0])

    led_pin.on()  # turn on LED when ready

    #return ip address
    return access_point.ifconfig()[0]


# ------------------------
# Socket
# ------------------------
def start_server(ip):
    address = socket.getaddrinfo(ip, 80)[0][-1]
    connection = socket.socket()
    connection.bind(addr)
    connection.listen(1)

    print("Server running at http://" + ip)
    return connection


# ------------------------
# Server
# ------------------------
def server(connection):
    while True:
        client, addr = s.accept()
        request = client.recv(1024).decode()
        print(request)

        # Example control
        if '/on' in request:
            led_pin.on()

        elif '/off' in request:
            led_pin.off()


# ------------------------
# HTML Page
# ------------------------
def webpage():
    html = """HTTP/1.1 200 OK

    <html>
    <body>
    <h2>Pico Control</h2>
    <a href="/on"><button>LED ON</button></a>
    <a href="/off"><button>LED OFF</button></a>
    </body>
    </html>
    """

    client.send(response)
    client.close()


# ------------------------
# main
# ------------------------

ip = start_ap()
start_server(ip)








# import network
# import socket
# from time import sleep
# 
# import machine
# import rp2
# import sys
# 
# # setup onboard led
# led_pin = machine.Pin('LED', machine.Pin.OUT)
# output_pin = machine.Pin(0, machine.Pin.OUT)
# 
# 
# 
# # input current network ssid and password
# ssid = 'airuc-secure'
# auth = WLAN.WPA2_ENT,
# 
# # setup WLAN object
# def connect():
#     # Connect to WLAN
#     wlan = network.WLAN(network.STA_IF)
#     wlan.active(True)
#     wlan.connect(ssid='mywifi', auth=(WLAN.WPA2_ENT, 'username', 'password'), [identity='myidentity', ca_certs='/flash/cert/ca.pem'])
#     
#     # loop to keep requesting for connection until True
#     while (wlan.isconnected() == False):
#         print('Waiting for connection...')
#         sleep(1)
#     
#     #print wlan configuration
#     print(wlan.ifconfig())
#     
# 
# # main
# connect()