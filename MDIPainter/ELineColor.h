#pragma once

#include <windef.h>

#ifndef ELINECOLOR_H
#define ELINECOLOR_H

enum class ELineColor : COLORREF
{
	ELineColor_Black = RGB(0, 0, 0),
	ELineColor_Lightred = RGB(255, 0, 0),
	ELineColor_Lime = RGB(0, 255, 0),
	ELineColor_Blue = RGB(0, 0, 255)
};

#endif