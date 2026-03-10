#ifndef CONFIG_H
#define CONFIG_H

/* Calculation */
#define EARTH_OBLIQUITY 23.44       // deg, Earth obliquity (axial tilt)
#define SECONDS_PER_DAY 86400       // The number of seconds per day
#define SECONDS_PER_HOUR 3600       // The number of seconds per hour

/* Pin config */
// Stepper motor signal pins
// from PIN_1 to PIN_4 is the sequence of opening the blinds
// from PIN_4 to PIN_1 is the sequence of closing the blinds
#define MOTOR_PIN_1 4               // GPIO4 = D2
#define MOTOR_PIN_2 5               // GPIO5 = D3
#define MOTOR_PIN_3 6               // GPIO6 = D4
#define MOTOR_PIN_4 7               // GPIO7 = D5

/* Physical config */
#define total_step 46080            // Total steps for the blinds to turn 180 degrees
#define step_duration_ms 5          // Interval for toggling motor step in milliseconds
#define building_angle_deg 12       // The altitude angle of the top of the building in front of the blinds in degrees
#define motor_reactivation_interval_s 60

// blind_width and blind_spacing can be in any unit you want (e.g. mm, inches, meters),
// but they should be in the same unit.
// Here, millimeter is used as an example.
#define blind_width 25.0            // The width of a blind
#define blind_spacing 21.5          // The vertical distance between two blinds

/* Software */
#define request_delay_ms 10         // Delay between two groups of Notion API requests in millisenconds

/* Daily routine */
#define event_time_offset_s 30      // The target will change this much time in advance before an event in your daily routine

#endif