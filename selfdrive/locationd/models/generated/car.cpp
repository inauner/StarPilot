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
void err_fun(double *nom_x, double *delta_x, double *out_5959951015159528314) {
   out_5959951015159528314[0] = delta_x[0] + nom_x[0];
   out_5959951015159528314[1] = delta_x[1] + nom_x[1];
   out_5959951015159528314[2] = delta_x[2] + nom_x[2];
   out_5959951015159528314[3] = delta_x[3] + nom_x[3];
   out_5959951015159528314[4] = delta_x[4] + nom_x[4];
   out_5959951015159528314[5] = delta_x[5] + nom_x[5];
   out_5959951015159528314[6] = delta_x[6] + nom_x[6];
   out_5959951015159528314[7] = delta_x[7] + nom_x[7];
   out_5959951015159528314[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5356992719595228440) {
   out_5356992719595228440[0] = -nom_x[0] + true_x[0];
   out_5356992719595228440[1] = -nom_x[1] + true_x[1];
   out_5356992719595228440[2] = -nom_x[2] + true_x[2];
   out_5356992719595228440[3] = -nom_x[3] + true_x[3];
   out_5356992719595228440[4] = -nom_x[4] + true_x[4];
   out_5356992719595228440[5] = -nom_x[5] + true_x[5];
   out_5356992719595228440[6] = -nom_x[6] + true_x[6];
   out_5356992719595228440[7] = -nom_x[7] + true_x[7];
   out_5356992719595228440[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_81214266615305410) {
   out_81214266615305410[0] = 1.0;
   out_81214266615305410[1] = 0.0;
   out_81214266615305410[2] = 0.0;
   out_81214266615305410[3] = 0.0;
   out_81214266615305410[4] = 0.0;
   out_81214266615305410[5] = 0.0;
   out_81214266615305410[6] = 0.0;
   out_81214266615305410[7] = 0.0;
   out_81214266615305410[8] = 0.0;
   out_81214266615305410[9] = 0.0;
   out_81214266615305410[10] = 1.0;
   out_81214266615305410[11] = 0.0;
   out_81214266615305410[12] = 0.0;
   out_81214266615305410[13] = 0.0;
   out_81214266615305410[14] = 0.0;
   out_81214266615305410[15] = 0.0;
   out_81214266615305410[16] = 0.0;
   out_81214266615305410[17] = 0.0;
   out_81214266615305410[18] = 0.0;
   out_81214266615305410[19] = 0.0;
   out_81214266615305410[20] = 1.0;
   out_81214266615305410[21] = 0.0;
   out_81214266615305410[22] = 0.0;
   out_81214266615305410[23] = 0.0;
   out_81214266615305410[24] = 0.0;
   out_81214266615305410[25] = 0.0;
   out_81214266615305410[26] = 0.0;
   out_81214266615305410[27] = 0.0;
   out_81214266615305410[28] = 0.0;
   out_81214266615305410[29] = 0.0;
   out_81214266615305410[30] = 1.0;
   out_81214266615305410[31] = 0.0;
   out_81214266615305410[32] = 0.0;
   out_81214266615305410[33] = 0.0;
   out_81214266615305410[34] = 0.0;
   out_81214266615305410[35] = 0.0;
   out_81214266615305410[36] = 0.0;
   out_81214266615305410[37] = 0.0;
   out_81214266615305410[38] = 0.0;
   out_81214266615305410[39] = 0.0;
   out_81214266615305410[40] = 1.0;
   out_81214266615305410[41] = 0.0;
   out_81214266615305410[42] = 0.0;
   out_81214266615305410[43] = 0.0;
   out_81214266615305410[44] = 0.0;
   out_81214266615305410[45] = 0.0;
   out_81214266615305410[46] = 0.0;
   out_81214266615305410[47] = 0.0;
   out_81214266615305410[48] = 0.0;
   out_81214266615305410[49] = 0.0;
   out_81214266615305410[50] = 1.0;
   out_81214266615305410[51] = 0.0;
   out_81214266615305410[52] = 0.0;
   out_81214266615305410[53] = 0.0;
   out_81214266615305410[54] = 0.0;
   out_81214266615305410[55] = 0.0;
   out_81214266615305410[56] = 0.0;
   out_81214266615305410[57] = 0.0;
   out_81214266615305410[58] = 0.0;
   out_81214266615305410[59] = 0.0;
   out_81214266615305410[60] = 1.0;
   out_81214266615305410[61] = 0.0;
   out_81214266615305410[62] = 0.0;
   out_81214266615305410[63] = 0.0;
   out_81214266615305410[64] = 0.0;
   out_81214266615305410[65] = 0.0;
   out_81214266615305410[66] = 0.0;
   out_81214266615305410[67] = 0.0;
   out_81214266615305410[68] = 0.0;
   out_81214266615305410[69] = 0.0;
   out_81214266615305410[70] = 1.0;
   out_81214266615305410[71] = 0.0;
   out_81214266615305410[72] = 0.0;
   out_81214266615305410[73] = 0.0;
   out_81214266615305410[74] = 0.0;
   out_81214266615305410[75] = 0.0;
   out_81214266615305410[76] = 0.0;
   out_81214266615305410[77] = 0.0;
   out_81214266615305410[78] = 0.0;
   out_81214266615305410[79] = 0.0;
   out_81214266615305410[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3378346814935937794) {
   out_3378346814935937794[0] = state[0];
   out_3378346814935937794[1] = state[1];
   out_3378346814935937794[2] = state[2];
   out_3378346814935937794[3] = state[3];
   out_3378346814935937794[4] = state[4];
   out_3378346814935937794[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3378346814935937794[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3378346814935937794[7] = state[7];
   out_3378346814935937794[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4090498247982689107) {
   out_4090498247982689107[0] = 1;
   out_4090498247982689107[1] = 0;
   out_4090498247982689107[2] = 0;
   out_4090498247982689107[3] = 0;
   out_4090498247982689107[4] = 0;
   out_4090498247982689107[5] = 0;
   out_4090498247982689107[6] = 0;
   out_4090498247982689107[7] = 0;
   out_4090498247982689107[8] = 0;
   out_4090498247982689107[9] = 0;
   out_4090498247982689107[10] = 1;
   out_4090498247982689107[11] = 0;
   out_4090498247982689107[12] = 0;
   out_4090498247982689107[13] = 0;
   out_4090498247982689107[14] = 0;
   out_4090498247982689107[15] = 0;
   out_4090498247982689107[16] = 0;
   out_4090498247982689107[17] = 0;
   out_4090498247982689107[18] = 0;
   out_4090498247982689107[19] = 0;
   out_4090498247982689107[20] = 1;
   out_4090498247982689107[21] = 0;
   out_4090498247982689107[22] = 0;
   out_4090498247982689107[23] = 0;
   out_4090498247982689107[24] = 0;
   out_4090498247982689107[25] = 0;
   out_4090498247982689107[26] = 0;
   out_4090498247982689107[27] = 0;
   out_4090498247982689107[28] = 0;
   out_4090498247982689107[29] = 0;
   out_4090498247982689107[30] = 1;
   out_4090498247982689107[31] = 0;
   out_4090498247982689107[32] = 0;
   out_4090498247982689107[33] = 0;
   out_4090498247982689107[34] = 0;
   out_4090498247982689107[35] = 0;
   out_4090498247982689107[36] = 0;
   out_4090498247982689107[37] = 0;
   out_4090498247982689107[38] = 0;
   out_4090498247982689107[39] = 0;
   out_4090498247982689107[40] = 1;
   out_4090498247982689107[41] = 0;
   out_4090498247982689107[42] = 0;
   out_4090498247982689107[43] = 0;
   out_4090498247982689107[44] = 0;
   out_4090498247982689107[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4090498247982689107[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4090498247982689107[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4090498247982689107[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4090498247982689107[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4090498247982689107[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4090498247982689107[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4090498247982689107[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4090498247982689107[53] = -9.8100000000000005*dt;
   out_4090498247982689107[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4090498247982689107[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4090498247982689107[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4090498247982689107[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4090498247982689107[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4090498247982689107[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4090498247982689107[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4090498247982689107[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4090498247982689107[62] = 0;
   out_4090498247982689107[63] = 0;
   out_4090498247982689107[64] = 0;
   out_4090498247982689107[65] = 0;
   out_4090498247982689107[66] = 0;
   out_4090498247982689107[67] = 0;
   out_4090498247982689107[68] = 0;
   out_4090498247982689107[69] = 0;
   out_4090498247982689107[70] = 1;
   out_4090498247982689107[71] = 0;
   out_4090498247982689107[72] = 0;
   out_4090498247982689107[73] = 0;
   out_4090498247982689107[74] = 0;
   out_4090498247982689107[75] = 0;
   out_4090498247982689107[76] = 0;
   out_4090498247982689107[77] = 0;
   out_4090498247982689107[78] = 0;
   out_4090498247982689107[79] = 0;
   out_4090498247982689107[80] = 1;
}
void h_25(double *state, double *unused, double *out_2941759932276218077) {
   out_2941759932276218077[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8002552299589165215) {
   out_8002552299589165215[0] = 0;
   out_8002552299589165215[1] = 0;
   out_8002552299589165215[2] = 0;
   out_8002552299589165215[3] = 0;
   out_8002552299589165215[4] = 0;
   out_8002552299589165215[5] = 0;
   out_8002552299589165215[6] = 1;
   out_8002552299589165215[7] = 0;
   out_8002552299589165215[8] = 0;
}
void h_24(double *state, double *unused, double *out_3100385683736896980) {
   out_3100385683736896980[0] = state[4];
   out_3100385683736896980[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8986618170430478883) {
   out_8986618170430478883[0] = 0;
   out_8986618170430478883[1] = 0;
   out_8986618170430478883[2] = 0;
   out_8986618170430478883[3] = 0;
   out_8986618170430478883[4] = 1;
   out_8986618170430478883[5] = 0;
   out_8986618170430478883[6] = 0;
   out_8986618170430478883[7] = 0;
   out_8986618170430478883[8] = 0;
   out_8986618170430478883[9] = 0;
   out_8986618170430478883[10] = 0;
   out_8986618170430478883[11] = 0;
   out_8986618170430478883[12] = 0;
   out_8986618170430478883[13] = 0;
   out_8986618170430478883[14] = 1;
   out_8986618170430478883[15] = 0;
   out_8986618170430478883[16] = 0;
   out_8986618170430478883[17] = 0;
}
void h_30(double *state, double *unused, double *out_4392391324905101244) {
   out_4392391324905101244[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3474855969461557017) {
   out_3474855969461557017[0] = 0;
   out_3474855969461557017[1] = 0;
   out_3474855969461557017[2] = 0;
   out_3474855969461557017[3] = 0;
   out_3474855969461557017[4] = 1;
   out_3474855969461557017[5] = 0;
   out_3474855969461557017[6] = 0;
   out_3474855969461557017[7] = 0;
   out_3474855969461557017[8] = 0;
}
void h_26(double *state, double *unused, double *out_3042752109523599569) {
   out_3042752109523599569[0] = state[7];
}
void H_26(double *state, double *unused, double *out_4261048980715108991) {
   out_4261048980715108991[0] = 0;
   out_4261048980715108991[1] = 0;
   out_4261048980715108991[2] = 0;
   out_4261048980715108991[3] = 0;
   out_4261048980715108991[4] = 0;
   out_4261048980715108991[5] = 0;
   out_4261048980715108991[6] = 0;
   out_4261048980715108991[7] = 1;
   out_4261048980715108991[8] = 0;
}
void h_27(double *state, double *unused, double *out_6523014969426492774) {
   out_6523014969426492774[0] = state[3];
}
void H_27(double *state, double *unused, double *out_5698450040645500234) {
   out_5698450040645500234[0] = 0;
   out_5698450040645500234[1] = 0;
   out_5698450040645500234[2] = 0;
   out_5698450040645500234[3] = 1;
   out_5698450040645500234[4] = 0;
   out_5698450040645500234[5] = 0;
   out_5698450040645500234[6] = 0;
   out_5698450040645500234[7] = 0;
   out_5698450040645500234[8] = 0;
}
void h_29(double *state, double *unused, double *out_609281942755744401) {
   out_609281942755744401[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3985087313775949201) {
   out_3985087313775949201[0] = 0;
   out_3985087313775949201[1] = 1;
   out_3985087313775949201[2] = 0;
   out_3985087313775949201[3] = 0;
   out_3985087313775949201[4] = 0;
   out_3985087313775949201[5] = 0;
   out_3985087313775949201[6] = 0;
   out_3985087313775949201[7] = 0;
   out_3985087313775949201[8] = 0;
}
void h_28(double *state, double *unused, double *out_6922055836622213197) {
   out_6922055836622213197[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5948717585341275452) {
   out_5948717585341275452[0] = 1;
   out_5948717585341275452[1] = 0;
   out_5948717585341275452[2] = 0;
   out_5948717585341275452[3] = 0;
   out_5948717585341275452[4] = 0;
   out_5948717585341275452[5] = 0;
   out_5948717585341275452[6] = 0;
   out_5948717585341275452[7] = 0;
   out_5948717585341275452[8] = 0;
}
void h_31(double *state, double *unused, double *out_3216953994560723966) {
   out_3216953994560723966[0] = state[8];
}
void H_31(double *state, double *unused, double *out_3634840878481757515) {
   out_3634840878481757515[0] = 0;
   out_3634840878481757515[1] = 0;
   out_3634840878481757515[2] = 0;
   out_3634840878481757515[3] = 0;
   out_3634840878481757515[4] = 0;
   out_3634840878481757515[5] = 0;
   out_3634840878481757515[6] = 0;
   out_3634840878481757515[7] = 0;
   out_3634840878481757515[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5959951015159528314) {
  err_fun(nom_x, delta_x, out_5959951015159528314);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5356992719595228440) {
  inv_err_fun(nom_x, true_x, out_5356992719595228440);
}
void car_H_mod_fun(double *state, double *out_81214266615305410) {
  H_mod_fun(state, out_81214266615305410);
}
void car_f_fun(double *state, double dt, double *out_3378346814935937794) {
  f_fun(state,  dt, out_3378346814935937794);
}
void car_F_fun(double *state, double dt, double *out_4090498247982689107) {
  F_fun(state,  dt, out_4090498247982689107);
}
void car_h_25(double *state, double *unused, double *out_2941759932276218077) {
  h_25(state, unused, out_2941759932276218077);
}
void car_H_25(double *state, double *unused, double *out_8002552299589165215) {
  H_25(state, unused, out_8002552299589165215);
}
void car_h_24(double *state, double *unused, double *out_3100385683736896980) {
  h_24(state, unused, out_3100385683736896980);
}
void car_H_24(double *state, double *unused, double *out_8986618170430478883) {
  H_24(state, unused, out_8986618170430478883);
}
void car_h_30(double *state, double *unused, double *out_4392391324905101244) {
  h_30(state, unused, out_4392391324905101244);
}
void car_H_30(double *state, double *unused, double *out_3474855969461557017) {
  H_30(state, unused, out_3474855969461557017);
}
void car_h_26(double *state, double *unused, double *out_3042752109523599569) {
  h_26(state, unused, out_3042752109523599569);
}
void car_H_26(double *state, double *unused, double *out_4261048980715108991) {
  H_26(state, unused, out_4261048980715108991);
}
void car_h_27(double *state, double *unused, double *out_6523014969426492774) {
  h_27(state, unused, out_6523014969426492774);
}
void car_H_27(double *state, double *unused, double *out_5698450040645500234) {
  H_27(state, unused, out_5698450040645500234);
}
void car_h_29(double *state, double *unused, double *out_609281942755744401) {
  h_29(state, unused, out_609281942755744401);
}
void car_H_29(double *state, double *unused, double *out_3985087313775949201) {
  H_29(state, unused, out_3985087313775949201);
}
void car_h_28(double *state, double *unused, double *out_6922055836622213197) {
  h_28(state, unused, out_6922055836622213197);
}
void car_H_28(double *state, double *unused, double *out_5948717585341275452) {
  H_28(state, unused, out_5948717585341275452);
}
void car_h_31(double *state, double *unused, double *out_3216953994560723966) {
  h_31(state, unused, out_3216953994560723966);
}
void car_H_31(double *state, double *unused, double *out_3634840878481757515) {
  H_31(state, unused, out_3634840878481757515);
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
