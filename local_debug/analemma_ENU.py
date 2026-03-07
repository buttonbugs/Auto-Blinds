'''
This Python code is a quick test to varify the calculation process described in README.md
It is not part of the real codes that will be run on the microcontroller.
'''
import numpy as np                              # For matrix calculation
from datetime import datetime, timezone         # Use current time as an example
import math

'''Constants'''
# Table 27.C: Periodic terms for the March Equinox
# Format: [A (Coefficient), B (Constant), C (Multiplier)]
MEEUS_PERIODIC_TERMS = [
    [485, 313.23, 36252.75], [203, 255.69, 2130.30], [199, 21.07, 1007.49],
    [182, 176.58, 422.40],   [156, 8.03, 581.35],    [136, 124.99, 12427.14],
    [77, 176.20, 322.55],    [74, 147.09, 337.91],   [70, 305.52, 3944.40],
    [58, 201.81, 168.15],    [52, 291.57, 1326.69],  [50, 115.39, 1572.00],
    [45, 334.88, 14509.41],  [44, 217.31, 2243.10],  [29, 200.51, 28600.00],
    [18, 129.89, 19.89],     [17, 43.58, 19866.00],  [16, 325.39, 39041.40],
    [14, 77.63, 149.10],     [12, 10.22, 1884.90],   [12, 269.14, 298.90],
    [12, 314.16, 52.30],     [12, 335.83, 155.20],   [11, 147.92, 10000.00]
]
EARTH_OBLIQUITY = 23.44

'''Time calculation'''
# Note: all time should be calculated in UTC to avaid Daylight Savings transitions

def get_periodic_correction(jd):
    """Calculates the sum of periodic perturbations in Julian Days."""
    # T is the number of Julian centuries since J2000.0
    T = (jd - 2451545.0) / 36525
    total_correction = 0
    
    for a, b, c in MEEUS_PERIODIC_TERMS:
        # Argument of the sine function in degrees
        angle_deg = b + (c * T)
        total_correction += a * math.sin(math.radians(angle_deg))
    
    # The sum is in units of 0.00001 days
    return total_correction / 100000

def calculate_true_vernal_equinox(year):
    # 1. Calculate Mean Equinox (The secular polynomial)
    y = (year - 2000) / 1000
    mean_jd = (2451623.80984 + 
               (365242.37404 * y) + 
               (0.05169 * (y**2)) - 
               (0.00411 * (y**3)) - 
               (0.00057 * (y**4)))
    
    # 2. Add Periodic Corrections
    correction = get_periodic_correction(mean_jd)
    true_jd = mean_jd + correction
    
    # 3. Convert Julian Day to Unix Timestamp for Python datetime
    # (Julian Day 2440587.5 is Jan 1, 1970 00:00:00 UTC)
    unix_time = (true_jd - 2440587.5) * 86400
    return datetime.fromtimestamp(unix_time, tz=timezone.utc)

def get_percentage_of_the_utc_year():
    '''Return the percentage of the UTC year that has passed ranging from 0.0 to 1.0'''
    now_utc = datetime.now(timezone.utc)
    current_year = now_utc.year
    
    # Calculate the boundaries based on the current moment
    eq_this_year = calculate_true_vernal_equinox(current_year)
    
    # If we haven't reached the equinox yet this year, the 'start' was last year's equinox.
    if now_utc < eq_this_year:
        year_start = calculate_true_vernal_equinox(current_year - 1)
        year_end = eq_this_year
    else:
        year_start = eq_this_year
        year_end = calculate_true_vernal_equinox(current_year + 1)

    total_seconds = (year_end - year_start).total_seconds()
    elapsed_seconds = (now_utc - year_start).total_seconds()
    
    return (elapsed_seconds / total_seconds)

def get_percentage_of_the_utc_day():
    '''Return the percentage of the UTC day that has passed ranging from 0.0 to 1.0'''

    # 1. Get current UTC time
    now_utc = datetime.now(timezone.utc)

    # 2. Define the start of today (UTC Midnight)
    today_start = now_utc.replace(hour=0, minute=0, second=0, microsecond=0)

    # 3. Calculate elapsed seconds
    elapsed_seconds = (now_utc - today_start).total_seconds()

    # 4. Total seconds in a standard day
    seconds_in_day = 86400

    # 5. Calculate percentage
    return (elapsed_seconds / seconds_in_day)

def main():
    '''Main Function'''

    '''Ecliptic coordinate system'''
    ecliptic_longitude = 2 * math.pi * get_percentage_of_the_utc_year()
    unit_vector_sun_earth = np.array([
        math.cos(ecliptic_longitude),
        math.sin(ecliptic_longitude),
        0
    ])
    unit_vector_north_celestial_pole = np.array([
        0,
        math.sin(EARTH_OBLIQUITY),
        math.cos(EARTH_OBLIQUITY)
    ])

    # Caculate the Sun’s declination
    sin_sun_declination = -np.dot(unit_vector_sun_earth, unit_vector_north_celestial_pole)

    '''Earth-centered, Earth-fixed coordinate system'''
    # Calculate the subsolar point and the subsolar vector
    sin_subsolar_latitude = sin_sun_declination
    cos_subsolar_latitude = math.sqrt(1 - sin_sun_declination**2)

    subsolar_longitude = - 2 * math.pi * (get_percentage_of_the_utc_day() - 0.5)

    subsolar_vector = np.array([
        cos_subsolar_latitude * math.cos(subsolar_longitude),
        cos_subsolar_latitude * math.sin(subsolar_longitude),
        sin_subsolar_latitude
    ])

    # If you are in London (51.5072° N, 0.1276° W), please type "51.5072, -0.1276" in the terminal
    observer_latitude, observer_longitude = eval(input("Enter observer's latitude and longitude:\n# If you are in London (51.5072° N, 0.1276° W), please type \"51.5072, -0.1276\" in the terminal\n"))

    # convert degrees to radians
    observer_latitude *= math.pi /180
    observer_longitude *= math.pi /180
    # Caculate the unit vector of observer's ENU coordinate system
    u = [
        -math.sin(observer_longitude),
        math.cos(observer_longitude),
        0
    ]
    v = [
        -math.sin(observer_latitude) * math.cos(observer_longitude),
        -math.sin(observer_latitude) * math.sin(observer_longitude),
        math.cos(observer_latitude)
    ]
    w = [
        math.cos(observer_latitude) * math.cos(observer_longitude),
        math.cos(observer_latitude) * math.sin(observer_longitude),
        math.sin(observer_latitude)
    ]
    Rotation_Matrix = np.array([
        u,
        v,
        w
    ])
    subsolar_vector_ENU = np.dot(Rotation_Matrix, subsolar_vector)
    print(subsolar_longitude)
    print(subsolar_vector)
    print(subsolar_vector_ENU)

if __name__ == "__main__":
    main()