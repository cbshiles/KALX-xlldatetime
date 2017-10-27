// schedule.cpp - schedule of dates
#include "xlldatetime.h"

using namespace xll;

static AddInX xai_datetime_schedule(
	FunctionX(XLL_FPX, _T("?xll_datetime_schedule"), _T("DATETIME.SCHEDULE"))
	.Arg(XLL_DATEX, _T("Effective"), _T("is the first date in the schedule."))
	.Arg(XLL_LONGX, _T("Count"), IS_COUNT)
	.Arg(XLL_LONGX, _T("Unit"), IS_UNIT)
	.Arg(XLL_LONGX, _T("Frequency"), IS_FREQ)
	.Arg(XLL_LONGX, _T("Roll"), IS_ROLL)
	.Arg(XLL_HANDLEX,   _T("Calendar"), IS_CAL)
	.Category(CATEGORY)
	.FunctionHelp(_T("Generate periodic dates."))
#ifdef _DEBUG
	.Documentation()
#endif
);
xfp* WINAPI
xll_datetime_schedule(double eff, int count, time_unit unit, payment_frequency freq, roll_convention roll, HANDLEX pcal)
{
#pragma XLLEXPORT
	static FPX s;

	try {
		std::vector<datetime::date> d = datetime::schedule(datetime::date(eff), count, unit, freq, roll, HOLIDAY_CALENDAR(pcal));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return s.get();
}