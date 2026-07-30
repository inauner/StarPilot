#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_5959951015159528314);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5356992719595228440);
void car_H_mod_fun(double *state, double *out_81214266615305410);
void car_f_fun(double *state, double dt, double *out_3378346814935937794);
void car_F_fun(double *state, double dt, double *out_4090498247982689107);
void car_h_25(double *state, double *unused, double *out_2941759932276218077);
void car_H_25(double *state, double *unused, double *out_8002552299589165215);
void car_h_24(double *state, double *unused, double *out_3100385683736896980);
void car_H_24(double *state, double *unused, double *out_8986618170430478883);
void car_h_30(double *state, double *unused, double *out_4392391324905101244);
void car_H_30(double *state, double *unused, double *out_3474855969461557017);
void car_h_26(double *state, double *unused, double *out_3042752109523599569);
void car_H_26(double *state, double *unused, double *out_4261048980715108991);
void car_h_27(double *state, double *unused, double *out_6523014969426492774);
void car_H_27(double *state, double *unused, double *out_5698450040645500234);
void car_h_29(double *state, double *unused, double *out_609281942755744401);
void car_H_29(double *state, double *unused, double *out_3985087313775949201);
void car_h_28(double *state, double *unused, double *out_6922055836622213197);
void car_H_28(double *state, double *unused, double *out_5948717585341275452);
void car_h_31(double *state, double *unused, double *out_3216953994560723966);
void car_H_31(double *state, double *unused, double *out_3634840878481757515);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}