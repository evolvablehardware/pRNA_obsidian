Notes by Isaac.

Finesses were 0 because bitstreams were fake.
## Thomas
- Suggested continuing to use arduino for the time-being for running experiments. 
- Showed script and running evolution on his personal pi over ssh.
- Showed using picoice rather than icestick.
- Thomas was excited to have a pulse count generator to test the connection with the Arduino.

## Brooklyn
- Not much. 
- Wants to implement adc measurement and pulse count measurement with header of info from pico to help coordinate this.

## Isaac
- I have been trying to program with AI, will talk to yoder about it.

## Jackson
- Many small improvements. Added testing framework. Now must write tests :'(
- There were a couple of issues uploading bitstream. If upload fails, the picoice will say no then stop communicating. For example whan a LUT output connects to inputs, or if it is a loop or something like that happening.
- Double Check Time & Exploration mode. 
- Only one particular picoice seemed to be having a challenge, all revision 2, a different device is having challenges and being noncooperative. The failure is consistant on the bad device.
- Hasn't looked on the device side of what has been happening.
- Maybe callback is only function being executed.
- Using bitstream over esc firmware. Seems to be modified so it is hard to debug in some way.

## Overview on Experiment Progress
- We seem to be almost ready to run experiments. Didn't yet get to on-board fitness evaluation, but it seems like a good intermediate step. 
- Next step is how to do evolutoin of asc files. Concerned with them because the location of stuff is different and may result in problems.
- Jackson walked through how to use the pulse count on https://github.com/heiljj/iCEFARM/tree/main/examples/pulse_count_driver/precompiled_circuits
- Current plan is to isolate variables so we are just changing FPGA without messing with fitness measurements on the Arduino. 
	- Variance maximization would be a good enough test to validate it worked. 

## Final Warning
- We won't get full accuracy for pulse clock.

We are done now.