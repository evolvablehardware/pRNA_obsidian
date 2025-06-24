1. For the instantiation of the neural models, is the implementation and evolution such that the neural/synaptic parameters adjust purely by changing BRAM? Or are there changes to the CRAM as well?
2. Reset back to an elite or keep patching (based on the 1+9 evolving, I am guessing resetting will be the desired option)
3. Should we implement both? e.g. two controllers: one that overwrite BRAM and one that doesn't
4. Is there any importance loading different arch (assuming CRAM)? Is this even desirable for Vivum?


Notes:

BRAM - intermediate representation or DNN (adjusting parameters) for digital side
EHW - CRAM needed on analog side

Priority: Until Airforce or formal customer -> stick to general evolution

3 main available icestick 1k, 5k, 8k
icesugar(nano) (has usb-c)

LP1K

ehw -> partial reconfiguration, open source, extensible, availability of parts
other candidates of the ice family

on the fly compute-> high accuracy and low compute power, intrinsic reconfigurability

partial reconfig on ice40-> continuous time task to evolve in realtime, fitting a sine wave, matching audio wave form

-------- creating benchmark continuous time domain on a chip --------------
* seems to me that partial reconfiguration + parallel evolution has a reasonable chance for something like this
* 