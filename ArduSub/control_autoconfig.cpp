#include "Sub.h"


/*
 * control_autoconfig.pde - automatic configuration of thruster directions
 */

// Anonymous namespace to hold variables used only in this file
namespace {
uint8_t  config_attempts;
uint8_t  chan;
uint8_t  max_chan           = 6;
uint16_t pwm                = 1300;
uint16_t pwm_neutral        = 1500;
uint32_t config_start_time;
float    initial_rpy[3];
}

// autoconfig_init - initialise auto-configuration controller
bool Sub::acro_init()
{
    // set target altitude to zero for reporting
//     pos_control.set_alt_target(0);

    // stop all motors
    motors.output_min();

    // reset to channel 1
    chan = 0;

    // reset start time
    config_start_time = AP_HAL::millis();

    // measure starting attitude
    initial_rpy[0] = ahrs_view.roll;
    initial_rpy[1] = ahrs_view.pitch;
    initial_rpy[2] = ahrs_view.yaw;

    // reset attempts counter
    config_attempts = 0;

    return true;
}

// autoconfig_run - runs the auto-configuration controller
// should be called at 100hz or more
void Sub::acro_run()
{
    // if not armed set throttle to zero and exit immediately
    if (!motors.armed()) {
        motors.set_desired_spool_state(AP_Motors::DESIRED_SPIN_WHEN_ARMED);
        attitude_control.set_throttle_out_unstabilized(0,true,g.throttle_filt);
        return;
    }

    motors.set_desired_spool_state(AP_Motors::DESIRED_THROTTLE_UNLIMITED);

    // get current time
    uint32_t now = AP_HAL::millis();

    // run one motor at a time
    bool motor_set = motors.set_output(chan, pwm);

    // check gyro (and accelerometer?)
    // integrate rotation rate over __ seconds
    // if enough time has elapsed
    if (now - config_start_time > 1000) {
//         float final_rpy[] = {ahrs_view.roll, ahrs_view.pitch, ahrs_view.yaw};
//         float delta_rpy[3];
//         for (int i=0; i<3; i++) {
//             delta_rpy[i] = final_rpy[i] - initial_rpy[i];
//         }
        // dot product against reference (xref,yref,zref)*(dx,dy,dz)
            // motors.get_rpy_factor(chan) .* (dx, dy, dz)
//         float dot_product = 0;
//         for (int i=0; i<3; i++) {
//             dot_product += motors.get_rpy_factor(chan)[i] * delta_rpy[i];
//         }
        // if dot product is greater than threshold, non-reversed
            // move on to next motor
            chan++;

        // else if less than negative threshold, reversed
            // move on to next motor
//             chan++;

        // else inconclusive, redo

        // quit if next motor is beyond the last one
        if (chan >= max_chan) {
            // go to manual mode
            set_mode(MANUAL, MODE_REASON_UNKNOWN);
        }

        // stop all motors
        motors.output_min();

        // measure starting attitude
        initial_rpy[0] = ahrs_view.roll;
        initial_rpy[1] = ahrs_view.pitch;
        initial_rpy[2] = ahrs_view.yaw;

        // reset attempts counter
        config_attempts = 0;

        // reset start time
        config_start_time = AP_HAL::millis();
    }

}


// get_pilot_desired_angle_rates - transform pilot's roll pitch and yaw input into a desired lean angle rates
// returns desired angle rates in centi-degrees-per-second
void Sub::get_pilot_desired_angle_rates(int16_t roll_in, int16_t pitch_in, int16_t yaw_in, float &roll_out, float &pitch_out, float &yaw_out)
{
//     float rate_limit;
//     Vector3f rate_ef_level, rate_bf_level, rate_bf_request;

    // apply circular limit to pitch and roll inputs
//     float total_in = norm(pitch_in, roll_in);

//     if (total_in > ROLL_PITCH_INPUT_MAX) {
//         float ratio = (float)ROLL_PITCH_INPUT_MAX / total_in;
//         roll_in *= ratio;
//         pitch_in *= ratio;
//     }

    // calculate roll, pitch rate requests
//     if (g.acro_expo <= 0) {
//         rate_bf_request.x = roll_in * g.acro_rp_p;
//         rate_bf_request.y = pitch_in * g.acro_rp_p;
//     } else {
        // expo variables
//         float rp_in, rp_in3, rp_out;

        // range check expo
//         if (g.acro_expo > 1.0f) {
//             g.acro_expo = 1.0f;
//         }

        // roll expo
//         rp_in = float(roll_in)/ROLL_PITCH_INPUT_MAX;
//         rp_in3 = rp_in*rp_in*rp_in;
//         rp_out = (g.acro_expo * rp_in3) + ((1 - g.acro_expo) * rp_in);
//         rate_bf_request.x = ROLL_PITCH_INPUT_MAX * rp_out * g.acro_rp_p;

        // pitch expo
//         rp_in = float(pitch_in)/ROLL_PITCH_INPUT_MAX;
//         rp_in3 = rp_in*rp_in*rp_in;
//         rp_out = (g.acro_expo * rp_in3) + ((1 - g.acro_expo) * rp_in);
//         rate_bf_request.y = ROLL_PITCH_INPUT_MAX * rp_out * g.acro_rp_p;
//     }

    // calculate yaw rate request
//     rate_bf_request.z = yaw_in * g.acro_yaw_p;

    // calculate earth frame rate corrections to pull the vehicle back to level while in ACRO mode

//     if (g.acro_trainer != ACRO_TRAINER_DISABLED) {
        // Calculate trainer mode earth frame rate command for roll
//         int32_t roll_angle = wrap_180_cd(ahrs.roll_sensor);
//         rate_ef_level.x = -constrain_int32(roll_angle, -ACRO_LEVEL_MAX_ANGLE, ACRO_LEVEL_MAX_ANGLE) * g.acro_balance_roll;

        // Calculate trainer mode earth frame rate command for pitch
//         int32_t pitch_angle = wrap_180_cd(ahrs.pitch_sensor);
//         rate_ef_level.y = -constrain_int32(pitch_angle, -ACRO_LEVEL_MAX_ANGLE, ACRO_LEVEL_MAX_ANGLE) * g.acro_balance_pitch;

        // Calculate trainer mode earth frame rate command for yaw
//         rate_ef_level.z = 0;

        // Calculate angle limiting earth frame rate commands
//         if (g.acro_trainer == ACRO_TRAINER_LIMITED) {
//             if (roll_angle > aparm.angle_max) {
//                 rate_ef_level.x -=  g.acro_balance_roll*(roll_angle-aparm.angle_max);
//             } else if (roll_angle < -aparm.angle_max) {
//                 rate_ef_level.x -=  g.acro_balance_roll*(roll_angle+aparm.angle_max);
//             }

//             if (pitch_angle > aparm.angle_max) {
//                 rate_ef_level.y -=  g.acro_balance_pitch*(pitch_angle-aparm.angle_max);
//             } else if (pitch_angle < -aparm.angle_max) {
//                 rate_ef_level.y -=  g.acro_balance_pitch*(pitch_angle+aparm.angle_max);
//             }
//         }

        // convert earth-frame level rates to body-frame level rates
//         attitude_control.euler_rate_to_ang_vel(attitude_control.get_att_target_euler_cd()*radians(0.01f), rate_ef_level, rate_bf_level);

        // combine earth frame rate corrections with rate requests
//         if (g.acro_trainer == ACRO_TRAINER_LIMITED) {
//             rate_bf_request.x += rate_bf_level.x;
//             rate_bf_request.y += rate_bf_level.y;
//             rate_bf_request.z += rate_bf_level.z;
//         } else {
//             float acro_level_mix = constrain_float(1-MAX(MAX(abs(roll_in), abs(pitch_in)), abs(yaw_in))/4500.0, 0, 1)*ahrs.cos_pitch();

            // Scale leveling rates by stick input
//             rate_bf_level = rate_bf_level*acro_level_mix;

            // Calculate rate limit to prevent change of rate through inverted
//             rate_limit = fabsf(fabsf(rate_bf_request.x)-fabsf(rate_bf_level.x));
//             rate_bf_request.x += rate_bf_level.x;
//             rate_bf_request.x = constrain_float(rate_bf_request.x, -rate_limit, rate_limit);

            // Calculate rate limit to prevent change of rate through inverted
//             rate_limit = fabsf(fabsf(rate_bf_request.y)-fabsf(rate_bf_level.y));
//             rate_bf_request.y += rate_bf_level.y;
//             rate_bf_request.y = constrain_float(rate_bf_request.y, -rate_limit, rate_limit);

            // Calculate rate limit to prevent change of rate through inverted
//             rate_limit = fabsf(fabsf(rate_bf_request.z)-fabsf(rate_bf_level.z));
//             rate_bf_request.z += rate_bf_level.z;
//             rate_bf_request.z = constrain_float(rate_bf_request.z, -rate_limit, rate_limit);
//         }
//     }

    // hand back rate request
//     roll_out = rate_bf_request.x;
//     pitch_out = rate_bf_request.y;
//     yaw_out = rate_bf_request.z;
}
