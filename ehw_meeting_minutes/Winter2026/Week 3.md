## Mateo/Kieran
- Python script to get data from oscilloscope and plot fast fourier transform

## Jackson
- Moved all communication between device and client to websockets
	- Significant performance boost
- USB/IP Docker privilege issue
	- Needs to access modules in `/sys/`
	- Workaround: binding `/dev/bus/` to container.
		- Resulting in "odd" behavior
		- Container runtime isn't supposed to support devices (normal device tag)
	- Running server on host that exposes USB/IP behavior is still an option
	- Maybe don't use USB/IP because Websockets is so much faster.

## Sandya
- Wants to get work over break

## Brooklyn
- Cleaning up code for sending bitstream to FPGA
- Made it slightly more modular