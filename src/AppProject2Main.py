# -*- coding: utf-8 -*-

import serial
import time
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# Configure Plotly for rendering
import plotly.io as pio
pio.renderers.default = 'browser'

# Open Serial Port
ser = serial.Serial(port="COM7", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

# Initializations
adc_values = []       # List to store ADC readings
intensity_levels = [] # List to store intensity levels (0-100%)
timestamps = []       # List to store timestamps
start_time = None

# Set duration for capturing data (1 minute)
capture_duration = 60

try:
    print("Waiting for PB3 to start data transmission...")
    
    # Wait for PB3 activation (ensure data starts being received)
    while True:
        line = ser.readline().decode('Ascii').strip()
        if line:  # Check if data has started being received
            print("Data transmission started.")
            start_time = time.time()
            break

    # Capture data for the specified duration
    while time.time() - start_time < capture_duration:
        line = ser.readline().decode('Ascii').strip()
        line = line.replace('\x00','')  #\x00 seems to be sent with Disp2String()
        if line:
            try:
                # Split the received data into ADC and intensity components
                adc, intensity = map(float, line.split(','))
                adc_values.append(adc)
                intensity_levels.append(intensity / 35 * 100)
                timestamps.append(time.time() - start_time)  # Time since capture started
            except ValueError:
                print(f"Error processing line: {line}")  # Handle unexpected data

finally:
    # Close the serial port after capturing data
    ser.close()

# Create DataFrame to organize the captured data
data = pd.DataFrame({
    "Time (s)": timestamps,
    "ADC Reading": adc_values,
    "Intensity (%)": intensity_levels
})

# Save data to CSV
csv_filename = "group_27_data.csv"  # Replace 'group_name' with your group's name
data.to_csv(csv_filename, index=False)
print(f"Data saved to {csv_filename}")

# Create Plots
fig = make_subplots(rows=2, cols=1, subplot_titles=[
    "Intensity Level (0-100%) vs. Time",
    "ADC Reading vs. Time"
])

# Plot Intensity vs. Time
fig.add_trace(
    go.Scatter(x=data["Time (s)"], y=data["Intensity (%)"], mode='lines', name="Intensity (%)"),
    row=1, col=1
)

# Plot ADC Reading vs. Time
fig.add_trace(
    go.Scatter(x=data["Time (s)"], y=data["ADC Reading"], mode='lines', name="ADC Reading"),
    row=2, col=1
)

# Update layout
fig.update_layout(
    height=800,
    width=1000,
    title_text="UART Data: Intensity and ADC Reading vs. Time",
    showlegend=True
)

fig.update_xaxes(title_text="Time (s)", row=1, col=1)
fig.update_yaxes(title_text="Intensity (%)", row=1, col=1)

fig.update_xaxes(title_text="Time (s)", row=2, col=1)
fig.update_yaxes(title_text="ADC Reading", row=2, col=1)

# Show the plots
fig.show()
