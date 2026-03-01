# Auto-Blinds

**Board:** [Seeed Studio XIAO-ESP32-C3](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) [^1] [^2]

**VS Code Extension:** [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

**Framework:** Arduino

## Preview (Manual Mode)
<img src="README/preview_1.gif" alt="Preiew"/>

## Analemma Calculation

**Ecliptic coordinate system** [^3] is used to calculate the Sun’s declination ($\delta$) [^4], where the origin is the center of the Sun, $X$ points to the vernal equinox, $Z$ is the North Ecliptic Pole, and $Y$ completes the right-handed system.

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
\sin{\\,\varepsilon}\\
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
&=-\left[\begin{matrix} \cos{\\,\lambda_\odot}\\\\ \sin{\\,\lambda_\odot}\\\\0\end{matrix}\right]\bullet\left[\begin{matrix}0\\\\ \sin{\\,\varepsilon}\\\\ \cos{\\,\varepsilon}\end{matrix}\right]\\
&=-\sin{\\,\lambda_\odot}\\,\sin{\\,\varepsilon}.
\end{aligned}
$$

where:

- ${\hat{r}}_{SE}$ is the unit vector pointing from the Sun to Earth in the ecliptic coordinate system

- ${\hat{n}}_{NCP}$ is the unit vector pointing toward the **north celestial pole (NCP)** in the ecliptic coordinate system

- $\delta$ is the Sun’s declination

- $\lambda_\odot$ is the ecliptic longitude of Earth [^3]

- $\varepsilon$ is the obliquity of Earth

Since the subsolar latitude ($\phi_s$) [^5] is the same as the Sun’s declination ($\delta$) [^4], we have

$$\phi_s=\delta=-\arcsin{(\sin{\\,\lambda_\odot}\\,\sin{\\,\varepsilon})}$$

In the **Earth-centered, Earth-fixed coordinate system** (ECEF) [^5], the subsolar vector (${\hat{u}}_s$) can be calculated as:

$${\hat{u}}_s=\left[\begin{matrix}\\,\cos{(\phi_s)}\\,\cos{(\lambda_s)}\\\\ \cos{(\phi_s)}\\,\sin{(\lambda_s)}\\\\ \sin{(\phi_s)}\\ \end{matrix}\right]$$

where:

- $\phi_s$ is the subsolar latitude
- $\lambda_s$ is the subsolar longitude, which can be calculated according to the current time [^7]

Similarly, the normalized observer vector (${\hat{u}}_o$), which points directly from the center of Earth to the observer, in ECEF coordinate system can be calculated as:

$${\hat{u}}_s=\left[\begin{matrix}\\,\cos{(\phi_o)}\\,\cos{(\lambda_o)}\\\\ \cos{(\phi_o)}\\,\sin{(\lambda_o)}\\\\ \sin{(\phi_o)}\\ \end{matrix}\right]$$

where:

- $\phi_o$ is the latitude of the observer
- $\lambda_o$ is the longitude of the observer



### References

[^1]: “Getting Started with Seeed Studio XIAO ESP32C3 | Seeed Studio Wiki.” Accessed: Feb. 27, 2026. [Online]. Available: https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/

[^2]: “Amazon.com: Seeed Studio XIAO ESP32C3 - Tiny MCU Board with Wi-Fi and BLE for IoT Controlling Scenarios. Microcontroller with Battery Charge, Power Efficient, and Rich Interface for Tiny Machine Learning. … : Electronics.” Accessed: Feb. 27, 2026. [Online]. Available: https://www.amazon.com/gp/product/B0B94JZ2YF/?th=1

[^3]: “Ecliptic coordinate system,” *Wikipedia*. Feb. 17, 2026. Accessed: Feb. 27, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Ecliptic_coordinate_system&oldid=1338843308

[^4]: “Declination,” *Wikipedia*. Feb. 18, 2025. Accessed: Mar. 01, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Declination&oldid=1276361201

[^5]: “Subsolar point,” *Wikipedia*. Jan. 26, 2026. Accessed: Feb. 27, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Subsolar_point&oldid=1334981706

[^6]: “Earth-centered, Earth-fixed coordinate system,” *Wikipedia*. Dec. 14, 2024. Accessed: Feb. 28, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Earth-centered,_Earth-fixed_coordinate_system&oldid=1263086982

[^7]: T. Zhang, P. W. Stackhouse Jr, B. Macpherson, and J. C. Mikovitz, [“A solar azimuth formula that renders circumstantial treatment unnecessary without compromising mathematical rigor: Mathematical setup, application and extension of a formula based on the subsolar point and atan2 function,”](https://www.sciencedirect.com/science/article/pii/S0960148121004031) *Renew. Energy*, vol. 172, pp. 1333–1340, 2021.