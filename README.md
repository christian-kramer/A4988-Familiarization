# A4988-Familiarization
Making a random stepper motor I found at Axman Surplus move with the A4988

![Ax-Man](http://www.vermontficks.org/pic/images/axman_stuff_2.jpg)

*(Inside of Axman Surplus, St. Paul)*

---




At a recent trip to Axman Surplus yielded some interesting stuff to add to my home collection of electronic stuff. Project boxes, seven-segment displays, the tiniest of tiny diaphragm speakers, nice potentiometers with knobs, isolation transformers for an audio project... and a Nema 17 stepper motor, plus A4988 stepper motor driver! Having never worked with motors or motor controllers in the context of microcontrollers, this was of course the first thing I had to try out from my haul.

First things first, I had absolutely no idea how the A4988 worked. I knew it connected directly up to the coils of a stepper motor, but as far as inputs, that was something I needed to do some research on.

Thankfully, there's a wonderful guide on [last-minute engineers](https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/) detailing exactly what each pin does and how to hook it up to a microcontroller... very handy when never having done this before!

![Pinout](https://lastminuteengineers.com/wp-content/uploads/2018/11/A4988-Stepper-Motor-Driver-Pinout.png)

Essentially, there's two main inputs: "direction" and "step". "Direction" is driven either low or high, and the driver subsequently switches to either "clockwise" or "counter-clockwise" direction. "Step", when pulsed, intuitively steps the motor one step in the direction given by the aforementioned pin. There's an additional 3 "microstep" pins that you can tie  either low or high to get the combination for the resolution of "microstepping" you want.

But what is microstepping, you might ask? Well, microstepping is a technique many stepper motor controllers offer to increase the precision of a stepper motor. For instance, the Nema 17 motor I got at Axman takes 200 steps to complete one revolution. But, if I set the A4988 microstep pins to give me half-steps, now it will take 400 steps to complete one revolution. Likewise, if I set the A4988 to give me quarter-steps, the motor will now take 800 steps to complete one revolution. The A4988 goes all the way down to sixteenth-steps, so you can see how this would be exceptionally beneficial for applications where ultra-precise motor movements are desirable... like 3D-printing!

Here's the table of microstep pins and the corresponding settings:

![Microstepping](https://i.imgur.com/2Hxlbtc.png)

Aside from these 5 main pins, we have "sleep", "reset", "enable", and of course our motor and logic power supplies. I haven't played around with the first 3 very much, but they seem self-explanatory enough. The logic power supply takes the same 3.3v supply the microcontroller takes, and for the motor power supply I'm just supplying 9 volts from my bench-top unit.

There is one caveat though, and that's that we need to limit the current going to the motor so it doesn't overheat. The back of the motor says it's rated for 900mA, so what we do is tie the "step" pin high and twist the tiny little potentiometer on the A4988 board until the bench-top power supply says it's drawing something like half of what it's rated for. I don't like it getting too hot, so I actually just turned it down to 350mA... even derated to this amount, the shaft won't budge when I put all my strength against it. *Now* we're good to go.

I went ahead and wired up the direction and step pins to a couple GPIO pins on my STM32F030F4P6, and wrote some [for loops](https://github.com/christian-kramer/A4988-Familiarization/blob/master/Source/A4988%20Familiarization/Src/main.c) to pulse the step pin 200 times to complete one revolution at various speeds and in both directions.

The result:

[Motor Gif](https://i.imgur.com/doPhKrc.mp4)

Success! The motor does get a little warm, though... definitely within spec, and it's absolutely able to dissipate that heat on its own without additional cooling.

I did feel like a motor on its own was a tad boring, however, so I figured I'd also try out my new seven-segment display and 74HC595 shift register to add a counter for each step of the program.

[Motor and Display Gif](https://i.imgur.com/doPhKrc.mp4)

There... that's better! I even made the decimal point into an indicator for whether or not the motor was currently in motion.

Conclusion... Nema 17 motors are easy to drive and very fun to play with! There's still more to play with, like hooking the microstep pins up to the STM32 so I can adjust the motor resolution on-the-fly to combine quick position-finding movement with slow, precision movement.

I need to figure out a project to use these in, in the future...
