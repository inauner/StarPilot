#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1596408993574171648);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4927362141599677432);
void pose_H_mod_fun(double *state, double *out_7423446859784669653);
void pose_f_fun(double *state, double dt, double *out_5202695727626385220);
void pose_F_fun(double *state, double dt, double *out_5491057847988530682);
void pose_h_4(double *state, double *unused, double *out_6452546827916926575);
void pose_H_4(double *state, double *unused, double *out_2270928474745588918);
void pose_h_10(double *state, double *unused, double *out_7382153153143119100);
void pose_H_10(double *state, double *unused, double *out_8704391623487425268);
void pose_h_13(double *state, double *unused, double *out_6604156015444971944);
void pose_H_13(double *state, double *unused, double *out_941345350586743883);
void pose_h_14(double *state, double *unused, double *out_3089011552067498211);
void pose_H_14(double *state, double *unused, double *out_2706045001390472517);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}