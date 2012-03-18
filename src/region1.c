/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <math.h>

#include "consts.h"
#include "region1.h"
#include "xmath.h"

/* Based on IF97-Rev, s. 5: Equations for Region 1 */

/* coefficient table; n[0] added for convenience */
static const double n[] = {
	+0.00000000000000E+00,

	+0.14632971213167E+00, -0.84548187169114E+00,
	-0.37563603672040E+01, +0.33855169168385E+01,
	-0.95791963387872E+00, +0.15772038513228E+00,
	-0.16616417199501E-01, +0.81214629983568E-03, /* [8] */

	+0.28319080123804E-03, -0.60706301565874E-03,
	-0.18990068218419E-01, -0.32529748770505E-01,
	-0.21841717175414E-01, -0.52838357969930E-04,
	-0.47184321073267E-03, -0.30001780793026E-03, /* [16] */

	+0.47661393906987E-04, -0.44141845330846E-05,
	-0.72694996297594E-15, -0.31679644845054E-04,
	-0.28270797985312E-05, -0.85205128120103E-09,
	-0.22425281908000E-05, -0.65171222895601E-06, /* [24] */

	-0.14341729937924E-12, -0.40516996860117E-06,
	-0.12734301741641E-08, -0.17424871230634E-09,
	-0.68762131295531E-18, +0.14478307828521E-19,
	+0.26335781662795E-22, -0.11947622640071E-22, /* [32] */

	+0.18228094581404E-23, -0.93537087292458E-25
};

static const double Ipows[] = {
	0, 1, 2, 3, 4, 5, 8, 21, 23, 29, 30, 31, 32
};

/* using Ipows[] indexes */
static const int I[] = {
	0,

	0, 0, 0, 0, 0, 0, 0, 0, /* [8] */
	1, 1, 1, 1, 1, 1, /* [14] */
	2, 2, 2, 2, 2, /* [19] */
	3, 3, 3,
	4, 4, 4, /* [25] */
	5,
	6, 6, /* [28] */
	7, 8, 9, 10, 11, 12
};

static const int J[] = {
	0,

	-2, -1, 0, 1, 2, 3, 4, 5, /* [8] */
	-9, -7, -1, 0, 1, 3, /* [14] */
	-3, 0, 1, 3, 17, /* [19] */
	-4, 0, 6,
	-5, -2, 10, /* [25] */
	-8,
	-11, -6, /* [28] */
	-29, -31, -38, -39, -40, -41
};

static const double pstar = 16.53; /* [MPa] */
static const double Tstar = 1386; /* [K] */

static inline double h2o_region1_gamma_pT(double p, double T, int pider, int tauder)
	/* p [MPa], T [K], pider, tauder: 0/1 */
{
	double pi = p / pstar;
	double tau = Tstar / T;

	double piexpr = 7.1 - pi;
	double tauexpr = tau - 1.222;

	double sum = 0;

	int i;

	double pipowers[13], taupowers_store[7 + 11];
		/* shift it for negative indices */
	double* taupowers = &taupowers_store[11];

	double tmp = piexpr;

	pipowers[pider] = 1;
	pipowers[pider + 1] = tmp;

	for (i = pider + 2; i <= 13; ++i)
	{
		if (Ipows[i] - 1 == Ipows[i - 1])
			tmp *= piexpr;
		else
			tmp = pow(piexpr, Ipows[i] - pider);
		pipowers[i] = tmp;
	}

	tmp = 1.0;
	for (i = pider - 1; i >= 0; --i)
	{
		if (Ipows[i] + 1 == Ipows[i + 1])
			tmp /= piexpr;
		else
			tmp = pow(piexpr, Ipows[i] - pider);
		pipowers[i] = tmp;
	}

	taupowers[tauder] = 1;
	taupowers[tauder + 1] = tauexpr;

	for (i = tauder + 2; i <= 6; ++i)
		taupowers[i] = taupowers[i - 1] * tauexpr;
	for (i = tauder - 1; i >= -11; --i)
		taupowers[i] = taupowers[i + 1] / tauexpr;

	for (i = 1; i <= 34; ++i)
	{
		double pipow = pipowers[I[i]];
		double taupow = J[i] >= -11 && J[i] <= 6 ? taupowers[J[i]]
				: pow(tauexpr, J[i] - tauder);

		double memb = n[i] * pipow * taupow;
		if (pider == 1)
			memb *= Ipows[I[i]];
		if (tauder == 1)
			memb *= J[i];

		sum += memb;
	}

	if (pider == 1)
		sum *= -pi;
	if (tauder == 1)
		sum *= tau;

	return sum;
}

double h2o_region1_v_pT(double p, double T) /* [MPa, K] -> [m³/kg] */
{
	double gammapi = h2o_region1_gamma_pT(p, T, 1, 0);

	return gammapi * R * T / p * 1E-3;
}

double h2o_region1_u_pT(double p, double T) /* [MPa, K] -> [kJ/kg] */
{
	double gammatau = h2o_region1_gamma_pT(p, T, 0, 1);
	double gammapi = h2o_region1_gamma_pT(p, T, 1, 0);

	return (gammatau - gammapi) * R * T;
}

double h2o_region1_s_pT(double p, double T) /* [MPa, K] -> [kJ/kgK] */
{
	double gammatau = h2o_region1_gamma_pT(p, T, 0, 1);
	double gamma = h2o_region1_gamma_pT(p, T, 0, 0);

	return (gammatau - gamma) * R;
}

double h2o_region1_h_pT(double p, double T) /* [MPa, K] -> [kJ/kg] */
{
	double gammatau = h2o_region1_gamma_pT(p, T, 0, 1);

	return gammatau * R * T;
}
