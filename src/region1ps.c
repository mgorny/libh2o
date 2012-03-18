/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <math.h>

#include "region1.h"
#include "xmath.h"

/* Based on IF97-Rev, s. 5.2.2: The Backward Equation T(p, s) */

/* coefficient table; n[0] added for convenience */
static const double n[] = {
	+0.00000000000000E+00,

	+0.17478268058307E+03, +0.34806930892873E+02,
	+0.65292584978455E+01, +0.33039981775489E+00,
	-0.19281382923196E-06, -0.24909197244573E-22,
	-0.26107636489332E+00, +0.22592965981586E+00,
	-0.64256463395226E-01, +0.78876289270526E-02, /* [10] */

	+0.35672110607366E-09, +0.17332496994895E-23,
	+0.56608900654837E-03, -0.32635483139717E-03,
	+0.44778286690632E-04, -0.51322156908507E-09,
	-0.42522657042207E-25, +0.26400441360689E-12,
	+0.78124600459723E-28, -0.30732199903668E-30
};

static const int I[] = {
	0,

	0, 0, 0, 0, 0, 0, /* [6] */
	1, 1, 1, 1, 1, 1, /* [12] */
	2, 2, 2, 2, 2, /* [17] */
	3, 3, 4
};

static const int J[] = {
	0,

	0, 1, 2, 3, 11, 31, /* [6] */
	0, 1, 2, 3, 12, 31, /* [12] */
	0, 1, 2, 9, 31, /* [17] */
	10, 32, 32
};

double h2o_region1_T_ps(double p, double s) /* [MPa, kJ/kgK] -> [K] */
{
	double sexpr = s + 2;

	double sum = 0;

	int i;

	double ppowers[5], spowers[33];

	ppowers[0] = 1;
	ppowers[1] = p;

	for (i = 2; i <= 4; ++i)
		ppowers[i] = ppowers[i - 1] * p;

	spowers[0] = 1;
	spowers[1] = sexpr;
	spowers[2] = spowers[1] * sexpr;
	spowers[3] = spowers[2] * sexpr;

	spowers[9] = pow(sexpr, 9);
	for (i = 10; i <= 12; ++i)
		spowers[i] = spowers[i - 1] * sexpr;

	spowers[31] = pow(sexpr, 31);
	spowers[32] = spowers[31] * sexpr;

	for (i = 1; i <= 20; ++i)
	{
		double pipow = ppowers[I[i]];
		double sigmapow = spowers[J[i]];

		sum += n[i] * pipow * sigmapow;
	}

	return sum;
}
