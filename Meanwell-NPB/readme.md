# Meanwell NPB Chargers
NBP-450, NBP-750, NBP-1200, NBP-1700
Price on NBP-1200 is around 200 eur on Ali. I think it's a good choice for this money. 

I started from https://energieoptionen.de/2025/11/22/wie-speichere-ich-nur-den-photovoltaik-ueberschuss-gesteuert-durch-home-assistant-mit-einem-meanwell-ladegeraet-in-einer-batterie/

This code has many changes and improvements; about 70% of it has been rewritten.

# Idea
I need a Power Supply Unit (PSU) connected in parallel with my battery to work 24/7 as a UPS for my smart home. Some high-power loads (low-voltage lighting, 220V inverter) are also connected in parallel to the battery. After a blackout, the PSU should charge the battery as fast as possible. The charge current should be reduced if the battery temperature is too low. 3 stage charging sdould work correclty with external added load. 

# Adapter
1. The connector is 2x7p from https://a.aliexpress.com/_EHHKm92 it's
2. I use a PC817 optocoupler to remotely turn the charger on/off. It has priority over CAN-BUS control and can remotely restart the charger after some protections are triggered (though not all). 
3. I use sn65hvd230 can-bus module. https://esphome.io/components/canbus/esp32_can/#wiring-options.

<img src="https://github.com/bob-tm/ha-psu-chargers/blob/main/Meanwell-NPB/apapter/1.jpeg" width="300" />
<img src="https://github.com/bob-tm/ha-psu-chargers/blob/main/Meanwell-NPB/apapter/2.jpeg" width="300" />
<img src="https://github.com/bob-tm/ha-psu-chargers/blob/main/Meanwell-NPB/apapter/3.jpeg" width="300" />
<img src="https://github.com/bob-tm/ha-psu-chargers/blob/main/Meanwell-NPB/apapter/4.jpeg" width="300" />
<img src="https://github.com/bob-tm/ha-psu-chargers/blob/main/Meanwell-NPB/apapter/5.jpeg" width="300" />

# Changes 
1. My config can be used to change way more params and read almost everything available sensors.
2. It has 3 stage Charger emulation (Boost Charger in PSU Mode).

# Install
1. Copy yaml to ESPHome builder (\\homeassistan.local\config\esphome\)
2. do not forget to copy packages folder
3. Setup GPIO and Board params.
4. Install
   
# NPB Hardware limitations
1. PSU and Charger modes can be changed only by cycling the AC power. You can add a remote-controlled socket to automate this.
2. For the 12V model, the minimum voltage is 10.5V. If you connect a load that drops the voltage below 10.5V, the unit will trigger Overload Protection and shut down the output. While this is unlikely with a live battery connected, high loads without a buffer battery can trigger errors. The output voltage must remain above 77% of the target voltage in PSU mode and 55% in Charger mode.
<img width="600" alt="image" src="https://github.com/user-attachments/assets/a8181c1c-bd58-430f-89f0-8a32fa730595" />

3. In Charger Mode, cycling the output on/off is required to apply new charging parameters.
4. The unit will not start without a battery in Charger Mode. You can bypass this by adding a 5-amp load (for NBP 1200-12) for the first 5 seconds.
5. In PSU Mode, the unit can start without any load.
6. It is recommended to disable EEPROM writing after all parameters are configured. This allows you to change almost everything on the fly, as settings will revert to the saved values after an AC power cycle.
7. It seems the internal Charger Mode should handle 3-stage charging correctly, even with an additional load. You can set up timeouts so the charger returns to float voltage, even if a large load keeps the current higher than the taper current. However, I am not sure and need more real-world testing, so I added Virtual Charger emulation.

# Boost Charger in PSU Mode
1. Unit runs in PSU mode with voltage from PSU Target Voltage and current limitation from PSU Target Current.
2. Home Assistant detects recovery after blackout
3. If everything is ok - it can switch on "Boost Charger in PSU Mode"
4. Voltage is increased to Charge Const Voltage
5. If Boost Charger Stage 2 (Boost CV) Max Time is set - Boost stage is limited to this time.
6. If measure current is lovered to Charge Taper Current, than output voltage is back to Charge Float Voltage
7. It's required to setup Charge Const Voltage, Charge Float Voltage, Taper Current. In my example it's 14.4, 13.6 and 7 amp for 70A charging current. In my mind " Boost Charger Stage 2 (Boost CV) Max Time" should calculated each time based on current battery capacity.
8. The loginc is the same as with original 3 stage charger, but you can change everything on the fly.

# how to use
1. Select mode you need (PSU or charger)
2. AC on/off to apply changes
3. Set Target Voltage and Current for PSU or Charger
4. Disable EEROM writing
5. Device is ready to use
