# Device
I have Sinilink sk150x with original case. It requres some modifications to work much better. I think this yaml can work with any Sinilink PSU with minimal modifications. 

# Hardware
Connector is jst 1.25mm

I'm using an ESP32-C3 with RX, TX, GND, and 5V connected. My setup also includes an optional button and LED. I added a 220µF capacitor to the 5V line, which massively reduced power noise, especially while the fan is running.

# Cooling
The unit has two fans, but the second one doesn’t work properly. It is connected to a separate board and takes too long to trigger.
1. Y-Adapter: I made a Y-adapter to connect both fans to the main board in parallel.
2. Airflow Direction: I reversed the fan direction. Since swapping the red and black wires doesn't work on these fans, I simply flipped the fan over. Now it intakes cold air into the case. Originally, it was set to exhaust, but that was ineffective. With this mod, the onboard temperature stabilized around 56°C, whereas with the original setup, I had to abort testing when it reached 70°C.
3. Ventilation: I drilled additional small ventilation holes on the right side (diagonal to the rear fan). This further lowered the temperature from 56°C to 50°C.

# Firmware
I use an ESP32-C3 with ESPHome to control the unit. I have implemented and tested the most important settings:

1. Master Switch: The Master Switch toggles unexpectedly during power-on/off. This happens because the original firmware (v1.40) uses "1" to indicate the device is OFF, yet "1" is also used to turn it ON. Fixing this requires complex global variables or templates in the config, so I’ve left it as is since it works well enough.

2. Power on Boot (S-INI): The behavior varies depending on the Master Switch state. I’ve observed three scenarios:
    - Power on -> Master Switch ON -> Output Enabled.
    - Power on -> Master Switch ON -> Output Disabled.
    - Power on -> Master Switch OFF -> Output Disabled.
      
    It seems that if the Master Switch is off, "Power on Boot" applies to the switch itself; if it's on, it applies to the Output. The most reliable method is to default everything to "OFF" and let the ESP32 handle the power-up sequence (not preset in this config)
