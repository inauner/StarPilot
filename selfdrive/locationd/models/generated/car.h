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
void car_err_fun(double *nom_x, double *delta_x, double *out_3258091067705697569);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2755855052909692674);
void car_H_mod_fun(double *state, double *out_7781028056435054793);
void car_f_fun(double *state, double dt, double *out_2481579766534270415);
void car_F_fun(double *state, double dt, double *out_3144021140418811034);
void car_h_25(double *state, double *unused, double *out_6782490269790769427);
void car_H_25(double *state, double *unused, double *out_4168693057384466872);
void car_h_24(double *state, double *unused, double *out_8556983568758949116);
void car_H_24(double *state, double *unused, double *out_9004556733412072150);
void car_h_30(double *state, double *unused, double *out_3145040741536747077);
void car_H_30(double *state, double *unused, double *out_359003272743141326);
void car_h_26(double *state, double *unused, double *out_4127124834872045189);
void car_H_26(double *state, double *unused, double *out_427189738510410648);
void car_h_27(double *state, double *unused, double *out_1780817002129923782);
void car_H_27(double *state, double *unused, double *out_2533766584543566237);
void car_h_29(double *state, double *unused, double *out_8902661814758955393);
void car_H_29(double *state, double *unused, double *out_151228071571250858);
void car_h_28(double *state, double *unused, double *out_7683957899572787069);
void car_H_28(double *state, double *unused, double *out_4931170945498279716);
void car_h_31(double *state, double *unused, double *out_678068494458118898);
void car_H_31(double *state, double *unused, double *out_199018363722940828);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}