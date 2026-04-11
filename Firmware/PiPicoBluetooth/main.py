import network
import socket
from time import sleep
import machine

# private imports
import motor

# leds
led_pin = machine.Pin('LED', machine.Pin.OUT)
headlight = machine.Pin(0, machine.Pin.OUT)				#HEYHEYHEY CHANGE THE PINS SIL VOUS PLEASE

headlight.off()
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
    sleep(1)
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
			h1   	{color: black;
            		 font-size: 75px;}
                     
    		button	{background-color: white;
          			 padding: 50px 40px;
             		 text-align: center;
         		     border-radius: 4px;
          	 		 margin: 3px;}
  		</style>
	</head>


    <body>
    	<h1>Remote Control</h1>
    		<button id="headlight" onclick="toggleLight()"
        >Light</button>          
    	<br><br>
    		<button 
                ontouchstart="pressBtn(this, '/forward')" 
                ontouchend="releaseBtn(this)"
              >Forward</button>

		<br>
            <button 
              ontouchstart="pressBtn(this, '/left')" 
              ontouchend="releaseBtn(this)"
            >Left</button>

            <button 
              ontouchstart="pressBtn(this, '/right')" 
              ontouchend="releaseBtn(this)"
            >Right</button>
		<br>
            <button 
              ontouchstart="pressBtn(this, '/backward')" 
              ontouchend="releaseBtn(this)"
            >Backward</button>
    
    </body>
    
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

      // PRESS
      function pressBtn(btn, route) {
          btn.style.backgroundColor = "lightgreen";
          fetch(route);
      }

      // RELEASE
      function releaseBtn(btn) {
          btn.style.backgroundColor = "white";
          fetch("/stop");
      }
      </script>
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
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # 👈 ADD THIS
    s.bind(address)
    s.listen(1)

    print("Server running at http://" + ip)
    led_pin.off()

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
                print('f')
            elif '/left' in request:
                motor.move_left()
                print('l')
            elif '/right' in request:
                motor.move_right()
                print('r')
            elif '/backward' in request:
                motor.move_backward()
                print('b')
            elif '/stop' in request:
                motor.stop()
                print('s')
            elif '/led' in request:
                headlight.toggle()
                print('led')
            
            response = webpage()
            client.send(response)
            client.close()	# must close after opening, it's like opening a door to answer someone and then closing
        
        except Exception as e:
            print("Error:", e)
            client.close()
            
        finally:
            if client:
                try:
                    client.close()
                except:
                    pass
        

# ------------------------
# main
# ------------------------

ip = start_ap()
start_server(ip)
