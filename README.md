<div align="center">

# 💧 The Hygrometer
### *An analog-style humidity gauge — the first instrument of the AQI Board*

<img src="https://RolandThsive.github.io/FeaturedImages/9164757120907738216.jpg" alt="The Hygrometer (AQI Board)" width="720"/>

[![Blog](https://img.shields.io/badge/Build%20Guide-ro--h--bots-FF5722?style=for-the-badge&logo=blogger&logoColor=white)](https://ro-h-bots.blogspot.com/2026/06/Hygrometer-AQI-Board.html)
[![YouTube](https://img.shields.io/badge/Watch%20it%20move-YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/watch?v=CXAemQT397Y)
[![Onshape](https://img.shields.io/badge/Open%20in-Onshape-0A6EB4?style=for-the-badge&logo=onshape&logoColor=white)](https://cad.onshape.com/documents/6ddfd53336dc1c46c65b50b1/w/d053af0db53c4d615bc00639/e/21806321932fd312f15b5a1c?renderMode=0&uiState=6add1867e21be264acb28bf0)
[![Instagram](https://img.shields.io/badge/@ro__h__bots-Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=white)](https://www.instagram.com/ro_h_bots)

![ESP32-C3](https://img.shields.io/badge/MCU-ESP32--C3-E7352C?style=flat-square&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/Firmware-PlatformIO-F5822A?style=flat-square&logo=platformio&logoColor=white)
![Blender](https://img.shields.io/badge/CAD-Blender-F5792A?style=flat-square&logo=blender&logoColor=white)
![PETG](https://img.shields.io/badge/Printed%20in-PETG-2E7D32?style=flat-square)
[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg?style=flat-square)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

</div>

---

## ✨ Overview

This repository is a part of the **AQI Board** project by [@RolandThsive](https://ro-h-bots.blogspot.com/p/about-me.html) — a wall of analog-style instruments (hygrometer, thermometer, AQI meter and VOC meter) that are as much a showpiece as they are useful.

The Hygrometer was built to keep an eye on the humidity of the room *and* of 3D printer filament storage. An **N20 micro gear motor** turns a **globoid worm gear set**, which drives the tick gear train behind a **125 mm dome glass**. An **AS5600** magnetic encoder tracks the tick's exact position, and an **SHT40** does the sensing.

The face carries **Hindi numerals**, painted black mid-print with Posca markers so the letters show through the gaps in the printed layer.

---

## 🔧 Features

- 🐌 **Globoid worm drive** — smooth, self-locking motion for the tick
- 🎯 **Closed-loop positioning** with an AS5600 magnetic encoder
- 🔋 **Two power modes:** wall-mounted on magnetic pogo pins, or portable on 2× AAA batteries
- 🧲 **Magnetic mounting** — sticks to any ferrous surface; the battery cover snaps on with washers
- 🔗 **USART link** (115200 baud) to a master board when powered from the wall
- 🧵 **Build-plate texture** transferred onto the battery interface with a print-in-place trick
- 🔩 Three-part housing (top, bottom, battery cover) held together with M2 bolts and embedded nuts

---

## 📸 Gallery

<div align="center">

| Full Assembly | Face with Tick Gear System | Bottom Housing Assembly |
| :---: | :---: | :---: |
| <img src="CAD/GLTF%20Model/Full%20Assembly/Model%20Viewer%20-%20Assembly_Hygrometer_Compressed/Assembly_Hygrometer_Compressed.png" width="250"/> | <img src="CAD/GLTF%20Model/Assembly%20-%20Face%20with%20tick%20gear%20system/Model%20Viewer%20-%20Face%20with%20tick%20gear%20assembly/Face_with_tick_gear_system_compressed.png" width="250"/> | <img src="CAD/GLTF%20Model/Assembly%20-%20Bottom%20Housing/Model%20Viewer%20-%20Assembly-BottomHousing/Assembly_bottomHousing_Compressed.png" width="250"/> |
| **Top Housing** | **Bottom Housing** | **Battery Cover** |
| <img src="CAD/GLTF%20Model/Top%20Housing/Model%20Viewer%20-%20Top%20Housing/Top_Housing_Compressed.png" width="250"/> | <img src="CAD/GLTF%20Model/Bottom%20Housing/Model%20Viewer%20-%20Bottom%20Housing/Bottom_Housing_Compressed.png" width="250"/> | <img src="CAD/GLTF%20Model/Battery%20Cover/Model%20Viewer%20-%20Battery%20Cover/Battery_cover.png" width="250"/> |

*Want to spin these around? The blog post has an interactive 3D viewer of the full assembly.*

</div>

---

## 📖 Instructions

For complete printing and assembly instructions, scroll all the way down to the **'Manufacturing'** section on the official blog post:
👉 [The Hygrometer (AQI Board) Guide](https://ro-h-bots.blogspot.com/2026/06/Hygrometer-AQI-Board.html)

### 🖨️ The bottom housing — four pauses

The bottom housing is the tricky print. Each pause has its own Blender file and GLTF model so you can see exactly what goes in:

<div align="center">

| ⏸️ 1st Pause | ⏸️ 2nd Pause | ⏸️ 3rd Pause | ⏸️ 4th Pause |
| :---: | :---: | :---: | :---: |
| <img src="CAD/GLTF%20Model/1st%20Pause/Model%20Viewer%20-%201st%20Pause/BottomHousing_1stPause_Compressed.png" width="180"/> | <img src="CAD/GLTF%20Model/2nd%20Pause/Model%20Viewer%20-%202nd%20Pause/BottomHousing_2ndPause_Compressed.png" width="180"/> | <img src="CAD/GLTF%20Model/3rd%20Pause/Model%20Viewer%20-%203rd%20Pause/BottomHousing_3rdPause_Compressed.png" width="180"/> | <img src="CAD/GLTF%20Model/4th%20Pause/Model%20Viewer%20-%204th%20Pause/BottomHousing_4thPause_Compressed.png" width="180"/> |
| Embed the **PVC mesh** (cut with the pre-printed stencil, bonded with B7000) | Drop in the pre-printed part on the model-in-place support for the **build-plate texture** | Insert the **magnets** | Insert the **nuts** and glue the caps over them |

</div>

> [!TIP]
> The face also needs a pause: stop at the marked layer and paint it black so the Hindi numerals show through. Take care around the edges.

---

## 🛠️ Bill of Materials (BOM)

| Quantity | Item Description |
| :---: | :--- |
| **8x** | M2 x 16 mm Hex Head Allen Type Screws |
| **8x** | M2 Nut |
| **4x** | MR74 bearing 4 x 7 x 2 mm |
| **1x** | AAA 2x Battery holder |
| **1x** | N20 motor |
| **1x** | Set of 3D printed parts *(Found in the `CAD/Printables` folder)* |
| **1x** | AS5600 with magnet (4mm x 2mm) |
| **11x** | M2 x 3 mm Phillips Head CSK Screws |
| **1x** | 1mm Brass rod *(Cut to size)* |
| **7x** | 15mm x 5mm N52 Neodymium magnets |
| **1x** | 120mm x 120mm PC fan |
| **1x** | 0.6mm PVC mesh *(Cut to size)* |
| **1x** | 1 set of 5-pin pogo pin connector |
| **1x** | Tube of B7000 glue |

---

## ⚡ Electronics

| Component | Role |
| :--- | :--- |
| Seeed Studio XIAO ESP32-C3 | Main controller |
| SHT40 | Temperature & humidity sensor |
| AS5600 | Magnetic encoder for the tick position |
| DRV8833 | Motor driver for the N20 |

For now the development boards are wired together directly — a custom PCB is planned.

---

## 💻 Firmware

Built with **PlatformIO** on the Arduino framework for `seeed_xiao_esp32c3`.

```bash
cd Firmware/humidity_level_device
pio run -t upload
pio device monitor -b 115200
```

On boot the firmware checks the mode switch and picks a power mode:

| Mode | When | What it does |
| :--- | :--- | :--- |
| 🔋 **Battery** | Running on the AAA pack | Samples humidity (median-filtered) and moves the tick on its own |
| 🔌 **Power** | Connected through the pogo pins | Opens USART at 115200 baud and answers a master board with a framed (`0xAA` header) 32-bit reading |

---

## 📁 Repository Structure

```
Hygrometer/
├── CAD/
│   ├── Blender Project/         # Source .blend files, incl. each print-pause stage
│   ├── GLTF Model/              # Web-ready 3D models + model-viewer pages
│   ├── Printables/              # STL files to print
│   ├── Step Files/              # Full assembly + unique-part STEP files
│   └── Miscellaneous Stuff/     # Bambu Lab build-plate texture
├── Firmware/
│   └── humidity_level_device/   # PlatformIO project
├── LICENSE.txt
└── README.md
```

---

## 🧩 The AQI Board Family

| Project | Description |
| :--- | :--- |
| 💧 **Hygrometer** *(you are here)* | Analog humidity gauge with a globoid worm drive |
| 🌡️ [**Thermometer**](https://github.com/RolandThsive/Thermometer) | Temperature & dew point gauge with a planetary gear set |
| 🌫️ AQI Meter · 🧪 VOC Meter | Coming soon |

---

## 🌐 Connect With Me
* **Blog:** [ro-h-bots.blogspot.com](https://ro-h-bots.blogspot.com/)
* **Insta:** [@ro_h_bots](https://www.instagram.com/ro_h_bots?igsh=cDNkcnNpYnB5aWR1)

---

## License

[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

<div align="center">

Made with love by **RolandThsive ([@RolandThsive](https://github.com/RolandThsive))**

</div>
