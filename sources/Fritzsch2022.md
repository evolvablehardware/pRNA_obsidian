[Reduction of Bitstream Size for Low-Cost iCE40 FPGAs](https://ieeexplore.ieee.org/document/10035254)
[Alt. link](https://easychair.org/publications/preprint/TPpq/open)
Reducing the bitstream size is important to lower external storage requirements and to speed-up the reconfiguration of field-programmable gate arrays (FPGAs). The most common methods for bitstream size reduction are based on dedicated hardware elements or dynamic partial reconfiguration. All of these properties are usually missing in low-cost FPGAs such as the Lattice iCE40 device family. In this paper we propose a lightweight compaction approach for iCE40 FPGAs. We present five methods for bitstream compaction: two adapted and three new. The methods work directly on the bitstream by removing unnecessary data and redundant commands. They are applicable independent of the synthesis toolchain and require neither repetition of synthesis steps nor modifications of the target system. Although our focus is on iCE40 devices, we additionally discuss the conditions for applying our approach to other targets. All five methods were implemented in an open-source compaction tool. We evaluate our approach with an iCE40 HX8K FPGA by synthesizing and compacting various projects. As a result, we achieve a reduction in bitstream size and reconfiguration time by up to 79 %.
___
- Bitstream compactification for the iCE40s: 2 adapted and 3 new
- https://github.com/nmi-leipzig/compact_bitstream
- Compaction vs. compression: compaction requires "no explicit reversal of the size reduction"

- CRAM us cleared each time the chip is reconfigured. CRAM is split into 4 quadrants, ==is it possible to clear/write one at a time?==
> To write CRAM data, a couple of selection commands followed by the actual write command are necessary. The selection commands set the bank number, width, offset, and height. Bank number, width, and offset select the actual chip resources while the height defines the number of rows to be written. The subsequent write command starts the writing process and is followed by the actual data. Writing block RAM data follows the same sequence of commands but replaces the final command with a BRAM write command.

- Adapted options
	- Skip comment section 
	- Skip BRAM section. Only can be used if BRAM is not relevant or the content is resused. ==Is it possible to only write to *some* of the BRAM blocks?==

- Utilization of Value Persistence
	- Bank number, width, height, and offset do not have to be specified again if they do not change between commands
	- ![[Pasted image 20250609133730.png]]

- Zero Row Skipping
	- "Not explicitly written CRAM row will have all values set to zero", so we can skip these rows
	- Adds some overhead with more selection and write commands
	- Not practical to do the same for BRAM as BRAM is written in columns
	- ![[Pasted image 20250609134141.png]]

- Chunk Sorting
	- Order CRAM chunks by height and write them in that order
	- Increases the efficacy of value persistence

![[Pasted image 20250609135039.png]]
- Not much detail is provided on the time it takes to compact the bitstreams ("the generation of the compacted bitstreams always took less than 0.3 s on an Intel i5-7200U CPU," while reconfiguration always took less than ~0.2s)

- Discussion suggests multiple of these compactified designs can be loaded at once
---
creation date: 2025-06-09 13:15
___
