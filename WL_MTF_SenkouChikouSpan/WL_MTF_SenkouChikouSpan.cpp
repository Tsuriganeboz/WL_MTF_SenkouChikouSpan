#define	_CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>

#include "IndicatorInterfaceUnit.h"
#include "TechnicalFunctions.h"

//--- 外部変数
int g_timeFrame = 0;			// タイムフレーム
int g_chikouSpanShift = 26;

//---
int g_barsLimit = 21 * 24 * 60 / 5;

// バッファ
TIndexBuffer g_indSenkouChikouSpan;


//+------------------------------------------------------------------+
//| 　　　　　　　　                                                         |
//+------------------------------------------------------------------+
double WL_MTFCalcIchimokuSenkouChikouSpan(char* symbol, int timeFrame, int index)
{
	return iClose(symbol, timeFrame, index);
}


//---------------------------------------------------------------------------
// Initialize indicator
//---------------------------------------------------------------------------
EXPORT void __stdcall Init()
{
	IndicatorShortName("MTF Senkou-Chikou Span");
	SetOutputWindow(ow_ChartWindow);


	RegOption("Time Frame", ot_TimeFrame, &g_timeFrame);
	g_timeFrame = PERIOD_M5;

	RegOption("Chikou-Span Shift", ot_Integer, &g_chikouSpanShift);
	SetOptionRange("Shift period", 1, MaxInt);
	g_chikouSpanShift = 26;


	g_indSenkouChikouSpan = CreateIndexBuffer();


	IndicatorBuffers(1);
	SetIndexBuffer(0, g_indSenkouChikouSpan);

	SetIndexStyle(0, ds_Line, psSolid, 2, RGB(0x00, 0x80, 0x00));
	SetIndexLabel(0, "Senkou-Chikou Span");

}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
EXPORT void __stdcall OnParamsChange()
{
	SetBufferShift(0, g_chikouSpanShift * (g_timeFrame / Timeframe()));
}

//---------------------------------------------------------------------------
// Calculate requested bar
//---------------------------------------------------------------------------
EXPORT void __stdcall Calculate(int index)
{
	if (Timeframe() > g_timeFrame)
		return;

	// 処理高速化のため。
	if (index >= g_barsLimit)
		return;

	double indexTime = Time(index);

	int timeFrameIndex = iBarShift(Symbol(), g_timeFrame, indexTime, false);
	if (timeFrameIndex == -1)
		return;

	double senkouChikouSpan = WL_MTFCalcIchimokuSenkouChikouSpan(Symbol(), g_timeFrame, 
																timeFrameIndex);

	g_indSenkouChikouSpan[index] = senkouChikouSpan;
}
