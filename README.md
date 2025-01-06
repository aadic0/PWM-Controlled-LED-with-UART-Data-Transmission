# Project Title: PWM-Controlled LED with UART Data Transmission

## Overview
This project involves controlling an LED's brightness using Pulse Width Modulation (PWM) and transmitting real-time data over UART to a PC. The transmitted data includes:
- The LED intensity as a percentage (0-100%).
- ADC readings corresponding to an analog input.

A Python script running on the PC receives the data, stores it in a CSV file, and generates graphical plots for analysis.

## Features
1. **PWM LED Control**:
   - Adjust the LED brightness using a PWM signal.
   - Toggle between ON (non-blink), ON (blink), and OFF modes using button inputs.

2. **UART Communication**:
   - Send LED intensity and ADC values as a CSV string format: `<intensity>,<adcValue>`.
   - Data transmission occurs only when a specific button (PB3) is pressed.

3. **PC Integration**:
   - A Python script captures UART data for 1 minute.
   - Generates CSV files and plots:
     - LED intensity (%) vs. Time (s).
     - ADC reading vs. Time (s).

4. **Seamless Transition Between Modes**:
   - Transmit data during both ON (non-blink) and ON (blink) modes without interrupting ongoing UART communication.

---

## Hardware Requirements
- PIC24F16KA101 Microcontroller
- Push Buttons (PB3 for UART trigger, others for mode selection)
- LED
- Analog input for ADC testing
- UART-to-USB converter
- PC with Python 3 installed

## Software Requirements
- MPLAB X IDE
- XC16 Compiler
- Python 3 with the following libraries:
  - numpy
  - pandas
  - pyserial
  - plotly

---

## Project Structure
### 1. Microcontroller Code
The firmware manages:
- **PWM Generation**:
  - Configures PWM to control LED brightness.
- **ADC Reading**:
  - Reads analog input and converts it to a digital value.
- **UART Transmission**:
  - Sends formatted data to the PC upon PB3 press.

Key Functions:
- `set_PWM_duty_cycle(int dutyCycle)`:
  Sets the PWM duty cycle and updates the LED brightness.
- `readADC()`:
  Reads the ADC value from the analog input.
- `sendDataOverUART()`:
  Transmits the LED intensity and ADC value over UART.

### 2. Python Script
The Python script:
- Captures data over UART for 1 minute.
- Saves the data in a CSV file.
- Generates two graphs:
  1. LED Intensity vs. Time
  2. ADC Reading vs. Time

---

## Usage Instructions

### Hardware Setup
1. Connect the LED and analog input to the microcontroller.
2. Connect push buttons for mode selection and UART transmission.
3. Establish a UART connection to the PC via UART-to-USB converter.

### Microcontroller Configuration
1. Open the MPLAB X project.
2. Compile and upload the code to the PIC24F16KA101.

### Python Script Execution
1. Ensure the required Python libraries are installed.
2. Connect the PC to the microcontroller's UART.
3. Run the Python script:
   ```bash
   python uart_data_capture.py
   ```
4. Observe the generated CSV file and graphs.

---

## Data Transmission Format
Data is sent in the following format:
```
<intensity>,<adcValue>\n
```
- **Intensity**: LED brightness as a percentage (0-100%).
- **ADC Value**: 10-bit ADC reading (0-1023).

Example:
```
75,512
```

---

## Graphs and Analysis
- **Graph 1**: LED Intensity (%) vs. Time (s)
  - Shows the PWM control effect over time.
- **Graph 2**: ADC Reading vs. Time (s)
  - Illustrates the variation of the analog input signal.

---

## Future Improvements
1. Add error handling for UART communication.
2. Implement real-time data visualization on the PC.
3. Optimize ADC sampling rate to align with PWM frequency.

---

## Acknowledgments
This project was developed as part of an embedded systems coursework. Special thanks to the University of Calgary's engineering faculty for guidance and resources.

---

## License
This project is for personal coursework requirements. Do not copy or plagiarize.

