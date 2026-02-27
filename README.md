# Auto-Blinds

**Board:** Seeed Studio XIAO-ESP32-C3
- [Wiki](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [Amazon](https://www.amazon.com/gp/product/B0B94JZ2YF/)

**Framework:** Arduino

**Visual Studio Extension:** [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## Preview (Manual Mode)
<!-- <img src="README/preview_1.gif" alt="Preiew"/> -->

## Analemma Calculation

### 1. Subsolar point

Ecliptic coordinate system [^1] is used to calculate the subsolar point longitude $\\,\lambda$ [^2], where $X$ points to the vernal equinox, $Z$ is the North Ecliptic Pole, and $Y$ completes the right-handed system.

Since

$${\hat{r}}_{SE}=\left[
\begin{matrix}
\cos{\\,\lambda}\\
\sin{\\,\lambda}\\
0\\
\end{matrix}
\right]
$$

and

$${\hat{N}}_{\bigoplus{,ecl}}=\left[
\begin{matrix}0\\
\sin{\\,\varepsilon}\\
\cos{\\,\varepsilon}\\
\end{matrix}
\right],
$$

we have

$$
\begin{aligned}
\sin{\\,\delta}&=\sin{\left[\frac{\pi}{2}-\angle\left(-{\hat{r}}_{SE},{\hat{N}}_{\bigoplus{,ecl}}\right)\right]}\\
&=\cos{\angle(-{\hat{r}}_{SE},{\hat{N}}_{\bigoplus{,ecl}})}\\
&=\frac{{\hat{N}}_{\bigoplus{,ecl}}\bullet(-{\hat{r}}_{SE})}{\left|{\hat{N}}_{\bigoplus{,ecl}}\right|\left|{\hat{r}}_{SE}\right|}\\
&=-\left[\begin{matrix} \cos{\\,\lambda}\\\\ \sin{\\,\lambda}\\\\0\end{matrix}\right]\bullet\left[\begin{matrix}0\\\\ \sin{\\,\varepsilon}\\\\ \cos{\\,\varepsilon}\end{matrix}\right]\\
&=-\sin{\\,\lambda}\\,\sin{\\,\varepsilon}.
\end{aligned}
$$

where:

- ${\hat{r}}_{SE}$ is the unit vector pointing from the Sun to Earth in the ecliptic coordinate system

- ${\hat{N}}_{\bigoplus{,ecl}}$ is the unit vector pointing toward the north celestial pole in the ecliptic coordinate system

- $\delta$ is the longitude of the subsolar point

- $\lambda$ is the ecliptic longitude of Earth [^1]

- $\varepsilon$ is the obliquity of Earth

Therefore,

$$\delta=-\sin{\\,\lambda}\\,\sin{\\,\varepsilon}$$

### References

[^1]: “Ecliptic coordinate system,” *Wikipedia*. Feb. 17, 2026. Accessed: Feb. 27, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Ecliptic_coordinate_system&oldid=1338843308

[^2]:	“Subsolar point,” *Wikipedia*. Jan. 26, 2026. Accessed: Feb. 27, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Subsolar_point&oldid=1334981706
