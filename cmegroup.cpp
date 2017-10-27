// cmegroup.cpp
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "xlldatetime.h"
#include "../fmsdatetime/cmegroup.h"

#ifndef CATEGORY
#define CATEGORY _T("Exchange")
#endif 

using namespace xll;
using namespace cmegroup;

#ifdef _DEBUG
static AddInX xai_gmegroup(
	ArgsX(CATEGORY)
	.Documentation(
		_T("Various functions associated with exchanges.")
		,
		xml::externalLink(_T("CME Group"), _T("http://www.cmegroup.com/"))
	)
);	
#endif

static AddInX xai_option_month_code(
	FunctionX(XLL_LPOPER, _T("?xll_option_month_code"), DATETIME_PREFIX _T("OPTION.MONTH.CODE"))
	.Arg(XLL_LONG, _T("Month"), _T(	"is the month the option expires as an integer. "),
		_T("=(IF(RAND() > .5, 1, -1)*(1 + INT(12*RAND())))"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Map integer month to the letter code for a call (if positive) or put (if negative) option."))
	.Documentation(
		_T("Use the negative of the month for put option codes. ")
		,
		xml::element()
			.content(xml::xlink(_T("OPTION.CODE.MONTH")))
			.content(xml::xlink(_T("FUTURES.MONTH.CODE")))
			.content(xml::xlink(_T("FUTURES.CODE.MONTH")))
	)
);
LPOPER WINAPI
xll_option_month_code(LONG m)
{
#pragma XLLEXPORT
	static OPER oResult;

	int c = option_month_code(m);

	if (!c)
		oResult = Err(xlerrNum);
	else
		oResult = Excel<XLOPER>(xlfChar, Num(c));

	return &oResult;
}

static AddInX xai_option_code_month(
	FunctionX(XLL_LPOPER, _T("?xll_option_code_month"), DATETIME_PREFIX _T("OPTION.CODE.MONTH")) 
	.Arg(XLL_PSTRING, "Code", _T("is the single letter code of the option contract. "), 
		_T("=CHAR(CODE(\"A\") + INT(24*RAND()))"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Map the option letter code for equity to the expiration month of the call or the negative of the month if a put."))
	.Documentation(	
		_T("The negative of the month is returned for put options.")
		,
		xml::element()
			.content(xml::xlink(_T("OPTION.MONTH.CODE")))
			.content(xml::xlink(_T("FUTURES.MONTH.CODE")))
			.content(xml::xlink(_T("FUTURES.CODE.MONTH")))
	)
);

LPOPER WINAPI
xll_option_code_month(const char* c)
{
#pragma XLLEXPORT
	static OPER oResult;

	if (!c || c[0] != 1) {
		oResult = Err(xlerrNum);
	}
	else {
		int m = option_code_month(c[1]);

		if (!m)
			oResult = Err(xlerrNum);
		else
			oResult = m;
	}

	return &oResult;
}

static AddInX xai_futures_month_code(
	FunctionX(XLL_LPOPER, _T("?xll_futures_month_code"), DATETIME_PREFIX _T("FUTURES.MONTH.CODE"))
	.Arg(XLL_LONG, _T("Month"), _T("is the month the futures expires. "), 
		_T("=1 + INT(12*RAND())"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Map integer month to the letter code for a futures."))
	.Documentation(	
		_T("The month should be in the range 1 to 12.")
		,
		xml::element()
			.content(xml::xlink(_T("OPTION.CODE.MONTH")))
			.content(xml::xlink(_T("OPTION.MONTH.CODE")))
			.content(xml::xlink(_T("FUTURES.CODE.MONTH")))
	)
);
LPOPER WINAPI
xll_futures_month_code(LONG m)
{
#pragma XLLEXPORT
	static OPER oResult;

	int c = futures_month_code(m);

	if (!c)
		oResult = Err(xlerrNum);
	else
		oResult = Excel<XLOPER>(xlfChar, Num(c));

	return &oResult;
}

static AddInX xai_futures_code_month(
	FunctionX(XLL_LPOPER, _T("?xll_futures_code_month"), DATETIME_PREFIX _T("FUTURES.CODE.MONTH"))
	.Arg(XLL_PSTRING, _T("Code"), _T("is the single letter code of the futures contract. "),
		_T("=FUTURES.MONTH.CODE(1 + INT(12*RAND()))"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Map the futures letter code to the expiration month of the futures."))
	.Documentation(
		_T("The month will be in the range 1 to 12.")
		,
		xml::element()
			.content(xml::xlink(_T("OPTION.CODE.MONTH")))
			.content(xml::xlink(_T("OPTION.MONTH.CODE")))
			.content(xml::xlink(_T("FUTURES.MONTH.CODE")))
	)
);
LPOPER WINAPI
xll_futures_code_month(const char* c)
{
#pragma XLLEXPORT
	static OPER oResult;

	if (!c || c[0] != 1) {
		oResult = Err(xlerrNum);
	}
	else {
		int m = futures_code_month(c[1]);

		if (!m)
			oResult = Err(xlerrNum);
		else
			oResult = m;
	}

	return &oResult;
}