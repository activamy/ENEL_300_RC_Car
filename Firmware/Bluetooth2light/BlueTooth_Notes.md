**HAL\_UART\_Receive\_IT(\&huart4, \&rxData, 1);**

* Interrupt based UART receive function
* The 1 indicates to interrupt when 1 bytes arrives
* if using a different line, ensure the \&huart4 is changed to it's respective UART\_HandleTypeDef variable
* 



**void HAL\_UART\_RxCpltCallback(UART\_HandleTypeDef \*huart)**

* Runs when the 1 byte arrives



if(huart->Instance == \[UART Line])

* Where \[UART Line] can be USART1 and etc
* This is used to check to make sure the right UART/USART line is sending the byte if you have multiple lines active





**\*\*\*Make sure:**

* *Global interrupt is enabled* on the UART/USART line or even in all lines and stuff
* Baud rate is set to 9600bit/s
* Tx -> Rx and Rx -> Tx
* Call HAL\_UART\_Receive\_IT(\&huart4, \&rxData, 1); again in your main code loop so it restarts
