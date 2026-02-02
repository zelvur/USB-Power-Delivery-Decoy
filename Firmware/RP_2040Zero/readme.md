# RP2040 Zero

This repository is forked from ADF4351 Controller for better integration and controller using one microcontroller.

## Changes

- Unified variable as an array for GPIO odd and GPIO even, details below
    - Set GPIO 13,15,17,19 in an array for single variable named "CTRL_A" as one group to control pin 6 of the RF_Switch
    -  Set GPIO 14,16,18,20 in an array for single variable named "CTRL_B" as one group to control pin 4 of the RF_Switch
- Add new case, shortcut "f" and "g" for RFC -> RF1 and RFC -> RF2
- 