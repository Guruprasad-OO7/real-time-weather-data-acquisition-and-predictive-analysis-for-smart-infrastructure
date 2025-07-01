📡 STM32F103C8T6 Weather Station Transmitter (Bare-Metal)
This repository contains bare-metal C code for a weather data transmitter using the STM32F103C8T6 microcontroller (Blue Pill). It reads data from multiple environmental sensors and transmits it via the LoRa SX1278 module to a receiver node for further analysis and display.

🔧 Features
🌡️ DHT11 – Temperature & Humidity sensor (GPIO)

💡 BH1750 – Light intensity (Lux) sensor (I2C)

🌬️ BMP280 – Temperature, Pressure, and Altitude sensor (I2C)

☔ Rain Sensor – Digital input (GPIO)

❄️ Snow Sensor – Digital input (GPIO)

📶 LoRa SX1278 – Data transmission (SPI)

🪵 Fully bare-metal (No HAL, no CMSIS libraries)

🗂️ Project Structure

/Transmitter
│
├── Core
│   ├── main.c             # Application logic
│   ├── startup.s          # Startup vector table (Cortex-M3)
│   ├── linker.ld          # Linker script for STM32F103C8T6
│
├── Drivers
│   ├── dht11.c/.h         # DHT11 temperature/humidity sensor
│   ├── bh1750.c/.h        # BH1750 light sensor (I2C)
│   ├── bmp280.c/.h        # BMP280 sensor driver
│   ├── lora.c/.h          # SX1278 LoRa driver (SPI)
│   ├── gpio.c/.h          # GPIO helper for rain/snow detection
│   ├── usart.c/.h         # UART for debugging
│   ├── delay.c/.h         # SysTick delay implementation
│   ├── spi.c/.h           # SPI1 driver for LoRa
│   └── i2c.c/.h           # I2C1 driver for BH1750 & BMP280
│
└── Makefile               # (Optional) For CLI build setup


🛠️ Hardware Setup
Module	Connection	MCU Pin
DHT11	GPIO	PA1
BH1750	I2C	PB6 (SCL), PB7 (SDA)
BMP280	I2C	PB6 (SCL), PB7 (SDA)
Rain Sensor	GPIO	PA8
Snow Sensor	GPIO	PA11
LoRa SX1278	SPI	PA4 (CS), PA5 (SCK), PA6 (MISO), PA7 (MOSI)
UART Debug	USART1	PA9 (TX)


🔁 LoRa Payload Format (Example)
mathematica
Copy
Edit
T:25C H:62% L:48Lux TP:24.3C P:1009.3 hPa A:62.8 m Rain:0 Snow:1


🧪 How It Works
Sensors are initialized via GPIO, I2C, and SPI.

Sensor values are read every 3 seconds.

A message is formatted and sent via LoRa.

The same message is printed via UART for debugging.
