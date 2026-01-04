
# 🧠⚡ STM32 “Pot-to-Stepper” Project (HAL)  
> A tiny embedded creature that reads a potentiometer, decides its mood (inactive/work/error), and spins a stepper motor accordingly.  
> Built mostly with STM32 HAL, designed to be simple, readable, and easy to extend.

---

## 🧩 What this code does (in human words)

- ✅ **Reads a potentiometer** via **ADC1**
- ✅ Converts ADC value (0…4095) → **percentage (0…100)** with safe math
- ✅ Uses the percentage to choose a **system mode**
  - **≤ 20%** → `inactive_mode`
  - **20–80%** → `work_mode`
  - **≥ 80%** → `error_mode`
- ✅ In `work_mode`, selects **motor direction** based on thresholds
  - **30–40%** → clockwise
  - **60–70%** → anticlockwise
  - else → motor inactive
- ✅ Drives a **stepper motor (half-step / 8-step sequence)** on **GPIOF pins PF0..PF3**
- ✅ Uses **TIM3 interrupt** as a simple “cycle tick” (increments a counter)
- ✅ Sends a tiny UART “hello” (currently transmits byte `3`) over **USART3**

If this were a pet:  
- Potentiometer = its “food meter”  
- LEDs = its “mood ring”  
- Stepper motor = its “legs” 🐾

---


## ⏱️ Timing model (how the “scheduler” works)

TIM3 as a periodic tick:

* `HAL_TIM_PeriodElapsedCallback()` checks if it was **TIM3**
* then calls `increment_cycle_count(&global_system)`
* which increments: `global_system.u8_system_cycle_time++`

In `main.c`, the loop likely checks this counter and runs the algorithm once per “cycle”, then resets it to 0.

So it’s basically a tiny cooperative scheduler:

* ISR: tick++
* main loop: if tick reached → do work → tick = 0

---

## 🔁 The main workflow (high level)

### Mermaid Flowchart

```mermaid
flowchart TD
  A[Boot] --> B[initialization()\nHAL + clocks + GPIO + ADC + TIM3 + UART3]
  B --> C[system_init(&global_system)]
  C --> D{Main Loop}
  D -->|cycle tick reached| E[read_sensor_value()\nADC read]
  E --> F[process_sensor_readings()\nADC -> %]
  F --> G[system_mode_selection()\n% -> system_mode]
  G --> H[system_mode_operation()\nLED + work logic]
  H --> I[system_execution()\nstepper move or idle]
  I --> J[send_info_on_bus()\nUART3 byte]
  J --> K[change_in_mode()\nreset LED on mode change]
  K --> L[reset cycle counter]
  L --> D
  D -->|no tick| D
```

### ASCII fallback (if Mermaid isn’t supported)

```text
[ init HAL ] -> [ system_init() ] -> main loop:
      |
      +--> if cycle tick:
              ADC read -> % calc -> mode select -> mode op
                          -> motor exec -> UART tx -> mode change check
                          -> reset tick
```

---

## 🌀 Stepper motor sequence (half-step, 8 states)

`stepper_motor_sequence(step)` uses these patterns:

| Step | Coil pattern |
| ---: | ------------ |
|    0 | 1000         |
|    1 | 1100         |
|    2 | 0100         |
|    3 | 0110         |
|    4 | 0010         |
|    5 | 0011         |
|    6 | 0001         |
|    7 | 1001         |

And then `energize_pins()` writes the states to GPIO.

Direction functions:

* `clockwise_movement()` steps **7 → 0**
* `anitclockwise_movement()` steps **0 → 7**

---

## 🎛️ Mode logic (the “personality” of the system)

### 1) System mode selection (from pot percentage)

In `system_mode_selection()`:

* **≥ 80%** → `error_mode`
* **≤ 20%** → `inactive_mode`
* otherwise → `work_mode`

### 2) Work mode motor direction selection

In `work_mode_operation()`:

* **30–40%** → clockwise (`clock_wise_rotation`)
* **60–70%** → anticlockwise (`anticlock_wise_rotatin`)
* else → motor inactive

---

## 🧪 UART “bus” output

`send_info_on_bus()` currently transmits a single byte:

* `tx_uart = 3`

There’s also a commented snippet to transmit the potentiometer percentage.
If you want telemetry, you can expand this to send a small structured frame like:

```c
[0xAA][percent][mode][direction][CRC]
```

(Keeping it simple but “debugger-friendly”.)

---

## 🛠️ Build & Flash (typical STM32CubeIDE flow)

1. Open the project in **STM32CubeIDE**
2. Build
3. Flash via ST-Link
4. Watch LEDs + motor behavior as you rotate the potentiometer

> If you’re using a Nucleo board, make sure the GPIOF pins are actually available (some boards don’t break out all PF pins).

---

## 🧯 Troubleshooting

* **Motor doesn’t move**

  * Check stepper driver wiring + power (separate motor supply often needed)
  * Confirm PF0..PF3 are configured as outputs in CubeMX
  * Confirm coil order matches your driver inputs (swap IN pins if direction/steps look wrong)

* **ADC always reads 0 or max**

  * Check ADC channel pin mapping in CubeMX (`MX_ADC1_Init()`)
  * Verify pot wiring: 3.3V – wiper – GND

* **Timer tick not happening**

  * Ensure `HAL_TIM_Base_Start_IT(&htim3);` is called (it is in `initialization()`)
  * Confirm TIM3 interrupt enabled in NVIC

---

## 🧾 License

If you don’t have a license yet: choose one (MIT is friendly).
Otherwise it’s the classic: “works on my desk” 😄

---

## 👤 Author

VIN — January 2026
(Embedded + HAL + a stepper motor that refuses to be boring.)


