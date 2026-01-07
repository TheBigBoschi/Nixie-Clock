# Nixie-Clock-Digital
This is my attempt at creating a nixie clock, as every electronic hobbyist should do at least once. Timekeeping is by itself an interesting subject, pair it with 60's vibes and hundreds of volts to get an unusual piece to spark conversations (or a nice gift in my case).
  
BEFORE GOING ANY FURTHER: This device (as already mentioned) uses quite some high voltages everything you do it's at your own risk. I don't take any responsibility for your actions.
 
I'm going to comment on each page of the pdf to make everything more clear.
 
![1](https://user-images.githubusercontent.com/70335408/111007063-43bd3480-838e-11eb-8ff9-e3c6245a8be6.jpg)
 
#1: This is the driving section. Tt employs three 74HC595 (shift register) to control two nixies. Two modules to drive all four nixies, one drives the hours, one the minutes. The shift registers are daisy-chained to use fewer pins of the microcontroller specifically just 2 to drive all 40 cathodes. The labels "DOT1" and "DOT2" are used to drive the glowing lamp that represents the colons between the minutes and the hours.
 
![2](https://user-images.githubusercontent.com/70335408/111007039-356f1880-838e-11eb-9923-5e14bf6009ec.jpg)
 
#2: The schematics above describes the HV PSU. It uses an older ic (UC3843) but is enough for this application. The coil is driven up to a certain current instead of using a fixed period, which is helpful in this case because allows us to make everything work easily, with a normal (more recent) ic this wouldn’t be possible (or not as simple) thanks to the faster switching frequency. The efficiency is acceptable, I didn’t measure it but during normal use at full brightness, the whole system draws around 150mA at 12V, more than adequate. I want to point out detail about POT1, sometimes potentiometers fail, and the wiper won’t make contact with the internal trace, which is really bad news especially if you just connect the wiper directly to your feedback pin, because the ic doesn't sense any feedback voltage and it tries to rise its output voltage, giving out higher and higher tension till something starts smoking. the way I connected the potentiometer if this happens (or if the whole pot opens) the voltage lowers, posing no risk for the circuit.
 
![3](https://user-images.githubusercontent.com/70335408/111007120-65b6b700-838e-11eb-9886-5dd90b9882f1.jpg)
 
#3: The schematics above describes the main logic. the green blocks on the right are the PSU section and the driver section. This page is used to link everything together, such as the microcontroller (attiny1616), the voltage regulator, some input buttons and the RTC.
An important detail to note is that as my main supply voltage I used 4.2V, the reason is that simply is the maximum safe voltage that the RTC can tolerate. this way I didn’t have to use any level shifting technique to make the various part of the circuit talk.
Another key point is the extensive use of connectors on the schematics, they are simply a place-holder for a set of holes, this way if I have to probe something or I need to move stuff away from the board (for example, the buttons) I have an easily accessible point to solder wires to.
Initially, I anticipated using an LDR to sense the ambient light and dim the nixie accordingly, after some tests I opted to leave the luminosity constant because even at low brightness the nixies are well visible even in a lit room.
 
 
 




