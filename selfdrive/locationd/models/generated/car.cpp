#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_3258091067705697569) {
   out_3258091067705697569[0] = delta_x[0] + nom_x[0];
   out_3258091067705697569[1] = delta_x[1] + nom_x[1];
   out_3258091067705697569[2] = delta_x[2] + nom_x[2];
   out_3258091067705697569[3] = delta_x[3] + nom_x[3];
   out_3258091067705697569[4] = delta_x[4] + nom_x[4];
   out_3258091067705697569[5] = delta_x[5] + nom_x[5];
   out_3258091067705697569[6] = delta_x[6] + nom_x[6];
   out_3258091067705697569[7] = delta_x[7] + nom_x[7];
   out_3258091067705697569[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2755855052909692674) {
   out_2755855052909692674[0] = -nom_x[0] + true_x[0];
   out_2755855052909692674[1] = -nom_x[1] + true_x[1];
   out_2755855052909692674[2] = -nom_x[2] + true_x[2];
   out_2755855052909692674[3] = -nom_x[3] + true_x[3];
   out_2755855052909692674[4] = -nom_x[4] + true_x[4];
   out_2755855052909692674[5] = -nom_x[5] + true_x[5];
   out_2755855052909692674[6] = -nom_x[6] + true_x[6];
   out_2755855052909692674[7] = -nom_x[7] + true_x[7];
   out_2755855052909692674[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7781028056435054793) {
   out_7781028056435054793[0] = 1.0;
   out_7781028056435054793[1] = 0.0;
   out_7781028056435054793[2] = 0.0;
   out_7781028056435054793[3] = 0.0;
   out_7781028056435054793[4] = 0.0;
   out_7781028056435054793[5] = 0.0;
   out_7781028056435054793[6] = 0.0;
   out_7781028056435054793[7] = 0.0;
   out_7781028056435054793[8] = 0.0;
   out_7781028056435054793[9] = 0.0;
   out_7781028056435054793[10] = 1.0;
   out_7781028056435054793[11] = 0.0;
   out_7781028056435054793[12] = 0.0;
   out_7781028056435054793[13] = 0.0;
   out_7781028056435054793[14] = 0.0;
   out_7781028056435054793[15] = 0.0;
   out_7781028056435054793[16] = 0.0;
   out_7781028056435054793[17] = 0.0;
   out_7781028056435054793[18] = 0.0;
   out_7781028056435054793[19] = 0.0;
   out_7781028056435054793[20] = 1.0;
   out_7781028056435054793[21] = 0.0;
   out_7781028056435054793[22] = 0.0;
   out_7781028056435054793[23] = 0.0;
   out_7781028056435054793[24] = 0.0;
   out_7781028056435054793[25] = 0.0;
   out_7781028056435054793[26] = 0.0;
   out_7781028056435054793[27] = 0.0;
   out_7781028056435054793[28] = 0.0;
   out_7781028056435054793[29] = 0.0;
   out_7781028056435054793[30] = 1.0;
   out_7781028056435054793[31] = 0.0;
   out_7781028056435054793[32] = 0.0;
   out_7781028056435054793[33] = 0.0;
   out_7781028056435054793[34] = 0.0;
   out_7781028056435054793[35] = 0.0;
   out_7781028056435054793[36] = 0.0;
   out_7781028056435054793[37] = 0.0;
   out_7781028056435054793[38] = 0.0;
   out_7781028056435054793[39] = 0.0;
   out_7781028056435054793[40] = 1.0;
   out_7781028056435054793[41] = 0.0;
   out_7781028056435054793[42] = 0.0;
   out_7781028056435054793[43] = 0.0;
   out_7781028056435054793[44] = 0.0;
   out_7781028056435054793[45] = 0.0;
   out_7781028056435054793[46] = 0.0;
   out_7781028056435054793[47] = 0.0;
   out_7781028056435054793[48] = 0.0;
   out_7781028056435054793[49] = 0.0;
   out_7781028056435054793[50] = 1.0;
   out_7781028056435054793[51] = 0.0;
   out_7781028056435054793[52] = 0.0;
   out_7781028056435054793[53] = 0.0;
   out_7781028056435054793[54] = 0.0;
   out_7781028056435054793[55] = 0.0;
   out_7781028056435054793[56] = 0.0;
   out_7781028056435054793[57] = 0.0;
   out_7781028056435054793[58] = 0.0;
   out_7781028056435054793[59] = 0.0;
   out_7781028056435054793[60] = 1.0;
   out_7781028056435054793[61] = 0.0;
   out_7781028056435054793[62] = 0.0;
   out_7781028056435054793[63] = 0.0;
   out_7781028056435054793[64] = 0.0;
   out_7781028056435054793[65] = 0.0;
   out_7781028056435054793[66] = 0.0;
   out_7781028056435054793[67] = 0.0;
   out_7781028056435054793[68] = 0.0;
   out_7781028056435054793[69] = 0.0;
   out_7781028056435054793[70] = 1.0;
   out_7781028056435054793[71] = 0.0;
   out_7781028056435054793[72] = 0.0;
   out_7781028056435054793[73] = 0.0;
   out_7781028056435054793[74] = 0.0;
   out_7781028056435054793[75] = 0.0;
   out_7781028056435054793[76] = 0.0;
   out_7781028056435054793[77] = 0.0;
   out_7781028056435054793[78] = 0.0;
   out_7781028056435054793[79] = 0.0;
   out_7781028056435054793[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2481579766534270415) {
   out_2481579766534270415[0] = state[0];
   out_2481579766534270415[1] = state[1];
   out_2481579766534270415[2] = state[2];
   out_2481579766534270415[3] = state[3];
   out_2481579766534270415[4] = state[4];
   out_2481579766534270415[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2481579766534270415[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2481579766534270415[7] = state[7];
   out_2481579766534270415[8] = state[8];
}
void F_fun(double *state, double dt, double *out_3144021140418811034) {
   out_3144021140418811034[0] = 1;
   out_3144021140418811034[1] = 0;
   out_3144021140418811034[2] = 0;
   out_3144021140418811034[3] = 0;
   out_3144021140418811034[4] = 0;
   out_3144021140418811034[5] = 0;
   out_3144021140418811034[6] = 0;
   out_3144021140418811034[7] = 0;
   out_3144021140418811034[8] = 0;
   out_3144021140418811034[9] = 0;
   out_3144021140418811034[10] = 1;
   out_3144021140418811034[11] = 0;
   out_3144021140418811034[12] = 0;
   out_3144021140418811034[13] = 0;
   out_3144021140418811034[14] = 0;
   out_3144021140418811034[15] = 0;
   out_3144021140418811034[16] = 0;
   out_3144021140418811034[17] = 0;
   out_3144021140418811034[18] = 0;
   out_3144021140418811034[19] = 0;
   out_3144021140418811034[20] = 1;
   out_3144021140418811034[21] = 0;
   out_3144021140418811034[22] = 0;
   out_3144021140418811034[23] = 0;
   out_3144021140418811034[24] = 0;
   out_3144021140418811034[25] = 0;
   out_3144021140418811034[26] = 0;
   out_3144021140418811034[27] = 0;
   out_3144021140418811034[28] = 0;
   out_3144021140418811034[29] = 0;
   out_3144021140418811034[30] = 1;
   out_3144021140418811034[31] = 0;
   out_3144021140418811034[32] = 0;
   out_3144021140418811034[33] = 0;
   out_3144021140418811034[34] = 0;
   out_3144021140418811034[35] = 0;
   out_3144021140418811034[36] = 0;
   out_3144021140418811034[37] = 0;
   out_3144021140418811034[38] = 0;
   out_3144021140418811034[39] = 0;
   out_3144021140418811034[40] = 1;
   out_3144021140418811034[41] = 0;
   out_3144021140418811034[42] = 0;
   out_3144021140418811034[43] = 0;
   out_3144021140418811034[44] = 0;
   out_3144021140418811034[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_3144021140418811034[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_3144021140418811034[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3144021140418811034[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3144021140418811034[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_3144021140418811034[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_3144021140418811034[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_3144021140418811034[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_3144021140418811034[53] = -9.8100000000000005*dt;
   out_3144021140418811034[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_3144021140418811034[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_3144021140418811034[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3144021140418811034[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3144021140418811034[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_3144021140418811034[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_3144021140418811034[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_3144021140418811034[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3144021140418811034[62] = 0;
   out_3144021140418811034[63] = 0;
   out_3144021140418811034[64] = 0;
   out_3144021140418811034[65] = 0;
   out_3144021140418811034[66] = 0;
   out_3144021140418811034[67] = 0;
   out_3144021140418811034[68] = 0;
   out_3144021140418811034[69] = 0;
   out_3144021140418811034[70] = 1;
   out_3144021140418811034[71] = 0;
   out_3144021140418811034[72] = 0;
   out_3144021140418811034[73] = 0;
   out_3144021140418811034[74] = 0;
   out_3144021140418811034[75] = 0;
   out_3144021140418811034[76] = 0;
   out_3144021140418811034[77] = 0;
   out_3144021140418811034[78] = 0;
   out_3144021140418811034[79] = 0;
   out_3144021140418811034[80] = 1;
}
void h_25(double *state, double *unused, double *out_6782490269790769427) {
   out_6782490269790769427[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4168693057384466872) {
   out_4168693057384466872[0] = 0;
   out_4168693057384466872[1] = 0;
   out_4168693057384466872[2] = 0;
   out_4168693057384466872[3] = 0;
   out_4168693057384466872[4] = 0;
   out_4168693057384466872[5] = 0;
   out_4168693057384466872[6] = 1;
   out_4168693057384466872[7] = 0;
   out_4168693057384466872[8] = 0;
}
void h_24(double *state, double *unused, double *out_8556983568758949116) {
   out_8556983568758949116[0] = state[4];
   out_8556983568758949116[1] = state[5];
}
void H_24(double *state, double *unused, double *out_9004556733412072150) {
   out_9004556733412072150[0] = 0;
   out_9004556733412072150[1] = 0;
   out_9004556733412072150[2] = 0;
   out_9004556733412072150[3] = 0;
   out_9004556733412072150[4] = 1;
   out_9004556733412072150[5] = 0;
   out_9004556733412072150[6] = 0;
   out_9004556733412072150[7] = 0;
   out_9004556733412072150[8] = 0;
   out_9004556733412072150[9] = 0;
   out_9004556733412072150[10] = 0;
   out_9004556733412072150[11] = 0;
   out_9004556733412072150[12] = 0;
   out_9004556733412072150[13] = 0;
   out_9004556733412072150[14] = 1;
   out_9004556733412072150[15] = 0;
   out_9004556733412072150[16] = 0;
   out_9004556733412072150[17] = 0;
}
void h_30(double *state, double *unused, double *out_3145040741536747077) {
   out_3145040741536747077[0] = state[4];
}
void H_30(double *state, double *unused, double *out_359003272743141326) {
   out_359003272743141326[0] = 0;
   out_359003272743141326[1] = 0;
   out_359003272743141326[2] = 0;
   out_359003272743141326[3] = 0;
   out_359003272743141326[4] = 1;
   out_359003272743141326[5] = 0;
   out_359003272743141326[6] = 0;
   out_359003272743141326[7] = 0;
   out_359003272743141326[8] = 0;
}
void h_26(double *state, double *unused, double *out_4127124834872045189) {
   out_4127124834872045189[0] = state[7];
}
void H_26(double *state, double *unused, double *out_427189738510410648) {
   out_427189738510410648[0] = 0;
   out_427189738510410648[1] = 0;
   out_427189738510410648[2] = 0;
   out_427189738510410648[3] = 0;
   out_427189738510410648[4] = 0;
   out_427189738510410648[5] = 0;
   out_427189738510410648[6] = 0;
   out_427189738510410648[7] = 1;
   out_427189738510410648[8] = 0;
}
void h_27(double *state, double *unused, double *out_1780817002129923782) {
   out_1780817002129923782[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2533766584543566237) {
   out_2533766584543566237[0] = 0;
   out_2533766584543566237[1] = 0;
   out_2533766584543566237[2] = 0;
   out_2533766584543566237[3] = 1;
   out_2533766584543566237[4] = 0;
   out_2533766584543566237[5] = 0;
   out_2533766584543566237[6] = 0;
   out_2533766584543566237[7] = 0;
   out_2533766584543566237[8] = 0;
}
void h_29(double *state, double *unused, double *out_8902661814758955393) {
   out_8902661814758955393[0] = state[1];
}
void H_29(double *state, double *unused, double *out_151228071571250858) {
   out_151228071571250858[0] = 0;
   out_151228071571250858[1] = 1;
   out_151228071571250858[2] = 0;
   out_151228071571250858[3] = 0;
   out_151228071571250858[4] = 0;
   out_151228071571250858[5] = 0;
   out_151228071571250858[6] = 0;
   out_151228071571250858[7] = 0;
   out_151228071571250858[8] = 0;
}
void h_28(double *state, double *unused, double *out_7683957899572787069) {
   out_7683957899572787069[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4931170945498279716) {
   out_4931170945498279716[0] = 1;
   out_4931170945498279716[1] = 0;
   out_4931170945498279716[2] = 0;
   out_4931170945498279716[3] = 0;
   out_4931170945498279716[4] = 0;
   out_4931170945498279716[5] = 0;
   out_4931170945498279716[6] = 0;
   out_4931170945498279716[7] = 0;
   out_4931170945498279716[8] = 0;
}
void h_31(double *state, double *unused, double *out_678068494458118898) {
   out_678068494458118898[0] = state[8];
}
void H_31(double *state, double *unused, double *out_199018363722940828) {
   out_199018363722940828[0] = 0;
   out_199018363722940828[1] = 0;
   out_199018363722940828[2] = 0;
   out_199018363722940828[3] = 0;
   out_199018363722940828[4] = 0;
   out_199018363722940828[5] = 0;
   out_199018363722940828[6] = 0;
   out_199018363722940828[7] = 0;
   out_199018363722940828[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_3258091067705697569) {
  err_fun(nom_x, delta_x, out_3258091067705697569);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2755855052909692674) {
  inv_err_fun(nom_x, true_x, out_2755855052909692674);
}
void car_H_mod_fun(double *state, double *out_7781028056435054793) {
  H_mod_fun(state, out_7781028056435054793);
}
void car_f_fun(double *state, double dt, double *out_2481579766534270415) {
  f_fun(state,  dt, out_2481579766534270415);
}
void car_F_fun(double *state, double dt, double *out_3144021140418811034) {
  F_fun(state,  dt, out_3144021140418811034);
}
void car_h_25(double *state, double *unused, double *out_6782490269790769427) {
  h_25(state, unused, out_6782490269790769427);
}
void car_H_25(double *state, double *unused, double *out_4168693057384466872) {
  H_25(state, unused, out_4168693057384466872);
}
void car_h_24(double *state, double *unused, double *out_8556983568758949116) {
  h_24(state, unused, out_8556983568758949116);
}
void car_H_24(double *state, double *unused, double *out_9004556733412072150) {
  H_24(state, unused, out_9004556733412072150);
}
void car_h_30(double *state, double *unused, double *out_3145040741536747077) {
  h_30(state, unused, out_3145040741536747077);
}
void car_H_30(double *state, double *unused, double *out_359003272743141326) {
  H_30(state, unused, out_359003272743141326);
}
void car_h_26(double *state, double *unused, double *out_4127124834872045189) {
  h_26(state, unused, out_4127124834872045189);
}
void car_H_26(double *state, double *unused, double *out_427189738510410648) {
  H_26(state, unused, out_427189738510410648);
}
void car_h_27(double *state, double *unused, double *out_1780817002129923782) {
  h_27(state, unused, out_1780817002129923782);
}
void car_H_27(double *state, double *unused, double *out_2533766584543566237) {
  H_27(state, unused, out_2533766584543566237);
}
void car_h_29(double *state, double *unused, double *out_8902661814758955393) {
  h_29(state, unused, out_8902661814758955393);
}
void car_H_29(double *state, double *unused, double *out_151228071571250858) {
  H_29(state, unused, out_151228071571250858);
}
void car_h_28(double *state, double *unused, double *out_7683957899572787069) {
  h_28(state, unused, out_7683957899572787069);
}
void car_H_28(double *state, double *unused, double *out_4931170945498279716) {
  H_28(state, unused, out_4931170945498279716);
}
void car_h_31(double *state, double *unused, double *out_678068494458118898) {
  h_31(state, unused, out_678068494458118898);
}
void car_H_31(double *state, double *unused, double *out_199018363722940828) {
  H_31(state, unused, out_199018363722940828);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
