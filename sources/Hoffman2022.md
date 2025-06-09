[CoBEA: Framework for Evolving Hardware by Direct Manipulation of FPGA Bitstreams](https://nmi.informatik.uni-leipzig.de/wp-content/uploads/2022/08/cobea_gecco2022.pdf)
Evolvable Hardware is a general approach to apply Evolutionary Algorithms to hardware in order to design, improve, or adapt circuits. Approaches that directly manipulate the bitstream of field-programmable gate arrays (FPGAs) had been abandoned due to thelack of well-documented bitstream formats.
Recent advancements in open source FPGA toolchains fundamentally changed the feasibility of direct bitstream manipulation yet again. Unfortunately, contemporary tools are slow and waste valuable time calling external tools.
Therefore, we present an integrated approach that combines bitstream manipulation, low-level communication, and hardware evaluation into a single framework called CoBEA. In addition, the framework allows compaction of the bitstream and direct configuration of the FPGA device without having to program flash memory. Compared to the state of the art, our framework achieves an acceleration of 130 times for FPGA reconfiguration. This allows complex hardware evolution experiments to be performed.
___
- Focusing on accelerating reconfiguration
- Process
	- Converts circuit directly to bitstream representation
	- Compacts bitstream ([[Fritzsch2022]])
	- Reconfigures the FPGA, bypassing the flash (not possible on a 1K board w/out HW modifications)
- https://github.com/nmi-leipzig/cobea
- ![[Pasted image 20250609144955.png]]
___
creation date: 2025-06-09 14:23
___
