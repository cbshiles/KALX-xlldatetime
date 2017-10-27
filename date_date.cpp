// date_date.cpp - replacement for Excel DATE function
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xlldatetime.h"

using namespace xll;
using namespace datetime;
using namespace xml;

static AddInX xai_date_date(
	FunctionX(XLL_DATEX, _T("?xll_date_date"), DATETIME_PREFIX _T("DATE.DATE"))
	.Arg(XLL_FPX, _T("Year"),_T("is the year of the date."), 
		_T("=1970 + int(100*rand())"))
	.Arg(XLL_LONGX, _T("Month"),_T("is the month of the date starting from January = 1."),
		_T("=int(13*rand())"))
	.Arg(XLL_LONGX, _T("Day"), _T("is the day of the date."),
		_T("=int(32*rand())"))
	.Arg(XLL_LONGX, _T("Hour"), _T("is the hour of the time."),
		_T("=int(24*rand())"))
	.Arg(XLL_LONGX, _T("Minute"), _T("is the minute of the time."),
		_T("=int(60*rand())"))
	.Arg(XLL_LONGX, _T("Second"), _T("is the second of the time."),
		_T("=int(60*rand())"))
	.Category(CATEGORY) 
	.FunctionHelp(_T("Replacement for the Excel built-in DATE() function."))
	.Documentation(
		_T("This function can also be called with just one argument. ")
		_T("If <codeInline>Year</codeInline> is greater than 19700101 then it is assumed to be in the ")
		_T("<codeInline>yyyymmdd.hhmmss</codeInline> format. ")
		_T("If <codeInline>Year</codeInline> is an array, then it is assumed to contain the ")
		_T("<codeInline>Year</codeInline>, <codeInline>Month</codeInline>, and <codeInline>Day</codeInline> ")
		_T("and optionally the <codeInline>Hour</codeInline>, <codeInline>Minute</codeInline>, and <codeInline>Second</codeInline>. ")
	)
);
double WINAPI
xll_date_date(xfp* py, int m, int d, int h, int n, int s)
{
#pragma XLLEXPORT
	double t;

	try {
		int y;

		if (0 == h && 0 == n && 0 == s && 0 == m && 0 == d) {
			xword ny = size(*py);
			// single arg - deduce likely meaning
			if (ny == 1) {
				ensure (py->array[0] >= 19700101); // unix epoch
				breakdown(py->array[0], &y, &m, &d, &h, &n, &s);
			}
			else {
				ensure (ny >= 3 || !"First argument must specify at least year, month, and day");
				y = static_cast<int>(py->array[0]);
				m = static_cast<int>(py->array[1]);
				d = static_cast<int>(py->array[2]);
				if (ny > 3)
					h = static_cast<int>(py->array[3]);
				if (ny > 4)
					n = static_cast<int>(py->array[4]);
				if (ny > 5)
					s = static_cast<int>(py->array[5]);
			}
		}
		else {
			y = static_cast<int>(py->array[0]);
		}

		t = date(y, m, d, h, n, s).excel();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		t = std::numeric_limits<double>::quiet_NaN();
	}

	return t;
}

#ifdef _DEBUG
#include "xll/utility/srng.h"

#define NTESTS 100

int
test_date_date(void)
{
	try {
		double e, e_;
		date f;
		time_t t;
		int y, m, d, h, n, s, y_, m_, d_, h_, n_, s_;
		int i, i_, hms, hms_, dt, dlt, dst;
		utility::srng r(true); // set to true to get stored seed

		date epoch(1970, 1, 1);
		e = epoch.excel();
		ensure (e == EXCEL_EPOCH);

		y = 2086;
		m = 3;
		d = 10;
		h = 2; // dst leaping forward
		n = 52;
		s = 54;

		f = date(y, m , d, h, n, s);
		f.localtime(&y_, &m_, &d_, &h_, &n_, &s_, 0, 0, &dst);
		ensure (y == y_);
		ensure (m == m_);
		ensure (d == d_);
	//	ensure (h == h_); 
		ensure (h == h_ + 1); // dst
		ensure (n == n_);
		ensure (s == s_);

		t = f.time();
		date g(t);
		ensure (f == g);

		for (i = 0; i < NTESTS; ++i) {
			y = r.between(1970, 2100);
			m = r.between(1, 12);
			d = r.between(1, 28);
			h = r.between(0, 23);
			n = r.between(0, 59);
			s = r.between(0, 59);

			f = date(y, m , d, h, n, s);
			f.localtime(&y_, &m_, &d_, &h_, &n_, &s_, 0, 0, &dst);
			ensure (y == y_);
			ensure (m == m_);
			ensure (d == d_);
			ensure (h == h_);
			ensure (n == n_);
			ensure (s == s_);

			xll::FP yy(1,1);
			yy[0] = y;
			e = xll_date_date(yy.get(), m, d, h, n, s);
			f = date(e);
			e_ = f.excel();
			i_ = static_cast<int>((e - e_)*SECS_PER_DAY);
			ensure (abs(i_) <= 1);
			dlt = _daylight;
			dst = f.is_dst();
			ensure (y == f.year());
			ensure (m == f.month());
			ensure (d == f.day());
			hms = h*3600 + n*60 + s;
			hms_ = f.hour()*3600 + f.minute()*60 + f.second();
			dt = hms - hms_;
			ensure (abs(dt) <= 1); // sometimes off by a second

			ensure (Excel<XLOPERX>(xlfYear, Num(e)) == y);
			ensure (Excel<XLOPERX>(xlfMonth, Num(e)) == m);
			ensure (Excel<XLOPERX>(xlfDay, Num(e)) == d);
			ensure (Excel<XLOPERX>(xlfYear, Num(e)) == y);
			ensure (Excel<XLOPERX>(xlfMonth, Num(e)) == m);
			ensure (Excel<XLOPERX>(xlfDay, Num(e)) == d);
			h_ = (int)Excel<XLOPER>(xlfHour, Num(e));
			n_ = (int)Excel<XLOPER>(xlfMinute, Num(e));
			s_ = (int)Excel<XLOPER>(xlfSecond, Num(e));
			hms_ = h_*3600 + n_*60 + s_;
			dt = hms - hms_;
			ensure (abs(dt) <= 1); // sometimes off by a second

		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

static Auto<OpenAfter> xao_test_date_date(test_date_date);

#endif // _DEBUG
