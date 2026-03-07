#include <Arduino.h>        // For trigonometric functions and PI
#include "time.h"

#include "config.h"         // Get earth obliquity
#include "url_list.h"       // Get ntpServer
#include "secret.h"         // Get local longitude and latitude

/* Get time from ntpServer */
void init_time() {
    configTime(0, 0, ntpServer);
}

/* epoch (time_t) and timeinfo (struct tm) conversion */
time_t get_utc_epoch() {
    time_t now;
    time(&now);
    return now;
}

time_t get_utc_epoch(struct tm timeinfo) {
    return mktime(&timeinfo);
}

struct tm get_utc_timeinfo() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}

struct tm get_utc_timeinfo(time_t epoch) {
    struct tm * timeinfo;
    timeinfo = gmtime(&epoch);
    return *timeinfo;
}

/* vernal equinox calculation */
const double MEEUS_PERIODIC_TERMS[24][3] = {
    {485, 313.23, 36252.75}, {203, 255.69, 2130.30}, {199, 21.07, 1007.49},
    {182, 176.58, 422.40},   {156, 8.03, 581.35},    {136, 124.99, 12427.14},
    {77, 176.20, 322.55},    {74, 147.09, 337.91},   {70, 305.52, 3944.40},
    {58, 201.81, 168.15},    {52, 291.57, 1326.69},  {50, 115.39, 1572.00},
    {45, 334.88, 14509.41},  {44, 217.31, 2243.10},  {29, 200.51, 28600.00},
    {18, 129.89, 19.89},     {17, 43.58, 19866.00},  {16, 325.39, 39041.40},
    {14, 77.63, 149.10},     {12, 10.22, 1884.90},   {12, 269.14, 298.90},
    {12, 314.16, 52.30},     {12, 335.83, 155.20},   {11, 147.92, 10000.00}
};

// Calculates the sum of periodic perturbations in Julian Days.
double get_periodic_correction(double jd) {
    // T is the number of Julian centuries since J2000.0
    double T = (jd - 2451545.0) / 36525;
    double total_correction = 0;
    for (int i = 0; i < 24; i++) {
        // Get a, b, c from the MEEUS_PERIODIC_TERMS table
        double a = MEEUS_PERIODIC_TERMS[i][0];
        double b = MEEUS_PERIODIC_TERMS[i][1];
        double c = MEEUS_PERIODIC_TERMS[i][2];

        // Argument of the sine function in degrees
        double angle_deg = b + (c * T);
        total_correction += a * sin(radians(angle_deg));
    }
    return total_correction / 100000;
}

// Calculate the epoch of the vernal equinox for the given year
time_t calculate_true_vernal_equinox(int year) {
    // 1. Calculate Mean Equinox (The secular polynomial)
    double y = double(year - 2000) / 1000;
    double mean_jd = (2451623.80984 + 
               (365242.37404 * y) + 
               (0.05169 * pow(y, 2)) - 
               (0.00411 * pow(y, 3)) - 
               (0.00057 * pow(y, 4)));

    // 2. Add Periodic Corrections
    double correction = get_periodic_correction(mean_jd);
    double true_jd = mean_jd + correction;

    // 3. Convert Julian Day to Unix Timestamp for Python datetime
    // (Julian Day 2440587.5 is Jan 1, 1970 00:00:00 UTC)
    return (true_jd - 2440587.5) * 86400;
}

/* Time percentage calculation */
// Return the percentage of the UTC year that has passed ranging from 0.0 to 1.0
double get_percentage_of_the_utc_year() {
    // Get current UTC time
    time_t now_utc_epoch = get_utc_epoch();
    struct tm *now_utc_timeinfo;
    now_utc_timeinfo = gmtime(&now_utc_epoch);

    // Get current year
    int current_year = now_utc_timeinfo->tm_year + 1900;    // 	"timeinfo.tm_year = 126" means "year = 2026"

    // Calculate the boundaries based on the current moment
    time_t eq_this_year = calculate_true_vernal_equinox(current_year);
    time_t year_start, year_end;
    
    // If we haven't reached the equinox yet this year, the 'start' was last year's equinox.
    if (now_utc_epoch < eq_this_year) {
        year_start = calculate_true_vernal_equinox(current_year - 1);
        year_end = eq_this_year;
    } else {
        year_start = eq_this_year;
        year_end = calculate_true_vernal_equinox(current_year + 1);
    }

    // Convert to double at the same time
    double elapsed_seconds = (now_utc_epoch - year_start);
    double total_seconds = (year_end - year_start);
    
    return elapsed_seconds / total_seconds;
}

// Return the percentage of the UTC day that has passed ranging from 0.0 to 1.0
double get_percentage_of_the_utc_day() {
    // 1. Get current UTC time
    time_t now_utc_epoch = get_utc_epoch();
    
    // 2. Define the start of today (UTC Midnight)
    time_t today_start_epoch = now_utc_epoch / SECONDS_PER_DAY * SECONDS_PER_DAY;

    // 5. Calculate percentage
    return double(now_utc_epoch - today_start_epoch) / SECONDS_PER_DAY;
}

/* Matrix calculation */
double dot_product_3(double vector_1[3], double vector_2[3]) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += vector_1[i] * vector_2 [i];
    }
    return sum;
}

// Get current position of the Sun in ENU coordinate system
void get_sun_position_ENU(double * sun_u, double * sun_v, double * sun_w) {
    /* Ecliptic coordinate system */
    double ecliptic_longitude = 2 * PI * get_percentage_of_the_utc_year();

    // Caculate the Sun’s declination
    double sin_sun_declination = - sin(ecliptic_longitude) * sin(EARTH_OBLIQUITY);

    /* Earth-centered, Earth-fixed coordinate system */
    // Calculate the subsolar point and the subsolar vector
    double sin_subsolar_latitude = sin_sun_declination;
    double cos_subsolar_latitude = sqrt(1 - pow(sin_sun_declination, 2));

    double subsolar_longitude = - 2 * PI * (get_percentage_of_the_utc_day() - 0.5);

    double subsolar_vector[3] = {
        cos_subsolar_latitude * cos(subsolar_longitude),
        cos_subsolar_latitude * sin(subsolar_longitude),
        sin_subsolar_latitude
    };

    // Caculate the unit vector of observer's ENU coordinate system
    double observer_latitude = radians(OBSERVER_LATITUDE_DEG);
    double observer_longitude = radians(OBSERVER_LONGITUDE_DEG);
    double u[3] = {
        -sin(observer_longitude),
        cos(observer_longitude),
        0
    };
    double v[3] = {
        -sin(observer_latitude) * cos(observer_longitude),
        -sin(observer_latitude) * sin(observer_longitude),
        cos(observer_latitude)
    };
    double w[3] = {
        cos(observer_latitude) * cos(observer_longitude),
        cos(observer_latitude) * sin(observer_longitude),
        sin(observer_latitude)
    };
    *sun_u = dot_product_3(u, subsolar_vector);
    *sun_v = dot_product_3(v, subsolar_vector);
    *sun_w = dot_product_3(w, subsolar_vector);
}