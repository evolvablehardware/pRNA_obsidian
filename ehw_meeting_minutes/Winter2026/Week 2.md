### Thomas:
* arbitrary connection between luts and nets (as opposed to local tracks immediately adjacent to a net)
* Will be looking to reimplement

### Sayura:
* worked on catching up to speed/ reading documentation
### Isaac:
* not much work, lots of planning on website
### Jackson:
* process on the server completed
* having trouble making a docker container for the project server
	* To use usb-ip you need privilege
	* Docker swarm doesn't support priveleged containers
	* potential resolution by adding a udev rule
### Kieran:
* researched obtaining data from labrador (simple scope)
* packets are very easy to manage, all processing is done on PC
* no python api, but 
* looking into first example using a FFT / SNR data
### Mateo:
* "finally got some code working"
* python script that interacts with labrador
	* plots data automatically
	* basic data processing (FFT) working
	* calibration needed
	* 10ms snapshots plotted
	* working on occasional voltage spikes (thinks its an issue with the communcation)

### Brooklyn:
* code much more polished
* more modular
* needs more verification (large changes made without more verification)
* Soon to be folded into the overall BitstreamEvolution repo