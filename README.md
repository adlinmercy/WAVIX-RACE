# Smart Remote-Controlled Car Using ESP32

## Project Overview

WAVIX-RACE is an ESP32-based smart car that uses a built-in web server to provide browser-based wireless control. Users can connect to the ESP32's Wi-Fi access point and control the car using a web interface.

## Features

* Browser-based car control
* Forward and backward movement
* Left and right movement
* Stop control
* Responsive control interface for mobile devices
* LED activation during movement
* ESP32 Wi-Fi access point and web server

## Hardware Components

* ESP32 development board
* L298N motor driver
* Four DC motors
* Car chassis and wheels
* Rechargeable batteries
* LED and connecting wires

## Technologies Used

* ESP32
* Arduino IDE
* Embedded C/C++
* HTML
* CSS
* JavaScript
* Wi-Fi and HTTP

## How It Works

1. The ESP32 creates a Wi-Fi network named `ESP32_CAR`.
2. Connect a phone or computer to this network.
3. Open the ESP32's IP address in a web browser.
4. Use the on-screen directional buttons to control the car.
5. Release a movement button to send a stop command.

## Pin Configuration

| Component       | ESP32 GPIO |
| --------------- | ---------: |
| Motor input IN1 |         26 |
| Motor input IN2 |         27 |
| Motor input IN3 |         14 |
| Motor input IN4 |         12 |
| LED             |          4 |

## Developer

Adlin Mercy
