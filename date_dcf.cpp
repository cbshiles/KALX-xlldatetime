// date_dcf.cpp - day count fractions
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xlldatetime.h"

using namespace xll;
using namespace datetime;
using namespace xml;

static AddInX xai_date_dcf(
	FunctionX(XLL_DOUBLE, _T("?xll_date_dcf"), DATETIME_PREFIX _T("DATE.DCF"))    
	.Arg(XLL_DATEX, _T("Start"), _T("is the first date of the period."),
		_T("=TODAY()"))
	.Arg(XLL_DATEX, _T("End"), _T("is the last date of the period."),
		_T("=R[-1]C[0] + INT(365*RAND())"))
	.Arg(XLL_LONGX, _T("Dcb"), IS_DCB _T(" "),
		_T("=DCB_30U_360()"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the day count fraction of the period from Start to End using a day count basis."))
#ifdef _DEBUG
	.Documentation(xml::File(Directory::Basename(_T(__FILE__)).append(_T("date_dcf.xml")).c_str()))
#endif
);
double WINAPI
xll_date_dcf(double start, double end, day_count_basis dcb)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		date sd(start), ed(end);

		result = ed.diff_dcb(sd, dcb);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

#ifdef _DEBUG

int
test_date_dcf(void)
{
	try {
//		double f;
		date d1, d2;

		d1 = date(2007, 12, 15);
		d2 = date(2008, 1, 10);
		ensure (d2.diff_actual_actual_isda(d1) == 17./365 + 9./366);

		d1 = date(2003, 10, 22);
		d2 = date(2003, 12, 23);
		ensure (d2.diff_30u_360(d1) == 2./12 + 1./360);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfter> xao_test_date_dcf(test_date_dcf);

#endif DEBUG

