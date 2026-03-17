
1. Partial reconfiguration via FTDI and SPI Flash (900 ms best case, 1500ms very achievable)
	1. requires an initial init phase of ~2 seconds but only once
	2. Subsectors 4k must be written as a unit
	3. default 6 -> 3.5 current
	4. Confident lowering to 1.5
	5. Theoretical limit if we remove resets and lower verification ~0.9 seconds
2. Direct writing to SRAM
	1. Uses FTDI to write to SRAM
		1. no erase needed
		2. all data *might* be reset
		3. programming still required
		4. maybe our own verification?
		5. resets still required (not certain how long)
		6. 179 x 4 = ~720ms  ( writing from FTDI? )
	2. we need exact address and exact data
3. Warm-boot (load all circuits on to SPI flash - s)
	1. slow FTDI -> SPI Flash probably 10s of seconds on initial load but then very fast
	2. If 4 full bitstreams are already on it the flash then we can swap between them
		1. CRAM gets swapped very fast < 1 ms
		2. to support more than 4 (8) we might need to do something tricky with the swapping the loading locations (1K stick cant fit a whole RISK-V implement)
		3. upper limit to the number of full bitstreams we load and there will be a slow operation when loading the whole population
4. Memory Controller with the neural network (can we really just modify the BRAM)
	1. Ideally load the ANN in CRAM and avoid needing to change CRAM
	2. Hopefully we can just write to BRAM with changes to the parameters of the ANN
	3. Writing to the BRAM fast - depends on clock speeds...
		1. would allow us to pass the address and data (one parameter at a time)
		2. 1 bit - might be 1ms-100ms???