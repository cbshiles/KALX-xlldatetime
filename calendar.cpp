// calendar.cpp - Implement holiday calendar and option conventions.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include <iterator>
#include "xlldatetime.h"
#include "../fmsdatetime/calendar.h"

#ifndef CATEGORY
#define CATEGORY _T("Calendar")
#endif 

using namespace xll;

static AddIn xai_calendar(
	Args(CATEGORY)
	.Documentation(xml::Conceptual("Holiday and exchange calendars. ")
		.section(_T("Introduction"),
			xml::element()
			.content(xml::para
				(
				_T("Holiday calendars conform to the Swaps Monitor naming convention but are not based on ")
				_T("static data that needs to be regularly updated. Instead the calendar is determined by the ")
				_T("holidays that define it and are computed at runtime. ")

				)
			)
			.content(xml::para
				(
				_T("The <codeInline>CALENDAR_*</codeInline> functions true if the date argument is ")
				_T("a holiday in the correspondng calendar. ")
				)
			)
		)
		,
		xml::element()
		._(xml::externalLink(_T("Swaps Monitor Publications, Inc."), _T("http://www.financialcalendar.com/Data/Holidays/Coverage")))
	)
);

#define XLL_CALENDAR(cal, CAL, CAT, HELP, DOC) static AddIn xai_##CAL( \
	Function(XLL_BOOL, XLL_STRZ_(_xll_##CAL##@8), #CAL) \
	.Arg(XLL_DATE, "Date", "is a date to be tested .", "=TODAY()") \
	.Category(CAT) \
	.FunctionHelp("Return TRUE if Date belongs to the " HELP) \
	.Documentation(DOC) \
); \
extern "C" __declspec(dllexport) BOOL WINAPI \
xll_##CAL(double d) \
{ \
	BOOL b(0); try { b =  cal(datetime::date(d)); } \
	catch (const std::exception& ex) { XLL_ERROR(ex.what()); } \
	return b; \
}

XLL_CALENDAR(calendar::BMA, CALENDAR_BMA, CATEGORY,
	"United States government bond calendar."
	,
	"United States govenment bond market holiday calendar."
)
XLL_CALENDAR(calendar::CMM, CALENDAR_CMM, CATEGORY,
	"CME Group calendar."
	,
	"CME Group holiday calendar."
)
XLL_CALENDAR(calendar::EUR, CALENDAR_EUR, CATEGORY,
	"BBA LIBOR currency calendar."
	,
	""
)
XLL_CALENDAR(calendar::LnB, CALENDAR_LnB, CATEGORY,
	"London bank holiday calendar."
	,
	""
)
XLL_CALENDAR(calendar::NER, CALENDAR_NER, CATEGORY,
	"North American Electric Reliability Corporation calendar."
	,
	"The North American Electric Reliability Corporation's (NERC) mission is to ensure the reliability of the North American bulk "
	"power system. NERC is the electric reliability organization (ERO) certified by the Federal Energy Regulatory Commission to "
	"establish and enforce reliability standards for the bulk-power system. NERC develops and enforces reliability standards; "
	"assesses adequacy annually via a 10-year forecast, and summer and winter forecasts; monitors the bulk power system; and educates, "
	"trains and certifies industry personnel. ERO activities in Canada related to the reliability of the bulk-power system are "
	"recognized and overseen by the appropriate governmental authorities in that country. "
)
XLL_CALENDAR(calendar::NYB, CALENDAR_NYB, CATEGORY,
	"New York bank calendar."
	,
	"New York bank holiday calendar."
)
XLL_CALENDAR(calendar::NYS, CALENDAR_NYS, CATEGORY,
	"New York Stock Exchange calendar."
	,
	"The New York Stock Exchange (NYSE) is a stock exchange located at 11 Wall Street in Lower Manhattan, New York City, USA. "
	"It is by far the world's largest stock exchange by market capitalization of its listed companies. "
	"The NYSE is operated by NYSE Euronext, which was formed by the NYSE's 2007 merger with the fully electronic stock exchange Euronext. "
)
