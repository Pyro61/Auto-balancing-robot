#include "pid.hpp"

void pid::pid_reset()
{
    /* Reset controller variables */
    integrator = 0.0f;
    prev_err = 0.0f;
    differentiator = 0.0f;
    prev_meas = 0.0f;
}

void pid::pid_set_gains(const float p_gain, const float i_gain, const float d_gain)
{
    Kp = p_gain;
    Ki = i_gain;
    Kd = d_gain;
}

void pid::pid_set_tau(const float set_tau)
{
    tau = set_tau;
}

void pid::pid_set_output_limits(const float min, const float max)
{
    out_min = min;
    out_max = max;
}

void pid::pid_set_integrator_limits(const float min, const float max)
{
    integr_min = min;
    integr_max = max;
}

void pid::pid_set_sample_time(const float time_s)
{
    t = time_s;
}

float pid::pid_calculate(const float setpoint, const float measurement)
{
    /* Error signal */
    float error = setpoint - measurement;

	/* Proportional */
    float proportional = Kp * error;

    /* Integral */
    integrator = integrator + 0.5f * Ki * t * (error + prev_err);

	/* Anti-wind-up via integrator clamping */
    if (integrator > integr_max)
    {
        integrator = integr_max;
    } 

    else if (integrator < integr_min)
    {
        integrator = integr_min;
    }


	/* Derivative (band-limited differentiator) */
	differentiator = -(2.0f * Kd * (measurement - prev_meas)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                     + (2.0f * tau - t) * differentiator) / (2.0f * tau + t);

    /* Compute output and apply limits */
    float out = proportional + integrator + differentiator;

    if (out > out_max)
    {
        out = out_max;
    } 
    
    else if (out < out_min)
    {
        out = out_min;
    }

	/* Store error and measurement for later use */
    prev_err = error;
    prev_meas = measurement;

	/* Return controller output */
    return out;
}