# Auto Blinds

**Auto Blinds** is an automation project that dynamically adjusts blind orientation based on the Sun's position ([Auto Mode](#auto-mode)). It also includes a mobile interface for convenient remote manual control ([Manual Mode](#manual-mode)).

<sub>*Note:* This `README.md` file includes the introduction of the project. For how to use the code, please visit [SUPPORT.md](/SUPPORT.md).</sub>

## Preview

### Auto Mode

<p align="center">
    <br>
    <sub>Fig. 1. The blinds adjusts the orientation automatically based on time and the Sun's position.</sub>
</p>

### Manual Mode

Auto Blinds provides remote accessibility via smartphone from any distance. Fig. 2 illustrates the Auto Blinds (left) alongside a screen recording of the control interface (right). Users may utilize the on-screen buttons to adjust the blinds to preferred orientations; meanwhile, the dynamic animation provides a real-time preview of the blinds' status, orientation, and the current position of the sun.

<p align="center">
    <a href="https://www.youtube.com/watch?v=zLtPkMw64kk">
        <img src="README/manual_mode.gif" alt="Auto Blinds angle definition" width="72%"/>
    </a>
    <br>
    <sub>Fig. 2. The Auto Blinds (left) alongside a screen recording of the control interface (right).</sub>
</p>

## Background and Motivation

In my dorm room, my desk and a set of bunk beds are positioned next to a window fitted with east-facing horizontal blinds. The blinds are adjusted using an acrylic hexagonal prism wand. For clarity (Fig. 3), I define 0° as fully tilted down, 90° as horizontal (maximizing outward visibility), and 180° as fully tilted up (minimizing incoming sunlight).

<p align="center">
    <img src="README/background_1_tilting_angle.svg" alt="Auto Blinds angle definition" width="72%"/>
    <br>
    <sub>Fig. 3. Auto Blinds angle definition. (a) fully tilted down - 0°. (b) horizontal - 90°. (c) fully tilted up - 180°</sub>
</p>

A nearby building partially affects lighting conditions. Its roof forms an altitude angle of approximately 12° relative to the window (Fig. 4. (a)), which blocks sunlight when the Sun’s altitude is below this threshold (Fig. 4. (b)). However, when the Sun rises above 12°, direct sunlight enters the room unless the blinds are tilted upward (Fig. 4. (c)). This makes it difficult to work comfortably at my desk. At night, exterior lighting shines upward into the room, reflecting off the ceiling (Fig. 4. (d)). In this case, the blinds must be tilted downward to block the light and maintain a suitable sleeping environment. During most of the daytime, however, both my roommate and I prefer the blinds to remain at 90° to allow visibility outside.

<p align="center">
    <img src="README/background_2_sun_position.svg" alt="Altitude angle of the building" width="72%"/>
    <br>
    <sub>Fig. 4. (a) 12° altitude angle of the building. (b)(c) Different positions of the Sun. (d) Exterior lighting.</sub>
</p>

A practical limitation arises from the bunk bed design: the upper bunk obstructs most of the wand, leaving only about 0.5 inches (1.3 cm) accessible unless I climb onto the bed (Fig. 5). Since the furniture cannot be rearranged, manual adjustment is inconvenient. This motivated the development of a system that enables both autonomous operation (Auto Mode) and remote manual control (Manual Mode).

<p align="center">
    <img src="README/background_3_short_wand.png" alt="The wand of the blinds" width="72%"/>
    <br>
    <sub>Fig. 5. The wand of the blinds.</sub>
</p>

In Auto Mode, a microcontroller retrieves the current date and time from the Internet and calculates the Sun’s position to determine the optimal blind angle. The system also incorporates a simple daily routine, automatically setting the blinds to 0° during sleep hours and 90° upon waking. In Manual Mode, users can remotely set the blind angle or switch between modes via a mobile interface.

## Basic Information

To support reliable network connectivity, particularly with WPA2-Enterprise (university Wi-Fi), an ESP32 microcontroller is used. A 28BYJ-48 stepper motor provides sufficient torque and precise control for rotating the wand. Notion, via its API, serves as an intermediary “server”, enabling communication between mobile devices and the ESP32.

### Electronics

- **Microcontroller:** Seeed Studio XIAO-ESP32-C3 [^1] [^2]

- **Motor:** 28BYJ-48 stepper motor

- **Language:** C++, Notion API

- **Directory:** [src/](src/)

### User Interface

- **Language:** Notion, HTML, JavaScript, CSS

- **Directory:** [docs/status/](docs/status/)

Note: A Notion page, where there are buttons and Notion database, is used as the interface for users to control the blinds on their phones. A [github webpage](docs/status/) is embedded in the Notion page to provide a real-time preview of the orientation of the blinds and the dynamic position of the Sun.

### Algorithm Tests

- **Language:** Python

- **Directory:** [local_debug/](local_debug/)

## 3D Model

This 3D model (Fig. 6) illustrates the motorized blind system and its mounting assembly:

- **Blue Frame:** 3D-printed housing, secured to the bunk frame with **M6x16mm** bolts.
- **Stepper Motor:** A 28BYJ-48 motor (hidden) fastened using **M4x8mm** screws.
- **Motor Driver:** The green ULN2003 module, secured with **M2x12mm** screws.
- **Gearing:** Two blue 3D-printed gears (**10:24** ratio) used to increase output torque.
- **Wand:** A transparent hexagonal prism that controls the blinds.
- **Mounting:** Attaches directly to the vertical white metal frame of the upper bunk.

Click [here](3D_models/auto_blinds_v3.step) to download the 3D model.

<p align="center">
    <img src="README/3D_model.gif" alt="Wiring" width="72%"/>
    <br>
    <sub>Fig. 6. Auto Blinds 3D model</sub>
</p>

## Wiring

The wiring diagram (Fig. 7) for this project is relatively straightforward.

- **GPIO4 (D2)** -> IN4
- **GPIO5 (D3)** -> IN3
- **GPIO6 (D4)** -> IN2
- **GPIO7 (D5)** -> IN1

<p align="center">
    <img src="README/wiring.png" alt="Wiring" width="72%"/>
    <br>
    <sub>Fig. 7. Auto Blinds wiring diagram</sub>
</p>

## Network Communication

Fig. 8 illustrates the network architecture. The ESP32 retrieves commands from the Notion server while concurrently transmitting the blinds' status and the Sun's position. Simultaneously, the Notion app interface synchronizes with the server to reflect real-time updates. Upon user interaction, commands are transmitted from the interface directly to the Notion server. The infrastructure facilitates multi-user concurrency, enabling several individuals to manage the Auto Blinds at the same time.

<p align="center">
    <img src="README/network_brief_illustration_a.svg" alt="Network Communication" width="72%"/>
    <img src="README/network_brief_illustration_b.svg" alt="Network Communication" width="72%"/>
    <br>
    <sub>Fig. 8. Network architecture for Auto Blinds. (a) Single-user configuration; (b) Multi-user configuration.</sub>
</p>

### Sequence Diagram

```mermaid
sequenceDiagram
    participant user as User
    participant app as Notion App
    participant github as GitHub Webpage
    participant server as Notion Server
    participant esp32_notion as ESP32<br>request_handler.h
    participant esp32_main as ESP32<br>main.cpp
    participant esp32_calc as ESP32<br>sun_calculation.h
    participant esp32_motor as ESP32<br>motor_controller.h
    par ESP32 Notion API
        esp32_main->>esp32_main: Connect to WiFi
        esp32_main->>esp32_notion: Get current status
        esp32_notion->>server: Get current status
        server-->>esp32_notion: Return current status
        esp32_notion->esp32_notion: Get blind angle <br> from current status
        esp32_notion-->>esp32_main: Set "angle"
        Note over server,esp32_main: ESP32 does not have non-volatile storage to <br>store current blind angle, so Notion Server is used <br>to store the angle when the ESP32 is being reset.
        esp32_main->>esp32_notion: Get lastest command
        esp32_notion->>server: Get command list
        server-->>esp32_notion: Return command list
        esp32_notion-->>esp32_notion: Get latest command
        opt
            esp32_notion-)server: Update lastest <br>command status
        end
        esp32_notion-->>esp32_main: Set "target" <br>and "auto_mode"
        esp32_main->>esp32_calc: Get current <br> position of the Sun
        esp32_calc->>esp32_calc: Calculate current <br> position of the Sun
        esp32_calc-->>esp32_main: Set "sun_u", <br>"sun_v", "sun_w"
        alt If auto mode is on
            esp32_main->>esp32_calc: Get "target"
            esp32_calc->>esp32_calc: Calculate "target"
            esp32_calc-->>esp32_main: Set "target"
        end
        esp32_notion->>esp32_motor: Initialize motor
        par ESP32 parallel thread: main.cpp
            loop void loop() in main.cpp
                esp32_main->>esp32_notion: Get lastest command
                esp32_notion->>server: Get command list
                server-->>esp32_notion: Return command list
                esp32_notion-->>esp32_notion: Get latest command
                esp32_notion-)server: Update lastest <br>command status
                opt If Notion App is open
                    server-)app: Send updated database
                    app-)user: Show updated database
                end
                esp32_notion-->>esp32_main: Set "target" <br>and "auto_mode"
                esp32_main->>esp32_calc: Get current <br> position of the Sun
                esp32_calc->>esp32_calc: Calculate current <br> position of the Sun
                esp32_calc-->>esp32_main: Set "sun_u", <br>"sun_v", "sun_w"
                alt If auto mode is on
                    esp32_main->>esp32_calc: Get "target"
                    esp32_calc->>esp32_calc: Calculate "target"
                    esp32_calc-->>esp32_main: Set "target"
                end
                esp32_main->>esp32_notion: Send current status <br>to Notion
                esp32_notion-)server: Update current status
                opt If Notion App is open
                    server-)app: Send current status
                    app->>github: Update current status
                    github->>github: Render preview
                    github->>app: Embed rendered <br>prevew
                    app-)user: Show updated preview
                end
            end
        and ESP32 parallel thread: motor_controller.h
            loop while (true) in motor_controller.h
                esp32_motor->>esp32_main: Get "target"
                esp32_main->>esp32_motor: Return "target"
                esp32_motor->>esp32_motor: Calculate next <br>motor step
                esp32_motor->>esp32_motor: set_motor_phase()
                esp32_motor->>esp32_main: Return "angle"
            end
        end
    and User interference
        opt
            alt
                user->>app: Click a button
                else
                user->>app: Set a specific angle
                else
                user->>app: Calibration
            end
            app->>server: Add command to Notion database
            server-->>app: Return updated database
            app-)user: Show updated database
        end
    end
```

<p align="center">
    <sub>Fig. 9. The sequence diagram of Auto Blinds.</sub>
</p>

## ENU Analemma Calculation

Firstly, the [ecliptic coordinate system](https://en.wikipedia.org/w/index.php?title=Ecliptic_coordinate_system&oldid=1338843308) is used to calculate the Sun’s [declination](https://en.wikipedia.org/w/index.php?title=Declination&oldid=1276361201) ($\delta$), where the origin is the center of the Sun, $X$ points to the vernal equinox, $Z$ is the North Ecliptic Pole, and $Y$ completes the right-handed system. Notably, Jean Meeus’s Algorithm [^3] is used to calculate the approximate time of the vernal equinox in this project (also see [local_debug/analemma_ENU.py](/local_debug/analemma_ENU.py#L10-L57)).

Since

$${\hat{r}}_{SE}=\left[
\begin{matrix}
\cos{\\,\lambda_\odot}\\
\sin{\\,\lambda_\odot}\\
0\\
\end{matrix}
\right]
$$

and

$${\hat{n}}_{NCP}=\left[
\begin{matrix}0\\
-\sin{\\,\varepsilon}\\
\cos{\\,\varepsilon}\\
\end{matrix}
\right],
$$

we have

$$
\begin{aligned}
\sin{\\,\delta}&=\sin{\left[\frac{\pi}{2}-\angle\left(-{\hat{r}}_{SE},{\hat{n}}_{NCP}\right)\right]}\\
&=\cos{\angle(-{\hat{r}}_{SE},{\hat{n}}_{NCP})}\\
&=\frac{{\hat{n}}_{NCP}\bullet(-{\hat{r}}_{SE})}{\left|{\hat{n}}_{NCP}\right|\left|{\hat{r}}_{SE}\right|}\\
&=-\left[\begin{matrix} \cos{\\,\lambda_\odot}\\\\ \sin{\\,\lambda_\odot}\\\\0\end{matrix}\right]\left[\begin{matrix}0\\\\ -\sin{\\,\varepsilon}\\\\ \cos{\\,\varepsilon}\end{matrix}\right]\\
&=\sin{\\,\lambda_\odot}\\,\sin{\\,\varepsilon}.
\end{aligned}
$$

where:

- ${\hat{r}}_{SE}$ is the unit vector pointing from the Sun to Earth in the ecliptic coordinate system

- ${\hat{n}}_{NCP}$ is the unit vector pointing toward the **north celestial pole (NCP)** in the ecliptic coordinate system

- $\delta$ is the Sun’s declination

- $\lambda_\odot$ is the [ecliptic longitude](https://en.wikipedia.org/w/index.php?title=Ecliptic_coordinate_system&oldid=1338843308) of Earth

- $\varepsilon$ is the obliquity of Earth

Since the [subsolar latitude](https://en.wikipedia.org/w/index.php?title=Subsolar_point&oldid=1334981706) ($\phi_s$) is the same as the Sun’s declination ($\delta$), we have

$$\phi_s=\delta=\arcsin{(\sin{\\,\lambda_\odot}\\,\sin{\\,\varepsilon})}$$

In the [Earth-centered, Earth-fixed coordinate system](https://en.wikipedia.org/w/index.php?title=Earth-centered,_Earth-fixed_coordinate_system&oldid=1263086982) (ECEF), the subsolar vector (${\hat{u}}_s$) can be calculated as:

$${\hat{u}}_s=\left[\begin{matrix}\\,\cos{(\phi_s)}\\,\cos{(\lambda_s)}\\\\ \cos{(\phi_s)}\\,\sin{(\lambda_s)}\\\\ \sin{(\phi_s)}\\ \end{matrix}\right]$$

where:

- $\phi_s$ is the subsolar latitude
- $\lambda_s$ is the subsolar longitude, which can be calculated according to the current time ($T_{UTC}$) using the following formula [^4]:

$$\lambda_s=-15\\,(T_{UTC}-12)$$

Similarly, the normalized observer vector (${\hat{u}}_o$), which points directly from the center of Earth to the observer, in ECEF coordinate system can be calculated as:

$${\hat{u}}_o=\left[\begin{matrix}\\,\cos{(\phi_o)}\\,\cos{(\lambda_o)}\\\\ \cos{(\phi_o)}\\,\sin{(\lambda_o)}\\\\ \sin{(\phi_o)}\\ \end{matrix}\right]$$

where:

- $\phi_o$ is the latitude of the observer
- $\lambda_o$ is the longitude of the observer

Note: $ϕ_o$ is the [geodetic latitude](https://en.wikipedia.org/wiki/Geodetic_coordinates) of the observer. However, $ϕ_s$ is the [geocentric latitude](https://en.wikipedia.org/wiki/Latitude#Geocentric_latitude) of the Sun, ignoring the [parallax effect](https://en.wikipedia.org/wiki/Parallax).

Then the coordinate system needs to be transformed into the observer’s coordinate system. The [horizontal coordinate system](https://en.wikipedia.org/w/index.php?title=Horizontal_coordinate_system&oldid=1267888614) is commonly used to represent the position of the sun [^4] [^5]. However, since the blinds and the roof edges mentioned in [Background and Motivation](#background-and-motivation) are parallel to the lines of latitude and longitude, we should use the local [east-north-up](https://en.wikipedia.org/w/index.php?title=Local_tangent_plane_coordinates&oldid=1275515987#Local_east,_north,_up_(ENU)_coordinates) (ENU) coordinate system.

The unit vectors of the **ENU** coordinate system **expressed in** the **ECEF** coordinate system ($\hat{u}$, $\hat{v}$ and $\hat{w}$) can be calculated based on the normalized observer vector (${\hat{u}}_o$):

```math
\begin{aligned}
&\hat{u}=\left[\begin{matrix}u_x&u_y&u_z\\\end{matrix}\right]=\left[\begin{matrix}-\sin{(\lambda_o)}&\cos{(\lambda_o)}&0\\\end{matrix}\right],\\
&\hat{v}=\left[\begin{matrix}v_x&v_y&v_z\\\end{matrix}\right]=\left[\begin{matrix}-\sin{(\phi_o)}\,\cos{(\lambda_o)}&-\sin{(\phi_o)}\,\sin{(\lambda_o)}&\cos{(\phi_o)}\\\end{matrix}\right],\\
&\hat{w}=\left[\begin{matrix}w_x&w_y&w_x\\\end{matrix}\right]={{\hat{u}}_o}^T=\left[\begin{matrix}\cos{(\phi_o)}\,\cos{(\lambda_o)}&\cos{(\phi_o)}\,\sin{(\lambda_o)}&\sin{(\phi_o)}\\\end{matrix}\right].
\end{aligned}
```

where:
$\hat{u}$ is the unit vector pointing to the east
$\hat{v}$ is the unit vector pointing to the north
$\hat{w}$ is the unit vector pointing vertically upward
Subsequently, the Rotation Matrix ($R$) of the coordinate system transformation can be expressed as:

```math
R=\left[\begin{matrix}\hat{u}\\\hat{v}\\\hat{w}\\\end{matrix}\right]=\left[\begin{matrix}-\sin{(\lambda_o)}&\cos{(\lambda_o)}&0\\-\sin{(\phi_o)}\,\cos{(\lambda_o)}&-\sin{(\phi_o)}\,\sin{(\lambda_o)}&\cos{(\phi_o)}\\\cos{(\phi_o)}\,\cos{(\lambda_o)}&\cos{(\phi_o)}\,\sin{(\lambda_o)}&\sin{(\phi_o)}\\\end{matrix}\right]
```

Applying the Rotation Matrix ($R$) to the normalized observer vector (${\hat{u}}_o$) gives the subsolar vector in the ENU coordinate system (${\hat{u}}_s\prime$) [^6]:

```math
{\hat{u}}_s\prime=R{\hat{u}}_s=\left[\begin{matrix}-\sin{(\lambda_o)}&\cos{(\lambda_o)}&0\\-\sin{(\phi_o)}\,\cos{(\lambda_o)}&-\sin{(\phi_o)}\,\sin{(\lambda_o)}&\cos{(\phi_o)}\\\cos{(\phi_o)}\,\cos{(\lambda_o)}&\cos{(\phi_o)}\,\sin{(\lambda_o)}&\sin{(\phi_o)}\\\end{matrix}\right]\left[\begin{matrix}\cos{(\phi_s)}\,\cos{(\lambda_s)}\\\cos{(\phi_s)}\,\sin{(\lambda_s)}\\\sin{(\phi_s)}\\\end{matrix}\right]
```

Let

```math
{\hat{u}}_s\prime=\left[\begin{matrix}s_u\\s_v\\s_w\\\end{matrix}\right],
```

then:

```math
\begin{aligned}
&s_u=-\sin(\lambda_o)\,\cos(\phi_s)\,\cos(\lambda_s)+\cos(\lambda_o)\,\cos(\phi_s)\,\sin(\lambda_s),\\
&s_v=-\sin{(\phi_o)}\,\cos{(\lambda_o)}\,\cos{(\phi_s)}\,\cos{(\lambda_s)}-\sin{(\phi_o)}\,\sin{(\lambda_o)}\,\cos{(\phi_s)}\,\sin{(\lambda_s)}+\cos{(\phi_o)}\,\sin{(\phi_s)},\\
&s_w=s_w=\cos{(\phi_o)}\,\cos{(\lambda_o)}\,\cos{(\phi_s)}\,\cos{(\lambda_s)}+\cos{(\phi_o)}\,\sin{(\lambda_o)}\,\cos{(\phi_s)}\,\sin{(\lambda_s)}+\sin{(\phi_o)}\,\sin{(\phi_s)}.
\end{aligned}
```

Finally, $\arccos{(s_u)}$ is used to represent the solar altitude angle ($a_s\prime$) in the $uw$-plane:

```math
a_s\prime = 
\begin{cases} 
\arccos(s_u) & \text{if } s_w \ge 0\\
-\arccos(s_u) & \text{if } s_w < 0 
\end{cases}
```

## Blind Orientation Calculation

After the Sun rises from the top of the building, the blind target orientation ($θ$) is calculated to block the sunlight.

For simplicity, we let $α$ equal the solar altitude angle ($a_s'$) in the uw-plane and $β$ equal the complementary angle of the blind target orientation ($θ$) in Fig. 10.

<p align="center">
    <img src="README/target_calculation.gif" alt="Target Calculation Image" width="72%"/>
    <br>
    <sub>Fig. 10. A vertical cross section of the blinds and the sunlight.</sub>
</p>

In Fig. 10, The sunlight is represented by the red line with an arrow, and the cross section of the blinds is represented by the tilted black lines. $AC$ and $BC$ are constants, where $AC$ is the width of a blind and $BC$ is the vertical distance between two blinds.

According to the geometric relations, we have

```math
\begin{aligned}
\angle ABC=\frac{\pi}{2}-\alpha\\
\angle ACB=\beta
\end{aligned}
```

Based on the sum of the interior angles of triangle ABC,

```math
\angle BAC=\pi-\angle ABC-\angle ACB=\frac{\pi}{2}+\alpha-\beta
```

Applying the Law of Sines in triangle ABC,

```math
\frac{BC}{\sin{\left(\frac{\pi}{2}+\alpha-\beta\right)}}=\frac{AC}{\sin{\left(\frac{\pi}{2}-\alpha\right)}}
```

Therefore,

```math
\cos{\left(\alpha-\beta\right)}=\frac{BC}{AC}\cos{\left(\alpha\right)}
```

In this case, $\beta>\alpha$, so

```math
\begin{aligned}
\beta-\alpha=\arccos{\left(\frac{BC}{AC}\cos{\left(\alpha\right)}\right)}\\
\beta=\arccos{\left(\frac{BC}{AC}\cos{\left(\alpha\right)}\right)}-\alpha
\end{aligned}
```

Finally, since $\beta$ equals the complementary angle of the blind target orientation ($\theta$), we have

```math
\theta=\pi-\beta=\pi-\arccos{\left(\frac{BC}{AC}\cos{\left(\alpha\right)}\right)}+\alpha
```

### References

[^1]: “Getting Started with Seeed Studio XIAO ESP32C3 | Seeed Studio Wiki.” Accessed: Feb. 27, 2026. [Online]. Available: https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/

[^2]: “Amazon.com: Seeed Studio XIAO ESP32C3 - Tiny MCU Board with Wi-Fi and BLE for IoT Controlling Scenarios. Microcontroller with Battery Charge, Power Efficient, and Rich Interface for Tiny Machine Learning. … : Electronics.” Accessed: Feb. 27, 2026. [Online]. Available: https://www.amazon.com/gp/product/B0B94JZ2YF/?th=1

[^3]: J. H. Meeus, [*Astronomical algorithms*](https://dl.acm.org/doi/abs/10.5555/532892). Willmann-Bell, Incorporated, 1991.

[^4]: T. Zhang, P. W. Stackhouse Jr, B. Macpherson, and J. C. Mikovitz, [“A solar azimuth formula that renders circumstantial treatment unnecessary without compromising mathematical rigor: Mathematical setup, application and extension of a formula based on the subsolar point and atan2 function,”](https://www.sciencedirect.com/science/article/pii/S0960148121004031) *Renew. Energy*, vol. 172, pp. 1333–1340, 2021.

[^5]: R. Walraven, [“Calculating the position of the sun,”](https://www.sciencedirect.com/science/article/pii/0038092X7890155X) *Sol. Energy*, vol. 20, no. 5, pp. 393–397, 1978.

[^6]: “Geographic coordinate conversion,” *Wikipedia*. Dec. 22, 2025. Accessed: Mar. 04, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Geographic_coordinate_conversion&oldid=1328905303#From_ECEF_to_ENU
