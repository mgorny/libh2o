/* libh2o -- h2o & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "tests.h"
#include "region1.h"
#include "saturation.h"

int main(void)
{
	CHECK(h2o_saturation_p_T(300), 0.353658941E-2, 1E-11);
	CHECK(h2o_saturation_p_T(500), 0.263889776E+1, 1E-8);
	CHECK(h2o_saturation_p_T(600), 0.123443146E+2, 1E-7);

	CHECK(h2o_saturation_T_p(0.1), 0.372755919E3, 1E-6);
	CHECK(h2o_saturation_T_p(1.0), 0.453035632E3, 1E-6);
	CHECK(h2o_saturation_T_p(10.), 0.584149488E3, 1E-6);

	CHECK(h2o_region1_v_pT(3., 300), 0.100215168E-2, 1E-11);
	CHECK(h2o_region1_v_pT(80, 300), 0.971180894E-3, 1E-12);
	CHECK(h2o_region1_v_pT(3., 500), 0.120241800E-2, 1E-11);
	CHECK(h2o_region1_h_pT(3., 300), 0.115331273E+3, 1E-6);
	CHECK(h2o_region1_h_pT(80, 300), 0.184142828E+3, 1E-6);
	CHECK(h2o_region1_h_pT(3., 500), 0.975542239E+3, 1E-6);
	CHECK(h2o_region1_u_pT(3., 300), 0.112324818E+3, 1E-6);
	CHECK(h2o_region1_u_pT(80, 300), 0.106448356E+3, 1E-6);
	CHECK(h2o_region1_u_pT(3., 500), 0.971934985E+3, 1E-6);
	CHECK(h2o_region1_s_pT(3., 300), 0.392294792E+0, 1E-6);
	CHECK(h2o_region1_s_pT(80, 300), 0.368563852E+0, 1E-6);
	CHECK(h2o_region1_s_pT(3., 500), 0.258041912E+1, 1E-5);

	return exit_status;
}
