* The box is an arduino module connected to: 4 response buttons, a photocell and a usb connector
* The button box has 3 modes: Initialised, b, c, p, d
* Initialised: This is the mode the box is in when it is powered on. The box sends the string "Initialised" to its buffer.
* Mode P: Prepares the box for input upon recieving the byte 'p'. I use this to check that the box is recieving bytes. The box immediately sends the string "waiting for input" after receiving the byte.
* Mode B: We send the byte 'b' to the box. The box then waits until the photocell detects an increase in light, at which point it starts its clock.
    * The first button press after the photocell detects a change in light is sent to the serial buffer.
    * The string sent to the buffer contains two values: the time, in ms, from the change in luminance to the button press and the number of the button that was pressed. I.e. "1370  2"
      * The string returned by the box is of the form "[time]\t[button]\r\n"
    * If the box does not detect a change of light in 3000 ms. It sends the string "No Trigger"
    * If the box detects a change in light, but no press is made within 3000ms of this change. It sends the string "Timed Out"
* Mode C: We send a string made up of 'c' and some value in milliseconds. The box returns the min and max values of luminance recorded by the photocell in the time interval in the string.
    * For instance: 'c3000' produces:
            "temt6000Max  203
             temt6000Min  14"
    * 'temt6000' is the photocell

* Mode D: I use this to check that the photocell is working. It returns one of two strings, so - unlike in Mode C - you can use the return in a Boolean comparison. In other words, it's easier to test `ButtonBoxLine == 'Triggered'` than `ButtonBoxLine == [Some regex that matches the output of C when the box is triggered]`
	* The box returns "Triggered" if the photocell is triggered within 3000ms of receiving the input byte
	* The box returns "Not Triggered" if the photocell is not triggered within 3000ms of receiving the input byte. 