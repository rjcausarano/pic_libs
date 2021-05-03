#include "pid.h"

static char P_ = 1;
static char I_ = 4;
static char D_ = 1;
static int old_error = 0;

int do_pid(int current, int set_point){
    int error = set_point - current;

    // Proportional
    int p_error = P_ * error;
    // Integral
    int i_error = (error - old_error) / I_;
    
    old_error = error;
    return p_error + i_error;
}
