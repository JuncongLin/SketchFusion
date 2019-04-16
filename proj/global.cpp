#include "stdafx.h"
#include "global.h"
#include "CSObject.h"
#include <fstream>
#include < math.h>
using namespace std;
ofstream outfile("Export.txt");

CCSObject gCriticalSection;

GLubyte* gpPixelBuffer=NULL;

unsigned char ColorTable[][3] = {
	{255, 250, 250},            // Snow
	{248, 248, 255},            // GhostWhite
	{245, 245, 245},            // WhiteSmoke
	{220, 220, 220},            // Gainsboro
	{255, 250, 240},            // FloralWhite
	{253, 245, 230},            // OldLace
	{250, 240, 230},            // Linen
	{250, 235, 215},            // AntiqueWhite
	{255, 239, 213},            // PapayaWhip
	{255, 235, 205},            // BlanchedAlmond
	{255, 228, 196},            // Bisque
	{255, 218, 185},            // PeachPuff
	{255, 222, 173},            // NavajoWhite
	{255, 228, 181},            // Moccasin
	{255, 248, 220},            // Cornsilk
	{255, 255, 240},            // Ivory
	{255, 250, 205},            // LemonChiffon
	{255, 245, 238},            // Seashell
	{240, 255, 240},            // Honeydew
	{245, 255, 250},            // MintCream
	{240, 255, 255},            // Azure
	{240, 248, 255},            // AliceBlue
	{230, 230, 250},            // lavender
	{255, 240, 245},            // LavenderBlush
	{255, 228, 225},            // MistyRose
	{255, 255, 255},            // White
	{0, 0, 0},                  // Black
	{47, 79, 79},               // DarkSlateGray
	{105, 105, 105},            // DimGrey
	{112, 128, 144},            // SlateGrey
	{119, 136, 153},            // LightSlateGray
	{190, 190, 190},            // Grey
	{211, 211, 211},            // LightGray
	{25, 25, 112},              // MidnightBlue
	{0, 0, 128},                // NavyBlue
	{100, 149, 237},            // CornflowerBlue
	{72, 61, 139},              // DarkSlateBlue
	{106, 90, 205},             // SlateBlue
	{123, 104, 238},            // MediumSlateBlue
	{132, 112, 255},            // LightSlateBlue
	{0, 0, 205},                // MediumBlue
	{65, 105, 225},             // RoyalBlue
	{0, 0, 255},                // Blue
	{30, 144, 255},             // DodgerBlue
	{0, 191, 255},              // DeepSkyBlue
	{135, 206, 235},            // SkyBlue
	{135, 206, 250},            // LightSkyBlue
	{70, 130, 180},             // SteelBlue
	{176, 196, 222},            // LightSteelBlue
	{173, 216, 230},            // LightBlue
	{176, 224, 230},            // PowderBlue
	{175, 238, 238},            // PaleTurquoise
	{0, 206, 209},              // DarkTurquoise
	{72, 209, 204},             // MediumTurquoise
	{64, 224, 208},             // Turquoise
	{0, 255, 255},              // Cyan
	{224, 255, 255},            // LightCyan
	{95, 158, 160},             // CadetBlue
	{102, 205, 170},            // MediumAquamarine
	{127, 255, 212},            // Aquamarine
	{0, 100, 0},                // DarkGreen
	{85, 107, 47},              // DarkOliveGreen
	{143, 188, 143},            // DarkSeaGreen
	{46, 139, 87},              // SeaGreen
	{60, 179, 113},             // MediumSeaGreen
	{32, 178, 170},             // LightSeaGreen
	{152, 251, 152},            // PaleGreen
	{0, 255, 127},              // SpringGreen
	{124, 252, 0},              // LawnGreen
	{0, 255, 0},                // Green
	{152, 207, 64},             // MonsterGreen
	{127, 255, 0},              // Chartreuse
	{0, 250, 154},              // MedSpringGreen
	{173, 255, 47},             // GreenYellow
	{50, 205, 50},              // LimeGreen
	{154, 205, 50},             // YellowGreen
	{34, 139, 34},              // ForestGreen
	{107, 142, 35},             // OliveDrab
	{189, 183, 107},            // DarkKhaki
	{238, 232, 170},            // PaleGoldenrod
	{250, 250, 210},            // LtGoldenrodYello
	{255, 255, 224},            // LightYellow
	{255, 255, 0},              // Yellow
	{255, 215, 0},              // Gold
	{238, 221, 130},            // LightGoldenrod
	{218, 165, 32},             // goldenrod
	{184, 134, 11},             // DarkGoldenrod
	{188, 143, 143},            // RosyBrown
	{205, 92, 92},              // IndianRed
	{139, 69, 19},              // SaddleBrown
	{160, 82, 45},              // Sienna
	{205, 133, 63},             // Peru
	{222, 184, 135},            // Burlywood
	{245, 245, 220},            // Beige
	{245, 222, 179},            // Wheat
	{244, 164, 96},             // SandyBrown
	{210, 180, 140},            // Tan
	{210, 105, 30},             // Chocolate
	{178, 34, 34},              // Firebrick
	{165, 42, 42},              // Brown
	{233, 150, 122},            // DarkSalmon
	{250, 128, 114},            // Salmon
	{255, 160, 122},            // LightSalmon
	{255, 165, 0},              // Orange
	{255, 140, 0},              // DarkOrange
	{255, 127, 80},             // Coral
	{240, 128, 128},            // LightCoral
	{255, 99, 71},              // Tomato
	{255, 69, 0},               // OrangeRed
	{255, 0, 0},                // Red
	{255, 105, 180},            // HotPink
	{255, 20, 147},             // DeepPink
	{255, 192, 203},            // Pink
	{255, 182, 193},            // LightPink
	{219, 112, 147},            // PaleVioletRed
	{176, 48, 96},              // Maroon
	{199, 21, 133},             // MediumVioletRed
	{208, 32, 144},             // VioletRed
	{255, 0, 255},              // Magenta
	{238, 130, 238},            // Violet
	{221, 160, 221},            // Plum
	{218, 112, 214},            // Orchid
	{186, 85, 211},             // MediumOrchid
	{153, 50, 204},             // DarkOrchid
	{148, 0, 211},              // DarkViolet
	{138, 43, 226},             // BlueViolet
	{160, 32, 240},             // Purple
	{147, 112, 219},            // MediumPurple
	{216, 191, 216},            // Thistle
	{255, 250, 250},            // Snow1
	{238, 233, 233},            // Snow2
	{205, 201, 201},            // Snow3
	{139, 137, 137},            // Snow4
	{255, 245, 238},            // Seashell1
	{238, 229, 222},            // Seashell2
	{205, 197, 191},            // Seashell3
	{139, 134, 130},            // Seashell4
	{255, 239, 219},            // AntiqueWhite1
	{238, 223, 204},            // AntiqueWhite2
	{205, 192, 176},            // AntiqueWhite3
	{139, 131, 120},            // AntiqueWhite4
	{255, 228, 196},            // Bisque1
	{238, 213, 183},            // Bisque2
	{205, 183, 158},            // Bisque3
	{139, 125, 107},            // Bisque4
	{255, 218, 185},            // PeachPuff1
	{238, 203, 173},            // PeachPuff2
	{205, 175, 149},            // PeachPuff3
	{139, 119, 101},            // PeachPuff4
	{255, 222, 173},            // NavajoWhite1
	{238, 207, 161},            // NavajoWhite2
	{205, 179, 139},            // NavajoWhite3
	{139, 121, 94},             // NavajoWhite4
	{255, 250, 205},            // LemonChiffon1
	{238, 233, 191},            // LemonChiffon2
	{205, 201, 165},            // LemonChiffon3
	{139, 137, 112},            // LemonChiffon4
	{255, 248, 220},            // Cornsilk1
	{238, 232, 205},            // Cornsilk2
	{205, 200, 177},            // Cornsilk3
	{139, 136, 120},            // Cornsilk4
	{255, 255, 240},            // Ivory1
	{238, 238, 224},            // Ivory2
	{205, 205, 193},            // Ivory3
	{139, 139, 131},            // Ivory4
	{240, 255, 240},            // Honeydew1
	{224, 238, 224},            // Honeydew2
	{193, 205, 193},            // Honeydew3
	{131, 139, 131},            // Honeydew4
	{255, 240, 245},            // LavenderBlush1
	{238, 224, 229},            // LavenderBlush2
	{205, 193, 197},            // LavenderBlush3
	{139, 131, 134},            // LavenderBlush4
	{255, 228, 225},            // MistyRose1
	{238, 213, 210},            // MistyRose2
	{205, 183, 181},            // MistyRose3
	{139, 125, 123},            // MistyRose4
	{240, 255, 255},            // Azure1
	{224, 238, 238},            // Azure2
	{193, 205, 205},            // Azure3
	{131, 139, 139},            // Azure4
	{131, 111, 255},            // SlateBlue1
	{122, 103, 238},            // SlateBlue2
	{105, 89, 205},             // SlateBlue3
	{71, 60, 139},              // SlateBlue4
	{72, 118, 255},             // RoyalBlue1
	{67, 110, 238},             // RoyalBlue2
	{58, 95, 205},              // RoyalBlue3
	{39, 64, 139},              // RoyalBlue4
	{0, 0, 255},                // Blue1
	{0, 0, 238},                // Blue2
	{0, 0, 205},                // Blue3
	{0, 0, 139},                // Blue4
	{30, 144, 255},             // DodgerBlue1
	{28, 134, 238},             // DodgerBlue2
	{24, 116, 205},             // DodgerBlue3
	{16, 78, 139},              // DodgerBlue4
	{99, 184, 255},             // SteelBlue1
	{92, 172, 238},             // SteelBlue2
	{79, 148, 205},             // SteelBlue3
	{54, 100, 139},             // SteelBlue4
	{0, 191, 255},              // DeepSkyBlue1
	{0, 178, 238},              // DeepSkyBlue2
	{0, 154, 205},              // DeepSkyBlue3
	{0, 104, 139},              // DeepSkyBlue4
	{135, 206, 255},            // SkyBlue1
	{126, 192, 238},            // SkyBlue2
	{108, 166, 205},            // SkyBlue3
	{74, 112, 139},             // SkyBlue4
	{176, 226, 255},            // LightSkyBlue1
	{164, 211, 238},            // LightSkyBlue2
	{141, 182, 205},            // LightSkyBlue3
	{96, 123, 139},             // LightSkyBlue4
	{198, 226, 255},            // SlateGray1
	{185, 211, 238},            // SlateGray2
	{159, 182, 205},            // SlateGray3
	{108, 123, 139},            // SlateGray4
	{202, 225, 255},            // LightSteelBlue1
	{188, 210, 238},            // LightSteelBlue2
	{162, 181, 205},            // LightSteelBlue3
	{110, 123, 139},            // LightSteelBlue4
	{191, 239, 255},            // LightBlue1
	{178, 223, 238},            // LightBlue2
	{154, 192, 205},            // LightBlue3
	{104, 131, 139},            // LightBlue4
	{224, 255, 255},            // LightCyan1
	{209, 238, 238},            // LightCyan2
	{180, 205, 205},            // LightCyan3
	{122, 139, 139},            // LightCyan4
	{187, 255, 255},            // PaleTurquoise1
	{174, 238, 238},            // PaleTurquoise2
	{150, 205, 205},            // PaleTurquoise3
	{102, 139, 139},            // PaleTurquoise4
	{152, 245, 255},            // CadetBlue1
	{142, 229, 238},            // CadetBlue2
	{122, 197, 205},            // CadetBlue3
	{83, 134, 139},             // CadetBlue4
	{0, 245, 255},              // Turquoise1
	{0, 229, 238},              // Turquoise2
	{0, 197, 205},              // Turquoise3
	{0, 134, 139},              // Turquoise4
	{0, 255, 255},              // Cyan1
	{0, 238, 238},              // Cyan2
	{0, 205, 205},              // Cyan3
	{0, 139, 139},              // Cyan4
	{151, 255, 255},            // DarkSlateGray1
	{141, 238, 238},            // DarkSlateGray2
	{121, 205, 205},            // DarkSlateGray3
	{82, 139, 139},             // DarkSlateGray4
	{127, 255, 212},            // Aquamarine1
	{118, 238, 198},            // Aquamarine2
	{102, 205, 170},            // Aquamarine3
	{69, 139, 116},             // Aquamarine4
	{193, 255, 193},            // DarkSeaGreen1
	{180, 238, 180},            // DarkSeaGreen2
	{155, 205, 155},            // DarkSeaGreen3
	{105, 139, 105},            // DarkSeaGreen4
	{84, 255, 159},             // SeaGreen1
	{78, 238, 148},             // SeaGreen2
	{67, 205, 128},             // SeaGreen3
	{46, 139, 87},              // SeaGreen4
	{154, 255, 154},            // PaleGreen1
	{144, 238, 144},            // PaleGreen2
	{124, 205, 124},            // PaleGreen3
	{84, 139, 84},              // PaleGreen4
	{0, 255, 127},              // SpringGreen1
	{0, 238, 118},              // SpringGreen2
	{0, 205, 102},              // SpringGreen3
	{0, 139, 69},               // SpringGreen4
	{0, 255, 0},                // Green1
	{0, 238, 0},                // Green2
	{0, 205, 0},                // Green3
	{0, 139, 0},                // Green4
	{127, 255, 0},              // Chartreuse1
	{118, 238, 0},              // Chartreuse2
	{102, 205, 0},              // Chartreuse3
	{69, 139, 0},               // Chartreuse4
	{192, 255, 62},             // OliveDrab1
	{179, 238, 58},             // OliveDrab2
	{154, 205, 50},             // OliveDrab3
	{105, 139, 34},             // OliveDrab4
	{202, 255, 112},            // DarkOliveGreen1
	{188, 238, 104},            // DarkOliveGreen2
	{162, 205, 90},             // DarkOliveGreen3
	{110, 139, 61},             // DarkOliveGreen4
	{255, 246, 143},            // Khaki1
	{238, 230, 133},            // Khaki2
	{205, 198, 115},            // Khaki3
	{139, 134, 78},             // Khaki4
	{255, 236, 139},            // LightGoldenrod1
	{238, 220, 130},            // LightGoldenrod2
	{205, 190, 112},            // LightGoldenrod3
	{139, 129, 76},             // LightGoldenrod4
	{255, 255, 224},            // LightYellow1
	{238, 238, 209},            // LightYellow2
	{205, 205, 180},            // LightYellow3
	{139, 139, 122},            // LightYellow4
	{255, 255, 0},              // Yellow1
	{238, 238, 0},              // Yellow2
	{205, 205, 0},              // Yellow3
	{139, 139, 0},              // Yellow4
	{255, 215, 0},              // Gold1
	{238, 201, 0},              // Gold2
	{205, 173, 0},              // Gold3
	{139, 117, 0},              // Gold4
	{255, 193, 37},             // Goldenrod1
	{238, 180, 34},             // Goldenrod2
	{205, 155, 29},             // Goldenrod3
	{139, 105, 20},             // Goldenrod4
	{255, 185, 15},             // DarkGoldenrod1
	{238, 173, 14},             // DarkGoldenrod2
	{205, 149, 12},             // DarkGoldenrod3
	{139, 101, 8},              // DarkGoldenrod4
	{255, 193, 193},            // RosyBrown1
	{238, 180, 180},            // RosyBrown2
	{205, 155, 155},            // RosyBrown3
	{139, 105, 105},            // RosyBrown4
	{255, 106, 106},            // IndianRed1
	{238, 99, 99},              // IndianRed2
	{205, 85, 85},              // IndianRed3
	{139, 58, 58},              // IndianRed4
	{255, 130, 71},             // Sienna1
	{238, 121, 66},             // Sienna2
	{205, 104, 57},             // Sienna3
	{139, 71, 38},              // Sienna4
	{255, 211, 155},            // Burlywood1
	{238, 197, 145},            // Burlywood2
	{205, 170, 125},            // Burlywood3
	{139, 115, 85},             // Burlywood4
	{255, 231, 186},            // Wheat1
	{238, 216, 174},            // Wheat2
	{205, 186, 150},            // Wheat3
	{139, 126, 102},            // Wheat4
	{255, 165, 79},             // Tan1
	{238, 154, 73},             // Tan2
	{205, 133, 63},             // Tan3
	{139, 90, 43},              // Tan4
	{255, 127, 36},             // Chocolate1
	{238, 118, 33},             // Chocolate2
	{205, 102, 29},             // Chocolate3
	{139, 69, 19},              // Chocolate4
	{255, 48, 48},              // Firebrick1
	{238, 44, 44},              // Firebrick2
	{205, 38, 38},              // Firebrick3
	{139, 26, 26},              // Firebrick4
	{255, 64, 64},              // Brown1
	{238, 59, 59},              // Brown2
	{205, 51, 51},              // Brown3
	{139, 35, 35},              // Brown4
	{255, 140, 105},            // Salmon1
	{238, 130, 98},             // Salmon2
	{205, 112, 84},             // Salmon3
	{139, 76, 57},              // Salmon4
	{255, 160, 122},            // LightSalmon1
	{238, 149, 114},            // LightSalmon2
	{205, 129, 98},             // LightSalmon3
	{139, 87, 66},              // LightSalmon4
	{255, 165, 0},              // Orange1
	{238, 154, 0},              // Orange2
	{205, 133, 0},              // Orange3
	{139, 90, 0},               // Orange4
	{255, 127, 0},              // DarkOrange1
	{238, 118, 0},              // DarkOrange2
	{205, 102, 0},              // DarkOrange3
	{139, 69, 0},               // DarkOrange4
	{255, 114, 86},             // Coral1
	{238, 106, 80},             // Coral2
	{205, 91, 69},              // Coral3
	{139, 62, 47},              // Coral4
	{255, 99, 71},              // Tomato1
	{238, 92, 66},              // Tomato2
	{205, 79, 57},              // Tomato3
	{139, 54, 38},              // Tomato4
	{255, 69, 0},               // OrangeRed1
	{238, 64, 0},               // OrangeRed2
	{205, 55, 0},               // OrangeRed3
	{139, 37, 0},               // OrangeRed4
	{255, 0, 0},                // Red1
	{238, 0, 0},                // Red2
	{205, 0, 0},                // Red3
	{139, 0, 0},                // Red4
	{255, 20, 147},             // DeepPink1
	{238, 18, 137},             // DeepPink2
	{205, 16, 118},             // DeepPink3
	{139, 10, 80},              // DeepPink4
	{255, 110, 180},            // HotPink1
	{238, 106, 167},            // HotPink2
	{205, 96, 144},             // HotPink3
	{139, 58, 98},              // HotPink4
	{255, 181, 197},            // Pink1
	{238, 169, 184},            // Pink2
	{205, 145, 158},            // Pink3
	{139, 99, 108},             // Pink4
	{255, 174, 185},            // LightPink1
	{238, 162, 173},            // LightPink2
	{205, 140, 149},            // LightPink3
	{139, 95, 101},             // LightPink4
	{255, 130, 171},            // PaleVioletRed1
	{238, 121, 159},            // PaleVioletRed2
	{205, 104, 137},            // PaleVioletRed3
	{139, 71, 93},              // PaleVioletRed4
	{255, 52, 179},             // Maroon1
	{238, 48, 167},             // Maroon2
	{205, 41, 144},             // Maroon3
	{139, 28, 98},              // Maroon4
	{255, 62, 150},             // VioletRed1
	{238, 58, 140},             // VioletRed2
	{205, 50, 120},             // VioletRed3
	{139, 34, 82},              // VioletRed4
	{255, 0, 255},              // Magenta1
	{238, 0, 238},              // Magenta2
	{205, 0, 205},              // Magenta3
	{139, 0, 139},              // Magenta4
	{255, 131, 250},            // Orchid1
	{238, 122, 233},            // Orchid2
	{205, 105, 201},            // Orchid3
	{139, 71, 137},             // Orchid4
	{255, 187, 255},            // Plum1
	{238, 174, 238},            // Plum2
	{205, 150, 205},            // Plum3
	{139, 102, 139},            // Plum4
	{224, 102, 255},            // MediumOrchid1
	{209, 95, 238},             // MediumOrchid2
	{180, 82, 205},             // MediumOrchid3
	{122, 55, 139},             // MediumOrchid4
	{191, 62, 255},             // DarkOrchid1
	{178, 58, 238},             // DarkOrchid2
	{154, 50, 205},             // DarkOrchid3
	{104, 34, 139},             // DarkOrchid4
	{155, 48, 255},             // Purple1
	{145, 44, 238},             // Purple2
	{125, 38, 205},             // Purple3
	{85, 26, 139},              // Purple4
	{171, 130, 255},            // MediumPurple1
	{159, 121, 238},            // MediumPurple2
	{137, 104, 205},            // MediumPurple3
	{93, 71, 139},              // MediumPurple4
	{255, 225, 255},            // Thistle1
	{238, 210, 238},            // Thistle2
	{205, 181, 205},            // Thistle3
	{139, 123, 139},            // Thistle4
	{28, 28, 28},               // grey11
	{54, 54, 54},               // grey21
	{79, 79, 79},               // grey31
	{105, 105, 105},            // grey41
	{130, 130, 130},            // grey51
	{156, 156, 156},            // grey61
	{181, 181, 181},            // grey71
	{207, 207, 207},            // gray81
	{232, 232, 232},            // gray91
	{169, 169, 169},            // DarkGrey
	{0, 0, 139},                // DarkBlue
	{0, 139, 139},              // DarkCyan
	{139, 0, 139},              // DarkMagenta
	{139, 0, 0},                // DarkRed
	{144, 238, 144},            // LightGreen
};
//int ncolors = sizeof(ColorTable) / (sizeof(unsigned char)*3);
const char *ColorName[] = {
	"Snow",
		"GhostWhite",
		"WhiteSmoke",
		"Gainsboro",
		"FloralWhite",
		"OldLace",
		"Linen",
		"AntiqueWhite",
		"PapayaWhip",
		"BlanchedAlmond",
		"Bisque",
		"PeachPuff",
		"NavajoWhite",
		"Moccasin",
		"Cornsilk",
		"Ivory",
		"LemonChiffon",
		"Seashell",
		"Honeydew",
		"MintCream",
		"Azure",
		"AliceBlue",
		"lavender",
		"LavenderBlush",
		"MistyRose",
		"White",
		"Black",
		"DarkSlateGray",
		"DimGrey",
		"SlateGrey",
		"LightSlateGray",
		"Grey",
		"LightGray",
		"MidnightBlue",
		"NavyBlue",
		"CornflowerBlue",
		"DarkSlateBlue",
		"SlateBlue",
		"MediumSlateBlue",
		"LightSlateBlue",
		"MediumBlue",
		"RoyalBlue",
		"Blue",
		"DodgerBlue",
		"DeepSkyBlue",
		"SkyBlue",
		"LightSkyBlue",
		"SteelBlue",
		"LightSteelBlue",
		"LightBlue",
		"PowderBlue",
		"PaleTurquoise",
		"DarkTurquoise",
		"MediumTurquoise",
		"Turquoise",
		"Cyan",
		"LightCyan",
		"CadetBlue",
		"MediumAquamarine",
		"Aquamarine",
		"DarkGreen",
		"DarkOliveGreen",
		"DarkSeaGreen",
		"SeaGreen",
		"MediumSeaGreen",
		"LightSeaGreen",
		"PaleGreen",
		"SpringGreen",
		"LawnGreen",
		"Green",
		"MonsterGreen",
		"Chartreuse",
		"MedSpringGreen",
		"GreenYellow",
		"LimeGreen",
		"YellowGreen",
		"ForestGreen",
		"OliveDrab",
		"DarkKhaki",
		"PaleGoldenrod",
		"LtGoldenrodYello",
		"LightYellow",
		"Yellow",
		"Gold",
		"LightGoldenrod",
		"goldenrod",
		"DarkGoldenrod",
		"RosyBrown",
		"IndianRed",
		"SaddleBrown",
		"Sienna",
		"Peru",
		"Burlywood",
		"Beige",
		"Wheat",
		"SandyBrown",
		"Tan",
		"Chocolate",
		"Firebrick",
		"Brown",
		"DarkSalmon",
		"Salmon",
		"LightSalmon",
		"Orange",
		"DarkOrange",
		"Coral",
		"LightCoral",
		"Tomato",
		"OrangeRed",
		"Red",
		"HotPink",
		"DeepPink",
		"Pink",
		"LightPink",
		"PaleVioletRed",
		"Maroon",
		"MediumVioletRed",
		"VioletRed",
		"Magenta",
		"Violet",
		"Plum",
		"Orchid",
		"MediumOrchid",
		"DarkOrchid",
		"DarkViolet",
		"BlueViolet",
		"Purple",
		"MediumPurple",
		"Thistle",
		"Snow1",
		"Snow2",
		"Snow3",
		"Snow4",
		"Seashell1",
		"Seashell2",
		"Seashell3",
		"Seashell4",
		"AntiqueWhite1",
		"AntiqueWhite2",
		"AntiqueWhite3",
		"AntiqueWhite4",
		"Bisque1",
		"Bisque2",
		"Bisque3",
		"Bisque4",
		"PeachPuff1",
		"PeachPuff2",
		"PeachPuff3",
		"PeachPuff4",
		"NavajoWhite1",
		"NavajoWhite2",
		"NavajoWhite3",
		"NavajoWhite4",
		"LemonChiffon1",
		"LemonChiffon2",
		"LemonChiffon3",
		"LemonChiffon4",
		"Cornsilk1",
		"Cornsilk2",
		"Cornsilk3",
		"Cornsilk4",
		"Ivory1",
		"Ivory2",
		"Ivory3",
		"Ivory4",
		"Honeydew1",
		"Honeydew2",
		"Honeydew3",
		"Honeydew4",
		"LavenderBlush1",
		"LavenderBlush2",
		"LavenderBlush3",
		"LavenderBlush4",
		"MistyRose1",
		"MistyRose2",
		"MistyRose3",
		"MistyRose4",
		"Azure1",
		"Azure2",
		"Azure3",
		"Azure4",
		"SlateBlue1",
		"SlateBlue2",
		"SlateBlue3",
		"SlateBlue4",
		"RoyalBlue1",
		"RoyalBlue2",
		"RoyalBlue3",
		"RoyalBlue4",
		"Blue1",
		"Blue2",
		"Blue3",
		"Blue4",
		"DodgerBlue1",
		"DodgerBlue2",
		"DodgerBlue3",
		"DodgerBlue4",
		"SteelBlue1",
		"SteelBlue2",
		"SteelBlue3",
		"SteelBlue4",
		"DeepSkyBlue1",
		"DeepSkyBlue2",
		"DeepSkyBlue3",
		"DeepSkyBlue4",
		"SkyBlue1",
		"SkyBlue2",
		"SkyBlue3",
		"SkyBlue4",
		"LightSkyBlue1",
		"LightSkyBlue2",
		"LightSkyBlue3",
		"LightSkyBlue4",
		"SlateGray1",
		"SlateGray2",
		"SlateGray3",
		"SlateGray4",
		"LightSteelBlue1",
		"LightSteelBlue2",
		"LightSteelBlue3",
		"LightSteelBlue4",
		"LightBlue1",
		"LightBlue2",
		"LightBlue3",
		"LightBlue4",
		"LightCyan1",
		"LightCyan2",
		"LightCyan3",
		"LightCyan4",
		"PaleTurquoise1",
		"PaleTurquoise2",
		"PaleTurquoise3",
		"PaleTurquoise4",
		"CadetBlue1",
		"CadetBlue2",
		"CadetBlue3",
		"CadetBlue4",
		"Turquoise1",
		"Turquoise2",
		"Turquoise3",
		"Turquoise4",
		"Cyan1",
		"Cyan2",
		"Cyan3",
		"Cyan4",
		"DarkSlateGray1",
		"DarkSlateGray2",
		"DarkSlateGray3",
		"DarkSlateGray4",
		"Aquamarine1",
		"Aquamarine2",
		"Aquamarine3",
		"Aquamarine4",
		"DarkSeaGreen1",
		"DarkSeaGreen2",
		"DarkSeaGreen3",
		"DarkSeaGreen4",
		"SeaGreen1",
		"SeaGreen2",
		"SeaGreen3",
		"SeaGreen4",
		"PaleGreen1",
		"PaleGreen2",
		"PaleGreen3",
		"PaleGreen4",
		"SpringGreen1",
		"SpringGreen2",
		"SpringGreen3",
		"SpringGreen4",
		"Green1",
		"Green2",
		"Green3",
		"Green4",
		"Chartreuse1",
		"Chartreuse2",
		"Chartreuse3",
		"Chartreuse4",
		"OliveDrab1",
		"OliveDrab2",
		"OliveDrab3",
		"OliveDrab4",
		"DarkOliveGreen1",
		"DarkOliveGreen2",
		"DarkOliveGreen3",
		"DarkOliveGreen4",
		"Khaki1",
		"Khaki2",
		"Khaki3",
		"Khaki4",
		"LightGoldenrod1",
		"LightGoldenrod2",
		"LightGoldenrod3",
		"LightGoldenrod4",
		"LightYellow1",
		"LightYellow2",
		"LightYellow3",
		"LightYellow4",
		"Yellow1",
		"Yellow2",
		"Yellow3",
		"Yellow4",
		"Gold1",
		"Gold2",
		"Gold3",
		"Gold4",
		"Goldenrod1",
		"Goldenrod2",
		"Goldenrod3",
		"Goldenrod4",
		"DarkGoldenrod1",
		"DarkGoldenrod2",
		"DarkGoldenrod3",
		"DarkGoldenrod4",
		"RosyBrown1",
		"RosyBrown2",
		"RosyBrown3",
		"RosyBrown4",
		"IndianRed1",
		"IndianRed2",
		"IndianRed3",
		"IndianRed4",
		"Sienna1",
		"Sienna2",
		"Sienna3",
		"Sienna4",
		"Burlywood1",
		"Burlywood2",
		"Burlywood3",
		"Burlywood4",
		"Wheat1",
		"Wheat2",
		"Wheat3",
		"Wheat4",
		"Tan1",
		"Tan2",
		"Tan3",
		"Tan4",
		"Chocolate1",
		"Chocolate2",
		"Chocolate3",
		"Chocolate4",
		"Firebrick1",
		"Firebrick2",
		"Firebrick3",
		"Firebrick4",
		"Brown1",
		"Brown2",
		"Brown3",
		"Brown4",
		"Salmon1",
		"Salmon2",
		"Salmon3",
		"Salmon4",
		"LightSalmon1",
		"LightSalmon2",
		"LightSalmon3",
		"LightSalmon4",
		"Orange1",
		"Orange2",
		"Orange3",
		"Orange4",
		"DarkOrange1",
		"DarkOrange2",
		"DarkOrange3",
		"DarkOrange4",
		"Coral1",
		"Coral2",
		"Coral3",
		"Coral4",
		"Tomato1",
		"Tomato2",
		"Tomato3",
		"Tomato4",
		"OrangeRed1",
		"OrangeRed2",
		"OrangeRed3",
		"OrangeRed4",
		"Red1",
		"Red2",
		"Red3",
		"Red4",
		"DeepPink1",
		"DeepPink2",
		"DeepPink3",
		"DeepPink4",
		"HotPink1",
		"HotPink2",
		"HotPink3",
		"HotPink4",
		"Pink1",
		"Pink2",
		"Pink3",
		"Pink4",
		"LightPink1",
		"LightPink2",
		"LightPink3",
		"LightPink4",
		"PaleVioletRed1",
		"PaleVioletRed2",
		"PaleVioletRed3",
		"PaleVioletRed4",
		"Maroon1",
		"Maroon2",
		"Maroon3",
		"Maroon4",
		"VioletRed1",
		"VioletRed2",
		"VioletRed3",
		"VioletRed4",
		"Magenta1",
		"Magenta2",
		"Magenta3",
		"Magenta4",
		"Orchid1",
		"Orchid2",
		"Orchid3",
		"Orchid4",
		"Plum1",
		"Plum2",
		"Plum3",
		"Plum4",
		"MediumOrchid1",
		"MediumOrchid2",
		"MediumOrchid3",
		"MediumOrchid4",
		"DarkOrchid1",
		"DarkOrchid2",
		"DarkOrchid3",
		"DarkOrchid4",
		"Purple1",
		"Purple2",
		"Purple3",
		"Purple4",
		"MediumPurple1",
		"MediumPurple2",
		"MediumPurple3",
		"MediumPurple4",
		"Thistle1",
		"Thistle2",
		"Thistle3",
		"Thistle4",
		"Grey11",
		"Grey21",
		"Grey31",
		"Grey41",
		"Grey51",
		"Grey61",
		"Grey71",
		"Grey81",
		"Grey91",
		"DarkGrey",
		"DarkBlue",
		"DarkCyan",
		"DarkMagenta",
		"DarkRed",
		"LightGreen"
};

CVector3d Ortho3(const CVector3d &arg)
{
	static CVector3d result;

	if (arg.x()!=0.0) 
		result.Set(-arg.y(),arg.x(),0.0);
	else if (arg.y()!=0.0) 
		result.Set(0.0,-arg.z(),arg.y());
	else if (arg.z()!=0.0) 
		result.Set(-arg.z(),0.0,0.0);
	else {
		printf("ERROR: attempted to normalize NULL vector in vmath.cc\n");
		result.Set(0.0,0.0,0.0);
	}
	return &result;
}

double my_random(void)
{
	return (rand() % 32767)/32767.0;
}
void ClampInt(int *x,int max)
{
	if ( (*x) < 0)
		*x = 0;
	else if ((*x) >= max)
		*x = max-1;
}
#define MY_EXP_RES 10000
#define MY_EXP_RANGE 6.
#define MY_EXP_DELTA .0012
#define MY_EXP_TABLE_RES 10001   // Number of entries in exp function table 

float fast_exp(float arg)
{
	static float exp_table[MY_EXP_TABLE_RES];  
	static char first_time = TRUE;

	// First call initializes the table

	if (first_time) {
		for ( int i = 0; i < MY_EXP_TABLE_RES; i++)
			exp_table[i] = exp(MY_EXP_DELTA*(i-MY_EXP_TABLE_RES/2));
		first_time = FALSE;
	}

	// Look up in precomputed table

	if (arg < -MY_EXP_RANGE) 
		return 0.;
	else {
		short tmp = (short)((arg+MY_EXP_RANGE)/MY_EXP_DELTA);

		if (tmp >= MY_EXP_RES) 
			return exp_table[MY_EXP_RES-1];
		else if (tmp < 0) 
			return exp_table[0];
		else 
			return exp_table[tmp];
	}
}

double CalcBernstein(int n,int i, double t)
{
	double res=1.0,n_eta=1.0,i_eta=1.0,n_i_eta=1.0,t1=1.0,t2=1.0;
	int k;
	for(k=1;k<=n;k++)
		n_eta*=k;
	for(k=1;k<=i;k++)
		i_eta*=k;
	for(k=1;k<=(n-i);k++)
		n_i_eta*=k;
	for(k=0;k<i;k++)
		t1*=t;
	for(k=0;k<(n-i);k++)
		t2*=(1.0-t);
	res=(t1*t2*n_eta)/(i_eta*n_i_eta);
	return res;

}
