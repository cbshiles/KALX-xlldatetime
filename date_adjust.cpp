// date_dcf.cpp - day count fractions
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xlldatetime.h"
#include "../fmsdatetime/calendar.h"

using namespace xll;
using namespace datetime;
using namespace xml;

static AddInX xai_date_adjust(
	FunctionX(XLL_DATEX, _T("?xll_date_adjust"), DATETIME_PREFIX _T("DATE.ADJUST"))
	.Arg(XLL_DATEX, _T("Date"), IS_DATE,
		_T("=DATE.DATE(2013, 3, 30)"))
	.Arg(XLL_LONGX, _T("Roll"), IS_ROLL,
		_T("=ROLL_MODIFIED_FOLLOWING()"))
	.Arg(XLL_HANDLEX, _T("Calendar"), IS_CAL,
		_T("=CALENDAR_NYB"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Adjust Date based on Roll convention and holiday Calendar(s)."))
#ifdef _DEBUG
	.Documentation(xml::File(Directory::Basename(_T(__FILE__)).append(_T("date_adjust.xml")).c_str()))
#endif
);
double WINAPI
xll_date_adjust(double t, roll_convention roll, HANDLEX pcal)
{
#pragma XLLEXPORT
	date d;
	double nd = std::numeric_limits<double>::quiet_NaN();
	
	try {
		d = date(t);
		d.adjust(roll, HOLIDAY_CALENDAR(pcal));
		nd = d.excel();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return nd;
}

#ifdef _DEBUG

#include "../fmsdatetime/calendar.h"

int
test_date_adjust(void)
{
	try {
		ensure (date(2012, 1, 2).is_holiday(calendar::NYS));
		ensure (date(2012, 1, 2).weekday() == DAY_MON);

		ensure (date(2011, 12, 31).weekday() == DAY_SAT);
		ensure (!date(2011, 12, 31).is_bday(calendar::NYS));
		ensure (!date(2012, 1, 2).is_bday(calendar::NYS));

		ensure (date(2011, 12, 31).adjust(ROLL_NONE, calendar::NYS) == date(2011, 12, 31)); // only holidays rolled

		ensure (date(2011, 12, 31).adjust(ROLL_FOLLOWING_BUSINESS, calendar::NYS) == date(2012,  1,  3));
		ensure (date(2011, 12, 31).adjust(ROLL_MODIFIED_FOLLOWING, calendar::NYS) == date(2011, 12, 30));
		ensure (date(2011, 12, 31).adjust(ROLL_PREVIOUS_BUSINESS,  calendar::NYS) == date(2011, 12, 30));
		ensure (date(2012,  1,  1).adjust(ROLL_MODIFIED_PREVIOUS,  calendar::NYS) == date(2012,  1,  3));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfter> xao_test_date_adjust(test_date_adjust);

#endif // _DEBUG