![](media/33dc60f2b76c5b42182d299ae6d70cb5.png)

**Add AC power monitoring to your Arduino projects in the safest way ever.**

Monitoring your home power consumption is a desirable ability of many Arduino enthusiast, but it is also a dangerous adventure, dangerous before PQduino.

PQ stands for Power Quality and PQ also stands for P (active power) & Q (reactive power).

PQduino allows you to monitoring power consumption and main electrical parameters in safest way because there is not an electrical connection between PQduino and your Arduino board.

**The connection is made by Bluetooth or WiFi, after all, most of best insulators is the air. Don’t worry about damage your Arduino board or your laptop while you are trying to monitor your home electrical parameters.**

PQduino sends all communication by air in a simple, editable and transparent way in order you can use it and process it in the way you want.

PQduino is a single-phase electrical analyzer based on ADE7753 chip and ESP8266, compatible with 50Hz or 60Hz system, with capacity to measure from 85 Vca to 250 Vca voltage input and up to 10A by default, and possibility for using a CT with 5A secondary in order to be able to measure power in large electrical systems (see our sample applications). PQduino can monitor by default:

-   Voltage (True RMS).
-   Current (True RMS).
-   Active power in Watts.
-   Reactive power in vars.
-   Apparent power in VA.
-   Frequency
-   Ambient Temperature
-   Total Harmonic Distortion in voltage (THD V)
-   Total Harmonic Distortion in current (THD I)
-   Main harmonic spectrum (3° to 13° order) in voltage
-   Main harmonic spectrum (3° to 13° order) in current
-   Active energy accumulation
-   Voltage waveform
-   Current waveform
-   Phasor representation
-   Power triangle representation

Three-phase systems are possible to monitor by using three PQduinos and integrating all single-phase data in just one central Arduino board.

**PQduino is also standalone monitoring system, it haves a web server that shows parameters in a beautiful way, even the waveform and harmonic spectrum for voltage and current.**

**![](media/eafb3dc74315d4364d2043c8e3d8ec80.png)**

PQduino incorporates OTA programing, so you don´t have to disconnect it from AC system if you want to upgrade the code.

![](media/862c580c2e6a9b7f5d729dda4238d8c1.png)

All the code is open source, Arduino based, so you are free to play and adjust it exactly how you need.

PQduino does not need any more to operates, it already haves current and voltage sensors for direct power measuring, even PQduino haves its own power source.

**Make your Arduino projects compatible with AC power systems metering and take your creations to another level.**

**Details of PQduino:**

**Dimensions**

| Height | 14 cm  |
|--------|--------|
| Width  | 7.5 cm |
| Deep   | 5 cm   |
|        |        |

**Powering**

PQduino haves its own power supply but also is possible to power ESP8266 externally with 5V.

**Measuring Ranges**

AC Voltage from 85 V to 250 V, single-phase, Line-Neutral or Line-Line.

AC Current 5 Amp rated, 10 Amp max. If you want to measure more than 10A the only you need is a Current Transformer (CT) with right primary capacity and 5A secondary output, CT must be registered in setup page in webserver, so you can easily measure your entire home power consumption. PQduino can be easily modified to accept a SCT-013-000V as a current input, with the associated ability of non-invasive measuring.

**PQduino construction:**

![](media/b8c690854d90082412b02dcdb68084e8.png)

![](media/3aae1a9c407a404e9b09d1d7f294170b.png)

![](media/5b8e73d62c2137e20b87f3a7a0d0c80f.png)

![](media/52c13d8df7272160650c08aace05e99f.png)

Information and tutorials about configuring, connecting and upgrade code is found here:

<https://www.youtube.com/channel/UCoWQ7Pg5kH3r624XluI_oBQ>
