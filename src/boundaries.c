/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "boundaries.h"
#include "region1.h"
#include "region2.h"
#include "region4.h"
#include "region5.h"
#include "xmath.h"

enum h2o_region h2o_region_pT(double p, double T) /* [MPa, K] */
{
	if (T < 273.15 || T > 2273.15 || p <= 0 || p > 100)
		return H2O_REGION_OUT_OF_RANGE;

	else if (T <= 623.15) /* 1 or 2 */
	{
		if (p > h2o_region4_p_T(T))
			return H2O_REGION1;
		else
			return H2O_REGION2;
	}

	else if (T <= 1073.15) /* 3 or 2 */
	{
		if (p >= h2o_b23_p_T(T))
			return H2O_REGION3;
		else
			return H2O_REGION2;
	}

	else /* 5? */
	{
		if (p > 50)
			return H2O_REGION_OUT_OF_RANGE;
		else
			return H2O_REGION5;
	}
}

enum h2o_region h2o_region_ph(double p, double h) /* [MPa, kJ/kg] */
{
	double psatmax = 16.5291642;

	if (p < 0 || p > 100)
		return H2O_REGION_OUT_OF_RANGE;

	/* Left boundary. */
	if (h < h2o_region1_h_pT(p, 273.15))
		return H2O_REGION_OUT_OF_RANGE;

	/* Check the region4 curves. */
	if (p <= psatmax)
	{
		double Tsat = h2o_region4_T_p(p);

		if (h <= h2o_region1_h_pT(p, Tsat))
			return H2O_REGION1;
		else if (h < h2o_region2_h_pT(p, Tsat))
			return H2O_REGION4;
	}
	else /* Then, check the B13 & B23. */
	{
		if (h <= h2o_region1_h_pT(p, 623.15))
			return H2O_REGION1;
		else if (h < h2o_region2_h_pT(p, h2o_b23_T_p(p)))
			return H2O_REGION3;
	}

	/* Finally, check B25/right border. */
	if (h <= h2o_region2_h_pT(p, 1073.15))
		return H2O_REGION2;
	else if (p <= 50 && h <= h2o_region5_s_pT(p, 2273.15))
		return H2O_REGION5;
	else
		return H2O_REGION_OUT_OF_RANGE;
}

enum h2o_region h2o_region_ps(double p, double s) /* [MPa, kJ/kgK] */
{
	double psatmax = 16.5291642;

	if (p < 0 || p > 100 || s < 0)
		return H2O_REGION_OUT_OF_RANGE;

	/* First, check the region4 curves. */
	if (p <= psatmax)
	{
		double Tsat = h2o_region4_T_p(p);

		if (s <= h2o_region1_s_pT(p, Tsat))
			return H2O_REGION1;
		else if (s < h2o_region2_s_pT(p, Tsat))
			return H2O_REGION4;
	}
	else /* Then, check the B13 & B23. */
	{
		if (s <= h2o_region1_s_pT(p, 623.15))
			return H2O_REGION1;
		else if (s < h2o_region2_s_pT(p, h2o_b23_T_p(p)))
			return H2O_REGION3;
	}

	/* Finally, check B25/right border. */
	if (s <= h2o_region2_s_pT(p, 1073.15))
		return H2O_REGION2;
	else if (p <= 50 && s <= h2o_region5_s_pT(p, 2273.15))
		return H2O_REGION5;
	else
		return H2O_REGION_OUT_OF_RANGE;
}

enum h2o_region h2o_region_Tx(double T, double x) /* [K, 0..1] */
{
	if (x < 0 || x > 1 || T < 273.15 || T > 623.15)
		return H2O_REGION_OUT_OF_RANGE;

	return H2O_REGION4;
}

enum h2o_region h2o_region_px(double p, double x) /* [MPa, 0..1] */
{
	if (x < 0 || x > 1 || p < 611.213E-6 || p > 22.064)
		return H2O_REGION_OUT_OF_RANGE;

	return H2O_REGION4;
}

enum h2o_region h2o_region_rhoT(double rho, double T) /* [kg/m³, K] */
{
	/* XXX: support other regions */
	if (T >= 623.15)
	{
		double v = 1/rho;

		if (v <= h2o_region2_v_pT(h2o_b23_p_T(T), T))
			return H2O_REGION3;
		/* XXX: upper limit? */
	}

	return H2O_REGION_OUT_OF_RANGE;
}
