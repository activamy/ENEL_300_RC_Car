**--------SriTu Hobby----------**
According to RealTerm data the outputs are:   (Also stated in the setings)



'S' when idle (not pressing anything)



'U' when holding/pressing up (immediately flips to 'S' when released)

'D' for down

'L' for left

'R' for right



'T' when holding up and left simultaneously

'F' for both up and right

'H' for both down and left

'G' for both down and right



For the left button pressing it the 'first time' (turning red to black) outputs a '2', reverting it back to red outputs a 1



For the right button red to black outputs '4' and black to red outputs '3'







**-------Thoughts on how to implement-------**

(03/03/26) Initial ideas on how to proceed with this information:

* If 'U' is seen move both motors fwd, if 'D' move bkwd
* Seeing 'T' will decrease the left wheel speed to move forward and turn, vice versa for 'F', where right wheel will decrease in speed
* Don't know if backwards turning will be used, but same logic applied
* 'L' and 'R' aren't really useful as of the moment- or acc maybe it is :thinking:

 	- Good for on the spot turning, especially for distance sensing, if we have to drive the car over, spot turning will allow us to move as precisely (as the player) can move

* Left button ('1', '2') can control left headlights and vice versa for right button for right lights

 	- OR one for headlights and one for rear lights :D









**-------Android Studio Notes------**

Android tab is what displays all of the main components (?)



* Manifest folder: Has **AndroidManifest.xml** file. (xml = Extensible Markup Language, data is stored in a text file. CSS positions (fancy ups) elements in a xml file.)

&nbsp;	- AndroidManifest.xml declares permissions, app activities and how the app starts, it's the 'heart' of the app, providing all the essential









































































