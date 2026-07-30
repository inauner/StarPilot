#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1596408993574171648) {
   out_1596408993574171648[0] = delta_x[0] + nom_x[0];
   out_1596408993574171648[1] = delta_x[1] + nom_x[1];
   out_1596408993574171648[2] = delta_x[2] + nom_x[2];
   out_1596408993574171648[3] = delta_x[3] + nom_x[3];
   out_1596408993574171648[4] = delta_x[4] + nom_x[4];
   out_1596408993574171648[5] = delta_x[5] + nom_x[5];
   out_1596408993574171648[6] = delta_x[6] + nom_x[6];
   out_1596408993574171648[7] = delta_x[7] + nom_x[7];
   out_1596408993574171648[8] = delta_x[8] + nom_x[8];
   out_1596408993574171648[9] = delta_x[9] + nom_x[9];
   out_1596408993574171648[10] = delta_x[10] + nom_x[10];
   out_1596408993574171648[11] = delta_x[11] + nom_x[11];
   out_1596408993574171648[12] = delta_x[12] + nom_x[12];
   out_1596408993574171648[13] = delta_x[13] + nom_x[13];
   out_1596408993574171648[14] = delta_x[14] + nom_x[14];
   out_1596408993574171648[15] = delta_x[15] + nom_x[15];
   out_1596408993574171648[16] = delta_x[16] + nom_x[16];
   out_1596408993574171648[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4927362141599677432) {
   out_4927362141599677432[0] = -nom_x[0] + true_x[0];
   out_4927362141599677432[1] = -nom_x[1] + true_x[1];
   out_4927362141599677432[2] = -nom_x[2] + true_x[2];
   out_4927362141599677432[3] = -nom_x[3] + true_x[3];
   out_4927362141599677432[4] = -nom_x[4] + true_x[4];
   out_4927362141599677432[5] = -nom_x[5] + true_x[5];
   out_4927362141599677432[6] = -nom_x[6] + true_x[6];
   out_4927362141599677432[7] = -nom_x[7] + true_x[7];
   out_4927362141599677432[8] = -nom_x[8] + true_x[8];
   out_4927362141599677432[9] = -nom_x[9] + true_x[9];
   out_4927362141599677432[10] = -nom_x[10] + true_x[10];
   out_4927362141599677432[11] = -nom_x[11] + true_x[11];
   out_4927362141599677432[12] = -nom_x[12] + true_x[12];
   out_4927362141599677432[13] = -nom_x[13] + true_x[13];
   out_4927362141599677432[14] = -nom_x[14] + true_x[14];
   out_4927362141599677432[15] = -nom_x[15] + true_x[15];
   out_4927362141599677432[16] = -nom_x[16] + true_x[16];
   out_4927362141599677432[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7423446859784669653) {
   out_7423446859784669653[0] = 1.0;
   out_7423446859784669653[1] = 0.0;
   out_7423446859784669653[2] = 0.0;
   out_7423446859784669653[3] = 0.0;
   out_7423446859784669653[4] = 0.0;
   out_7423446859784669653[5] = 0.0;
   out_7423446859784669653[6] = 0.0;
   out_7423446859784669653[7] = 0.0;
   out_7423446859784669653[8] = 0.0;
   out_7423446859784669653[9] = 0.0;
   out_7423446859784669653[10] = 0.0;
   out_7423446859784669653[11] = 0.0;
   out_7423446859784669653[12] = 0.0;
   out_7423446859784669653[13] = 0.0;
   out_7423446859784669653[14] = 0.0;
   out_7423446859784669653[15] = 0.0;
   out_7423446859784669653[16] = 0.0;
   out_7423446859784669653[17] = 0.0;
   out_7423446859784669653[18] = 0.0;
   out_7423446859784669653[19] = 1.0;
   out_7423446859784669653[20] = 0.0;
   out_7423446859784669653[21] = 0.0;
   out_7423446859784669653[22] = 0.0;
   out_7423446859784669653[23] = 0.0;
   out_7423446859784669653[24] = 0.0;
   out_7423446859784669653[25] = 0.0;
   out_7423446859784669653[26] = 0.0;
   out_7423446859784669653[27] = 0.0;
   out_7423446859784669653[28] = 0.0;
   out_7423446859784669653[29] = 0.0;
   out_7423446859784669653[30] = 0.0;
   out_7423446859784669653[31] = 0.0;
   out_7423446859784669653[32] = 0.0;
   out_7423446859784669653[33] = 0.0;
   out_7423446859784669653[34] = 0.0;
   out_7423446859784669653[35] = 0.0;
   out_7423446859784669653[36] = 0.0;
   out_7423446859784669653[37] = 0.0;
   out_7423446859784669653[38] = 1.0;
   out_7423446859784669653[39] = 0.0;
   out_7423446859784669653[40] = 0.0;
   out_7423446859784669653[41] = 0.0;
   out_7423446859784669653[42] = 0.0;
   out_7423446859784669653[43] = 0.0;
   out_7423446859784669653[44] = 0.0;
   out_7423446859784669653[45] = 0.0;
   out_7423446859784669653[46] = 0.0;
   out_7423446859784669653[47] = 0.0;
   out_7423446859784669653[48] = 0.0;
   out_7423446859784669653[49] = 0.0;
   out_7423446859784669653[50] = 0.0;
   out_7423446859784669653[51] = 0.0;
   out_7423446859784669653[52] = 0.0;
   out_7423446859784669653[53] = 0.0;
   out_7423446859784669653[54] = 0.0;
   out_7423446859784669653[55] = 0.0;
   out_7423446859784669653[56] = 0.0;
   out_7423446859784669653[57] = 1.0;
   out_7423446859784669653[58] = 0.0;
   out_7423446859784669653[59] = 0.0;
   out_7423446859784669653[60] = 0.0;
   out_7423446859784669653[61] = 0.0;
   out_7423446859784669653[62] = 0.0;
   out_7423446859784669653[63] = 0.0;
   out_7423446859784669653[64] = 0.0;
   out_7423446859784669653[65] = 0.0;
   out_7423446859784669653[66] = 0.0;
   out_7423446859784669653[67] = 0.0;
   out_7423446859784669653[68] = 0.0;
   out_7423446859784669653[69] = 0.0;
   out_7423446859784669653[70] = 0.0;
   out_7423446859784669653[71] = 0.0;
   out_7423446859784669653[72] = 0.0;
   out_7423446859784669653[73] = 0.0;
   out_7423446859784669653[74] = 0.0;
   out_7423446859784669653[75] = 0.0;
   out_7423446859784669653[76] = 1.0;
   out_7423446859784669653[77] = 0.0;
   out_7423446859784669653[78] = 0.0;
   out_7423446859784669653[79] = 0.0;
   out_7423446859784669653[80] = 0.0;
   out_7423446859784669653[81] = 0.0;
   out_7423446859784669653[82] = 0.0;
   out_7423446859784669653[83] = 0.0;
   out_7423446859784669653[84] = 0.0;
   out_7423446859784669653[85] = 0.0;
   out_7423446859784669653[86] = 0.0;
   out_7423446859784669653[87] = 0.0;
   out_7423446859784669653[88] = 0.0;
   out_7423446859784669653[89] = 0.0;
   out_7423446859784669653[90] = 0.0;
   out_7423446859784669653[91] = 0.0;
   out_7423446859784669653[92] = 0.0;
   out_7423446859784669653[93] = 0.0;
   out_7423446859784669653[94] = 0.0;
   out_7423446859784669653[95] = 1.0;
   out_7423446859784669653[96] = 0.0;
   out_7423446859784669653[97] = 0.0;
   out_7423446859784669653[98] = 0.0;
   out_7423446859784669653[99] = 0.0;
   out_7423446859784669653[100] = 0.0;
   out_7423446859784669653[101] = 0.0;
   out_7423446859784669653[102] = 0.0;
   out_7423446859784669653[103] = 0.0;
   out_7423446859784669653[104] = 0.0;
   out_7423446859784669653[105] = 0.0;
   out_7423446859784669653[106] = 0.0;
   out_7423446859784669653[107] = 0.0;
   out_7423446859784669653[108] = 0.0;
   out_7423446859784669653[109] = 0.0;
   out_7423446859784669653[110] = 0.0;
   out_7423446859784669653[111] = 0.0;
   out_7423446859784669653[112] = 0.0;
   out_7423446859784669653[113] = 0.0;
   out_7423446859784669653[114] = 1.0;
   out_7423446859784669653[115] = 0.0;
   out_7423446859784669653[116] = 0.0;
   out_7423446859784669653[117] = 0.0;
   out_7423446859784669653[118] = 0.0;
   out_7423446859784669653[119] = 0.0;
   out_7423446859784669653[120] = 0.0;
   out_7423446859784669653[121] = 0.0;
   out_7423446859784669653[122] = 0.0;
   out_7423446859784669653[123] = 0.0;
   out_7423446859784669653[124] = 0.0;
   out_7423446859784669653[125] = 0.0;
   out_7423446859784669653[126] = 0.0;
   out_7423446859784669653[127] = 0.0;
   out_7423446859784669653[128] = 0.0;
   out_7423446859784669653[129] = 0.0;
   out_7423446859784669653[130] = 0.0;
   out_7423446859784669653[131] = 0.0;
   out_7423446859784669653[132] = 0.0;
   out_7423446859784669653[133] = 1.0;
   out_7423446859784669653[134] = 0.0;
   out_7423446859784669653[135] = 0.0;
   out_7423446859784669653[136] = 0.0;
   out_7423446859784669653[137] = 0.0;
   out_7423446859784669653[138] = 0.0;
   out_7423446859784669653[139] = 0.0;
   out_7423446859784669653[140] = 0.0;
   out_7423446859784669653[141] = 0.0;
   out_7423446859784669653[142] = 0.0;
   out_7423446859784669653[143] = 0.0;
   out_7423446859784669653[144] = 0.0;
   out_7423446859784669653[145] = 0.0;
   out_7423446859784669653[146] = 0.0;
   out_7423446859784669653[147] = 0.0;
   out_7423446859784669653[148] = 0.0;
   out_7423446859784669653[149] = 0.0;
   out_7423446859784669653[150] = 0.0;
   out_7423446859784669653[151] = 0.0;
   out_7423446859784669653[152] = 1.0;
   out_7423446859784669653[153] = 0.0;
   out_7423446859784669653[154] = 0.0;
   out_7423446859784669653[155] = 0.0;
   out_7423446859784669653[156] = 0.0;
   out_7423446859784669653[157] = 0.0;
   out_7423446859784669653[158] = 0.0;
   out_7423446859784669653[159] = 0.0;
   out_7423446859784669653[160] = 0.0;
   out_7423446859784669653[161] = 0.0;
   out_7423446859784669653[162] = 0.0;
   out_7423446859784669653[163] = 0.0;
   out_7423446859784669653[164] = 0.0;
   out_7423446859784669653[165] = 0.0;
   out_7423446859784669653[166] = 0.0;
   out_7423446859784669653[167] = 0.0;
   out_7423446859784669653[168] = 0.0;
   out_7423446859784669653[169] = 0.0;
   out_7423446859784669653[170] = 0.0;
   out_7423446859784669653[171] = 1.0;
   out_7423446859784669653[172] = 0.0;
   out_7423446859784669653[173] = 0.0;
   out_7423446859784669653[174] = 0.0;
   out_7423446859784669653[175] = 0.0;
   out_7423446859784669653[176] = 0.0;
   out_7423446859784669653[177] = 0.0;
   out_7423446859784669653[178] = 0.0;
   out_7423446859784669653[179] = 0.0;
   out_7423446859784669653[180] = 0.0;
   out_7423446859784669653[181] = 0.0;
   out_7423446859784669653[182] = 0.0;
   out_7423446859784669653[183] = 0.0;
   out_7423446859784669653[184] = 0.0;
   out_7423446859784669653[185] = 0.0;
   out_7423446859784669653[186] = 0.0;
   out_7423446859784669653[187] = 0.0;
   out_7423446859784669653[188] = 0.0;
   out_7423446859784669653[189] = 0.0;
   out_7423446859784669653[190] = 1.0;
   out_7423446859784669653[191] = 0.0;
   out_7423446859784669653[192] = 0.0;
   out_7423446859784669653[193] = 0.0;
   out_7423446859784669653[194] = 0.0;
   out_7423446859784669653[195] = 0.0;
   out_7423446859784669653[196] = 0.0;
   out_7423446859784669653[197] = 0.0;
   out_7423446859784669653[198] = 0.0;
   out_7423446859784669653[199] = 0.0;
   out_7423446859784669653[200] = 0.0;
   out_7423446859784669653[201] = 0.0;
   out_7423446859784669653[202] = 0.0;
   out_7423446859784669653[203] = 0.0;
   out_7423446859784669653[204] = 0.0;
   out_7423446859784669653[205] = 0.0;
   out_7423446859784669653[206] = 0.0;
   out_7423446859784669653[207] = 0.0;
   out_7423446859784669653[208] = 0.0;
   out_7423446859784669653[209] = 1.0;
   out_7423446859784669653[210] = 0.0;
   out_7423446859784669653[211] = 0.0;
   out_7423446859784669653[212] = 0.0;
   out_7423446859784669653[213] = 0.0;
   out_7423446859784669653[214] = 0.0;
   out_7423446859784669653[215] = 0.0;
   out_7423446859784669653[216] = 0.0;
   out_7423446859784669653[217] = 0.0;
   out_7423446859784669653[218] = 0.0;
   out_7423446859784669653[219] = 0.0;
   out_7423446859784669653[220] = 0.0;
   out_7423446859784669653[221] = 0.0;
   out_7423446859784669653[222] = 0.0;
   out_7423446859784669653[223] = 0.0;
   out_7423446859784669653[224] = 0.0;
   out_7423446859784669653[225] = 0.0;
   out_7423446859784669653[226] = 0.0;
   out_7423446859784669653[227] = 0.0;
   out_7423446859784669653[228] = 1.0;
   out_7423446859784669653[229] = 0.0;
   out_7423446859784669653[230] = 0.0;
   out_7423446859784669653[231] = 0.0;
   out_7423446859784669653[232] = 0.0;
   out_7423446859784669653[233] = 0.0;
   out_7423446859784669653[234] = 0.0;
   out_7423446859784669653[235] = 0.0;
   out_7423446859784669653[236] = 0.0;
   out_7423446859784669653[237] = 0.0;
   out_7423446859784669653[238] = 0.0;
   out_7423446859784669653[239] = 0.0;
   out_7423446859784669653[240] = 0.0;
   out_7423446859784669653[241] = 0.0;
   out_7423446859784669653[242] = 0.0;
   out_7423446859784669653[243] = 0.0;
   out_7423446859784669653[244] = 0.0;
   out_7423446859784669653[245] = 0.0;
   out_7423446859784669653[246] = 0.0;
   out_7423446859784669653[247] = 1.0;
   out_7423446859784669653[248] = 0.0;
   out_7423446859784669653[249] = 0.0;
   out_7423446859784669653[250] = 0.0;
   out_7423446859784669653[251] = 0.0;
   out_7423446859784669653[252] = 0.0;
   out_7423446859784669653[253] = 0.0;
   out_7423446859784669653[254] = 0.0;
   out_7423446859784669653[255] = 0.0;
   out_7423446859784669653[256] = 0.0;
   out_7423446859784669653[257] = 0.0;
   out_7423446859784669653[258] = 0.0;
   out_7423446859784669653[259] = 0.0;
   out_7423446859784669653[260] = 0.0;
   out_7423446859784669653[261] = 0.0;
   out_7423446859784669653[262] = 0.0;
   out_7423446859784669653[263] = 0.0;
   out_7423446859784669653[264] = 0.0;
   out_7423446859784669653[265] = 0.0;
   out_7423446859784669653[266] = 1.0;
   out_7423446859784669653[267] = 0.0;
   out_7423446859784669653[268] = 0.0;
   out_7423446859784669653[269] = 0.0;
   out_7423446859784669653[270] = 0.0;
   out_7423446859784669653[271] = 0.0;
   out_7423446859784669653[272] = 0.0;
   out_7423446859784669653[273] = 0.0;
   out_7423446859784669653[274] = 0.0;
   out_7423446859784669653[275] = 0.0;
   out_7423446859784669653[276] = 0.0;
   out_7423446859784669653[277] = 0.0;
   out_7423446859784669653[278] = 0.0;
   out_7423446859784669653[279] = 0.0;
   out_7423446859784669653[280] = 0.0;
   out_7423446859784669653[281] = 0.0;
   out_7423446859784669653[282] = 0.0;
   out_7423446859784669653[283] = 0.0;
   out_7423446859784669653[284] = 0.0;
   out_7423446859784669653[285] = 1.0;
   out_7423446859784669653[286] = 0.0;
   out_7423446859784669653[287] = 0.0;
   out_7423446859784669653[288] = 0.0;
   out_7423446859784669653[289] = 0.0;
   out_7423446859784669653[290] = 0.0;
   out_7423446859784669653[291] = 0.0;
   out_7423446859784669653[292] = 0.0;
   out_7423446859784669653[293] = 0.0;
   out_7423446859784669653[294] = 0.0;
   out_7423446859784669653[295] = 0.0;
   out_7423446859784669653[296] = 0.0;
   out_7423446859784669653[297] = 0.0;
   out_7423446859784669653[298] = 0.0;
   out_7423446859784669653[299] = 0.0;
   out_7423446859784669653[300] = 0.0;
   out_7423446859784669653[301] = 0.0;
   out_7423446859784669653[302] = 0.0;
   out_7423446859784669653[303] = 0.0;
   out_7423446859784669653[304] = 1.0;
   out_7423446859784669653[305] = 0.0;
   out_7423446859784669653[306] = 0.0;
   out_7423446859784669653[307] = 0.0;
   out_7423446859784669653[308] = 0.0;
   out_7423446859784669653[309] = 0.0;
   out_7423446859784669653[310] = 0.0;
   out_7423446859784669653[311] = 0.0;
   out_7423446859784669653[312] = 0.0;
   out_7423446859784669653[313] = 0.0;
   out_7423446859784669653[314] = 0.0;
   out_7423446859784669653[315] = 0.0;
   out_7423446859784669653[316] = 0.0;
   out_7423446859784669653[317] = 0.0;
   out_7423446859784669653[318] = 0.0;
   out_7423446859784669653[319] = 0.0;
   out_7423446859784669653[320] = 0.0;
   out_7423446859784669653[321] = 0.0;
   out_7423446859784669653[322] = 0.0;
   out_7423446859784669653[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5202695727626385220) {
   out_5202695727626385220[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5202695727626385220[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5202695727626385220[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5202695727626385220[3] = dt*state[12] + state[3];
   out_5202695727626385220[4] = dt*state[13] + state[4];
   out_5202695727626385220[5] = dt*state[14] + state[5];
   out_5202695727626385220[6] = state[6];
   out_5202695727626385220[7] = state[7];
   out_5202695727626385220[8] = state[8];
   out_5202695727626385220[9] = state[9];
   out_5202695727626385220[10] = state[10];
   out_5202695727626385220[11] = state[11];
   out_5202695727626385220[12] = state[12];
   out_5202695727626385220[13] = state[13];
   out_5202695727626385220[14] = state[14];
   out_5202695727626385220[15] = state[15];
   out_5202695727626385220[16] = state[16];
   out_5202695727626385220[17] = state[17];
}
void F_fun(double *state, double dt, double *out_5491057847988530682) {
   out_5491057847988530682[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5491057847988530682[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5491057847988530682[2] = 0;
   out_5491057847988530682[3] = 0;
   out_5491057847988530682[4] = 0;
   out_5491057847988530682[5] = 0;
   out_5491057847988530682[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5491057847988530682[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5491057847988530682[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5491057847988530682[9] = 0;
   out_5491057847988530682[10] = 0;
   out_5491057847988530682[11] = 0;
   out_5491057847988530682[12] = 0;
   out_5491057847988530682[13] = 0;
   out_5491057847988530682[14] = 0;
   out_5491057847988530682[15] = 0;
   out_5491057847988530682[16] = 0;
   out_5491057847988530682[17] = 0;
   out_5491057847988530682[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5491057847988530682[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5491057847988530682[20] = 0;
   out_5491057847988530682[21] = 0;
   out_5491057847988530682[22] = 0;
   out_5491057847988530682[23] = 0;
   out_5491057847988530682[24] = 0;
   out_5491057847988530682[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5491057847988530682[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5491057847988530682[27] = 0;
   out_5491057847988530682[28] = 0;
   out_5491057847988530682[29] = 0;
   out_5491057847988530682[30] = 0;
   out_5491057847988530682[31] = 0;
   out_5491057847988530682[32] = 0;
   out_5491057847988530682[33] = 0;
   out_5491057847988530682[34] = 0;
   out_5491057847988530682[35] = 0;
   out_5491057847988530682[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5491057847988530682[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5491057847988530682[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5491057847988530682[39] = 0;
   out_5491057847988530682[40] = 0;
   out_5491057847988530682[41] = 0;
   out_5491057847988530682[42] = 0;
   out_5491057847988530682[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5491057847988530682[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5491057847988530682[45] = 0;
   out_5491057847988530682[46] = 0;
   out_5491057847988530682[47] = 0;
   out_5491057847988530682[48] = 0;
   out_5491057847988530682[49] = 0;
   out_5491057847988530682[50] = 0;
   out_5491057847988530682[51] = 0;
   out_5491057847988530682[52] = 0;
   out_5491057847988530682[53] = 0;
   out_5491057847988530682[54] = 0;
   out_5491057847988530682[55] = 0;
   out_5491057847988530682[56] = 0;
   out_5491057847988530682[57] = 1;
   out_5491057847988530682[58] = 0;
   out_5491057847988530682[59] = 0;
   out_5491057847988530682[60] = 0;
   out_5491057847988530682[61] = 0;
   out_5491057847988530682[62] = 0;
   out_5491057847988530682[63] = 0;
   out_5491057847988530682[64] = 0;
   out_5491057847988530682[65] = 0;
   out_5491057847988530682[66] = dt;
   out_5491057847988530682[67] = 0;
   out_5491057847988530682[68] = 0;
   out_5491057847988530682[69] = 0;
   out_5491057847988530682[70] = 0;
   out_5491057847988530682[71] = 0;
   out_5491057847988530682[72] = 0;
   out_5491057847988530682[73] = 0;
   out_5491057847988530682[74] = 0;
   out_5491057847988530682[75] = 0;
   out_5491057847988530682[76] = 1;
   out_5491057847988530682[77] = 0;
   out_5491057847988530682[78] = 0;
   out_5491057847988530682[79] = 0;
   out_5491057847988530682[80] = 0;
   out_5491057847988530682[81] = 0;
   out_5491057847988530682[82] = 0;
   out_5491057847988530682[83] = 0;
   out_5491057847988530682[84] = 0;
   out_5491057847988530682[85] = dt;
   out_5491057847988530682[86] = 0;
   out_5491057847988530682[87] = 0;
   out_5491057847988530682[88] = 0;
   out_5491057847988530682[89] = 0;
   out_5491057847988530682[90] = 0;
   out_5491057847988530682[91] = 0;
   out_5491057847988530682[92] = 0;
   out_5491057847988530682[93] = 0;
   out_5491057847988530682[94] = 0;
   out_5491057847988530682[95] = 1;
   out_5491057847988530682[96] = 0;
   out_5491057847988530682[97] = 0;
   out_5491057847988530682[98] = 0;
   out_5491057847988530682[99] = 0;
   out_5491057847988530682[100] = 0;
   out_5491057847988530682[101] = 0;
   out_5491057847988530682[102] = 0;
   out_5491057847988530682[103] = 0;
   out_5491057847988530682[104] = dt;
   out_5491057847988530682[105] = 0;
   out_5491057847988530682[106] = 0;
   out_5491057847988530682[107] = 0;
   out_5491057847988530682[108] = 0;
   out_5491057847988530682[109] = 0;
   out_5491057847988530682[110] = 0;
   out_5491057847988530682[111] = 0;
   out_5491057847988530682[112] = 0;
   out_5491057847988530682[113] = 0;
   out_5491057847988530682[114] = 1;
   out_5491057847988530682[115] = 0;
   out_5491057847988530682[116] = 0;
   out_5491057847988530682[117] = 0;
   out_5491057847988530682[118] = 0;
   out_5491057847988530682[119] = 0;
   out_5491057847988530682[120] = 0;
   out_5491057847988530682[121] = 0;
   out_5491057847988530682[122] = 0;
   out_5491057847988530682[123] = 0;
   out_5491057847988530682[124] = 0;
   out_5491057847988530682[125] = 0;
   out_5491057847988530682[126] = 0;
   out_5491057847988530682[127] = 0;
   out_5491057847988530682[128] = 0;
   out_5491057847988530682[129] = 0;
   out_5491057847988530682[130] = 0;
   out_5491057847988530682[131] = 0;
   out_5491057847988530682[132] = 0;
   out_5491057847988530682[133] = 1;
   out_5491057847988530682[134] = 0;
   out_5491057847988530682[135] = 0;
   out_5491057847988530682[136] = 0;
   out_5491057847988530682[137] = 0;
   out_5491057847988530682[138] = 0;
   out_5491057847988530682[139] = 0;
   out_5491057847988530682[140] = 0;
   out_5491057847988530682[141] = 0;
   out_5491057847988530682[142] = 0;
   out_5491057847988530682[143] = 0;
   out_5491057847988530682[144] = 0;
   out_5491057847988530682[145] = 0;
   out_5491057847988530682[146] = 0;
   out_5491057847988530682[147] = 0;
   out_5491057847988530682[148] = 0;
   out_5491057847988530682[149] = 0;
   out_5491057847988530682[150] = 0;
   out_5491057847988530682[151] = 0;
   out_5491057847988530682[152] = 1;
   out_5491057847988530682[153] = 0;
   out_5491057847988530682[154] = 0;
   out_5491057847988530682[155] = 0;
   out_5491057847988530682[156] = 0;
   out_5491057847988530682[157] = 0;
   out_5491057847988530682[158] = 0;
   out_5491057847988530682[159] = 0;
   out_5491057847988530682[160] = 0;
   out_5491057847988530682[161] = 0;
   out_5491057847988530682[162] = 0;
   out_5491057847988530682[163] = 0;
   out_5491057847988530682[164] = 0;
   out_5491057847988530682[165] = 0;
   out_5491057847988530682[166] = 0;
   out_5491057847988530682[167] = 0;
   out_5491057847988530682[168] = 0;
   out_5491057847988530682[169] = 0;
   out_5491057847988530682[170] = 0;
   out_5491057847988530682[171] = 1;
   out_5491057847988530682[172] = 0;
   out_5491057847988530682[173] = 0;
   out_5491057847988530682[174] = 0;
   out_5491057847988530682[175] = 0;
   out_5491057847988530682[176] = 0;
   out_5491057847988530682[177] = 0;
   out_5491057847988530682[178] = 0;
   out_5491057847988530682[179] = 0;
   out_5491057847988530682[180] = 0;
   out_5491057847988530682[181] = 0;
   out_5491057847988530682[182] = 0;
   out_5491057847988530682[183] = 0;
   out_5491057847988530682[184] = 0;
   out_5491057847988530682[185] = 0;
   out_5491057847988530682[186] = 0;
   out_5491057847988530682[187] = 0;
   out_5491057847988530682[188] = 0;
   out_5491057847988530682[189] = 0;
   out_5491057847988530682[190] = 1;
   out_5491057847988530682[191] = 0;
   out_5491057847988530682[192] = 0;
   out_5491057847988530682[193] = 0;
   out_5491057847988530682[194] = 0;
   out_5491057847988530682[195] = 0;
   out_5491057847988530682[196] = 0;
   out_5491057847988530682[197] = 0;
   out_5491057847988530682[198] = 0;
   out_5491057847988530682[199] = 0;
   out_5491057847988530682[200] = 0;
   out_5491057847988530682[201] = 0;
   out_5491057847988530682[202] = 0;
   out_5491057847988530682[203] = 0;
   out_5491057847988530682[204] = 0;
   out_5491057847988530682[205] = 0;
   out_5491057847988530682[206] = 0;
   out_5491057847988530682[207] = 0;
   out_5491057847988530682[208] = 0;
   out_5491057847988530682[209] = 1;
   out_5491057847988530682[210] = 0;
   out_5491057847988530682[211] = 0;
   out_5491057847988530682[212] = 0;
   out_5491057847988530682[213] = 0;
   out_5491057847988530682[214] = 0;
   out_5491057847988530682[215] = 0;
   out_5491057847988530682[216] = 0;
   out_5491057847988530682[217] = 0;
   out_5491057847988530682[218] = 0;
   out_5491057847988530682[219] = 0;
   out_5491057847988530682[220] = 0;
   out_5491057847988530682[221] = 0;
   out_5491057847988530682[222] = 0;
   out_5491057847988530682[223] = 0;
   out_5491057847988530682[224] = 0;
   out_5491057847988530682[225] = 0;
   out_5491057847988530682[226] = 0;
   out_5491057847988530682[227] = 0;
   out_5491057847988530682[228] = 1;
   out_5491057847988530682[229] = 0;
   out_5491057847988530682[230] = 0;
   out_5491057847988530682[231] = 0;
   out_5491057847988530682[232] = 0;
   out_5491057847988530682[233] = 0;
   out_5491057847988530682[234] = 0;
   out_5491057847988530682[235] = 0;
   out_5491057847988530682[236] = 0;
   out_5491057847988530682[237] = 0;
   out_5491057847988530682[238] = 0;
   out_5491057847988530682[239] = 0;
   out_5491057847988530682[240] = 0;
   out_5491057847988530682[241] = 0;
   out_5491057847988530682[242] = 0;
   out_5491057847988530682[243] = 0;
   out_5491057847988530682[244] = 0;
   out_5491057847988530682[245] = 0;
   out_5491057847988530682[246] = 0;
   out_5491057847988530682[247] = 1;
   out_5491057847988530682[248] = 0;
   out_5491057847988530682[249] = 0;
   out_5491057847988530682[250] = 0;
   out_5491057847988530682[251] = 0;
   out_5491057847988530682[252] = 0;
   out_5491057847988530682[253] = 0;
   out_5491057847988530682[254] = 0;
   out_5491057847988530682[255] = 0;
   out_5491057847988530682[256] = 0;
   out_5491057847988530682[257] = 0;
   out_5491057847988530682[258] = 0;
   out_5491057847988530682[259] = 0;
   out_5491057847988530682[260] = 0;
   out_5491057847988530682[261] = 0;
   out_5491057847988530682[262] = 0;
   out_5491057847988530682[263] = 0;
   out_5491057847988530682[264] = 0;
   out_5491057847988530682[265] = 0;
   out_5491057847988530682[266] = 1;
   out_5491057847988530682[267] = 0;
   out_5491057847988530682[268] = 0;
   out_5491057847988530682[269] = 0;
   out_5491057847988530682[270] = 0;
   out_5491057847988530682[271] = 0;
   out_5491057847988530682[272] = 0;
   out_5491057847988530682[273] = 0;
   out_5491057847988530682[274] = 0;
   out_5491057847988530682[275] = 0;
   out_5491057847988530682[276] = 0;
   out_5491057847988530682[277] = 0;
   out_5491057847988530682[278] = 0;
   out_5491057847988530682[279] = 0;
   out_5491057847988530682[280] = 0;
   out_5491057847988530682[281] = 0;
   out_5491057847988530682[282] = 0;
   out_5491057847988530682[283] = 0;
   out_5491057847988530682[284] = 0;
   out_5491057847988530682[285] = 1;
   out_5491057847988530682[286] = 0;
   out_5491057847988530682[287] = 0;
   out_5491057847988530682[288] = 0;
   out_5491057847988530682[289] = 0;
   out_5491057847988530682[290] = 0;
   out_5491057847988530682[291] = 0;
   out_5491057847988530682[292] = 0;
   out_5491057847988530682[293] = 0;
   out_5491057847988530682[294] = 0;
   out_5491057847988530682[295] = 0;
   out_5491057847988530682[296] = 0;
   out_5491057847988530682[297] = 0;
   out_5491057847988530682[298] = 0;
   out_5491057847988530682[299] = 0;
   out_5491057847988530682[300] = 0;
   out_5491057847988530682[301] = 0;
   out_5491057847988530682[302] = 0;
   out_5491057847988530682[303] = 0;
   out_5491057847988530682[304] = 1;
   out_5491057847988530682[305] = 0;
   out_5491057847988530682[306] = 0;
   out_5491057847988530682[307] = 0;
   out_5491057847988530682[308] = 0;
   out_5491057847988530682[309] = 0;
   out_5491057847988530682[310] = 0;
   out_5491057847988530682[311] = 0;
   out_5491057847988530682[312] = 0;
   out_5491057847988530682[313] = 0;
   out_5491057847988530682[314] = 0;
   out_5491057847988530682[315] = 0;
   out_5491057847988530682[316] = 0;
   out_5491057847988530682[317] = 0;
   out_5491057847988530682[318] = 0;
   out_5491057847988530682[319] = 0;
   out_5491057847988530682[320] = 0;
   out_5491057847988530682[321] = 0;
   out_5491057847988530682[322] = 0;
   out_5491057847988530682[323] = 1;
}
void h_4(double *state, double *unused, double *out_6452546827916926575) {
   out_6452546827916926575[0] = state[6] + state[9];
   out_6452546827916926575[1] = state[7] + state[10];
   out_6452546827916926575[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2270928474745588918) {
   out_2270928474745588918[0] = 0;
   out_2270928474745588918[1] = 0;
   out_2270928474745588918[2] = 0;
   out_2270928474745588918[3] = 0;
   out_2270928474745588918[4] = 0;
   out_2270928474745588918[5] = 0;
   out_2270928474745588918[6] = 1;
   out_2270928474745588918[7] = 0;
   out_2270928474745588918[8] = 0;
   out_2270928474745588918[9] = 1;
   out_2270928474745588918[10] = 0;
   out_2270928474745588918[11] = 0;
   out_2270928474745588918[12] = 0;
   out_2270928474745588918[13] = 0;
   out_2270928474745588918[14] = 0;
   out_2270928474745588918[15] = 0;
   out_2270928474745588918[16] = 0;
   out_2270928474745588918[17] = 0;
   out_2270928474745588918[18] = 0;
   out_2270928474745588918[19] = 0;
   out_2270928474745588918[20] = 0;
   out_2270928474745588918[21] = 0;
   out_2270928474745588918[22] = 0;
   out_2270928474745588918[23] = 0;
   out_2270928474745588918[24] = 0;
   out_2270928474745588918[25] = 1;
   out_2270928474745588918[26] = 0;
   out_2270928474745588918[27] = 0;
   out_2270928474745588918[28] = 1;
   out_2270928474745588918[29] = 0;
   out_2270928474745588918[30] = 0;
   out_2270928474745588918[31] = 0;
   out_2270928474745588918[32] = 0;
   out_2270928474745588918[33] = 0;
   out_2270928474745588918[34] = 0;
   out_2270928474745588918[35] = 0;
   out_2270928474745588918[36] = 0;
   out_2270928474745588918[37] = 0;
   out_2270928474745588918[38] = 0;
   out_2270928474745588918[39] = 0;
   out_2270928474745588918[40] = 0;
   out_2270928474745588918[41] = 0;
   out_2270928474745588918[42] = 0;
   out_2270928474745588918[43] = 0;
   out_2270928474745588918[44] = 1;
   out_2270928474745588918[45] = 0;
   out_2270928474745588918[46] = 0;
   out_2270928474745588918[47] = 1;
   out_2270928474745588918[48] = 0;
   out_2270928474745588918[49] = 0;
   out_2270928474745588918[50] = 0;
   out_2270928474745588918[51] = 0;
   out_2270928474745588918[52] = 0;
   out_2270928474745588918[53] = 0;
}
void h_10(double *state, double *unused, double *out_7382153153143119100) {
   out_7382153153143119100[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7382153153143119100[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7382153153143119100[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8704391623487425268) {
   out_8704391623487425268[0] = 0;
   out_8704391623487425268[1] = 9.8100000000000005*cos(state[1]);
   out_8704391623487425268[2] = 0;
   out_8704391623487425268[3] = 0;
   out_8704391623487425268[4] = -state[8];
   out_8704391623487425268[5] = state[7];
   out_8704391623487425268[6] = 0;
   out_8704391623487425268[7] = state[5];
   out_8704391623487425268[8] = -state[4];
   out_8704391623487425268[9] = 0;
   out_8704391623487425268[10] = 0;
   out_8704391623487425268[11] = 0;
   out_8704391623487425268[12] = 1;
   out_8704391623487425268[13] = 0;
   out_8704391623487425268[14] = 0;
   out_8704391623487425268[15] = 1;
   out_8704391623487425268[16] = 0;
   out_8704391623487425268[17] = 0;
   out_8704391623487425268[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8704391623487425268[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8704391623487425268[20] = 0;
   out_8704391623487425268[21] = state[8];
   out_8704391623487425268[22] = 0;
   out_8704391623487425268[23] = -state[6];
   out_8704391623487425268[24] = -state[5];
   out_8704391623487425268[25] = 0;
   out_8704391623487425268[26] = state[3];
   out_8704391623487425268[27] = 0;
   out_8704391623487425268[28] = 0;
   out_8704391623487425268[29] = 0;
   out_8704391623487425268[30] = 0;
   out_8704391623487425268[31] = 1;
   out_8704391623487425268[32] = 0;
   out_8704391623487425268[33] = 0;
   out_8704391623487425268[34] = 1;
   out_8704391623487425268[35] = 0;
   out_8704391623487425268[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8704391623487425268[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8704391623487425268[38] = 0;
   out_8704391623487425268[39] = -state[7];
   out_8704391623487425268[40] = state[6];
   out_8704391623487425268[41] = 0;
   out_8704391623487425268[42] = state[4];
   out_8704391623487425268[43] = -state[3];
   out_8704391623487425268[44] = 0;
   out_8704391623487425268[45] = 0;
   out_8704391623487425268[46] = 0;
   out_8704391623487425268[47] = 0;
   out_8704391623487425268[48] = 0;
   out_8704391623487425268[49] = 0;
   out_8704391623487425268[50] = 1;
   out_8704391623487425268[51] = 0;
   out_8704391623487425268[52] = 0;
   out_8704391623487425268[53] = 1;
}
void h_13(double *state, double *unused, double *out_6604156015444971944) {
   out_6604156015444971944[0] = state[3];
   out_6604156015444971944[1] = state[4];
   out_6604156015444971944[2] = state[5];
}
void H_13(double *state, double *unused, double *out_941345350586743883) {
   out_941345350586743883[0] = 0;
   out_941345350586743883[1] = 0;
   out_941345350586743883[2] = 0;
   out_941345350586743883[3] = 1;
   out_941345350586743883[4] = 0;
   out_941345350586743883[5] = 0;
   out_941345350586743883[6] = 0;
   out_941345350586743883[7] = 0;
   out_941345350586743883[8] = 0;
   out_941345350586743883[9] = 0;
   out_941345350586743883[10] = 0;
   out_941345350586743883[11] = 0;
   out_941345350586743883[12] = 0;
   out_941345350586743883[13] = 0;
   out_941345350586743883[14] = 0;
   out_941345350586743883[15] = 0;
   out_941345350586743883[16] = 0;
   out_941345350586743883[17] = 0;
   out_941345350586743883[18] = 0;
   out_941345350586743883[19] = 0;
   out_941345350586743883[20] = 0;
   out_941345350586743883[21] = 0;
   out_941345350586743883[22] = 1;
   out_941345350586743883[23] = 0;
   out_941345350586743883[24] = 0;
   out_941345350586743883[25] = 0;
   out_941345350586743883[26] = 0;
   out_941345350586743883[27] = 0;
   out_941345350586743883[28] = 0;
   out_941345350586743883[29] = 0;
   out_941345350586743883[30] = 0;
   out_941345350586743883[31] = 0;
   out_941345350586743883[32] = 0;
   out_941345350586743883[33] = 0;
   out_941345350586743883[34] = 0;
   out_941345350586743883[35] = 0;
   out_941345350586743883[36] = 0;
   out_941345350586743883[37] = 0;
   out_941345350586743883[38] = 0;
   out_941345350586743883[39] = 0;
   out_941345350586743883[40] = 0;
   out_941345350586743883[41] = 1;
   out_941345350586743883[42] = 0;
   out_941345350586743883[43] = 0;
   out_941345350586743883[44] = 0;
   out_941345350586743883[45] = 0;
   out_941345350586743883[46] = 0;
   out_941345350586743883[47] = 0;
   out_941345350586743883[48] = 0;
   out_941345350586743883[49] = 0;
   out_941345350586743883[50] = 0;
   out_941345350586743883[51] = 0;
   out_941345350586743883[52] = 0;
   out_941345350586743883[53] = 0;
}
void h_14(double *state, double *unused, double *out_3089011552067498211) {
   out_3089011552067498211[0] = state[6];
   out_3089011552067498211[1] = state[7];
   out_3089011552067498211[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2706045001390472517) {
   out_2706045001390472517[0] = 0;
   out_2706045001390472517[1] = 0;
   out_2706045001390472517[2] = 0;
   out_2706045001390472517[3] = 0;
   out_2706045001390472517[4] = 0;
   out_2706045001390472517[5] = 0;
   out_2706045001390472517[6] = 1;
   out_2706045001390472517[7] = 0;
   out_2706045001390472517[8] = 0;
   out_2706045001390472517[9] = 0;
   out_2706045001390472517[10] = 0;
   out_2706045001390472517[11] = 0;
   out_2706045001390472517[12] = 0;
   out_2706045001390472517[13] = 0;
   out_2706045001390472517[14] = 0;
   out_2706045001390472517[15] = 0;
   out_2706045001390472517[16] = 0;
   out_2706045001390472517[17] = 0;
   out_2706045001390472517[18] = 0;
   out_2706045001390472517[19] = 0;
   out_2706045001390472517[20] = 0;
   out_2706045001390472517[21] = 0;
   out_2706045001390472517[22] = 0;
   out_2706045001390472517[23] = 0;
   out_2706045001390472517[24] = 0;
   out_2706045001390472517[25] = 1;
   out_2706045001390472517[26] = 0;
   out_2706045001390472517[27] = 0;
   out_2706045001390472517[28] = 0;
   out_2706045001390472517[29] = 0;
   out_2706045001390472517[30] = 0;
   out_2706045001390472517[31] = 0;
   out_2706045001390472517[32] = 0;
   out_2706045001390472517[33] = 0;
   out_2706045001390472517[34] = 0;
   out_2706045001390472517[35] = 0;
   out_2706045001390472517[36] = 0;
   out_2706045001390472517[37] = 0;
   out_2706045001390472517[38] = 0;
   out_2706045001390472517[39] = 0;
   out_2706045001390472517[40] = 0;
   out_2706045001390472517[41] = 0;
   out_2706045001390472517[42] = 0;
   out_2706045001390472517[43] = 0;
   out_2706045001390472517[44] = 1;
   out_2706045001390472517[45] = 0;
   out_2706045001390472517[46] = 0;
   out_2706045001390472517[47] = 0;
   out_2706045001390472517[48] = 0;
   out_2706045001390472517[49] = 0;
   out_2706045001390472517[50] = 0;
   out_2706045001390472517[51] = 0;
   out_2706045001390472517[52] = 0;
   out_2706045001390472517[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_1596408993574171648) {
  err_fun(nom_x, delta_x, out_1596408993574171648);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4927362141599677432) {
  inv_err_fun(nom_x, true_x, out_4927362141599677432);
}
void pose_H_mod_fun(double *state, double *out_7423446859784669653) {
  H_mod_fun(state, out_7423446859784669653);
}
void pose_f_fun(double *state, double dt, double *out_5202695727626385220) {
  f_fun(state,  dt, out_5202695727626385220);
}
void pose_F_fun(double *state, double dt, double *out_5491057847988530682) {
  F_fun(state,  dt, out_5491057847988530682);
}
void pose_h_4(double *state, double *unused, double *out_6452546827916926575) {
  h_4(state, unused, out_6452546827916926575);
}
void pose_H_4(double *state, double *unused, double *out_2270928474745588918) {
  H_4(state, unused, out_2270928474745588918);
}
void pose_h_10(double *state, double *unused, double *out_7382153153143119100) {
  h_10(state, unused, out_7382153153143119100);
}
void pose_H_10(double *state, double *unused, double *out_8704391623487425268) {
  H_10(state, unused, out_8704391623487425268);
}
void pose_h_13(double *state, double *unused, double *out_6604156015444971944) {
  h_13(state, unused, out_6604156015444971944);
}
void pose_H_13(double *state, double *unused, double *out_941345350586743883) {
  H_13(state, unused, out_941345350586743883);
}
void pose_h_14(double *state, double *unused, double *out_3089011552067498211) {
  h_14(state, unused, out_3089011552067498211);
}
void pose_H_14(double *state, double *unused, double *out_2706045001390472517) {
  H_14(state, unused, out_2706045001390472517);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
