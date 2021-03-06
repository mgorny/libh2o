/* libh2o -- water & steam properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "consts.h"
#include "h2o.h"

int tests_done = 0;
int tests_failed = 0;

typedef double (*func_get)(const h2o_t);
typedef h2o_t (*func_new)(double, double);

const char* name_by_prop(func_get prop)
{
	if (prop == &h2o_get_p)
		return "p";
	else if (prop == &h2o_get_T)
		return "T";
	else if (prop == &h2o_get_x)
		return "x";
	else if (prop == &h2o_get_u)
		return "u";
	else if (prop == &h2o_get_h)
		return "h";
	else if (prop == &h2o_get_s)
		return "s";
	else if (prop == &h2o_get_v)
		return "v";
	else if (prop == &h2o_get_cp)
		return "cp";
	else if (prop == &h2o_get_cv)
		return "cv";
	else if (prop == &h2o_get_w)
		return "w";
	else if (prop == &h2o_get_rho)
		return "rho";

	assert(not_reached);
	return NULL;
}

const char* name1_by_constr(func_new constr)
{
	if (constr == &h2o_new_pT)
		return "p";
	else if (constr == &h2o_new_ph)
		return "p";
	else if (constr == &h2o_new_ps)
		return "p";
	else if (constr == &h2o_new_hs)
		return "h";
	else if (constr == &h2o_new_Tx)
		return "T";
	else if (constr == &h2o_new_px)
		return "p";
	else if (constr == &h2o_new_rhoT)
		return "rho";

	assert(not_reached);
	return NULL;
}

const char* name2_by_constr(func_new constr)
{
	if (constr == &h2o_new_pT)
		return "T";
	else if (constr == &h2o_new_ph)
		return "h";
	else if (constr == &h2o_new_ps)
		return "s";
	else if (constr == &h2o_new_hs)
		return "s";
	else if (constr == &h2o_new_Tx)
		return "x";
	else if (constr == &h2o_new_px)
		return "x";
	else if (constr == &h2o_new_rhoT)
		return "T";

	assert(not_reached);
	return NULL;
}

void check(double result, double expected, double precision, const char* call,
		const char* arg1, double arg1_val,
		const char* arg2, double arg2_val)
{
	double difference = fabs(expected - result);

	if (difference >= precision)
	{
		fprintf(stderr, "[FAIL] %s(%3s=%.3e, %s=%.3e) = %.9e, while %.9e expected.\n",
				call, arg1, arg1_val, arg2, arg2_val, result, expected);

		++tests_failed;
	}
	else
		fprintf(stderr, "[ OK ] %s(%3s=%.3e, %s=%.3e) = %.9e.\n",
				call, arg1, arg1_val, arg2, arg2_val, result);

	++tests_done;
}

void check_any(func_new constr, double arg1, double arg2,
		func_get prop, double expected, double precision)
{
	check(prop(constr(arg1, arg2)), expected, precision, name_by_prop(prop),
			name1_by_constr(constr), arg1,
			name2_by_constr(constr), arg2);
}

void check_vuhs(double p, double T,
		double v_expected, double v_precision,
		double u_expected, double u_precision,
		double h_expected, double h_precision,
		double s_expected, double s_precision,
		double cp_expected, double cp_precision,
		double w_expected, double w_precision)
{
	check_any(&h2o_new_pT, p, T, &h2o_get_v,
			v_expected, v_precision);
	check_any(&h2o_new_pT, p, T, &h2o_get_u,
			u_expected, u_precision);
	check_any(&h2o_new_pT, p, T, &h2o_get_h,
			h_expected, h_precision);
	check_any(&h2o_new_pT, p, T, &h2o_get_s,
			s_expected, s_precision);
	check_any(&h2o_new_pT, p, T, &h2o_get_cp,
			cp_expected, cp_precision);
	check_any(&h2o_new_pT, p, T, &h2o_get_w,
			w_expected, w_precision);
}

void check_puhs(double rho, double T,
		double p_expected, double p_precision,
		double u_expected, double u_precision,
		double h_expected, double h_precision,
		double s_expected, double s_precision,
		double cp_expected, double cp_precision,
		double w_expected, double w_precision)
{
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_p,
			p_expected, p_precision);
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_u,
			u_expected, u_precision);
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_h,
			h_expected, h_precision);
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_s,
			s_expected, s_precision);
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_cp,
			cp_expected, cp_precision);
	check_any(&h2o_new_rhoT, rho, T, &h2o_get_w,
			w_expected, w_precision);
}

void check_Tv(func_new constr, double arg1, double arg2,
		double T_expected, double T_precision,
		double v_expected, double v_precision)
{
	check_any(constr, arg1, arg2, &h2o_get_T, T_expected, T_precision);
	check_any(constr, arg1, arg2, &h2o_get_v, v_expected, v_precision);
}

int main(void)
{
	/* Region 1 */
	check_vuhs(3., 300,
			0.100215168E-2, 1E-11,
			0.112324818E+3, 1E-6,
			0.115331273E+3, 1E-6,
			0.392294792E+0, 1E-9,
			0.417301218E+1, 1E-8,
			0.150773921E+4, 1E-5);
	check_vuhs(80, 300,
			0.971180894E-3, 1E-12,
			0.106448356E+3, 1E-6,
			0.184142828E+3, 1E-6,
			0.368563852E+0, 1E-9,
			0.401008987E+1, 1E-8,
			0.163469054E+4, 1E-5);
	check_vuhs(3., 500,
			0.120241800E-2, 1E-11,
			0.971934985E+3, 1E-6,
			0.975542239E+3, 1E-6,
			0.258041912E+1, 1E-8,
			0.465580682E+1, 1E-8,
			0.124071337E+4, 1E-5);

	/* Region 2 */
	check_vuhs(35E-4, 300,
			0.394913866E+2, 1E-7,
			0.241169160E+4, 1E-5,
			0.254991145E+4, 1E-5,
			0.852238967E+1, 1E-8,
			0.191300162E+1, 1E-8,
			0.427920172E+3, 1E-6);
	check_vuhs(35E-4, 700,
			0.923015898E+2, 1E-7,
			0.301262819E+4, 1E-5,
			0.333568375E+4, 1E-5,
			0.101749996E+2, 1E-7,
			0.208141274E+1, 1E-8,
			0.644289068E+3, 1E-6);
	check_vuhs(30E+0, 700,
			0.542946619E-2, 1E-11,
			0.246861076E+4, 1E-5,
			0.263149474E+4, 1E-5,
			0.517540298E+1, 1E-8,
			0.103505092E+2, 1E-7,
			0.480386523E+3, 1E-6);

	/* Region 3 */
	check_puhs(500, 650,
			0.255837018E2, 1E-7,
			0.181226279E4, 1E-5,
			0.186343019E4, 1E-5,
			0.405427273E1, 1E-8,
			0.138935717E2, 1E-7,
			0.502005554E3, 1E-6);
	check_puhs(200, 650,
			0.222930643E2, 1E-7,
			0.226365868E4, 1E-5,
			0.237512401E4, 1E-5,
			0.485438792E1, 1E-8,
			0.446579342E2, 1E-7,
			0.383444594E3, 1E-6);
	check_puhs(500, 750,
			0.783095639E2, 1E-7,
			0.210206932E4, 1E-5,
			0.225868845E4, 1E-5,
			0.446971906E1, 1E-8,
			0.634165359E1, 1E-8,
			0.760696041E3, 1E-6);

	/* Region 5 */
	check_vuhs(.5, 1500,
			0.138455090E+1, 1E-8,
			0.452749310E+4, 1E-5,
			0.521976855E+4, 1E-5,
			0.965408875E+1, 1E-8,
			0.261609445E+1, 1E-8,
			0.917068690E+3, 1E-6);
	check_vuhs(30, 1500,
			0.230761299E-1, 1E-10,
			0.447495124E+4, 1E-5,
			0.516723514E+4, 1E-5,
			0.772970133E+1, 1E-8,
			0.272724317E+1, 1E-8,
			0.928548002E+3, 1E-6);
	check_vuhs(30, 2000,
			0.311385219E-1, 1E-10,
			0.563707038E+4, 1E-5,
			0.657122604E+4, 1E-5,
			0.853640523E+1, 1E-8,
			0.288569882E+1, 1E-8,
			0.106736948E+4, 1E-5);

	/* saturation line */
	check_any(h2o_new_Tx, 300, 1, &h2o_get_p,
			0.353658941E-2, 1E-11);
	check_any(h2o_new_Tx, 500, 1, &h2o_get_p,
			0.263889776E+1, 1E-8);
	check_any(h2o_new_Tx, 600, 1, &h2o_get_p,
			0.123443146E+2, 1E-7);

	/* Region 1, f(p, h) */
	check_any(h2o_new_ph, 3., 500., &h2o_get_T,
			0.391798509E+3, 1E-6);
	check_any(h2o_new_ph, 80, 500., &h2o_get_T,
			0.378108626E+3, 1E-6);
	check_any(h2o_new_ph, 80, 1500, &h2o_get_T,
			0.611041229E+3, 1E-6);

	/* Region 2, f(p, h) */
	check_any(h2o_new_ph, 1E-3, 3000, &h2o_get_T,
			0.534433241E3, 1E-6);
	check_any(h2o_new_ph, 3.00, 3000, &h2o_get_T,
			0.575373370E3, 1E-6);
	check_any(h2o_new_ph, 3.00, 4000, &h2o_get_T,
			0.101077577E4, 1E-5);
	check_any(h2o_new_ph, 5.00, 3500, &h2o_get_T,
			0.801299102E3, 1E-6);
	check_any(h2o_new_ph, 5.00, 4000, &h2o_get_T,
			0.101531583E4, 1E-5);
	check_any(h2o_new_ph, 25.0, 3500, &h2o_get_T,
			0.875279054E3, 1E-6);
	check_any(h2o_new_ph, 40.0, 2700, &h2o_get_T,
			0.743056411E3, 1E-6);
	check_any(h2o_new_ph, 60.0, 2700, &h2o_get_T,
			0.791137067E3, 1E-6);
	check_any(h2o_new_ph, 60.0, 3200, &h2o_get_T,
			0.882756860E3, 1E-6);

	/* Region 1, f(p, s) */
	check_any(h2o_new_ps, 3., 0.5, &h2o_get_T,
			0.307842258E+3, 1E-6);
	check_any(h2o_new_ps, 80, 0.5, &h2o_get_T,
			0.309979785E+3, 1E-6);
	check_any(h2o_new_ps, 80, 3.0, &h2o_get_T,
			0.565899909E+3, 1E-6);

	/* Region 2, f(p, s) */
	check_any(h2o_new_ps, 0.1, 7.50, &h2o_get_T,
			0.399517097E3, 1E-6);
	check_any(h2o_new_ps, 0.1, 8.00, &h2o_get_T,
			0.514127081E3, 1E-6);
	check_any(h2o_new_ps, 2.5, 8.00, &h2o_get_T,
			0.103984917E4, 1E-5);
	check_any(h2o_new_ps, 8.0, 6.00, &h2o_get_T,
			0.600484040E3, 1E-6);
	check_any(h2o_new_ps, 8.0, 7.50, &h2o_get_T,
			0.106495556E4, 1E-5);
	check_any(h2o_new_ps, 90., 6.00, &h2o_get_T,
			0.103801126E4, 1E-5);
	check_any(h2o_new_ps, 20., 5.75, &h2o_get_T,
			0.697992849E3, 1E-6);
	check_any(h2o_new_ps, 80., 5.25, &h2o_get_T,
			0.854011484E3, 1E-6);
	check_any(h2o_new_ps, 80., 5.75, &h2o_get_T,
			0.949017998E3, 1E-6);

	/* Region 1, f(h, s) */
	check_any(h2o_new_hs, 1E-3, 0.0, &h2o_get_p,
			0.9800980612E-3, 1E-12);
	check_any(h2o_new_hs, 90E0, 0.0, &h2o_get_p,
			0.9192954727E+2, 1E-7);
	check_any(h2o_new_hs, 15E2, 3.4, &h2o_get_p,
			0.5868294423E+2, 1E-7);

	/* Region 2, f(h, s) */
	check_any(h2o_new_hs, 2800, 6.5, &h2o_get_p,
			0.1371012767E+1, 1E-9);
	check_any(h2o_new_hs, 2800, 9.5, &h2o_get_p,
			0.1879743844E-2, 1E-12);
	check_any(h2o_new_hs, 4100, 9.5, &h2o_get_p,
			0.1024788997E+0, 1E-10);
	check_any(h2o_new_hs, 2800, 6.0, &h2o_get_p,
			0.4793911442E+1, 1E-9);
	check_any(h2o_new_hs, 3600, 6.0, &h2o_get_p,
			0.8395519209E+2, 1E-8);
	check_any(h2o_new_hs, 3600, 7.0, &h2o_get_p,
			0.7527161441E+1, 1E-9);
	check_any(h2o_new_hs, 2800, 5.1, &h2o_get_p,
			0.9439202060E+2, 1E-8);
	check_any(h2o_new_hs, 2800, 5.8, &h2o_get_p,
			0.8414574124E+1, 1E-9);
	check_any(h2o_new_hs, 3400, 5.8, &h2o_get_p,
			0.8376903879E+2, 1E-8);

	/* Region 3, f(p, h) */
	check_Tv(h2o_new_ph, 20., 1700,
			0.6293083892E+3, 1E-7,
			0.1749903962E-2, 1E-12);
	check_Tv(h2o_new_ph, 50., 2000,
			0.6905718338E+3, 1E-7,
			0.1908139035E-2, 1E-12);
	check_Tv(h2o_new_ph, 100, 2100,
			0.7336163014E+3, 1E-7,
			0.1676229776E-2, 1E-12);
	check_Tv(h2o_new_ph, 20., 2500,
			0.6418418053E+3, 1E-7,
			0.6670547043E-2, 1E-12);
	check_Tv(h2o_new_ph, 50., 2400,
			0.7351848618E+3, 1E-7,
			0.2801244590E-2, 1E-12);
	check_Tv(h2o_new_ph, 100, 2700,
			0.8420460876E+3, 1E-7,
			0.2404234998E-2, 1E-12);

	/* Region 3, f(p, s) */
	check_Tv(h2o_new_ps, 20., 3.8,
			0.6282959869E+3, 1E-7,
			0.1733791463E-2, 1E-12);
	check_Tv(h2o_new_ps, 50., 3.6,
			0.6297158726E+3, 1E-7,
			0.1469680170E-2, 1E-12);
	check_Tv(h2o_new_ps, 100, 4.0,
			0.7056880237E+3, 1E-7,
			0.1555893131E-2, 1E-12);
	check_Tv(h2o_new_ps, 20., 5.0,
			0.6401176443E+3, 1E-7,
			0.6262101987E-2, 1E-12);
	check_Tv(h2o_new_ps, 50., 4.5,
			0.7163687517E+3, 1E-7,
			0.2332634294E-2, 1E-12);
	check_Tv(h2o_new_ps, 100, 5.0,
			0.8474332825E+3, 1E-7,
			0.2449610757E-2, 1E-12);

	/* Region 3, f(h, s) */
	/* (h,s)->(p,s)->(v,T)->p -- we've got to lose precision */
	check_any(h2o_new_hs, 1700, 3.8, &h2o_get_p,
			0.2555703246E+2, 1E-2);
	check_any(h2o_new_hs, 2000, 4.2, &h2o_get_p,
			0.4540873468E+2, 5E-3);
	check_any(h2o_new_hs, 2100, 4.3, &h2o_get_p,
			0.6078123340E+2, 5E-2);
	check_any(h2o_new_hs, 2600, 5.1, &h2o_get_p,
			0.3434999263E+2, 1E-3);
	check_any(h2o_new_hs, 2400, 4.7, &h2o_get_p,
			0.6363924887E+2, 5E-3);
	check_any(h2o_new_hs, 2700, 5.0, &h2o_get_p,
			0.8839043281E+2, 1E-3);

	/* Region 3, f(p, T) */
	check_any(h2o_new_pT, 50.000, 630.00, &h2o_get_v,
			0.1470853100E-2, 1E-12);
	check_any(h2o_new_pT, 80.000, 670.00, &h2o_get_v,
			0.1503831359E-2, 1E-12);
	check_any(h2o_new_pT, 50.000, 710.00, &h2o_get_v,
			0.2204728587E-2, 1E-12);
	check_any(h2o_new_pT, 80.000, 750.00, &h2o_get_v,
			0.1973692940E-2, 1E-12);
	check_any(h2o_new_pT, 20.000, 630.00, &h2o_get_v,
			0.1761696406E-2, 1E-12);
	check_any(h2o_new_pT, 30.000, 650.00, &h2o_get_v,
			0.1819560617E-2, 1E-12);
	check_any(h2o_new_pT, 26.000, 656.00, &h2o_get_v,
			0.2245587720E-2, 1E-12);
	check_any(h2o_new_pT, 30.000, 670.00, &h2o_get_v,
			0.2506897702E-2, 1E-12);
	check_any(h2o_new_pT, 26.000, 661.00, &h2o_get_v,
			0.2970225962E-2, 1E-12);
	check_any(h2o_new_pT, 30.000, 675.00, &h2o_get_v,
			0.3004627086E-2, 1E-12);
	check_any(h2o_new_pT, 26.000, 671.00, &h2o_get_v,
			0.5019029401E-2, 1E-12);
	check_any(h2o_new_pT, 30.000, 690.00, &h2o_get_v,
			0.4656470142E-2, 1E-12);
	check_any(h2o_new_pT, 23.600, 649.00, &h2o_get_v,
			0.2163198378E-2, 1E-12);
	check_any(h2o_new_pT, 24.000, 650.00, &h2o_get_v,
			0.2166044161E-2, 1E-12);
	check_any(h2o_new_pT, 23.600, 652.00, &h2o_get_v,
			0.2651081407E-2, 1E-12);
	check_any(h2o_new_pT, 24.000, 654.00, &h2o_get_v,
			0.2967802335E-2, 1E-12);
	check_any(h2o_new_pT, 23.600, 653.00, &h2o_get_v,
			0.3273916816E-2, 1E-12);
	check_any(h2o_new_pT, 24.000, 655.00, &h2o_get_v,
			0.3550329864E-2, 1E-12);
	check_any(h2o_new_pT, 23.500, 655.00, &h2o_get_v,
			0.4545001142E-2, 1E-12);
	check_any(h2o_new_pT, 24.000, 660.00, &h2o_get_v,
			0.5100267704E-2, 1E-12);
	check_any(h2o_new_pT, 23.000, 660.00, &h2o_get_v,
			0.6109525997E-2, 1E-12);
	check_any(h2o_new_pT, 24.000, 670.00, &h2o_get_v,
			0.6427325645E-2, 1E-12);
	check_any(h2o_new_pT, 22.600, 646.00, &h2o_get_v,
			0.2117860851E-2, 1E-12);
	check_any(h2o_new_pT, 23.000, 646.00, &h2o_get_v,
			0.2062374674E-2, 1E-12);
	check_any(h2o_new_pT, 22.600, 648.60, &h2o_get_v,
			0.2533063780E-2, 1E-12);
	check_any(h2o_new_pT, 22.800, 649.30, &h2o_get_v,
			0.2572971781E-2, 1E-12);
	check_any(h2o_new_pT, 22.600, 649.00, &h2o_get_v,
			0.2923432711E-2, 1E-12);
	check_any(h2o_new_pT, 22.800, 649.70, &h2o_get_v,
			0.2913311494E-2, 1E-12);
	check_any(h2o_new_pT, 22.600, 649.10, &h2o_get_v,
			0.3131208996E-2, 1E-12);
	check_any(h2o_new_pT, 22.800, 649.90, &h2o_get_v,
			0.3221160278E-2, 1E-12);
	check_any(h2o_new_pT, 22.600, 649.40, &h2o_get_v,
			0.3715596186E-2, 1E-12);
	check_any(h2o_new_pT, 22.800, 650.20, &h2o_get_v,
			0.3664754790E-2, 1E-12);
	check_any(h2o_new_pT, 21.100, 640.00, &h2o_get_v,
			0.1970999272E-2, 1E-12);
	check_any(h2o_new_pT, 21.800, 643.00, &h2o_get_v,
			0.2043919161E-2, 1E-12);
	check_any(h2o_new_pT, 21.100, 644.00, &h2o_get_v,
			0.5251009921E-2, 1E-12);
	check_any(h2o_new_pT, 21.800, 648.00, &h2o_get_v,
			0.5256844741E-2, 1E-12);
	check_any(h2o_new_pT, 19.100, 635.00, &h2o_get_v,
			0.1932829079E-2, 1E-12);
	check_any(h2o_new_pT, 20.000, 638.00, &h2o_get_v,
			0.1985387227E-2, 1E-12);
	check_any(h2o_new_pT, 17.000, 626.00, &h2o_get_v,
			0.8483262001E-2, 1E-12);
	check_any(h2o_new_pT, 20.000, 640.00, &h2o_get_v,
			0.6227528101E-2, 1E-12);
	check_any(h2o_new_pT, 21.500, 644.60, &h2o_get_v,
			0.2268366647E-2, 1E-12);
	check_any(h2o_new_pT, 22.000, 646.10, &h2o_get_v,
			0.2296350553E-2, 1E-12);
	check_any(h2o_new_pT, 22.500, 648.60, &h2o_get_v,
			0.2832373260E-2, 1E-12);
	check_any(h2o_new_pT, 22.300, 647.90, &h2o_get_v,
			0.2811424405E-2, 1E-12);
	check_any(h2o_new_pT, 22.150, 647.50, &h2o_get_v,
			0.3694032281E-2, 1E-12);
	check_any(h2o_new_pT, 22.300, 648.10, &h2o_get_v,
			0.3622226305E-2, 1E-12);
	check_any(h2o_new_pT, 22.110, 648.00, &h2o_get_v,
			0.4528072649E-2, 1E-12);
	check_any(h2o_new_pT, 22.300, 649.00, &h2o_get_v,
			0.4556905799E-2, 1E-12);
	check_any(h2o_new_pT, 22.000, 646.84, &h2o_get_v,
			0.2698354719E-2, 1E-12);
	check_any(h2o_new_pT, 22.064, 647.05, &h2o_get_v,
			0.2717655648E-2, 1E-12);
	check_any(h2o_new_pT, 22.000, 646.89, &h2o_get_v,
			0.3798732962E-2, 1E-12);
	check_any(h2o_new_pT, 22.064, 647.15, &h2o_get_v,
			0.3701940010E-2, 1E-12);

	if (tests_failed == 0)
		fprintf(stderr, "%d tests done. All tests suceeded.\n", tests_done);
	else
		fprintf(stderr, "%d of %d tests failed.\n", tests_failed, tests_done);

	return tests_failed ? 1 : 0;
}
