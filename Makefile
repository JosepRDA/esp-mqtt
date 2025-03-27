all:
	idf.py build

target:
	idf.py set-target esp32

flash:
	idf.py flash

monitor:
	idf.py monitor
