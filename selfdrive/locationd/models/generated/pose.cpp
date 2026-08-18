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
void err_fun(double *nom_x, double *delta_x, double *out_7195396181340429812) {
   out_7195396181340429812[0] = delta_x[0] + nom_x[0];
   out_7195396181340429812[1] = delta_x[1] + nom_x[1];
   out_7195396181340429812[2] = delta_x[2] + nom_x[2];
   out_7195396181340429812[3] = delta_x[3] + nom_x[3];
   out_7195396181340429812[4] = delta_x[4] + nom_x[4];
   out_7195396181340429812[5] = delta_x[5] + nom_x[5];
   out_7195396181340429812[6] = delta_x[6] + nom_x[6];
   out_7195396181340429812[7] = delta_x[7] + nom_x[7];
   out_7195396181340429812[8] = delta_x[8] + nom_x[8];
   out_7195396181340429812[9] = delta_x[9] + nom_x[9];
   out_7195396181340429812[10] = delta_x[10] + nom_x[10];
   out_7195396181340429812[11] = delta_x[11] + nom_x[11];
   out_7195396181340429812[12] = delta_x[12] + nom_x[12];
   out_7195396181340429812[13] = delta_x[13] + nom_x[13];
   out_7195396181340429812[14] = delta_x[14] + nom_x[14];
   out_7195396181340429812[15] = delta_x[15] + nom_x[15];
   out_7195396181340429812[16] = delta_x[16] + nom_x[16];
   out_7195396181340429812[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6214721926630491138) {
   out_6214721926630491138[0] = -nom_x[0] + true_x[0];
   out_6214721926630491138[1] = -nom_x[1] + true_x[1];
   out_6214721926630491138[2] = -nom_x[2] + true_x[2];
   out_6214721926630491138[3] = -nom_x[3] + true_x[3];
   out_6214721926630491138[4] = -nom_x[4] + true_x[4];
   out_6214721926630491138[5] = -nom_x[5] + true_x[5];
   out_6214721926630491138[6] = -nom_x[6] + true_x[6];
   out_6214721926630491138[7] = -nom_x[7] + true_x[7];
   out_6214721926630491138[8] = -nom_x[8] + true_x[8];
   out_6214721926630491138[9] = -nom_x[9] + true_x[9];
   out_6214721926630491138[10] = -nom_x[10] + true_x[10];
   out_6214721926630491138[11] = -nom_x[11] + true_x[11];
   out_6214721926630491138[12] = -nom_x[12] + true_x[12];
   out_6214721926630491138[13] = -nom_x[13] + true_x[13];
   out_6214721926630491138[14] = -nom_x[14] + true_x[14];
   out_6214721926630491138[15] = -nom_x[15] + true_x[15];
   out_6214721926630491138[16] = -nom_x[16] + true_x[16];
   out_6214721926630491138[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8844892526159179063) {
   out_8844892526159179063[0] = 1.0;
   out_8844892526159179063[1] = 0.0;
   out_8844892526159179063[2] = 0.0;
   out_8844892526159179063[3] = 0.0;
   out_8844892526159179063[4] = 0.0;
   out_8844892526159179063[5] = 0.0;
   out_8844892526159179063[6] = 0.0;
   out_8844892526159179063[7] = 0.0;
   out_8844892526159179063[8] = 0.0;
   out_8844892526159179063[9] = 0.0;
   out_8844892526159179063[10] = 0.0;
   out_8844892526159179063[11] = 0.0;
   out_8844892526159179063[12] = 0.0;
   out_8844892526159179063[13] = 0.0;
   out_8844892526159179063[14] = 0.0;
   out_8844892526159179063[15] = 0.0;
   out_8844892526159179063[16] = 0.0;
   out_8844892526159179063[17] = 0.0;
   out_8844892526159179063[18] = 0.0;
   out_8844892526159179063[19] = 1.0;
   out_8844892526159179063[20] = 0.0;
   out_8844892526159179063[21] = 0.0;
   out_8844892526159179063[22] = 0.0;
   out_8844892526159179063[23] = 0.0;
   out_8844892526159179063[24] = 0.0;
   out_8844892526159179063[25] = 0.0;
   out_8844892526159179063[26] = 0.0;
   out_8844892526159179063[27] = 0.0;
   out_8844892526159179063[28] = 0.0;
   out_8844892526159179063[29] = 0.0;
   out_8844892526159179063[30] = 0.0;
   out_8844892526159179063[31] = 0.0;
   out_8844892526159179063[32] = 0.0;
   out_8844892526159179063[33] = 0.0;
   out_8844892526159179063[34] = 0.0;
   out_8844892526159179063[35] = 0.0;
   out_8844892526159179063[36] = 0.0;
   out_8844892526159179063[37] = 0.0;
   out_8844892526159179063[38] = 1.0;
   out_8844892526159179063[39] = 0.0;
   out_8844892526159179063[40] = 0.0;
   out_8844892526159179063[41] = 0.0;
   out_8844892526159179063[42] = 0.0;
   out_8844892526159179063[43] = 0.0;
   out_8844892526159179063[44] = 0.0;
   out_8844892526159179063[45] = 0.0;
   out_8844892526159179063[46] = 0.0;
   out_8844892526159179063[47] = 0.0;
   out_8844892526159179063[48] = 0.0;
   out_8844892526159179063[49] = 0.0;
   out_8844892526159179063[50] = 0.0;
   out_8844892526159179063[51] = 0.0;
   out_8844892526159179063[52] = 0.0;
   out_8844892526159179063[53] = 0.0;
   out_8844892526159179063[54] = 0.0;
   out_8844892526159179063[55] = 0.0;
   out_8844892526159179063[56] = 0.0;
   out_8844892526159179063[57] = 1.0;
   out_8844892526159179063[58] = 0.0;
   out_8844892526159179063[59] = 0.0;
   out_8844892526159179063[60] = 0.0;
   out_8844892526159179063[61] = 0.0;
   out_8844892526159179063[62] = 0.0;
   out_8844892526159179063[63] = 0.0;
   out_8844892526159179063[64] = 0.0;
   out_8844892526159179063[65] = 0.0;
   out_8844892526159179063[66] = 0.0;
   out_8844892526159179063[67] = 0.0;
   out_8844892526159179063[68] = 0.0;
   out_8844892526159179063[69] = 0.0;
   out_8844892526159179063[70] = 0.0;
   out_8844892526159179063[71] = 0.0;
   out_8844892526159179063[72] = 0.0;
   out_8844892526159179063[73] = 0.0;
   out_8844892526159179063[74] = 0.0;
   out_8844892526159179063[75] = 0.0;
   out_8844892526159179063[76] = 1.0;
   out_8844892526159179063[77] = 0.0;
   out_8844892526159179063[78] = 0.0;
   out_8844892526159179063[79] = 0.0;
   out_8844892526159179063[80] = 0.0;
   out_8844892526159179063[81] = 0.0;
   out_8844892526159179063[82] = 0.0;
   out_8844892526159179063[83] = 0.0;
   out_8844892526159179063[84] = 0.0;
   out_8844892526159179063[85] = 0.0;
   out_8844892526159179063[86] = 0.0;
   out_8844892526159179063[87] = 0.0;
   out_8844892526159179063[88] = 0.0;
   out_8844892526159179063[89] = 0.0;
   out_8844892526159179063[90] = 0.0;
   out_8844892526159179063[91] = 0.0;
   out_8844892526159179063[92] = 0.0;
   out_8844892526159179063[93] = 0.0;
   out_8844892526159179063[94] = 0.0;
   out_8844892526159179063[95] = 1.0;
   out_8844892526159179063[96] = 0.0;
   out_8844892526159179063[97] = 0.0;
   out_8844892526159179063[98] = 0.0;
   out_8844892526159179063[99] = 0.0;
   out_8844892526159179063[100] = 0.0;
   out_8844892526159179063[101] = 0.0;
   out_8844892526159179063[102] = 0.0;
   out_8844892526159179063[103] = 0.0;
   out_8844892526159179063[104] = 0.0;
   out_8844892526159179063[105] = 0.0;
   out_8844892526159179063[106] = 0.0;
   out_8844892526159179063[107] = 0.0;
   out_8844892526159179063[108] = 0.0;
   out_8844892526159179063[109] = 0.0;
   out_8844892526159179063[110] = 0.0;
   out_8844892526159179063[111] = 0.0;
   out_8844892526159179063[112] = 0.0;
   out_8844892526159179063[113] = 0.0;
   out_8844892526159179063[114] = 1.0;
   out_8844892526159179063[115] = 0.0;
   out_8844892526159179063[116] = 0.0;
   out_8844892526159179063[117] = 0.0;
   out_8844892526159179063[118] = 0.0;
   out_8844892526159179063[119] = 0.0;
   out_8844892526159179063[120] = 0.0;
   out_8844892526159179063[121] = 0.0;
   out_8844892526159179063[122] = 0.0;
   out_8844892526159179063[123] = 0.0;
   out_8844892526159179063[124] = 0.0;
   out_8844892526159179063[125] = 0.0;
   out_8844892526159179063[126] = 0.0;
   out_8844892526159179063[127] = 0.0;
   out_8844892526159179063[128] = 0.0;
   out_8844892526159179063[129] = 0.0;
   out_8844892526159179063[130] = 0.0;
   out_8844892526159179063[131] = 0.0;
   out_8844892526159179063[132] = 0.0;
   out_8844892526159179063[133] = 1.0;
   out_8844892526159179063[134] = 0.0;
   out_8844892526159179063[135] = 0.0;
   out_8844892526159179063[136] = 0.0;
   out_8844892526159179063[137] = 0.0;
   out_8844892526159179063[138] = 0.0;
   out_8844892526159179063[139] = 0.0;
   out_8844892526159179063[140] = 0.0;
   out_8844892526159179063[141] = 0.0;
   out_8844892526159179063[142] = 0.0;
   out_8844892526159179063[143] = 0.0;
   out_8844892526159179063[144] = 0.0;
   out_8844892526159179063[145] = 0.0;
   out_8844892526159179063[146] = 0.0;
   out_8844892526159179063[147] = 0.0;
   out_8844892526159179063[148] = 0.0;
   out_8844892526159179063[149] = 0.0;
   out_8844892526159179063[150] = 0.0;
   out_8844892526159179063[151] = 0.0;
   out_8844892526159179063[152] = 1.0;
   out_8844892526159179063[153] = 0.0;
   out_8844892526159179063[154] = 0.0;
   out_8844892526159179063[155] = 0.0;
   out_8844892526159179063[156] = 0.0;
   out_8844892526159179063[157] = 0.0;
   out_8844892526159179063[158] = 0.0;
   out_8844892526159179063[159] = 0.0;
   out_8844892526159179063[160] = 0.0;
   out_8844892526159179063[161] = 0.0;
   out_8844892526159179063[162] = 0.0;
   out_8844892526159179063[163] = 0.0;
   out_8844892526159179063[164] = 0.0;
   out_8844892526159179063[165] = 0.0;
   out_8844892526159179063[166] = 0.0;
   out_8844892526159179063[167] = 0.0;
   out_8844892526159179063[168] = 0.0;
   out_8844892526159179063[169] = 0.0;
   out_8844892526159179063[170] = 0.0;
   out_8844892526159179063[171] = 1.0;
   out_8844892526159179063[172] = 0.0;
   out_8844892526159179063[173] = 0.0;
   out_8844892526159179063[174] = 0.0;
   out_8844892526159179063[175] = 0.0;
   out_8844892526159179063[176] = 0.0;
   out_8844892526159179063[177] = 0.0;
   out_8844892526159179063[178] = 0.0;
   out_8844892526159179063[179] = 0.0;
   out_8844892526159179063[180] = 0.0;
   out_8844892526159179063[181] = 0.0;
   out_8844892526159179063[182] = 0.0;
   out_8844892526159179063[183] = 0.0;
   out_8844892526159179063[184] = 0.0;
   out_8844892526159179063[185] = 0.0;
   out_8844892526159179063[186] = 0.0;
   out_8844892526159179063[187] = 0.0;
   out_8844892526159179063[188] = 0.0;
   out_8844892526159179063[189] = 0.0;
   out_8844892526159179063[190] = 1.0;
   out_8844892526159179063[191] = 0.0;
   out_8844892526159179063[192] = 0.0;
   out_8844892526159179063[193] = 0.0;
   out_8844892526159179063[194] = 0.0;
   out_8844892526159179063[195] = 0.0;
   out_8844892526159179063[196] = 0.0;
   out_8844892526159179063[197] = 0.0;
   out_8844892526159179063[198] = 0.0;
   out_8844892526159179063[199] = 0.0;
   out_8844892526159179063[200] = 0.0;
   out_8844892526159179063[201] = 0.0;
   out_8844892526159179063[202] = 0.0;
   out_8844892526159179063[203] = 0.0;
   out_8844892526159179063[204] = 0.0;
   out_8844892526159179063[205] = 0.0;
   out_8844892526159179063[206] = 0.0;
   out_8844892526159179063[207] = 0.0;
   out_8844892526159179063[208] = 0.0;
   out_8844892526159179063[209] = 1.0;
   out_8844892526159179063[210] = 0.0;
   out_8844892526159179063[211] = 0.0;
   out_8844892526159179063[212] = 0.0;
   out_8844892526159179063[213] = 0.0;
   out_8844892526159179063[214] = 0.0;
   out_8844892526159179063[215] = 0.0;
   out_8844892526159179063[216] = 0.0;
   out_8844892526159179063[217] = 0.0;
   out_8844892526159179063[218] = 0.0;
   out_8844892526159179063[219] = 0.0;
   out_8844892526159179063[220] = 0.0;
   out_8844892526159179063[221] = 0.0;
   out_8844892526159179063[222] = 0.0;
   out_8844892526159179063[223] = 0.0;
   out_8844892526159179063[224] = 0.0;
   out_8844892526159179063[225] = 0.0;
   out_8844892526159179063[226] = 0.0;
   out_8844892526159179063[227] = 0.0;
   out_8844892526159179063[228] = 1.0;
   out_8844892526159179063[229] = 0.0;
   out_8844892526159179063[230] = 0.0;
   out_8844892526159179063[231] = 0.0;
   out_8844892526159179063[232] = 0.0;
   out_8844892526159179063[233] = 0.0;
   out_8844892526159179063[234] = 0.0;
   out_8844892526159179063[235] = 0.0;
   out_8844892526159179063[236] = 0.0;
   out_8844892526159179063[237] = 0.0;
   out_8844892526159179063[238] = 0.0;
   out_8844892526159179063[239] = 0.0;
   out_8844892526159179063[240] = 0.0;
   out_8844892526159179063[241] = 0.0;
   out_8844892526159179063[242] = 0.0;
   out_8844892526159179063[243] = 0.0;
   out_8844892526159179063[244] = 0.0;
   out_8844892526159179063[245] = 0.0;
   out_8844892526159179063[246] = 0.0;
   out_8844892526159179063[247] = 1.0;
   out_8844892526159179063[248] = 0.0;
   out_8844892526159179063[249] = 0.0;
   out_8844892526159179063[250] = 0.0;
   out_8844892526159179063[251] = 0.0;
   out_8844892526159179063[252] = 0.0;
   out_8844892526159179063[253] = 0.0;
   out_8844892526159179063[254] = 0.0;
   out_8844892526159179063[255] = 0.0;
   out_8844892526159179063[256] = 0.0;
   out_8844892526159179063[257] = 0.0;
   out_8844892526159179063[258] = 0.0;
   out_8844892526159179063[259] = 0.0;
   out_8844892526159179063[260] = 0.0;
   out_8844892526159179063[261] = 0.0;
   out_8844892526159179063[262] = 0.0;
   out_8844892526159179063[263] = 0.0;
   out_8844892526159179063[264] = 0.0;
   out_8844892526159179063[265] = 0.0;
   out_8844892526159179063[266] = 1.0;
   out_8844892526159179063[267] = 0.0;
   out_8844892526159179063[268] = 0.0;
   out_8844892526159179063[269] = 0.0;
   out_8844892526159179063[270] = 0.0;
   out_8844892526159179063[271] = 0.0;
   out_8844892526159179063[272] = 0.0;
   out_8844892526159179063[273] = 0.0;
   out_8844892526159179063[274] = 0.0;
   out_8844892526159179063[275] = 0.0;
   out_8844892526159179063[276] = 0.0;
   out_8844892526159179063[277] = 0.0;
   out_8844892526159179063[278] = 0.0;
   out_8844892526159179063[279] = 0.0;
   out_8844892526159179063[280] = 0.0;
   out_8844892526159179063[281] = 0.0;
   out_8844892526159179063[282] = 0.0;
   out_8844892526159179063[283] = 0.0;
   out_8844892526159179063[284] = 0.0;
   out_8844892526159179063[285] = 1.0;
   out_8844892526159179063[286] = 0.0;
   out_8844892526159179063[287] = 0.0;
   out_8844892526159179063[288] = 0.0;
   out_8844892526159179063[289] = 0.0;
   out_8844892526159179063[290] = 0.0;
   out_8844892526159179063[291] = 0.0;
   out_8844892526159179063[292] = 0.0;
   out_8844892526159179063[293] = 0.0;
   out_8844892526159179063[294] = 0.0;
   out_8844892526159179063[295] = 0.0;
   out_8844892526159179063[296] = 0.0;
   out_8844892526159179063[297] = 0.0;
   out_8844892526159179063[298] = 0.0;
   out_8844892526159179063[299] = 0.0;
   out_8844892526159179063[300] = 0.0;
   out_8844892526159179063[301] = 0.0;
   out_8844892526159179063[302] = 0.0;
   out_8844892526159179063[303] = 0.0;
   out_8844892526159179063[304] = 1.0;
   out_8844892526159179063[305] = 0.0;
   out_8844892526159179063[306] = 0.0;
   out_8844892526159179063[307] = 0.0;
   out_8844892526159179063[308] = 0.0;
   out_8844892526159179063[309] = 0.0;
   out_8844892526159179063[310] = 0.0;
   out_8844892526159179063[311] = 0.0;
   out_8844892526159179063[312] = 0.0;
   out_8844892526159179063[313] = 0.0;
   out_8844892526159179063[314] = 0.0;
   out_8844892526159179063[315] = 0.0;
   out_8844892526159179063[316] = 0.0;
   out_8844892526159179063[317] = 0.0;
   out_8844892526159179063[318] = 0.0;
   out_8844892526159179063[319] = 0.0;
   out_8844892526159179063[320] = 0.0;
   out_8844892526159179063[321] = 0.0;
   out_8844892526159179063[322] = 0.0;
   out_8844892526159179063[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8367001063557654494) {
   out_8367001063557654494[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8367001063557654494[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8367001063557654494[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8367001063557654494[3] = dt*state[12] + state[3];
   out_8367001063557654494[4] = dt*state[13] + state[4];
   out_8367001063557654494[5] = dt*state[14] + state[5];
   out_8367001063557654494[6] = state[6];
   out_8367001063557654494[7] = state[7];
   out_8367001063557654494[8] = state[8];
   out_8367001063557654494[9] = state[9];
   out_8367001063557654494[10] = state[10];
   out_8367001063557654494[11] = state[11];
   out_8367001063557654494[12] = state[12];
   out_8367001063557654494[13] = state[13];
   out_8367001063557654494[14] = state[14];
   out_8367001063557654494[15] = state[15];
   out_8367001063557654494[16] = state[16];
   out_8367001063557654494[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2658931498525825673) {
   out_2658931498525825673[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2658931498525825673[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2658931498525825673[2] = 0;
   out_2658931498525825673[3] = 0;
   out_2658931498525825673[4] = 0;
   out_2658931498525825673[5] = 0;
   out_2658931498525825673[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2658931498525825673[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2658931498525825673[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2658931498525825673[9] = 0;
   out_2658931498525825673[10] = 0;
   out_2658931498525825673[11] = 0;
   out_2658931498525825673[12] = 0;
   out_2658931498525825673[13] = 0;
   out_2658931498525825673[14] = 0;
   out_2658931498525825673[15] = 0;
   out_2658931498525825673[16] = 0;
   out_2658931498525825673[17] = 0;
   out_2658931498525825673[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2658931498525825673[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2658931498525825673[20] = 0;
   out_2658931498525825673[21] = 0;
   out_2658931498525825673[22] = 0;
   out_2658931498525825673[23] = 0;
   out_2658931498525825673[24] = 0;
   out_2658931498525825673[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2658931498525825673[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2658931498525825673[27] = 0;
   out_2658931498525825673[28] = 0;
   out_2658931498525825673[29] = 0;
   out_2658931498525825673[30] = 0;
   out_2658931498525825673[31] = 0;
   out_2658931498525825673[32] = 0;
   out_2658931498525825673[33] = 0;
   out_2658931498525825673[34] = 0;
   out_2658931498525825673[35] = 0;
   out_2658931498525825673[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2658931498525825673[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2658931498525825673[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2658931498525825673[39] = 0;
   out_2658931498525825673[40] = 0;
   out_2658931498525825673[41] = 0;
   out_2658931498525825673[42] = 0;
   out_2658931498525825673[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2658931498525825673[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2658931498525825673[45] = 0;
   out_2658931498525825673[46] = 0;
   out_2658931498525825673[47] = 0;
   out_2658931498525825673[48] = 0;
   out_2658931498525825673[49] = 0;
   out_2658931498525825673[50] = 0;
   out_2658931498525825673[51] = 0;
   out_2658931498525825673[52] = 0;
   out_2658931498525825673[53] = 0;
   out_2658931498525825673[54] = 0;
   out_2658931498525825673[55] = 0;
   out_2658931498525825673[56] = 0;
   out_2658931498525825673[57] = 1;
   out_2658931498525825673[58] = 0;
   out_2658931498525825673[59] = 0;
   out_2658931498525825673[60] = 0;
   out_2658931498525825673[61] = 0;
   out_2658931498525825673[62] = 0;
   out_2658931498525825673[63] = 0;
   out_2658931498525825673[64] = 0;
   out_2658931498525825673[65] = 0;
   out_2658931498525825673[66] = dt;
   out_2658931498525825673[67] = 0;
   out_2658931498525825673[68] = 0;
   out_2658931498525825673[69] = 0;
   out_2658931498525825673[70] = 0;
   out_2658931498525825673[71] = 0;
   out_2658931498525825673[72] = 0;
   out_2658931498525825673[73] = 0;
   out_2658931498525825673[74] = 0;
   out_2658931498525825673[75] = 0;
   out_2658931498525825673[76] = 1;
   out_2658931498525825673[77] = 0;
   out_2658931498525825673[78] = 0;
   out_2658931498525825673[79] = 0;
   out_2658931498525825673[80] = 0;
   out_2658931498525825673[81] = 0;
   out_2658931498525825673[82] = 0;
   out_2658931498525825673[83] = 0;
   out_2658931498525825673[84] = 0;
   out_2658931498525825673[85] = dt;
   out_2658931498525825673[86] = 0;
   out_2658931498525825673[87] = 0;
   out_2658931498525825673[88] = 0;
   out_2658931498525825673[89] = 0;
   out_2658931498525825673[90] = 0;
   out_2658931498525825673[91] = 0;
   out_2658931498525825673[92] = 0;
   out_2658931498525825673[93] = 0;
   out_2658931498525825673[94] = 0;
   out_2658931498525825673[95] = 1;
   out_2658931498525825673[96] = 0;
   out_2658931498525825673[97] = 0;
   out_2658931498525825673[98] = 0;
   out_2658931498525825673[99] = 0;
   out_2658931498525825673[100] = 0;
   out_2658931498525825673[101] = 0;
   out_2658931498525825673[102] = 0;
   out_2658931498525825673[103] = 0;
   out_2658931498525825673[104] = dt;
   out_2658931498525825673[105] = 0;
   out_2658931498525825673[106] = 0;
   out_2658931498525825673[107] = 0;
   out_2658931498525825673[108] = 0;
   out_2658931498525825673[109] = 0;
   out_2658931498525825673[110] = 0;
   out_2658931498525825673[111] = 0;
   out_2658931498525825673[112] = 0;
   out_2658931498525825673[113] = 0;
   out_2658931498525825673[114] = 1;
   out_2658931498525825673[115] = 0;
   out_2658931498525825673[116] = 0;
   out_2658931498525825673[117] = 0;
   out_2658931498525825673[118] = 0;
   out_2658931498525825673[119] = 0;
   out_2658931498525825673[120] = 0;
   out_2658931498525825673[121] = 0;
   out_2658931498525825673[122] = 0;
   out_2658931498525825673[123] = 0;
   out_2658931498525825673[124] = 0;
   out_2658931498525825673[125] = 0;
   out_2658931498525825673[126] = 0;
   out_2658931498525825673[127] = 0;
   out_2658931498525825673[128] = 0;
   out_2658931498525825673[129] = 0;
   out_2658931498525825673[130] = 0;
   out_2658931498525825673[131] = 0;
   out_2658931498525825673[132] = 0;
   out_2658931498525825673[133] = 1;
   out_2658931498525825673[134] = 0;
   out_2658931498525825673[135] = 0;
   out_2658931498525825673[136] = 0;
   out_2658931498525825673[137] = 0;
   out_2658931498525825673[138] = 0;
   out_2658931498525825673[139] = 0;
   out_2658931498525825673[140] = 0;
   out_2658931498525825673[141] = 0;
   out_2658931498525825673[142] = 0;
   out_2658931498525825673[143] = 0;
   out_2658931498525825673[144] = 0;
   out_2658931498525825673[145] = 0;
   out_2658931498525825673[146] = 0;
   out_2658931498525825673[147] = 0;
   out_2658931498525825673[148] = 0;
   out_2658931498525825673[149] = 0;
   out_2658931498525825673[150] = 0;
   out_2658931498525825673[151] = 0;
   out_2658931498525825673[152] = 1;
   out_2658931498525825673[153] = 0;
   out_2658931498525825673[154] = 0;
   out_2658931498525825673[155] = 0;
   out_2658931498525825673[156] = 0;
   out_2658931498525825673[157] = 0;
   out_2658931498525825673[158] = 0;
   out_2658931498525825673[159] = 0;
   out_2658931498525825673[160] = 0;
   out_2658931498525825673[161] = 0;
   out_2658931498525825673[162] = 0;
   out_2658931498525825673[163] = 0;
   out_2658931498525825673[164] = 0;
   out_2658931498525825673[165] = 0;
   out_2658931498525825673[166] = 0;
   out_2658931498525825673[167] = 0;
   out_2658931498525825673[168] = 0;
   out_2658931498525825673[169] = 0;
   out_2658931498525825673[170] = 0;
   out_2658931498525825673[171] = 1;
   out_2658931498525825673[172] = 0;
   out_2658931498525825673[173] = 0;
   out_2658931498525825673[174] = 0;
   out_2658931498525825673[175] = 0;
   out_2658931498525825673[176] = 0;
   out_2658931498525825673[177] = 0;
   out_2658931498525825673[178] = 0;
   out_2658931498525825673[179] = 0;
   out_2658931498525825673[180] = 0;
   out_2658931498525825673[181] = 0;
   out_2658931498525825673[182] = 0;
   out_2658931498525825673[183] = 0;
   out_2658931498525825673[184] = 0;
   out_2658931498525825673[185] = 0;
   out_2658931498525825673[186] = 0;
   out_2658931498525825673[187] = 0;
   out_2658931498525825673[188] = 0;
   out_2658931498525825673[189] = 0;
   out_2658931498525825673[190] = 1;
   out_2658931498525825673[191] = 0;
   out_2658931498525825673[192] = 0;
   out_2658931498525825673[193] = 0;
   out_2658931498525825673[194] = 0;
   out_2658931498525825673[195] = 0;
   out_2658931498525825673[196] = 0;
   out_2658931498525825673[197] = 0;
   out_2658931498525825673[198] = 0;
   out_2658931498525825673[199] = 0;
   out_2658931498525825673[200] = 0;
   out_2658931498525825673[201] = 0;
   out_2658931498525825673[202] = 0;
   out_2658931498525825673[203] = 0;
   out_2658931498525825673[204] = 0;
   out_2658931498525825673[205] = 0;
   out_2658931498525825673[206] = 0;
   out_2658931498525825673[207] = 0;
   out_2658931498525825673[208] = 0;
   out_2658931498525825673[209] = 1;
   out_2658931498525825673[210] = 0;
   out_2658931498525825673[211] = 0;
   out_2658931498525825673[212] = 0;
   out_2658931498525825673[213] = 0;
   out_2658931498525825673[214] = 0;
   out_2658931498525825673[215] = 0;
   out_2658931498525825673[216] = 0;
   out_2658931498525825673[217] = 0;
   out_2658931498525825673[218] = 0;
   out_2658931498525825673[219] = 0;
   out_2658931498525825673[220] = 0;
   out_2658931498525825673[221] = 0;
   out_2658931498525825673[222] = 0;
   out_2658931498525825673[223] = 0;
   out_2658931498525825673[224] = 0;
   out_2658931498525825673[225] = 0;
   out_2658931498525825673[226] = 0;
   out_2658931498525825673[227] = 0;
   out_2658931498525825673[228] = 1;
   out_2658931498525825673[229] = 0;
   out_2658931498525825673[230] = 0;
   out_2658931498525825673[231] = 0;
   out_2658931498525825673[232] = 0;
   out_2658931498525825673[233] = 0;
   out_2658931498525825673[234] = 0;
   out_2658931498525825673[235] = 0;
   out_2658931498525825673[236] = 0;
   out_2658931498525825673[237] = 0;
   out_2658931498525825673[238] = 0;
   out_2658931498525825673[239] = 0;
   out_2658931498525825673[240] = 0;
   out_2658931498525825673[241] = 0;
   out_2658931498525825673[242] = 0;
   out_2658931498525825673[243] = 0;
   out_2658931498525825673[244] = 0;
   out_2658931498525825673[245] = 0;
   out_2658931498525825673[246] = 0;
   out_2658931498525825673[247] = 1;
   out_2658931498525825673[248] = 0;
   out_2658931498525825673[249] = 0;
   out_2658931498525825673[250] = 0;
   out_2658931498525825673[251] = 0;
   out_2658931498525825673[252] = 0;
   out_2658931498525825673[253] = 0;
   out_2658931498525825673[254] = 0;
   out_2658931498525825673[255] = 0;
   out_2658931498525825673[256] = 0;
   out_2658931498525825673[257] = 0;
   out_2658931498525825673[258] = 0;
   out_2658931498525825673[259] = 0;
   out_2658931498525825673[260] = 0;
   out_2658931498525825673[261] = 0;
   out_2658931498525825673[262] = 0;
   out_2658931498525825673[263] = 0;
   out_2658931498525825673[264] = 0;
   out_2658931498525825673[265] = 0;
   out_2658931498525825673[266] = 1;
   out_2658931498525825673[267] = 0;
   out_2658931498525825673[268] = 0;
   out_2658931498525825673[269] = 0;
   out_2658931498525825673[270] = 0;
   out_2658931498525825673[271] = 0;
   out_2658931498525825673[272] = 0;
   out_2658931498525825673[273] = 0;
   out_2658931498525825673[274] = 0;
   out_2658931498525825673[275] = 0;
   out_2658931498525825673[276] = 0;
   out_2658931498525825673[277] = 0;
   out_2658931498525825673[278] = 0;
   out_2658931498525825673[279] = 0;
   out_2658931498525825673[280] = 0;
   out_2658931498525825673[281] = 0;
   out_2658931498525825673[282] = 0;
   out_2658931498525825673[283] = 0;
   out_2658931498525825673[284] = 0;
   out_2658931498525825673[285] = 1;
   out_2658931498525825673[286] = 0;
   out_2658931498525825673[287] = 0;
   out_2658931498525825673[288] = 0;
   out_2658931498525825673[289] = 0;
   out_2658931498525825673[290] = 0;
   out_2658931498525825673[291] = 0;
   out_2658931498525825673[292] = 0;
   out_2658931498525825673[293] = 0;
   out_2658931498525825673[294] = 0;
   out_2658931498525825673[295] = 0;
   out_2658931498525825673[296] = 0;
   out_2658931498525825673[297] = 0;
   out_2658931498525825673[298] = 0;
   out_2658931498525825673[299] = 0;
   out_2658931498525825673[300] = 0;
   out_2658931498525825673[301] = 0;
   out_2658931498525825673[302] = 0;
   out_2658931498525825673[303] = 0;
   out_2658931498525825673[304] = 1;
   out_2658931498525825673[305] = 0;
   out_2658931498525825673[306] = 0;
   out_2658931498525825673[307] = 0;
   out_2658931498525825673[308] = 0;
   out_2658931498525825673[309] = 0;
   out_2658931498525825673[310] = 0;
   out_2658931498525825673[311] = 0;
   out_2658931498525825673[312] = 0;
   out_2658931498525825673[313] = 0;
   out_2658931498525825673[314] = 0;
   out_2658931498525825673[315] = 0;
   out_2658931498525825673[316] = 0;
   out_2658931498525825673[317] = 0;
   out_2658931498525825673[318] = 0;
   out_2658931498525825673[319] = 0;
   out_2658931498525825673[320] = 0;
   out_2658931498525825673[321] = 0;
   out_2658931498525825673[322] = 0;
   out_2658931498525825673[323] = 1;
}
void h_4(double *state, double *unused, double *out_84081779949576375) {
   out_84081779949576375[0] = state[6] + state[9];
   out_84081779949576375[1] = state[7] + state[10];
   out_84081779949576375[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7708340643954596463) {
   out_7708340643954596463[0] = 0;
   out_7708340643954596463[1] = 0;
   out_7708340643954596463[2] = 0;
   out_7708340643954596463[3] = 0;
   out_7708340643954596463[4] = 0;
   out_7708340643954596463[5] = 0;
   out_7708340643954596463[6] = 1;
   out_7708340643954596463[7] = 0;
   out_7708340643954596463[8] = 0;
   out_7708340643954596463[9] = 1;
   out_7708340643954596463[10] = 0;
   out_7708340643954596463[11] = 0;
   out_7708340643954596463[12] = 0;
   out_7708340643954596463[13] = 0;
   out_7708340643954596463[14] = 0;
   out_7708340643954596463[15] = 0;
   out_7708340643954596463[16] = 0;
   out_7708340643954596463[17] = 0;
   out_7708340643954596463[18] = 0;
   out_7708340643954596463[19] = 0;
   out_7708340643954596463[20] = 0;
   out_7708340643954596463[21] = 0;
   out_7708340643954596463[22] = 0;
   out_7708340643954596463[23] = 0;
   out_7708340643954596463[24] = 0;
   out_7708340643954596463[25] = 1;
   out_7708340643954596463[26] = 0;
   out_7708340643954596463[27] = 0;
   out_7708340643954596463[28] = 1;
   out_7708340643954596463[29] = 0;
   out_7708340643954596463[30] = 0;
   out_7708340643954596463[31] = 0;
   out_7708340643954596463[32] = 0;
   out_7708340643954596463[33] = 0;
   out_7708340643954596463[34] = 0;
   out_7708340643954596463[35] = 0;
   out_7708340643954596463[36] = 0;
   out_7708340643954596463[37] = 0;
   out_7708340643954596463[38] = 0;
   out_7708340643954596463[39] = 0;
   out_7708340643954596463[40] = 0;
   out_7708340643954596463[41] = 0;
   out_7708340643954596463[42] = 0;
   out_7708340643954596463[43] = 0;
   out_7708340643954596463[44] = 1;
   out_7708340643954596463[45] = 0;
   out_7708340643954596463[46] = 0;
   out_7708340643954596463[47] = 1;
   out_7708340643954596463[48] = 0;
   out_7708340643954596463[49] = 0;
   out_7708340643954596463[50] = 0;
   out_7708340643954596463[51] = 0;
   out_7708340643954596463[52] = 0;
   out_7708340643954596463[53] = 0;
}
void h_10(double *state, double *unused, double *out_7874962955445622717) {
   out_7874962955445622717[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7874962955445622717[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7874962955445622717[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8079274030706525455) {
   out_8079274030706525455[0] = 0;
   out_8079274030706525455[1] = 9.8100000000000005*cos(state[1]);
   out_8079274030706525455[2] = 0;
   out_8079274030706525455[3] = 0;
   out_8079274030706525455[4] = -state[8];
   out_8079274030706525455[5] = state[7];
   out_8079274030706525455[6] = 0;
   out_8079274030706525455[7] = state[5];
   out_8079274030706525455[8] = -state[4];
   out_8079274030706525455[9] = 0;
   out_8079274030706525455[10] = 0;
   out_8079274030706525455[11] = 0;
   out_8079274030706525455[12] = 1;
   out_8079274030706525455[13] = 0;
   out_8079274030706525455[14] = 0;
   out_8079274030706525455[15] = 1;
   out_8079274030706525455[16] = 0;
   out_8079274030706525455[17] = 0;
   out_8079274030706525455[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8079274030706525455[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8079274030706525455[20] = 0;
   out_8079274030706525455[21] = state[8];
   out_8079274030706525455[22] = 0;
   out_8079274030706525455[23] = -state[6];
   out_8079274030706525455[24] = -state[5];
   out_8079274030706525455[25] = 0;
   out_8079274030706525455[26] = state[3];
   out_8079274030706525455[27] = 0;
   out_8079274030706525455[28] = 0;
   out_8079274030706525455[29] = 0;
   out_8079274030706525455[30] = 0;
   out_8079274030706525455[31] = 1;
   out_8079274030706525455[32] = 0;
   out_8079274030706525455[33] = 0;
   out_8079274030706525455[34] = 1;
   out_8079274030706525455[35] = 0;
   out_8079274030706525455[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8079274030706525455[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8079274030706525455[38] = 0;
   out_8079274030706525455[39] = -state[7];
   out_8079274030706525455[40] = state[6];
   out_8079274030706525455[41] = 0;
   out_8079274030706525455[42] = state[4];
   out_8079274030706525455[43] = -state[3];
   out_8079274030706525455[44] = 0;
   out_8079274030706525455[45] = 0;
   out_8079274030706525455[46] = 0;
   out_8079274030706525455[47] = 0;
   out_8079274030706525455[48] = 0;
   out_8079274030706525455[49] = 0;
   out_8079274030706525455[50] = 1;
   out_8079274030706525455[51] = 0;
   out_8079274030706525455[52] = 0;
   out_8079274030706525455[53] = 1;
}
void h_13(double *state, double *unused, double *out_2170051261675760671) {
   out_2170051261675760671[0] = state[3];
   out_2170051261675760671[1] = state[4];
   out_2170051261675760671[2] = state[5];
}
void H_13(double *state, double *unused, double *out_7526129604422622352) {
   out_7526129604422622352[0] = 0;
   out_7526129604422622352[1] = 0;
   out_7526129604422622352[2] = 0;
   out_7526129604422622352[3] = 1;
   out_7526129604422622352[4] = 0;
   out_7526129604422622352[5] = 0;
   out_7526129604422622352[6] = 0;
   out_7526129604422622352[7] = 0;
   out_7526129604422622352[8] = 0;
   out_7526129604422622352[9] = 0;
   out_7526129604422622352[10] = 0;
   out_7526129604422622352[11] = 0;
   out_7526129604422622352[12] = 0;
   out_7526129604422622352[13] = 0;
   out_7526129604422622352[14] = 0;
   out_7526129604422622352[15] = 0;
   out_7526129604422622352[16] = 0;
   out_7526129604422622352[17] = 0;
   out_7526129604422622352[18] = 0;
   out_7526129604422622352[19] = 0;
   out_7526129604422622352[20] = 0;
   out_7526129604422622352[21] = 0;
   out_7526129604422622352[22] = 1;
   out_7526129604422622352[23] = 0;
   out_7526129604422622352[24] = 0;
   out_7526129604422622352[25] = 0;
   out_7526129604422622352[26] = 0;
   out_7526129604422622352[27] = 0;
   out_7526129604422622352[28] = 0;
   out_7526129604422622352[29] = 0;
   out_7526129604422622352[30] = 0;
   out_7526129604422622352[31] = 0;
   out_7526129604422622352[32] = 0;
   out_7526129604422622352[33] = 0;
   out_7526129604422622352[34] = 0;
   out_7526129604422622352[35] = 0;
   out_7526129604422622352[36] = 0;
   out_7526129604422622352[37] = 0;
   out_7526129604422622352[38] = 0;
   out_7526129604422622352[39] = 0;
   out_7526129604422622352[40] = 0;
   out_7526129604422622352[41] = 1;
   out_7526129604422622352[42] = 0;
   out_7526129604422622352[43] = 0;
   out_7526129604422622352[44] = 0;
   out_7526129604422622352[45] = 0;
   out_7526129604422622352[46] = 0;
   out_7526129604422622352[47] = 0;
   out_7526129604422622352[48] = 0;
   out_7526129604422622352[49] = 0;
   out_7526129604422622352[50] = 0;
   out_7526129604422622352[51] = 0;
   out_7526129604422622352[52] = 0;
   out_7526129604422622352[53] = 0;
}
void h_14(double *state, double *unused, double *out_352195394061766033) {
   out_352195394061766033[0] = state[6];
   out_352195394061766033[1] = state[7];
   out_352195394061766033[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6775162573415470624) {
   out_6775162573415470624[0] = 0;
   out_6775162573415470624[1] = 0;
   out_6775162573415470624[2] = 0;
   out_6775162573415470624[3] = 0;
   out_6775162573415470624[4] = 0;
   out_6775162573415470624[5] = 0;
   out_6775162573415470624[6] = 1;
   out_6775162573415470624[7] = 0;
   out_6775162573415470624[8] = 0;
   out_6775162573415470624[9] = 0;
   out_6775162573415470624[10] = 0;
   out_6775162573415470624[11] = 0;
   out_6775162573415470624[12] = 0;
   out_6775162573415470624[13] = 0;
   out_6775162573415470624[14] = 0;
   out_6775162573415470624[15] = 0;
   out_6775162573415470624[16] = 0;
   out_6775162573415470624[17] = 0;
   out_6775162573415470624[18] = 0;
   out_6775162573415470624[19] = 0;
   out_6775162573415470624[20] = 0;
   out_6775162573415470624[21] = 0;
   out_6775162573415470624[22] = 0;
   out_6775162573415470624[23] = 0;
   out_6775162573415470624[24] = 0;
   out_6775162573415470624[25] = 1;
   out_6775162573415470624[26] = 0;
   out_6775162573415470624[27] = 0;
   out_6775162573415470624[28] = 0;
   out_6775162573415470624[29] = 0;
   out_6775162573415470624[30] = 0;
   out_6775162573415470624[31] = 0;
   out_6775162573415470624[32] = 0;
   out_6775162573415470624[33] = 0;
   out_6775162573415470624[34] = 0;
   out_6775162573415470624[35] = 0;
   out_6775162573415470624[36] = 0;
   out_6775162573415470624[37] = 0;
   out_6775162573415470624[38] = 0;
   out_6775162573415470624[39] = 0;
   out_6775162573415470624[40] = 0;
   out_6775162573415470624[41] = 0;
   out_6775162573415470624[42] = 0;
   out_6775162573415470624[43] = 0;
   out_6775162573415470624[44] = 1;
   out_6775162573415470624[45] = 0;
   out_6775162573415470624[46] = 0;
   out_6775162573415470624[47] = 0;
   out_6775162573415470624[48] = 0;
   out_6775162573415470624[49] = 0;
   out_6775162573415470624[50] = 0;
   out_6775162573415470624[51] = 0;
   out_6775162573415470624[52] = 0;
   out_6775162573415470624[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_7195396181340429812) {
  err_fun(nom_x, delta_x, out_7195396181340429812);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6214721926630491138) {
  inv_err_fun(nom_x, true_x, out_6214721926630491138);
}
void pose_H_mod_fun(double *state, double *out_8844892526159179063) {
  H_mod_fun(state, out_8844892526159179063);
}
void pose_f_fun(double *state, double dt, double *out_8367001063557654494) {
  f_fun(state,  dt, out_8367001063557654494);
}
void pose_F_fun(double *state, double dt, double *out_2658931498525825673) {
  F_fun(state,  dt, out_2658931498525825673);
}
void pose_h_4(double *state, double *unused, double *out_84081779949576375) {
  h_4(state, unused, out_84081779949576375);
}
void pose_H_4(double *state, double *unused, double *out_7708340643954596463) {
  H_4(state, unused, out_7708340643954596463);
}
void pose_h_10(double *state, double *unused, double *out_7874962955445622717) {
  h_10(state, unused, out_7874962955445622717);
}
void pose_H_10(double *state, double *unused, double *out_8079274030706525455) {
  H_10(state, unused, out_8079274030706525455);
}
void pose_h_13(double *state, double *unused, double *out_2170051261675760671) {
  h_13(state, unused, out_2170051261675760671);
}
void pose_H_13(double *state, double *unused, double *out_7526129604422622352) {
  H_13(state, unused, out_7526129604422622352);
}
void pose_h_14(double *state, double *unused, double *out_352195394061766033) {
  h_14(state, unused, out_352195394061766033);
}
void pose_H_14(double *state, double *unused, double *out_6775162573415470624) {
  H_14(state, unused, out_6775162573415470624);
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
