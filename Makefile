all:
	idf.py build

target:
	idf.py set-target esp32

flash:
	idf.py flash -p /dev/ttyUSB0 # porta do meu usb no linux

monitor:
	idf.py monitor
