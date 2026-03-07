#ifndef MOTOR_CONTROLLER
#define MOTOR_CONTROLLER

void init_motor_controller(float * angle_pt, float * target_pt);
void set_motor_phase(int phase);
void calibration(float calibration_angle);

#endif