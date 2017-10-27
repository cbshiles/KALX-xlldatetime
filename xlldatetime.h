// xlldatetime.h - common header for Excel add-ins
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
//#define EXCEL12
#include "xll/xll.h"
#include "../fmsdatetime/datetime.h"

typedef xll::traits<XLOPERX>::xcstr xcstr;
typedef xll::traits<XLOPERX>::xfp xfp;
typedef xll::traits<XLOPERX>::xword xword;

#ifndef CATEGORY
#define CATEGORY _T("Datetime")
#endif 
#ifndef CATEGORY_ENUM
#define CATEGORY_ENUM _T("Datetime Enumerations")
#endif
#ifndef DATETIME_PREFIX
#define DATETIME_PREFIX
#endif

// Common arguments
#define IS_DATE   _T("is the Excel Julian date.")
#define IS_FREQ	  _T("is the frequency from the FREQ_* enumeration.")
#define IS_ROLL   _T("is the rolling convention from the ROLL_* enumeration.")
#define IS_CAL    _T("is one or more holiday calendars from the CALENDAR_* enumeration.")
#define IS_DCB    _T("is the day count basis from the DCB_* enumeration.")
#define IS_UNIT   _T("is the time unit from the UNIT_* enumeration.")
#define IS_COUNT  _T("is the count corresponding to the unit enumeration.")
#define IS_DAY    _T("is a value from the DAY_* enumeration.")

namespace xll {
	// holiday_calendar from FP pointer
	inline bool
	is_holiday(const xfp* pc, double t)
	{
		for (xword i = 0; i < xll::size(*pc); ++i)
			if (ExcelX(xlUDF, OPERX(pc->array[i]), OPERX(t)))
				return true;

		return false;
	}
}  
#define HOLIDAY_CALENDAR(pcal) [pcal](const datetime::date& d) -> bool { return OPERX(true) == ExcelX(xlUDF, OPERX(pcal), OPERX(d.excel())); }