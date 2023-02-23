# Design Challenge: Button-Based Delay

- 1.) How does your code work?

The code focuses around two timers and a button. One timer has a basic job of turning on the RED light everytime the timer goes off. Initially the program starts at a default speed, but when you press the P2.3 button a second timer begins. The second timer keeps count for as long as the button is held. Once, the button is released the second timer turns off, but the count it kept is used with the first timer. The first timer's new speed is however long the button was held for/ how long the second timer was going on for. When the P2.3 button is pressed again the process begins again and a new speed is created. To go back to the default speed the P4.1 button must be pressed.

- 2.) How are the peripherals configured (what modes, what settings)?
 
P2.3 and P4.1 are configured as input buttons with pullup resistors. And P1.0 is configured as an input LED

- 3.) What design considerations did you have to take to account for things like the timer rollover?

We had to consider which clock to use and it was very difficult to design two timers in one program. Had to consider how to create a counter and add that counter to a clock 

- 4.) How should the user use your code?

User should use my code as it is stated in the Part3.C, the user should also be aware that P2.3 is the Blink/Control Button and the P4.1 is the RESET Button
