Using bitstream_over_usb firmware and upload script to upload the bitstream. The delay after opening the initial serial connection has been reduced to 0.2 seconds.

Normal on laptop:
0.36
0.36
0.37
0.36
0.37

Using usba to usbc adapter on laptop:
0.36s
0.35s
0.36s
0.37s
0.36s

Adapter on rp4:
0.38
0.39
0.38
0.38
0.38

Transfer over TCP, sending from laptop, flashing on rp4 through adapter. Time starts at socket connection:
0.40s
0.40s
0.40s
0.38s
0.35s

Http using multipart form. The server saves the bitstream then uploads to the pico. Returns response after upload is done. Bitsteam is sent over python requests module, time is from request begining to request end.

Flask development server, flashing on rose laptop with adapter, sending from rp4:
0.42s
0.42s
0.43s
0.45s
0.44s

Waitress server, flashing on rose laptop with adapter, sending from rp4:
0.42s
0.43s
0.42s
0.42s
0.42s

Waitress server, flashing on rp4 with adapter, sending from rose laptop:
0.60s
0.60s
0.48s
0.59s
0.72s
0.48s
0.51s
0.71s
0.48s
0.50s

Usbip, uploading on rose laptop, device on rp4:
1.74s
1.70s
1.83s
1.76s
1.65s

Usbip, uploading on rp4, device on rose laptop:
1.97s
2.00s
2.04s
2.02s
2.71s

Usbip, uploading on rp5, device on rp4:
2.05s
1.90s
1.86s
1.83s
1.85s