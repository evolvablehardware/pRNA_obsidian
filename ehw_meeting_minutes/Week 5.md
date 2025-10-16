## Standup
- Writing bitstream
	- 150ms to copy bitstream from computer and write to FPGA (pico2-ice) 
	- Could try compressing to transfer faster over USB
	- Use header to know how much to skip
	- Could decompress with DMA instead of using processor
- Could use one pico core for decompression and the other for data gathering
- Not yet successful evolution
	- Trying to find the correct amount of constraints
	- May be an issue of evolution being on a LUT not connected to IO at all.
		- Use verilog to make sure LUT can connect to IO

## Possible Next Steps
- Could add flag to allow the type of FPGA being used to be chosen (icestick or pico2-ice)
- BitstreamEvolutionPico /exampleProjectsMicropython is not needed and can be removed