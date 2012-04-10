/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif


#include "region2.h"
#include "xmath.h"

/* Based on IF97-Rev, s. 6.3.2: The Backward Equation T(p, s) ... */

/* coefficient table; n[0] added for convenience */
static const double n[] = {
	+0.00000000000000E+00,

	-0.39235983861984E+6, +0.51526573827270E+6,
	+0.40482443161048E+5, -0.32193790923902E+3,
	+0.96961424218694E+2, -0.22867846371773E+2,
	-0.44942914124357E+6, -0.50118336020166E+4,
	+0.35684463560015E+0, +0.44235335848190E+5, /* [10] */

	-0.13673388811708E+5, +0.42163260207864E+6,
	+0.22516925837475E+5, +0.47442144865646E+3,
	-0.14931130797647E+3, -0.19781126320452E+6,
	-0.23554399470760E+5, -0.19070616302076E+5,
	+0.55375669883164E+5, +0.38293691437363E+4, /* [20] */

	-0.60391860580567E+3, +0.19363102620331E+4,
	+0.42660643698610E+4, -0.59780638872718E+4,
	-0.70401463926862E+3, +0.33836784107553E+3,
	+0.20862786635187E+2, +0.33834172656196E-1,
	-0.43124428414893E-4, +0.16653791356412E+3, /* [30] */

	-0.13986292055898E+3, -0.78849547999872E+0,
	+0.72132411753872E-1, -0.59754839398283E-2,
	-0.12141358953904E-4, +0.23227096733871E-6,
	-0.10538463566194E+2, +0.20718925496502E+1,
	-0.72193155260427E-1, +0.20749887081120E-6, /* [40] */

	-0.18340657911379E-1, +0.29036272348696E-6,
	+0.21037527893619E+0, +0.25681239729999E-3,
	-0.12799002933781E-1, -0.82198102652018E-5
};

/* I[] * 4 */
static const int I[] = {
	0,

	-6, -6, -6, -6, -6, -6,
	-5, -5, -5,
	-4, -4, -4, -4, -4, -4,
	-3, -3,
	-2, -2, -2, -2,
	-1, -1, -1, -1,
	1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3,
	4, 4,
	5, 5,
	6, 6
};

static const double Jpows[] = {
	-27, -26, -25, -24, -23, -21,
	-19, -17, -16, -15, -14, -13,
	-11, -10, -9, -8, -7, -6,
	0, 1, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 14, 15, 16, 17, 18
};

static const int J[] = {
	0,

	3, 4, 6, 11, 12, 13,
	6, 9, 17,
	1, 5, 7, 8, 14, 15,
	9, 10,
	1, 11, 14, 16,
	0, 2, 12, 17, 19, 21, 25, 28,
	18, 19, 22, 23, 27, 29, 31,
	18, 21, 26, 32,
	24, 33,
	20, 30,
	22, 33
};

static const double sstar = 2; /* [kJ/kgK] */

double h2o_region2a_T_ps(double p, double s) /* [MPa, kJ/kgK] -> [K] */
{
	double piexpr = sqrt(sqrt(p));
	double sigma = s / sstar;
	double sigmaexpr = sigma - 2;

	double sum = 0;

	int i;

	double pipowers_store[7+6], sigmapowers[34];
	double* pipowers = &pipowers_store[6];

	fill_powers_incr(pipowers, 7, piexpr, 0);
	fill_powers_decr(pipowers, -6, piexpr, 0);
	fill_powers(sigmapowers, Jpows, 18, 34, sigmaexpr, 0);

	for (i = 1; i <= 46; ++i)
	{
		double pipow = pipowers[I[i]];
		double sigmapow = sigmapowers[J[i]];

		sum += n[i] * pipow * sigmapow;
	}

	return sum;
}