// shortname_ric - convert CCM short name to RIC code
#include <regex>
#include "xll/xll.h"
#include "../fmsdatetime/cmegroup.h"

using namespace xll;

typedef traits<XLOPERX>::xcstr xcstr;

static AddIn xai_ccm(
	Document("CCM")
	.Documentation("Cormorant Capital Management specific routines.")
);

static AddIn xai_shortname_ric(
	Function(XLL_LPOPER, "?xll_shortname_ric", "CCM.SHORTNAME.RIC")
	.Arg(XLL_CSTRING, "ShortName", "is the CCM short name. ")
	.Category("CCM")
	.FunctionHelp("Convert CCM short name to appropriate RIC.")
	.Documentation("Convert symbol yymmdd [CP] strike fraction to RIC code.")
);
LPOPERX WINAPI
xll_shortname_ric(xcstr sn)
{
#pragma XLLEXPORT

	static OPERX o;
	static std::regex re("^([A-Za-z]+)(\\d{2})(\\d{2})(\\d{2})([CcPp])(\\d{5})(\\d{3})");
                         //1 symbol   2	yr    3 mo    4 dy    5       6strike 7 frac     

	try {
		std::match_results<xcstr> mr;
		std::regex_search(sn, mr, re);

		ensure (mr.size() == 8);
		o = mr[1].str().append(" ").c_str(); // symbol
		o = Excel<XLOPER>(xlfConcatenate, o, Excel<XLOPER>(xlfEvaluate, OPER(mr[6].str().c_str()))); // strike
		if (Excel<XLOPER>(xlfEvaluate, OPER(mr[7].str().c_str()))) {
			o = Excel<XLOPER>(xlfConcatenate, o, OPER("-"), OPER(mr[7].str().c_str()), OPER("0")); // frac
		}

		bool cp = (tolower(mr[5].str()[0]) == 'c');
		int mo = (int)Excel<XLOPER>(xlfEvaluate, OPER(mr[3].str().c_str()));
		int code = cmegroup::option_month_code(cp ? mo : -mo);
		o = Excel<XLOPER>(xlfConcatenate, o, Excel<XLOPER>(xlfChar, Num(code))); // option month code
		o = Excel<XLOPER>(xlfConcatenate, o, Excel<XLOPER>(xlfMod, OPER(mr[2].str().c_str()), Num(10))); // year
		o = Excel<XLOPER>(xlfConcatenate, o, OPER(mr[4].str().c_str())); // day
		o = Excel<XLOPER>(xlfConcatenate, o, OPER(".Q")); // day
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		o = ErrX(xlerrNA);
	}

	return &o;
}

#ifdef _DEBUG

int 
test_shortname_ric(void)
{
	try {
		OPER o;
		o = *xll_shortname_ric("BLMN121222P00015000");
		o = *xll_shortname_ric("KERX121222C00003500");
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<Open> xao_shortname_ric(test_shortname_ric);

#endif // _DEBUG