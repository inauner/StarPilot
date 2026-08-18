#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7195396181340429812);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6214721926630491138);
void pose_H_mod_fun(double *state, double *out_8844892526159179063);
void pose_f_fun(double *state, double dt, double *out_8367001063557654494);
void pose_F_fun(double *state, double dt, double *out_2658931498525825673);
void pose_h_4(double *state, double *unused, double *out_84081779949576375);
void pose_H_4(double *state, double *unused, double *out_7708340643954596463);
void pose_h_10(double *state, double *unused, double *out_7874962955445622717);
void pose_H_10(double *state, double *unused, double *out_8079274030706525455);
void pose_h_13(double *state, double *unused, double *out_2170051261675760671);
void pose_H_13(double *state, double *unused, double *out_7526129604422622352);
void pose_h_14(double *state, double *unused, double *out_352195394061766033);
void pose_H_14(double *state, double *unused, double *out_6775162573415470624);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}