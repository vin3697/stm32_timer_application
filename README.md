# STM32 Potentiometer → Mode Control → Stepper Motor + UART Telemetry

A small STM32 HAL project that reads a potentiometer via ADC, maps it to **0–100%**, selects a **system mode** (Inactive / Work / Error), drives **status LEDs**, controls a **4-wire stepper motor** in Work mode, and streams the current percentage via **UART (USART3 @ 115200 baud)**.

---

## Table of Contents
- [Overview](#overview)
- [How it works](#how-it-works)
- [Modes & behavior](#modes--behavior)
- [Stepper motor control](#stepper-motor-control)
- [UART telemetry](#uart-telemetry)
- [Project structure](#project-structure)
- [Build & flash](#build--flash)
- [Configuration](#configuration)
- [Hardware notes](#hardware-notes)
- [Troubleshooting](#troubleshooting)
- [Next improvements](#next-improvements)
- [License](#license)

---

## Overview

The firmware runs a simple **superloop** with a fixed cycle time (~100 ms) and performs:

1. **ADC read** of potentiometer (12-bit: 0…4095)
2. Convert ADC value → **percentage (0…100)**
3. Decide **system mode**
4. Update **LED indication**
5. In Work mode: decide stepper **direction** from percentage windows
6. Execute one **8-step half-step** sequence (or stop)
7. Send percentage via **UART** (binary 1 byte)

---

## How it works

Main cycle (conceptually):

```c
read_sensor_value(&global_system);
process_sensor_readings(&global_system);
system_mode_selection(&global_system);

global_system.current_cycle_mode = global_system.system_mode;
change_in_mode(&global_system);

system_mode_operation(&global_system);
system_execution(&global_system);
send_info_on_bus(&global_system);

global_system.last_cycle_mode = global_system.current_cycle_mode;
HAL_Delay(100);
````

All runtime information is stored inside a single global state struct (e.g. `global_system_t`), so each module operates on the same system context.

---

## Modes & behavior

Mode selection is based on potentiometer percentage:

* **Inactive mode**: `percent <= 20`
* **Work mode**: `21 <= percent <= 79`
* **Error mode**: `percent >= 80`

LED indication (typical mapping in the code):

* **Work**  → `LD1` ON
* **Inactive** → `LD2` ON
* **Error** → `LD3` ON

`change_in_mode()` detects transitions (current vs last mode) and clears the previous LED state before the new mode LED is set.

### Work mode motor windows

In Work mode the stepper direction is selected by percentage ranges:

* **30–40%** → Clockwise
* **60–70%** → Anti-clockwise
* Otherwise → Motor inactive

(There is also a simple debug variable updated to reflect state changes.)

---

## Stepper motor control

The stepper motor is controlled by four GPIO pins (IN1…IN4) using an **8-step half-step** pattern.

Half-step table (simplified):

| Step | IN1 | IN2 | IN3 | IN4 |
| ---: | :-: | :-: | :-: | :-: |
|    0 |  1  |  0  |  0  |  0  |
|    1 |  1  |  1  |  0  |  0  |
|    2 |  0  |  1  |  0  |  0  |
|    3 |  0  |  1  |  1  |  0  |
|    4 |  0  |  0  |  1  |  0  |
|    5 |  0  |  0  |  1  |  1  |
|    6 |  0  |  0  |  0  |  1  |
|    7 |  1  |  0  |  0  |  1  |

Direction:

* Clockwise: step index runs **7 → 0**
* Anti-clockwise: step index runs **0 → 7**

Timing:

* A short delay (e.g. `HAL_Delay(10)`) is used between micro-steps.
* The main loop also delays ~100 ms per cycle.

> Note: TIM3 is initialized in the project but stepping is currently performed using delays (blocking). A timer-driven approach is a good next step.

---

## UART telemetry

Every loop cycle, the firmware transmits the **percentage** as **one raw byte** (0…100) over USART3 at **115200 baud**.

This is **binary**, not ASCII. If you view it in a normal terminal, it may look like “weird characters”.
If you want readable output, change the transmit to formatted ASCII like `"42\r\n"`.


---

## Build & flash

### STM32CubeIDE

1. Open the project in **STM32CubeIDE**
2. Build the project
3. Flash using **ST-LINK**
4. Open a serial monitor:

   * Baud: **115200**
   * 8 data bits, no parity, 1 stop bit (**8N1**)

---

## Configuration

You can quickly tune behavior in these areas:

### Mode thresholds

In mode selection logic:

* Inactive threshold: `20`
* Error threshold: `80`

### Work mode direction windows

In work mode logic:

* CW window: `30..40`
* CCW window: `60..70`

### Speed

* Reduce/increase micro-step delay (e.g. `HAL_Delay(10)`)
* Reduce/increase main loop delay (e.g. `HAL_Delay(100)`)

---

## Hardware notes

* **Do not drive a stepper motor directly from STM32 GPIO pins.**
  Use a driver stage such as **ULN2003** (for 28BYJ-48) or a proper stepper driver module (A4988/DRV8825) depending on your motor.
* Ensure **common GND** between STM32 and driver.
* Potentiometer should be connected correctly (Vref range), and ADC pin must be configured for analog input.

---

## Troubleshooting

* **UART shows garbage**

  * You’re sending 1 raw byte. Use a binary-aware viewer, or switch to ASCII formatting.

* **Stepper not moving**

  * Check driver wiring + enable pins (if applicable)
  * Confirm GPIO pins configured as outputs
  * Verify motor supply voltage/current

* **Mode LED never changes**

  * Confirm ADC input changes and percentage calculation is correct
  * Check threshold values

---

## Next improvements

* Replace `HAL_Delay()` stepping with a **timer interrupt** (TIM3) for smoother, non-blocking motor control
* Add **UART framing** (start byte + payload + checksum) for robust PC parsing
* Add calibration for ADC min/max if the potentiometer does not reach full scale
* Add error handling (ADC/UART timeouts, sensor disconnect detection)

---


### Author

Vin