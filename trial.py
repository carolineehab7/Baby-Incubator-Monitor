import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
import pyqtgraph as pg
import numpy as np
import serial
import re

class RealTimePlot(QMainWindow):
    def __init__(self):
        super().__init__()

        # Establish serial connection with Arduino
        arduino_port = 'COM3'  # Change this to your Arduino's port
        baud_rate = 9600  # Make sure this matches your Arduino's baud rate
        self.ser = serial.Serial(arduino_port, baud_rate)

        # Set up the main window
        self.setWindowTitle('Real-Time Plot')
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        # Create a PlotWidget
        self.plot_widget = pg.PlotWidget()
        self.layout.addWidget(self.plot_widget)

        # Set up the plot
        self.plot_data_item = self.plot_widget.plot(pen='b')
        self.plot_widget.setTitle('Arduino Sensor Data')
        self.plot_widget.setLabel('left', 'Sensor Value', units='V')
        self.plot_widget.setLabel('bottom', 'Time', units='s')
        self.plot_widget.setBackground('w')
        vb = self.plot_widget.getViewBox()
        vb.setRange(yRange=(0, 50))

        # Initialize data
        self.x_data = []
        self.y_data = []

        # Start timer for updating plot
        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(50)  # Update plot every 50 ms

    def update_plot(self):
        
        # Read data from serial
        line = self.ser.readline().decode().strip()
        #s = "Humidity: 57.00 %	temperature: 22.55 *C 76.82 *F	Heat index: 76.88 *F"

        # Regular expression to find the temperature value
        temperature_pattern = r"temperature:\s+(\d+\.\d+)\s+\*C"

        # Search for the pattern
        match = re.search(temperature_pattern, line)

        # Extract the temperature value
        if match:
            temperature = float(match.group(1))
            sensor_value = float(temperature)

        # Append data to lists
            self.x_data.append(pg.QtCore.QTime.currentTime().msecsSinceStartOfDay() / 1000.0)
            self.y_data.append(sensor_value)

            # Update plot data
            self.plot_data_item.setData(self.x_data, self.y_data)

    def closeEvent(self, event):
        # Close serial connection when the window is closed
        if self.ser.isOpen():
            self.ser.close()
        event.accept()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = RealTimePlot()
    window.show()
    sys.exit(app.exec_())
