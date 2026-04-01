import network
import socket
from time import sleep
import machine

# private imports
import motor

# leds
led_pin = machine.Pin('LED', machine.Pin.OUT)
headlight = machine.Pin(0, machine.Pin.OUT)				#HEYHEYHEY CHANGE THE PINS SIL VOUS PLEASE

# ------------------------
# Access point setup
# ------------------------
def start_ap():
    access_point = network.WLAN(network.AP_IF)
    access_point.active(True)

    access_point.config(essid='Team37Network', password='Team37UNOS')

    print("Access Point Started")
    print("IP:", access_point.ifconfig()[0])

    led_pin.on()  # turn on LED when ready

    #return ip
    return access_point.ifconfig()[0]

# ------------------------
# HTML Page
# ------------------------
def webpage():

    html = """HTTP/1.1 200 OK

<!DOCTYPE html>
        <html>
    
    <head>
		<style>
			body 	{background-color: pink;
    				 text-align: center;}
			h1   	{color: black;}
    		button	{background-color: white;
          			 padding: 10px 20px;
             		 text-align: center;
         		     border-radius: 4px;
          	 		 margin: 3px;}
  		</style>
	</head>


    <body>
    	<h1>Remote Control</h1>
    		<button id="headlight" onclick="toggleLight()">Light</button>
              <script>
              let headLightsOn = false;
              function toggleLight() {
                  const btn = document.getElementById("headlight");
                  headLightsOn = !headLightsOn;
                  if (headLightsOn) {
                      btn.style.backgroundColor = "yellow";
                  } else {
                      btn.style.backgroundColor = "white";
                  }
                  fetch("/led");
              }
              </script>
              
    	<br><br>
    	<button>Forward</button>
    	<br>
    	<button>Left</button>
    	<button>Right</button>
    	<br>
		<button>Backward</button>
    
    </body>
    </html>


    """

    return html


# ------------------------
# Socket
# ------------------------
def start_server(ip):
    
    # IP Address
    address = socket.getaddrinfo(ip, 80)[0][-1]  # position 0,-1 is where the address is
    
    # Create socket and HTTP request
    s = socket.socket()
    s.bind(address)
    s.listen(1)

    print("Server running at http://" + ip)

    while True:
        try:
            client, addr = s.accept()
            print("Client connected from", addr)
            
            request = client.recv(1024).decode()
            # recv(1024) means it'll receive up to 1024 bytes
            # decode turns bytes into a string
            print(request)

            # command functions
            if '/forward' in request:
                motor.move_forward()
            elif '/left' in request:
                motor.move_left()
            elif '/right' in request:
                motor.move_right()
            elif '/backward' in request:
                motor.move_backward()
            elif '/led' in request:
                led_pin.toggle()
            
            response = webpage()
            client.send(response)
            client.close()	# must close after opening, it's like opening a door to answer someone and then closing
        
        except Exception as e:
            print("Error:", e)
            client.close()
        

# ------------------------
# main
# ------------------------

ip = start_ap()
start_server(ip)










# import network
# import socket
# from time import sleep
# import machine
# 
# # Yes, these could be in another file. But on the Pico! So no more secure. :)
# ssid = 'Your_Network_Name'
# password = 'Your_WiFi_Password'
# 
# def move_forward():
#     print ("Forward")
#     
# def move_backward():
#     print ("Backward")
#     
# def move_stop():
#     print ("Stop")
#     
# def move_left():
#     print ("Left")
#     
# def move_right():
#     print ("Right")
#     
# def connect():
#     #Connect to WLAN
#     wlan = network.WLAN(network.STA_IF)
#     wlan.active(True)
#     wlan.connect(ssid, password)
#     while wlan.isconnected() == False:
#         print('Waiting for connection...')
#         sleep(1)
#     ip = wlan.ifconfig()[0]
#     print(f'Connected on {ip}')
#     return ip
#     
# def open_socket(ip):
#     # Open a socket
#     address = (ip, 80)
#     connection = socket.socket()
#     connection.bind(address)
#     connection.listen(1)
#     return connection
# 
# def webpage():
#     #Template HTML
#     html = f"""
#             <!DOCTYPE html>
#             <html>
#             <head>
#             <title>Zumo Robot Control</title>
#             </head>
#             <center><b>
#             <form action="./forward">
#             <input type="submit" value="Forward" style="height:120px; width:120px" />
#             </form>
#             <table><tr>
#             <td><form action="./left">
#             <input type="submit" value="Left" style="height:120px; width:120px" />
#             </form></td>
#             <td><form action="./stop">
#             <input type="submit" value="Stop" style="height:120px; width:120px" />
#             </form></td>
#             <td><form action="./right">
#             <input type="submit" value="Right" style="height:120px; width:120px" />
#             </form></td>
#             </tr></table>
#             <form action="./back">
#             <input type="submit" value="Back" style="height:120px; width:120px" />
#             </form>
#             </body>
#             </html>
#             """
#     return str(html)
# 
# def serve(connection):
#     #Start web server
#     while True:
#         client = connection.accept()[0]
#         request = client.recv(1024)
#         request = str(request)
#         try:
#             request = request.split()[1]
#         except IndexError:
#             pass
#         if request == '/forward?':
#             move_forward()
#         elif request =='/left?':
#             move_left()
#         elif request =='/stop?':
#             move_stop()
#         elif request =='/right?':
#             move_right()
#         elif request =='/back?':
#             move_backward()
#         html = webpage()
#         client.send(html)
#         client.close()
# 
# try:
#     ip = connect()
#     connection = open_socket(ip)
#     serve(connection)
# except KeyboardInterrupt:
#     machine.reset()
# 
# 
# # import motor
# # 
# # 
# # 
# # 
# # # main
# # 
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
# #     
# # if (data == "f"):
# #     motor_a()
# #     motor_b()
