**dmesg of bootloader issue board connecting with sw1 pressed**

[  +5.875161] usb 1-1: new full-speed USB device number 39 using xhci_hcd
[  +0.125354] usb 1-1: New USB device found, idVendor=1209, idProduct=b1c0, bcdDevice= 1.00
[  +0.000018] usb 1-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  +0.000006] usb 1-1: Product: Board in FS mode
[  +0.000004] usb 1-1: Manufacturer: MicroPython
[  +0.000004] usb 1-1: SerialNumber: 1b66ce91ab184a50
[  +0.004107] cdc_acm 1-1:1.0: ttyACM0: USB ACM device
[  +0.000711] usb-storage 1-1:1.2: USB Mass Storage device detected
[  +0.000492] scsi host0: usb-storage 1-1:1.2
[  +1.054532] scsi 0:0:0:0: Direct-Access     MicroPy  Mass Storage     1.00 PQ: 0 ANSI: 2
[  +0.000751] sd 0:0:0:0: Attached scsi generic sg0 type 0
[  +0.006004] sd 0:0:0:0: [sda] 768 4096-byte logical blocks: (3.15 MB/3.00 MiB)
[  +0.000413] sd 0:0:0:0: [sda] Write Protect is off
[  +0.000006] sd 0:0:0:0: [sda] Mode Sense: 03 00 00 00
[  +0.000624] sd 0:0:0:0: [sda] No Caching mode page found
[  +0.000006] sd 0:0:0:0: [sda] Assuming drive cache: write through
[  +0.026056]  sda:
[  +0.000060] sd 0:0:0:0: [sda] Attached SCSI removable disk

**dmesg of working board connecting with sw1 pressed**

[ +20.672014] usb 1-1: new full-speed USB device number 40 using xhci_hcd
[  +0.138538] usb 1-1: New USB device found, idVendor=2e8a, idProduct=000f, bcdDevice= 1.00
[  +0.000003] usb 1-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  +0.000001] usb 1-1: Product: RP2350 Boot
[  +0.000001] usb 1-1: Manufacturer: Raspberry Pi
[  +0.000001] usb 1-1: SerialNumber: BB26BACCA68C3D58
[  +0.013127] usb-storage 1-1:1.0: USB Mass Storage device detected
[  +0.002755] scsi host0: usb-storage 1-1:1.0
[  +1.040534] scsi 0:0:0:0: Direct-Access     RPI      RP2350           1    PQ: 0 ANSI: 2
[  +0.000804] sd 0:0:0:0: Attached scsi generic sg0 type 0
[  +0.002192] sd 0:0:0:0: [sda] 262144 512-byte logical blocks: (134 MB/128 MiB)
[  +0.000907] sd 0:0:0:0: [sda] Write Protect is off
[  +0.000037] sd 0:0:0:0: [sda] Mode Sense: 03 00 00 00
[  +0.000463] sd 0:0:0:0: [sda] No Caching mode page found
[  +0.000010] sd 0:0:0:0: [sda] Assuming drive cache: write through
[  +0.019128]  sda: sda1
[  +0.000154] sd 0:0:0:0: [sda] Attached SCSI removable disk
