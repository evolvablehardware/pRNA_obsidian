[Tools for exploiting warm/cold boot in iCE40 FPGAs](https://umarcor.github.io/warmboot/)
This site documents existing examples, bootloaders and references about dynamic reconfiguration through the cold/warm boot feature available in iCE40 FPGAs.
___
- iCE40's can store up to 4 bitstreams on the chip by default and switch to them without external communication
	- Blog does not discuss how much faster this is compared to using one bitstream at a time
	- Need to write a module that controls when the bitstreams switch
- Hypothetically, ~128 images can be loaded at one (So far, only 8 have been done at once)
	- Each image is a full bitstream
	- More could be possibly stored with compression methods? This has not been done in practice.
- Links to various demos and documentation
	- https://github.com/sylefeb/Silice/tree/draft/projects/ice40-dynboot: Demo for loading and switching between 8 bitstreams
	- https://github.com/juanmard/icestorm modifications to icestorm that allow for packing any number of images
	- https://github.com/YosysHQ/nextpnr/blob/master/docs/constraints.md#absolute-placement-constraints nextpnr allows for the absolute placement of BELs (basic elements), which will probably be useful in defining where we want to reconfigure. ([more documentation](https://fabulous.readthedocs.io/en/latest/FPGA-to-bitstream/Nextpnr%20compilation.html))
___
creation date: 2025-06-09 11:58
___
