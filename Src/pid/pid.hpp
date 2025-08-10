#ifndef _PID_
#define _PID_

class pid
{
    private:
    float integrator;
	float prev_err;			/* Required for integrator */
	float differentiator;
	float prev_meas;		/* Required for differentiator */
	

    public:
    float Kp, Ki, Kd; /* Controller gains */
    float tau; /* Derivative low-pass filter time constant */
    float out_min, out_max; /* Output limits */
    float integr_min, integr_max; /* Integrator limits */
    float t; /* Sample time (in seconds) */

    void pid_reset();
    void pid_set_gains(const float p_gain, const float i_gain, const float d_gain);
    void pid_set_tau(const float tau);
    void pid_set_output_limits(const float min, const float max);
    void pid_set_integrator_limits(const float min, const float max);
    void pid_set_sample_time(const float time_s);
    float pid_calculate(const float setpoint, const float measurement);
};

#endif /* _PID_ */

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#endif
