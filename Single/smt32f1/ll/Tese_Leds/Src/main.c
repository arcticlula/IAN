/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM3_CCR1_Address 0x40000434	// physical memory address of Timer 3 CCR1 register

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t LED_BYTE_Buffer[100];

uint8_t eightbit[766][3] =
{
	{255, 0, 0},
	{254, 1, 0},
	{253, 2, 0},
	{252, 3, 0},
	{251, 4, 0},
	{250, 5, 0},
	{249, 6, 0},
	{248, 7, 0},
	{247, 8, 0},
	{246, 9, 0},
	{245, 10, 0},
	{244, 11, 0},
	{243, 12, 0},
	{242, 13, 0},
	{241, 14, 0},
	{240, 15, 0},
	{239, 16, 0},
	{238, 17, 0},
	{237, 18, 0},
	{236, 19, 0},
	{235, 20, 0},
	{234, 21, 0},
	{233, 22, 0},
	{232, 23, 0},
	{231, 24, 0},
	{230, 25, 0},
	{229, 26, 0},
	{228, 27, 0},
	{227, 28, 0},
	{226, 29, 0},
	{225, 30, 0},
	{224, 31, 0},
	{223, 32, 0},
	{222, 33, 0},
	{221, 34, 0},
	{220, 35, 0},
	{219, 36, 0},
	{218, 37, 0},
	{217, 38, 0},
	{216, 39, 0},
	{215, 40, 0},
	{214, 41, 0},
	{213, 42, 0},
	{212, 43, 0},
	{211, 44, 0},
	{210, 45, 0},
	{209, 46, 0},
	{208, 47, 0},
	{207, 48, 0},
	{206, 49, 0},
	{205, 50, 0},
	{204, 51, 0},
	{203, 52, 0},
	{202, 53, 0},
	{201, 54, 0},
	{200, 55, 0},
	{199, 56, 0},
	{198, 57, 0},
	{197, 58, 0},
	{196, 59, 0},
	{195, 60, 0},
	{194, 61, 0},
	{193, 62, 0},
	{192, 63, 0},
	{191, 64, 0},
	{190, 65, 0},
	{189, 66, 0},
	{188, 67, 0},
	{187, 68, 0},
	{186, 69, 0},
	{185, 70, 0},
	{184, 71, 0},
	{183, 72, 0},
	{182, 73, 0},
	{181, 74, 0},
	{180, 75, 0},
	{179, 76, 0},
	{178, 77, 0},
	{177, 78, 0},
	{176, 79, 0},
	{175, 80, 0},
	{174, 81, 0},
	{173, 82, 0},
	{172, 83, 0},
	{171, 84, 0},
	{170, 85, 0},
	{169, 86, 0},
	{168, 87, 0},
	{167, 88, 0},
	{166, 89, 0},
	{165, 90, 0},
	{164, 91, 0},
	{163, 92, 0},
	{162, 93, 0},
	{161, 94, 0},
	{160, 95, 0},
	{159, 96, 0},
	{158, 97, 0},
	{157, 98, 0},
	{156, 99, 0},
	{155, 100, 0},
	{154, 101, 0},
	{153, 102, 0},
	{152, 103, 0},
	{151, 104, 0},
	{150, 105, 0},
	{149, 106, 0},
	{148, 107, 0},
	{147, 108, 0},
	{146, 109, 0},
	{145, 110, 0},
	{144, 111, 0},
	{143, 112, 0},
	{142, 113, 0},
	{141, 114, 0},
	{140, 115, 0},
	{139, 116, 0},
	{138, 117, 0},
	{137, 118, 0},
	{136, 119, 0},
	{135, 120, 0},
	{134, 121, 0},
	{133, 122, 0},
	{132, 123, 0},
	{131, 124, 0},
	{130, 125, 0},
	{129, 126, 0},
	{128, 127, 0},
	{127, 128, 0},
	{126, 129, 0},
	{125, 130, 0},
	{124, 131, 0},
	{123, 132, 0},
	{122, 133, 0},
	{121, 134, 0},
	{120, 135, 0},
	{119, 136, 0},
	{118, 137, 0},
	{117, 138, 0},
	{116, 139, 0},
	{115, 140, 0},
	{114, 141, 0},
	{113, 142, 0},
	{112, 143, 0},
	{111, 144, 0},
	{110, 145, 0},
	{109, 146, 0},
	{108, 147, 0},
	{107, 148, 0},
	{106, 149, 0},
	{105, 150, 0},
	{104, 151, 0},
	{103, 152, 0},
	{102, 153, 0},
	{101, 154, 0},
	{100, 155, 0},
	{99, 156, 0},
	{98, 157, 0},
	{97, 158, 0},
	{96, 159, 0},
	{95, 160, 0},
	{94, 161, 0},
	{93, 162, 0},
	{92, 163, 0},
	{91, 164, 0},
	{90, 165, 0},
	{89, 166, 0},
	{88, 167, 0},
	{87, 168, 0},
	{86, 169, 0},
	{85, 170, 0},
	{84, 171, 0},
	{83, 172, 0},
	{82, 173, 0},
	{81, 174, 0},
	{80, 175, 0},
	{79, 176, 0},
	{78, 177, 0},
	{77, 178, 0},
	{76, 179, 0},
	{75, 180, 0},
	{74, 181, 0},
	{73, 182, 0},
	{72, 183, 0},
	{71, 184, 0},
	{70, 185, 0},
	{69, 186, 0},
	{68, 187, 0},
	{67, 188, 0},
	{66, 189, 0},
	{65, 190, 0},
	{64, 191, 0},
	{63, 192, 0},
	{62, 193, 0},
	{61, 194, 0},
	{60, 195, 0},
	{59, 196, 0},
	{58, 197, 0},
	{57, 198, 0},
	{56, 199, 0},
	{55, 200, 0},
	{54, 201, 0},
	{53, 202, 0},
	{52, 203, 0},
	{51, 204, 0},
	{50, 205, 0},
	{49, 206, 0},
	{48, 207, 0},
	{47, 208, 0},
	{46, 209, 0},
	{45, 210, 0},
	{44, 211, 0},
	{43, 212, 0},
	{42, 213, 0},
	{41, 214, 0},
	{40, 215, 0},
	{39, 216, 0},
	{38, 217, 0},
	{37, 218, 0},
	{36, 219, 0},
	{35, 220, 0},
	{34, 221, 0},
	{33, 222, 0},
	{32, 223, 0},
	{31, 224, 0},
	{30, 225, 0},
	{29, 226, 0},
	{28, 227, 0},
	{27, 228, 0},
	{26, 229, 0},
	{25, 230, 0},
	{24, 231, 0},
	{23, 232, 0},
	{22, 233, 0},
	{21, 234, 0},
	{20, 235, 0},
	{19, 236, 0},
	{18, 237, 0},
	{17, 238, 0},
	{16, 239, 0},
	{15, 240, 0},
	{14, 241, 0},
	{13, 242, 0},
	{12, 243, 0},
	{11, 244, 0},
	{10, 245, 0},
	{9, 246, 0},
	{8, 247, 0},
	{7, 248, 0},
	{6, 249, 0},
	{5, 250, 0},
	{4, 251, 0},
	{3, 252, 0},
	{2, 253, 0},
	{1, 254, 0},
	{0, 255, 0},
	{0, 254, 1},
	{0, 253, 2},
	{0, 252, 3},
	{0, 251, 4},
	{0, 250, 5},
	{0, 249, 6},
	{0, 248, 7},
	{0, 247, 8},
	{0, 246, 9},
	{0, 245, 10},
	{0, 244, 11},
	{0, 243, 12},
	{0, 242, 13},
	{0, 241, 14},
	{0, 240, 15},
	{0, 239, 16},
	{0, 238, 17},
	{0, 237, 18},
	{0, 236, 19},
	{0, 235, 20},
	{0, 234, 21},
	{0, 233, 22},
	{0, 232, 23},
	{0, 231, 24},
	{0, 230, 25},
	{0, 229, 26},
	{0, 228, 27},
	{0, 227, 28},
	{0, 226, 29},
	{0, 225, 30},
	{0, 224, 31},
	{0, 223, 32},
	{0, 222, 33},
	{0, 221, 34},
	{0, 220, 35},
	{0, 219, 36},
	{0, 218, 37},
	{0, 217, 38},
	{0, 216, 39},
	{0, 215, 40},
	{0, 214, 41},
	{0, 213, 42},
	{0, 212, 43},
	{0, 211, 44},
	{0, 210, 45},
	{0, 209, 46},
	{0, 208, 47},
	{0, 207, 48},
	{0, 206, 49},
	{0, 205, 50},
	{0, 204, 51},
	{0, 203, 52},
	{0, 202, 53},
	{0, 201, 54},
	{0, 200, 55},
	{0, 199, 56},
	{0, 198, 57},
	{0, 197, 58},
	{0, 196, 59},
	{0, 195, 60},
	{0, 194, 61},
	{0, 193, 62},
	{0, 192, 63},
	{0, 191, 64},
	{0, 190, 65},
	{0, 189, 66},
	{0, 188, 67},
	{0, 187, 68},
	{0, 186, 69},
	{0, 185, 70},
	{0, 184, 71},
	{0, 183, 72},
	{0, 182, 73},
	{0, 181, 74},
	{0, 180, 75},
	{0, 179, 76},
	{0, 178, 77},
	{0, 177, 78},
	{0, 176, 79},
	{0, 175, 80},
	{0, 174, 81},
	{0, 173, 82},
	{0, 172, 83},
	{0, 171, 84},
	{0, 170, 85},
	{0, 169, 86},
	{0, 168, 87},
	{0, 167, 88},
	{0, 166, 89},
	{0, 165, 90},
	{0, 164, 91},
	{0, 163, 92},
	{0, 162, 93},
	{0, 161, 94},
	{0, 160, 95},
	{0, 159, 96},
	{0, 158, 97},
	{0, 157, 98},
	{0, 156, 99},
	{0, 155, 100},
	{0, 154, 101},
	{0, 153, 102},
	{0, 152, 103},
	{0, 151, 104},
	{0, 150, 105},
	{0, 149, 106},
	{0, 148, 107},
	{0, 147, 108},
	{0, 146, 109},
	{0, 145, 110},
	{0, 144, 111},
	{0, 143, 112},
	{0, 142, 113},
	{0, 141, 114},
	{0, 140, 115},
	{0, 139, 116},
	{0, 138, 117},
	{0, 137, 118},
	{0, 136, 119},
	{0, 135, 120},
	{0, 134, 121},
	{0, 133, 122},
	{0, 132, 123},
	{0, 131, 124},
	{0, 130, 125},
	{0, 129, 126},
	{0, 128, 127},
	{0, 127, 128},
	{0, 126, 129},
	{0, 125, 130},
	{0, 124, 131},
	{0, 123, 132},
	{0, 122, 133},
	{0, 121, 134},
	{0, 120, 135},
	{0, 119, 136},
	{0, 118, 137},
	{0, 117, 138},
	{0, 116, 139},
	{0, 115, 140},
	{0, 114, 141},
	{0, 113, 142},
	{0, 112, 143},
	{0, 111, 144},
	{0, 110, 145},
	{0, 109, 146},
	{0, 108, 147},
	{0, 107, 148},
	{0, 106, 149},
	{0, 105, 150},
	{0, 104, 151},
	{0, 103, 152},
	{0, 102, 153},
	{0, 101, 154},
	{0, 100, 155},
	{0, 99, 156},
	{0, 98, 157},
	{0, 97, 158},
	{0, 96, 159},
	{0, 95, 160},
	{0, 94, 161},
	{0, 93, 162},
	{0, 92, 163},
	{0, 91, 164},
	{0, 90, 165},
	{0, 89, 166},
	{0, 88, 167},
	{0, 87, 168},
	{0, 86, 169},
	{0, 85, 170},
	{0, 84, 171},
	{0, 83, 172},
	{0, 82, 173},
	{0, 81, 174},
	{0, 80, 175},
	{0, 79, 176},
	{0, 78, 177},
	{0, 77, 178},
	{0, 76, 179},
	{0, 75, 180},
	{0, 74, 181},
	{0, 73, 182},
	{0, 72, 183},
	{0, 71, 184},
	{0, 70, 185},
	{0, 69, 186},
	{0, 68, 187},
	{0, 67, 188},
	{0, 66, 189},
	{0, 65, 190},
	{0, 64, 191},
	{0, 63, 192},
	{0, 62, 193},
	{0, 61, 194},
	{0, 60, 195},
	{0, 59, 196},
	{0, 58, 197},
	{0, 57, 198},
	{0, 56, 199},
	{0, 55, 200},
	{0, 54, 201},
	{0, 53, 202},
	{0, 52, 203},
	{0, 51, 204},
	{0, 50, 205},
	{0, 49, 206},
	{0, 48, 207},
	{0, 47, 208},
	{0, 46, 209},
	{0, 45, 210},
	{0, 44, 211},
	{0, 43, 212},
	{0, 42, 213},
	{0, 41, 214},
	{0, 40, 215},
	{0, 39, 216},
	{0, 38, 217},
	{0, 37, 218},
	{0, 36, 219},
	{0, 35, 220},
	{0, 34, 221},
	{0, 33, 222},
	{0, 32, 223},
	{0, 31, 224},
	{0, 30, 225},
	{0, 29, 226},
	{0, 28, 227},
	{0, 27, 228},
	{0, 26, 229},
	{0, 25, 230},
	{0, 24, 231},
	{0, 23, 232},
	{0, 22, 233},
	{0, 21, 234},
	{0, 20, 235},
	{0, 19, 236},
	{0, 18, 237},
	{0, 17, 238},
	{0, 16, 239},
	{0, 15, 240},
	{0, 14, 241},
	{0, 13, 242},
	{0, 12, 243},
	{0, 11, 244},
	{0, 10, 245},
	{0, 9, 246},
	{0, 8, 247},
	{0, 7, 248},
	{0, 6, 249},
	{0, 5, 250},
	{0, 4, 251},
	{0, 3, 252},
	{0, 2, 253},
	{0, 1, 254},
	{0, 0, 255},
	{1, 0, 254},
	{2, 0, 253},
	{3, 0, 252},
	{4, 0, 251},
	{5, 0, 250},
	{6, 0, 249},
	{7, 0, 248},
	{8, 0, 247},
	{9, 0, 246},
	{10, 0, 245},
	{11, 0, 244},
	{12, 0, 243},
	{13, 0, 242},
	{14, 0, 241},
	{15, 0, 240},
	{16, 0, 239},
	{17, 0, 238},
	{18, 0, 237},
	{19, 0, 236},
	{20, 0, 235},
	{21, 0, 234},
	{22, 0, 233},
	{23, 0, 232},
	{24, 0, 231},
	{25, 0, 230},
	{26, 0, 229},
	{27, 0, 228},
	{28, 0, 227},
	{29, 0, 226},
	{30, 0, 225},
	{31, 0, 224},
	{32, 0, 223},
	{33, 0, 222},
	{34, 0, 221},
	{35, 0, 220},
	{36, 0, 219},
	{37, 0, 218},
	{38, 0, 217},
	{39, 0, 216},
	{40, 0, 215},
	{41, 0, 214},
	{42, 0, 213},
	{43, 0, 212},
	{44, 0, 211},
	{45, 0, 210},
	{46, 0, 209},
	{47, 0, 208},
	{48, 0, 207},
	{49, 0, 206},
	{50, 0, 205},
	{51, 0, 204},
	{52, 0, 203},
	{53, 0, 202},
	{54, 0, 201},
	{55, 0, 200},
	{56, 0, 199},
	{57, 0, 198},
	{58, 0, 197},
	{59, 0, 196},
	{60, 0, 195},
	{61, 0, 194},
	{62, 0, 193},
	{63, 0, 192},
	{64, 0, 191},
	{65, 0, 190},
	{66, 0, 189},
	{67, 0, 188},
	{68, 0, 187},
	{69, 0, 186},
	{70, 0, 185},
	{71, 0, 184},
	{72, 0, 183},
	{73, 0, 182},
	{74, 0, 181},
	{75, 0, 180},
	{76, 0, 179},
	{77, 0, 178},
	{78, 0, 177},
	{79, 0, 176},
	{80, 0, 175},
	{81, 0, 174},
	{82, 0, 173},
	{83, 0, 172},
	{84, 0, 171},
	{85, 0, 170},
	{86, 0, 169},
	{87, 0, 168},
	{88, 0, 167},
	{89, 0, 166},
	{90, 0, 165},
	{91, 0, 164},
	{92, 0, 163},
	{93, 0, 162},
	{94, 0, 161},
	{95, 0, 160},
	{96, 0, 159},
	{97, 0, 158},
	{98, 0, 157},
	{99, 0, 156},
	{100, 0, 155},
	{101, 0, 154},
	{102, 0, 153},
	{103, 0, 152},
	{104, 0, 151},
	{105, 0, 150},
	{106, 0, 149},
	{107, 0, 148},
	{108, 0, 147},
	{109, 0, 146},
	{110, 0, 145},
	{111, 0, 144},
	{112, 0, 143},
	{113, 0, 142},
	{114, 0, 141},
	{115, 0, 140},
	{116, 0, 139},
	{117, 0, 138},
	{118, 0, 137},
	{119, 0, 136},
	{120, 0, 135},
	{121, 0, 134},
	{122, 0, 133},
	{123, 0, 132},
	{124, 0, 131},
	{125, 0, 130},
	{126, 0, 129},
	{127, 0, 128},
	{128, 0, 127},
	{129, 0, 126},
	{130, 0, 125},
	{131, 0, 124},
	{132, 0, 123},
	{133, 0, 122},
	{134, 0, 121},
	{135, 0, 120},
	{136, 0, 119},
	{137, 0, 118},
	{138, 0, 117},
	{139, 0, 116},
	{140, 0, 115},
	{141, 0, 114},
	{142, 0, 113},
	{143, 0, 112},
	{144, 0, 111},
	{145, 0, 110},
	{146, 0, 109},
	{147, 0, 108},
	{148, 0, 107},
	{149, 0, 106},
	{150, 0, 105},
	{151, 0, 104},
	{152, 0, 103},
	{153, 0, 102},
	{154, 0, 101},
	{155, 0, 100},
	{156, 0, 99},
	{157, 0, 98},
	{158, 0, 97},
	{159, 0, 96},
	{160, 0, 95},
	{161, 0, 94},
	{162, 0, 93},
	{163, 0, 92},
	{164, 0, 91},
	{165, 0, 90},
	{166, 0, 89},
	{167, 0, 88},
	{168, 0, 87},
	{169, 0, 86},
	{170, 0, 85},
	{171, 0, 84},
	{172, 0, 83},
	{173, 0, 82},
	{174, 0, 81},
	{175, 0, 80},
	{176, 0, 79},
	{177, 0, 78},
	{178, 0, 77},
	{179, 0, 76},
	{180, 0, 75},
	{181, 0, 74},
	{182, 0, 73},
	{183, 0, 72},
	{184, 0, 71},
	{185, 0, 70},
	{186, 0, 69},
	{187, 0, 68},
	{188, 0, 67},
	{189, 0, 66},
	{190, 0, 65},
	{191, 0, 64},
	{192, 0, 63},
	{193, 0, 62},
	{194, 0, 61},
	{195, 0, 60},
	{196, 0, 59},
	{197, 0, 58},
	{198, 0, 57},
	{199, 0, 56},
	{200, 0, 55},
	{201, 0, 54},
	{202, 0, 53},
	{203, 0, 52},
	{204, 0, 51},
	{205, 0, 50},
	{206, 0, 49},
	{207, 0, 48},
	{208, 0, 47},
	{209, 0, 46},
	{210, 0, 45},
	{211, 0, 44},
	{212, 0, 43},
	{213, 0, 42},
	{214, 0, 41},
	{215, 0, 40},
	{216, 0, 39},
	{217, 0, 38},
	{218, 0, 37},
	{219, 0, 36},
	{220, 0, 35},
	{221, 0, 34},
	{222, 0, 33},
	{223, 0, 32},
	{224, 0, 31},
	{225, 0, 30},
	{226, 0, 29},
	{227, 0, 28},
	{228, 0, 27},
	{229, 0, 26},
	{230, 0, 25},
	{231, 0, 24},
	{232, 0, 23},
	{233, 0, 22},
	{234, 0, 21},
	{235, 0, 20},
	{236, 0, 19},
	{237, 0, 18},
	{238, 0, 17},
	{239, 0, 16},
	{240, 0, 15},
	{241, 0, 14},
	{242, 0, 13},
	{243, 0, 12},
	{244, 0, 11},
	{245, 0, 10},
	{246, 0, 9},
	{247, 0, 8},
	{248, 0, 7},
	{249, 0, 6},
	{250, 0, 5},
	{251, 0, 4},
	{252, 0, 3},
	{253, 0, 2},
	{254, 0, 1},
	{255, 0, 0},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

void WS2812_send(uint8_t (*color)[3], uint16_t len)
{
	uint8_t i,j;
	uint8_t led;
	uint16_t memaddr;
	uint16_t buffersize;

	buffersize = (len*24)+42;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index
	led = 0;					// reset led index

	// fill transmit buffer with correct compare values to achieve
	// correct pulse widths according to color values
	while (len)
	{
		for (j = 0; j < 8; j++)					// GREEN data
		{
			if ( (color[led][1]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = 17; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = 9;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// RED data
		{
			if ( (color[led][0]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = 17; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = 9;	// compare value for logical 0
			}
			memaddr++;
		}

		for (j = 0; j < 8; j++)					// BLUE data
		{
			if ( (color[led][2]<<j) & 0x80 )	// data sent MSB first, j = 0 is MSB j = 7 is LSB
			{
				LED_BYTE_Buffer[memaddr] = 17; 	// compare value for logical 1
			}
			else
			{
				LED_BYTE_Buffer[memaddr] = 9;	// compare value for logical 0
			}
			memaddr++;
		}

		led++;
		len--;
	}

	// add needed delay at end of byte cycle, pulsewidth = 0
	while(memaddr < buffersize)
	{
		LED_BYTE_Buffer[memaddr] = 0;
		memaddr++;
	}

	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, buffersize);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6); // enable DMA channel 6
	LL_TIM_EnableCounter(TIM3);						// enable Timer 3
	while(!LL_DMA_IsActiveFlag_TC6(DMA1)); 	// wait until transfer complete
	LL_TIM_DisableCounter(TIM3);					// disable Timer 3
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);	// disable DMA channel 6
	LL_DMA_ClearFlag_TC6(DMA1);			// clear DMA1 Channel 6 transfer complete flag
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled 
  */
  LL_GPIO_AF_DisableRemap_SWJ();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int16_t i;
  while (1)
  {
	  for (i = 0; i < 766; i += 1)
	  	{
			WS2812_send(&eightbit[i], 4);
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
			Delay(50000); // 1 = 0.125 us -> 8 MHz 		50000 -> 6.25ms
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
			Delay(1);
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
	  	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
    Error_Handler();
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(24000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(24000000);
}
/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */

LL_TIM_InitTypeDef TIM_InitStruct = {0};
LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
LL_DMA_InitTypeDef DMA_InitStruct = {0};

static void MX_TIM3_Init(void)
{
  uint16_t PrescalerValue;

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration  
  PA6   ------> TIM3_CH1 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;

  /* Time base configuration */
  TIM_InitStruct.Autoreload = 29; //~805 khz (72/2/29 -> 1.25 us)
  TIM_InitStruct.Prescaler = PrescalerValue;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  LL_TIM_Init(TIM3, &TIM_InitStruct);

  //LL_TIM_DisableARRPreload(TIM3);
  //LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);

  //   LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH1);
  //   LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  //   LL_TIM_DisableMasterSlaveMode(TIM3);


    /* Init with LL driver */
  /* DMA controller clock enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* TIM3 DMA Init */
  	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_6);

  	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)TIM3_CCR1_Address;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)LED_BYTE_Buffer;
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.NbData = 42;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;

	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_6,&DMA_InitStruct);

	/* TIM3 CC1 DMA Request enable */
	//TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);
	LL_TIM_EnableDMAReq_CC1(TIM3);
}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{


  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
