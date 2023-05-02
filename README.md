# Mecanum Drive Tank
## ECE 4180 Final Project S23
### Oscar Baglivio, Ray Daw, Yunhao Hu, Steven Zhao

**Mecanum Drive Wheels** are designed similarly to tank wheels, allowing our vehicle to move forward and backward, as well as turn, but what makes them unique is the added ability to "strafe" or move side to side. This is thanks to the diagonal rollers, meaning that the wheels can exert force in the diagonal direction. With the proper combination of these forces, our vehicle can move **forward, backward, left, right**, as well as **rotate** about the **center** or **edge** of the vehicle.

<img width="230" alt="Screen Shot 2023-04-28 at 2 22 03 PM" src="https://user-images.githubusercontent.com/113135441/235224360-a8931893-abb8-4909-a532-3a1c8ad1d264.png">

## Drive Modes
Our drive modes will be toggled in a state machine as follows:
<img width="520" alt="Screen Shot 2023-04-28 at 2 31 21 PM" src="https://user-images.githubusercontent.com/113135441/235226007-f9ad2847-7def-4ca9-96de-c2296dda2b6e.png">
### Translational
**Translational Mode** allows the vehicle to move linearly foward, backward, right, and left.
### Diagonal
**Diagonal Mode** allows the vehicle to move linearly towards the upper right, upper left, lower right, and lower left.
### Tank
**Tank Mode** allows the vehicle to move linearly foward/backward and rotate about its center clockwise/counterclockwise (similar to a tank).
### Driving
**Driving Mode** allows the vehcile to move linearly foward/backward and turn about the rear axis left/right (similar to a standard car).
### 360 Translational
**Accelerometer (Tilt) Mode** allows vehicle to translate at any angle (small angles limited by the torque of weakly driven motors).

## Learning Resources
 [Wikipedia - Mecanum Wheel](https://en.wikipedia.org/wiki/Mecanum_wheel)
 
 [How a Mecanum Drive Works](https://seamonsters-2605.github.io/archive/mecanum)

## Controls
The vehicle will be controlled via the Adafruit Bluefruit Bluetooth module coonnected to the BluefruitConnect app with the following controls:

<img width="442" alt="Screen Shot 2023-04-28 at 2 52 15 PM" src="https://user-images.githubusercontent.com/113135441/235229974-18c711cd-7dcc-404d-bbc3-14bc213e8f87.png">

#### Directional Controls
| Button      | Translational | Diagonal | Tank | Driving |
| ----------- | ----------- | ----------- | -------- | -------- |
| Up          | Forward     |Forward/Left|Forward      |Forward       |
| Down        | Backward    |Backward/Right|Backward       |Backward       |
| Right       | Right Linear|Forward/Right|Clockwise Rotate |Right Turn      |
| Left        | Left Linear |Backward/Left|Counterclockwise Rotate|Left Turn       |

Accelerometer Mode uses the tilt direction of your phone to move in that direction.

#### Number Pad Controls
| Number Pad      | Function
| ----------- | ----------- | 
| 1           | Mode Toggle   |   
| 2           | -----------   |  
| 3           | Decrease Speed |
| 4           | Inrease Speed   |    

## Parts List
| Part | Count |
| ---- | ----- |
| mbed LPC1768 | 1 |
| uLCD-144-G2 | 1 |
| uLCD Ribbon Cable | 1 |
| Dual H Bridge Breakout Board (TB6612FNG) | 2 |
| Adafruit Bluefruit LE UART Friend | 1 |
| DC Gearbox Motor (TT Motor) | 4 |
| Mecanum Wheel | 4 |
| Small Breadboard | 2 |
| Barrel Jack | 2 |
| AA Battery (1.5V) | 4 |
| 4x AA Battery Holder | 1 |
| 9V Battery | 1 |
| 9V Battery Holder | 1 |
| 1000uF Electrolytic Capacitor | 1 |
| Chassis | 1 |
| Jumper Wires | Many |

 [Kit We Used](https://www.amazon.com/dp/B084TNLFYB)
 Includes Chassis, 4x Motors, 4x Mecanum Wheels


## Wiring Guide
### mbed
| mbed Pin | Voltage Source |
| -------- | -------------- |
| Vin | 6V (4x AA Battery) |

### Bluefruit
| Bluefruit Pin | mbed/Other Pin |
| ------------- | -------- |
| CTS           | GND      |
| TX            | P27      |
| RX            | P28      |
| VIN           | 6V (4x AA Battery) |
| GND           | GND |

### uLCD
| uLCD Pin | mbed/Other Pin |
| -------- | -------------- |
| 5V       | 6V (4x AA Battery) |
| TX       | P10            |
| RX       | P9             |
| GND      | GND            |
| RES      | P11            |

### Dual H-Bridge (left)
| Dual H-Bridge Pin | mbed/Other Pin |
| ----------------- | -------------- |
| VM | 9V (9V Battery) |
| VCC | mbed Vout (3V) |
| GND | GND |
| A01 | Front Left Motor (Negative Lead) |
| A02 | Front Left Motor (Positive Lead) |
| B02 | Back Left Motor (Positive Lead) |
| B01 | Back Left Motor (Negative Lead) |
| GND | GND |
| PWMA | P24 |
| AI2 | P14 |
| AI1 | P13 |
| STBY | mbed Vout (3V) |
| BI1 | P15 |
| BI2 | P16 |
| PWMB | P23 |
| GND | GND |

### Dual H-Bridge (right)
| Dual H-Bridge Pin | mbed/Other Pin |
| ----------------- | -------------- |
| VM | 9V (9V Battery) |
| VCC | mbed Vout (3V) |
| GND | GND |
| A01 | Front Right Motor (Positive Lead) |
| A02 | Front Right Motor (Negative Lead) |
| B02 | Back Right Motor (Negative Lead) |
| B01 | Back Right Motor (Positive Lead) |
| GND | GND |
| PWMA | P22 |
| AI2 | P18 |
| AI1 | P17 |
| STBY | mbed Vout (3V) |
| BI1 | P19 |
| BI2 | P20 |
| PWMB | P21 |
| GND | GND |

### Note
Reccomended to put large capactior (~100uF to 1000uF) across 9V & GND to help with large current spikes when suddenly reversing a motor.
Mecanum wheels must be placed such that the axis of the rollers form an "X" shape.

## Video Demonstrations
 [Control Demonstration](https://youtu.be/6uEOdRfjCMo)
  
 [LCD Demonstration](https://youtu.be/_klWCWm-qnU)
