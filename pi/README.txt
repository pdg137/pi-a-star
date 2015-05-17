* Put user into i2c and dialout groups
* Slow down frequency
sudo modprobe -r i2c_bcm2708; sudo modprobe i2c_bcm2708 baudrate=10000
newer pis:
options i2c_bcm2708 baudrate=10000
