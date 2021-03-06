/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#pragma once

#ifndef _H2O_REGION1_H
#define _H2O_REGION1_H 1

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

double h2o_region1_v_pT(double p, double T);
double h2o_region1_u_pT(double p, double T);
double h2o_region1_s_pT(double p, double T);
double h2o_region1_h_pT(double p, double T);
double h2o_region1_cp_pT(double p, double T);
double h2o_region1_cv_pT(double p, double T);
double h2o_region1_w_pT(double p, double T);

double h2o_region1_T_ph(double p, double h);
double h2o_region1_T_ps(double p, double s);
double h2o_region1_p_hs(double h, double s);

#ifdef __cplusplus
};
#endif /*__cplusplus*/

#endif /*_H2O_REGION1_H*/
