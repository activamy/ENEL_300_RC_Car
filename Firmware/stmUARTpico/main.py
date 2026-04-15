import network
import socket
from time import sleep
from machine import Pin, UART

# ------------------------
# LED variables
# ------------------------
led_pin = machine.Pin('LED', machine.Pin.OUT)
headlight = machine.Pin(0, machine.Pin.OUT)				#HEYHEYHEY CHANGE THE PINS SIL VOUS PLEASE
testLight = machine.Pin(16, machine.Pin.OUT)
someLight = machine.Pin(15, machine.Pin.OUT)

headlight.off()
testLight.off()

# ------------------------
# Data Variables
# ------------------------
uart1 = UART(1, baudrate = 115200, tx = Pin(4), rx = Pin(5))

cmd = "S"		#initial cmd is stop

# ------------------------
# Access point setup
# ------------------------
def start_ap():
    access_point = network.WLAN(network.AP_IF)
    access_point.active(True)

    access_point.config(essid='Team37lololol', password='lolololol')

    print("Access Point Started")
    print("IP:", access_point.ifconfig()[0])

    led_pin.on()  # turn on LED when ready
    sleep(2)
    #return ip
    return access_point.ifconfig()[0]

# ------------------------
# HTML Page
# ------------------------
def webpage():

    html = """HTTP/1.1 200 OK
Connection: close

<!DOCTYPE html>
<html>
    <head>
		<style>
			body 	{background-color: pink;
    				 text-align: center;}
			h1   	{color: black;
            		 font-size: 75px;}
                     
    		button	{background-color: white;
          			 padding: 120px 100px;
             		 text-align: center;
         		     border-radius: 15px;
          	 		 margin: 10px;
          	 		 font-size: 50;}
  		</style>
	</head>

    <body>
    	<h1>Remote Control</h1>
    	<div id="stm"
            style="font-size:40px;
            background:black;
            color:#ff3399;
            padding: 20px;
            margin: 20px;"
        >Waiting...</div>
        <button id="headlight"
            onclick="toggleLight()"
        >Light</button>          
    	<br><br>
        <button 
            ontouchstart="pressBtn(this, '/forward')" 
            ontouchend="releaseBtn(this)"
            ontouchcancel="releaseBtn(this)"
          >F</button>

		<br>
        <button 
            ontouchstart="pressBtn(this, '/left')" 
            ontouchend="releaseBtn(this)"
            ontouchcancel="releaseBtn(this)"
        >L</button>

        <button 
            ontouchstart="pressBtn(this, '/right')" 
            ontouchend="releaseBtn(this)"
            ontouchcancel="releaseBtn(this)"
        >R</button>
        
		<br>
        <button 
            ontouchstart="pressBtn(this, '/backward')" 
            ontouchend="releaseBtn(this)"
            ontouchcancel="releaseBtn(this)"
        >B</button>
        
        <br><br>
        <button id="fastSpeed" onclick="toggleSpeed('fast')"
        >Fast</button>
        
        <button id="slowSpeed" onclick="toggleSpeed('slow')"
        >Slow</button>
    </body>
    
    <script>
      let headLightsOn = false;
      let interval = null;
      let fullSpeedOn = false;
      let someSpeedOn = false;

      function toggleLight() {
          const btn = document.getElementById("headlight");
          headLightsOn = !headLightsOn;
          btn.style.backgroundColor = headLightsOn ? "yellow" : "white";

          fetch("/led");
      }
      
      function toggleSpeed(speed) {
          if (speed == "fast") {
              const btn2 = document.getElementById("fastSpeed");
              fullSpeedOn = !fullSpeedOn;
              btn2.style.backgroundColor = fullSpeedOn ? "cyan" : "white";
              fetch("/fullSpeed");
          }
          
          else if (speed == "slow") {
              const btn3 = document.getElementById("slowSpeed");
              someSpeedOn = !someSpeedOn;
              btn3.style.backgroundColor = someSpeedOn ? "cyan" : "white";
              fetch("/slowSpeed");
          }
      }
      
      function sendCmd(route) {
          return fetch(route)
              .then(r => r.text())
              .then(text => {
              if (text) document.getElementById("stm").innerText = text;
          });
      }

      // PRESS
      function pressBtn(btn, route) {
          if (interval !== null) {
              clearInterval(interval);
              interval = null;
          }
          
          btn.style.backgroundColor = "lightgreen";
          let isFetching = false;
          interval = setInterval(() => {
              if (!isFetching) {
                  isFetching = true;
                  sendCmd(route).finally(() => {isFetching = false; });
              }
          }, 300);  // send every 300 ms
      }

      // RELEASE
      function releaseBtn(btn) {
          if (interval !== null) {
              clearInterval(interval);
              interval = null;
          }
          btn.style.backgroundColor = "white";
          sendCmd("/stop");
      }
      
      document.addEventListener('touchmove', function(e) {
          if (interval !== null) {
              clearInterval(interval);
              interval = null;
          }
      }, { passive: true });
      
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
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(address)
    s.listen(5)
    s.settimeout(0.3)
    
    print("Server running at http://" + ip)
    led_pin.off()

    while True:
        if uart1.any():
            try:
                global stm_data
                stm_data = uart1.readline().decode().strip()
            except:
                pass
                
        try:
            client, addr = s.accept()
            print("Client connected from", addr)
            
            request = client.recv(512).decode()
            # recv(1024) means it'll receive up to 1024 bytes
            # decode turns bytes into a string
            print(request)

            # command functions
            if '/forward' in request:
                uart1.write("F")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                print('f')
                
            elif '/left' in request:
                uart1.write("L")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                print('l')
                
            elif '/right' in request:
                uart1.write("R")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                print('r')
                
            elif '/backward' in request:
                uart1.write("B")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                print('b')
                
            elif '/stop' in request:
                uart1.write("S")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                print('s')
                
            elif '/led' in request:
                headlight.toggle()
                print('led')
                uart1.write("LED")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
            
            elif '/fullSpeed' in request:
                testLight.toggle()
                print('fast speed')
                uart1.write("V")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")    
            
            elif '/slowSpeed' in request:
                someLight.toggle()
                print('slow speed')
                uart1.write("W")
                client.send("HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n")
                
            elif '/data' in request:
                client.send("HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n" + stm_data)
            
            else:
                client.send(webpage())
            
            client.close()	# must close after opening, it's like opening a door to answer someone and then closing
        
        except OSError:
            pass
        except Exception as e:
            print("Error:", e)
            client.close()
            
        finally:
            try:
                client.close()
            except:
                pass
        

# ------------------------
# main
# ------------------------

ip = start_ap()
start_server(ip)


