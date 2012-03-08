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

double h2o_region1_v_pT(double p, double T); /* [MPa, K] -> [m³/kg] */

#ifdef __cplusplus
};
#endif /*__cplusplus*/

#endif /*_H2O_REGION1_H*/