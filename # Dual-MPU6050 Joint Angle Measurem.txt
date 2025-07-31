# Dual-MPU6050 Joint Angle Measurement Device

This Arduino-based wearable device measures the **relative angle between two MPU6050 sensors** for biomechanical applications like **joint tracking** (knee, elbow, wrist, ankle). Designed for calibration flexibility and customizable joint logic, this project is lightweight and optimized for low-power, real-time motion tracking.

---

## 🚀 Features

- 📐 Measures **relative joint angle** using two MPU6050 sensors
- 🦾 Joint-specific sign logic (e.g., elbow flexion = positive, knee extension = positive)
- 🔄 Manual **invert toggle** to reverse angle sign (for flexibility)
- ⚙️ Joint selection menu (Knee, Elbow, Wrist, Ankle)
- 💤 Auto sleep after inactivity (with countdown)
- 🔋 Low-power design, wearable form factor
- 📺 OLED display (0.96" SSD1306) to show live angle and status

---

## 🧷 Wire Color Key

| Color   | Purpose                                             |
|---------|-----------------------------------------------------|
| 🔴 Red     | VCC (Power +5V or +3.3V)                            |
| 🟢 Green   | GND (Ground)                                        |
| ⚫ Black   | SCL (I2C Clock)                                     |
| 🟡 Yellow  | SDA (I2C Data)                                      |
| 🔵 Blue    | MPU6050 AD0 connections: one to GND, one to VCC     |
| ⚪ Grey    | Push button signal lines (D2, D3, D4)               |
| 🟠 Orange  | Breadboard-to-breadboard **negative (GND)** bridge |
| 🟤 Brown   | Breadboard-to-breadboard **positive (VCC)** bridge |

*This color scheme helps maintain consistent and error-free wiring across the wearable and breadboard setup.*

---

## 📦 Hardware Requirements

| Component                 | Quantity | Notes                              |
|--------------------------|----------|------------------------------------|
| Arduino Nano             | 1        | Can be replaced with Uno/Mini      |
| MPU6050 (GY-521)         | 2        | One with AD0 = GND, other = VCC    |
| OLED Display (SSD1306)   | 1        | 128x64 I2C, 0.96"                   |
| Push Buttons             | 3        | With pull-up resistors or use INPUT_PULLUP |
| SPST Toggle Switch (opt) | 1        | For power                          |
| 3.7V LiPo Battery        | 1        | With charger module (e.g., TP4056) |
| TP4056 Charging Module   | 1        | Optional, for rechargeable battery |
| Wires, Velcro, Hinges    | -        | For wearable setup                 |

---

## 🔌 Wiring Guide

### MPU6050 #1 (AD0 to GND)  
Used on **proximal limb**

| MPU6050 Pin | Arduino Nano | Wire Color |
|-------------|---------------|------------|
| VCC         | 5V            | 🔴 Red     |
| GND         | GND           | 🟢 Green   |
| SDA         | A4            | 🟡 Yellow  |
| SCL         | A5            | ⚫ Black   |
| AD0         | GND           | 🔵 Blue    |

### MPU6050 #2 (AD0 to VCC)  
Used on **distal limb**

| MPU6050 Pin | Arduino Nano | Wire Color |
|-------------|---------------|------------|
| VCC         | 5V            | 🔴 Red     |
| GND         | GND           | 🟢 Green   |
| SDA         | A4            | 🟡 Yellow  |
| SCL         | A5            | ⚫ Black   |
| AD0         | VCC           | 🔵 Blue    |

### OLED Display (I2C SSD1306)

| OLED Pin | Arduino Nano | Wire Color |
|----------|---------------|------------|
| VCC      | 5V            | 🔴 Red     |
| GND      | GND           | 🟢 Green   |
| SDA      | A4            | 🟡 Yellow  |
| SCL      | A5            | ⚫ Black   |

### Push Buttons (using INPUT_PULLUP mode)

| Function        | Pin | Wire Color |
|-----------------|-----|------------|
| Joint Selection | D2  | ⚪ Grey     |
| Calibrate       | D3  | ⚪ Grey     |
| Invert / Wake   | D4  | ⚪ Grey     |

### Breadboard Power Connections

| Purpose            | Wire Color |
|--------------------|------------|
| GND Bridge (–)     | 🟠 Orange  |
| VCC Bridge (+)     | 🟤 Brown   |

---

## 🧠 How It Works

1. **Joint Selection Mode**  
   Use the Up button (D2) to select the joint you're measuring.

2. **Calibration Mode**  
   Press Confirm (D3) to capture the base reference angle when the limb is in a "neutral" position.

3. **Measurement Mode**  
   The angle displayed is the difference between the current posture and the base posture. Joint logic is applied for sign correction.

4. **Invert Logic**  
   Use the Wake/Invert button (D4) to toggle the direction of angle if needed (indicated by "INV" on screen).

5. **Sleep Mode**  
   After 30 seconds of inactivity, a 5-second countdown is shown, then the OLED display turns off. Press Wake (D4) to turn it back on.

---

## 🧰 Software Setup

### 🔧 Dependencies

Install these libraries from Arduino IDE Library Manager:

- `Adafruit SSD1306`
- `Adafruit GFX`
- `Wire` (built-in)
- `MPU6050 by Electronic Cats` (or similar fork)

### ⬇️ Uploading Code

1. Select your Arduino board and port in Arduino IDE
2. Paste or open the `.ino` code file
3. Upload the sketch

---

## 🧪 Calibration and Testing

- Start with both sensors flat and aligned on a surface.
- Press **Confirm (D3)** to calibrate.
- Move one sensor to simulate joint flexion/extension.
- Ensure signs match expectations based on selected joint.
- Use **Invert (D4)** if needed.

---

## 🛠️ Customization Ideas

- Add **Bluetooth module** (e.g., HC-05) to stream angle to PC or mobile
- Integrate **microSD card** for data logging
- Add **EEPROM** support to store calibration across restarts
- Replace OLED with TFT display for graphs

---

## 🙌 Acknowledgments

Built by combining biomechanical insight and embedded systems expertise to support wearable rehab tech and research. Huge thanks to the Arduino and open-source communities!
