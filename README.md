# Wireless controlled power outlet
I have a simple idea for my lunch time. I have power outlet for two device, my lamp and my fan. How if i make a web page to control that thing? Mmm not bad. So my idea are:
- Create a webpage to toggle on or off for my lamp and my fan.
- Give an emergency button.
- Give one extra uncontrolled socket.

## What i need?
So, i need:
- Cheap esp8266 Wemos D1 Mini, you can get from marketplace.
- Cheap 5V 1A travel charger.
- Power outlet
- Triac
- Optocoupler
- Some resistor
- Some led
- Some cable

## schematic
Here, my schematic

## The arduino lib that i used
Hi man, the sea water was salty, i don't want to reinvent the wheel. Let's use the ready to served library. I need:
- Hw library for my wemos d1 mini, here https://wiki.wemos.cc/tutorials:get_started:get_started_in_arduino
- push button lib, i need debouncing feature without headache. I use this one from pololu https://github.com/pololu/pushbutton-arduino
