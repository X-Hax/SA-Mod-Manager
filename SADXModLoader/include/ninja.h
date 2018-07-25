/*
	Ninja Library
	
	COPYRIGHT (C) SEGA ENTERPRISES,LTD.
*/

#ifndef SEGA_NINJA_H
#define SEGA_NINJA_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

/* Chopped up by MainMemory for your convenience. */
#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef uint8_t     Uint8;        /*  unsigned 1 byte integer     */
#endif
#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef int8_t     Sint8;          /*  signed 1 byte integer       */
#endif
#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef uint16_t  Uint16;         /*  unsigned 2 byte integer     */
#endif
#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef int16_t    Sint16;         /*  signed 2 byte integer       */
#endif
#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef uint32_t   Uint32;         /*  unsigned 4 byte integer     */
#endif
#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef int32_t     Sint32;         /*  signed 4 byte integer       */
#endif
#ifndef _TYPEDEF_Float32
#define _TYPEDEF_Float32
typedef float           Float32;        /*  4 byte real number          */
#endif
#ifndef _TYPEDEF_Float64
#define _TYPEDEF_Float64
typedef double          Float64;        /*  8 byte real number          */
#endif
#ifndef _TYPEDEF_Float
#define _TYPEDEF_Float
typedef float           Float;          /*  4 byte real number          */
#endif
#ifndef _TYPEDEF_Double
#define _TYPEDEF_Double
typedef double           Double;        /*  8 byte real number          */
#endif
#ifndef _TYPEDEF_Void
#define _TYPEDEF_Void
typedef void            Void;           /*  void                        */
#endif
#ifndef _TYPEDEF_Bool
#define _TYPEDEF_Bool
typedef Sint32 Bool ;					/*  Bool                        */
#endif


#ifndef _NINJA_DEF_H_
#define _NINJA_DEF_H_
/*--------------------------------------*/
/*      BIT                             */
/*--------------------------------------*/
enum {
	_BIT0,  _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5,  _BIT6,  _BIT7,
	_BIT8,  _BIT9,  _BIT10, _BIT11, _BIT12, _BIT13, _BIT14, _BIT15,
	_BIT16, _BIT17, _BIT18, _BIT19, _BIT20, _BIT21, _BIT22, _BIT23,
	_BIT24, _BIT25, _BIT26, _BIT27, _BIT28, _BIT29, _BIT30, _BIT31
};

#define BIT_0   (1<<_BIT0)
#define BIT_1   (1<<_BIT1)
#define BIT_2   (1<<_BIT2)
#define BIT_3   (1<<_BIT3)
#define BIT_4   (1<<_BIT4)
#define BIT_5   (1<<_BIT5)
#define BIT_6   (1<<_BIT6)
#define BIT_7   (1<<_BIT7)
#define BIT_8   (1<<_BIT8)
#define BIT_9   (1<<_BIT9)
#define BIT_10  (1<<_BIT10)
#define BIT_11  (1<<_BIT11)
#define BIT_12  (1<<_BIT12)
#define BIT_13  (1<<_BIT13)
#define BIT_14  (1<<_BIT14)
#define BIT_15  (1<<_BIT15)
#define BIT_16  (1<<_BIT16)
#define BIT_17  (1<<_BIT17)
#define BIT_18  (1<<_BIT18)
#define BIT_19  (1<<_BIT19)
#define BIT_20  (1<<_BIT20)
#define BIT_21  (1<<_BIT21)
#define BIT_22  (1<<_BIT22)
#define BIT_23  (1<<_BIT23)
#define BIT_24  (1<<_BIT24)
#define BIT_25  (1<<_BIT25)
#define BIT_26  (1<<_BIT26)
#define BIT_27  (1<<_BIT27)
#define BIT_28  (1<<_BIT28)
#define BIT_29  (1<<_BIT29)
#define BIT_30  (1<<_BIT30)
#define BIT_31  (1<<_BIT31)


/*--------------------------------------*/
/*      Basic Define                    */
/*--------------------------------------*/
typedef int     Int;

typedef Sint32  Angle;

typedef Float   NJS_MATRIX[16];

typedef Float NJS_FOG_TABLE[128];

/*--------------------------------------*/
/*      SCREEN RESOLUTION               */
/*--------------------------------------*/
#define NJD_DSP_NTSC		(0x0)
#define NJD_DSP_VGA			(0x1)
#define NJD_DSP_PAL			(0x2)

#define NJD_DSP_NONINTER 	(0x00)
#define NJD_DSP_INTER    	(0x04)
#define NJD_DSP_PSINTER  	(0x08)

#define NJD_DSP_WIDTH_320	(0x000)
#define NJD_DSP_WIDTH_640	(0x010)

#define NJD_DSP_HEIGHT_240	(0x000)
#define NJD_DSP_HEIGHT_480	(0x020)
#define NJD_DSP_HEIGHT_NRM	(0x000)
#define NJD_DSP_HEIGHT_EXT	(0x040)


#define NJD_DSP_ANTI							(0x00010000)	
#define NJD_PALEXT_HEIGHT_RATIO_1_033			(0x00000000)
#define NJD_PALEXT_HEIGHT_RATIO_1_066			(0x00020000)
#define NJD_PALEXT_HEIGHT_RATIO_1_100			(0x00040000)
#define NJD_PALEXT_HEIGHT_RATIO_1_133			(0x00060000)
#define NJD_PALEXT_HEIGHT_RATIO_1_166			(0x00080000)

#define NJD_DSP_320x240		(NJD_DSP_WIDTH_320|NJD_DSP_HEIGHT_240)
#define NJD_DSP_640x240		(NJD_DSP_WIDTH_640|NJD_DSP_HEIGHT_240)
#define NJD_DSP_320x480		(NJD_DSP_WIDTH_320|NJD_DSP_HEIGHT_480)
#define NJD_DSP_640x480		(NJD_DSP_WIDTH_640|NJD_DSP_HEIGHT_480)
#define NJD_DSP_640x240_EXT	(NJD_DSP_WIDTH_640 | NJD_DSP_HEIGHT_240 | NJD_DSP_HEIGHT_EXT)
#define NJD_DSP_640x480_EXT	(NJD_DSP_WIDTH_640 | NJD_DSP_HEIGHT_480 | NJD_DSP_HEIGHT_EXT)

#define	NJD_RESOLUTION_VGA					(NJD_DSP_VGA|NJD_DSP_NONINTER|NJD_DSP_640x480)
#define	NJD_RESOLUTION_320x240_NTSCNI		(NJD_DSP_NTSC|NJD_DSP_NONINTER|NJD_DSP_320x240)
#define	NJD_RESOLUTION_320x240_NTSCI		(NJD_DSP_NTSC|NJD_DSP_INTER   |NJD_DSP_320x240)
#define	NJD_RESOLUTION_640x240_NTSCNI		(NJD_DSP_NTSC|NJD_DSP_NONINTER|NJD_DSP_640x240)
#define	NJD_RESOLUTION_640x240_NTSCI		(NJD_DSP_NTSC|NJD_DSP_INTER   |NJD_DSP_640x240)
#define	NJD_RESOLUTION_320x480_NTSCNI_FF 	(NJD_DSP_NTSC|NJD_DSP_NONINTER|NJD_DSP_320x480)
#define	NJD_RESOLUTION_320x480_NTSCNI		(NJD_DSP_NTSC|NJD_DSP_PSINTER |NJD_DSP_320x480)
#define	NJD_RESOLUTION_320x480_NTSCI		(NJD_DSP_NTSC|NJD_DSP_INTER   |NJD_DSP_320x480)
#define	NJD_RESOLUTION_640x480_NTSCNI_FF 	(NJD_DSP_NTSC|NJD_DSP_NONINTER|NJD_DSP_640x480)
#define NJD_RESOLUTION_640x480_NTSCNI	 	(NJD_DSP_NTSC|NJD_DSP_PSINTER |NJD_DSP_640x480)
#define	NJD_RESOLUTION_640x480_NTSCI		(NJD_DSP_NTSC|NJD_DSP_INTER   |NJD_DSP_640x480)
#define	NJD_RESOLUTION_320x240_PALNI		(NJD_DSP_PAL |NJD_DSP_NONINTER|NJD_DSP_320x240)
#define	NJD_RESOLUTION_320x240_PALI			(NJD_DSP_PAL |NJD_DSP_INTER   |NJD_DSP_320x240)
#define	NJD_RESOLUTION_640x240_PALNI		(NJD_DSP_PAL |NJD_DSP_NONINTER|NJD_DSP_640x240)
#define	NJD_RESOLUTION_640x240_PALI			(NJD_DSP_PAL |NJD_DSP_INTER   |NJD_DSP_640x240)
#define	NJD_RESOLUTION_320x480_PALNI_FF		(NJD_DSP_PAL |NJD_DSP_NONINTER|NJD_DSP_320x480)
#define	NJD_RESOLUTION_320x480_PALNI		(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_320x480)
#define	NJD_RESOLUTION_320x480_PALI			(NJD_DSP_PAL |NJD_DSP_INTER   |NJD_DSP_320x480)
#define	NJD_RESOLUTION_640x480_PALNI_FF		(NJD_DSP_PAL |NJD_DSP_NONINTER|NJD_DSP_640x480)
#define	NJD_RESOLUTION_640x480_PALNI		(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480)
#define	NJD_RESOLUTION_640x480_PALI			(NJD_DSP_PAL |NJD_DSP_INTER   |NJD_DSP_640x480)

#define	NJD_RESOLUTION_VGA_ANTI					(NJD_RESOLUTION_VGA				 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x240_NTSCNI_ANTI		(NJD_RESOLUTION_320x240_NTSCNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x240_NTSCI_ANTI		(NJD_RESOLUTION_320x240_NTSCI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x240_NTSCNI_ANTI		(NJD_RESOLUTION_640x240_NTSCNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x240_NTSCI_ANTI		(NJD_RESOLUTION_640x240_NTSCI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_NTSCNI_FF_ANTI 	(NJD_RESOLUTION_320x480_NTSCNI_FF|NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_NTSCNI_ANTI		(NJD_RESOLUTION_320x480_NTSCNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_NTSCI_ANTI		(NJD_RESOLUTION_320x480_NTSCI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x480_NTSCNI_FF_ANTI 	(NJD_RESOLUTION_640x480_NTSCNI_FF|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x480_NTSCNI_ANTI	 	(NJD_RESOLUTION_640x480_NTSCNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x480_NTSCI_ANTI		(NJD_RESOLUTION_640x480_NTSCI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x240_PALNI_ANTI		(NJD_RESOLUTION_320x240_PALNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x240_PALI_ANTI		(NJD_RESOLUTION_320x240_PALI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x240_PALNI_ANTI		(NJD_RESOLUTION_640x240_PALNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x240_PALI_ANTI		(NJD_RESOLUTION_640x240_PALI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_PALNI_FF_ANTI	(NJD_RESOLUTION_320x480_PALNI_FF |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_PALNI_ANTI		(NJD_RESOLUTION_320x480_PALNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_320x480_PALI_ANTI		(NJD_RESOLUTION_320x480_PALI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x480_PALNI_FF_ANTI	(NJD_RESOLUTION_640x480_PALNI_FF |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x480_PALNI_ANTI		(NJD_RESOLUTION_640x480_PALNI	 |NJD_DSP_ANTI)
#define	NJD_RESOLUTION_640x480_PALI_ANTI		(NJD_RESOLUTION_640x480_PALI	 |NJD_DSP_ANTI)

#define NJD_RESOLUTION_320x240_VGA       	(NJD_DSP_VGA |NJD_DSP_NONINTER|NJD_DSP_320x240)
#define NJD_RESOLUTION_320x480_VGA       	(NJD_DSP_VGA |NJD_DSP_NONINTER|NJD_DSP_320x480)
#define NJD_RESOLUTION_640x240_VGA       	(NJD_DSP_VGA |NJD_DSP_NONINTER|NJD_DSP_640x240)
#define	NJD_RESOLUTION_640x480_VGA			(NJD_DSP_VGA|NJD_DSP_NONINTER|NJD_DSP_640x480)
#define NJD_RESOLUTION_320x240_VGA_ANTI       	(NJD_RESOLUTION_320x240_VGA		 |NJD_DSP_ANTI)
#define NJD_RESOLUTION_320x480_VGA_ANTI       	(NJD_RESOLUTION_320x480_VGA		 |NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x240_VGA_ANTI       	(NJD_RESOLUTION_640x240_VGA		 |NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x480_VGA_ANTI       	(NJD_RESOLUTION_640x480_VGA		 |NJD_DSP_ANTI)

#define NJD_RESOLUTION_640x248_PALNI	(NJD_DSP_PAL |NJD_DSP_NONINTER |NJD_DSP_640x240_EXT|NJD_PALEXT_HEIGHT_RATIO_1_033)
#define NJD_RESOLUTION_640x256_PALNI	(NJD_DSP_PAL |NJD_DSP_NONINTER |NJD_DSP_640x240_EXT|NJD_PALEXT_HEIGHT_RATIO_1_066)
#define NJD_RESOLUTION_640x264_PALNI	(NJD_DSP_PAL |NJD_DSP_NONINTER |NJD_DSP_640x240_EXT|NJD_PALEXT_HEIGHT_RATIO_1_100)
#define NJD_RESOLUTION_640x272_PALNI	(NJD_DSP_PAL |NJD_DSP_NONINTER |NJD_DSP_640x240_EXT|NJD_PALEXT_HEIGHT_RATIO_1_133)
#define NJD_RESOLUTION_640x280_PALNI	(NJD_DSP_PAL |NJD_DSP_NONINTER |NJD_DSP_640x240_EXT|NJD_PALEXT_HEIGHT_RATIO_1_166)

#define NJD_RESOLUTION_640x496_PALNI	(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_033)
#define NJD_RESOLUTION_640x512_PALNI	(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_066)
#define NJD_RESOLUTION_640x528_PALNI	(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_100)
#define NJD_RESOLUTION_640x544_PALNI	(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_133)
#define NJD_RESOLUTION_640x560_PALNI	(NJD_DSP_PAL |NJD_DSP_PSINTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_166)

#define NJD_RESOLUTION_640x496_PALI	(NJD_DSP_PAL |NJD_DSP_INTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_033)
#define NJD_RESOLUTION_640x512_PALI	(NJD_DSP_PAL |NJD_DSP_INTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_066)
#define NJD_RESOLUTION_640x528_PALI	(NJD_DSP_PAL |NJD_DSP_INTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_100)
#define NJD_RESOLUTION_640x544_PALI	(NJD_DSP_PAL |NJD_DSP_INTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_133)
#define NJD_RESOLUTION_640x560_PALI	(NJD_DSP_PAL |NJD_DSP_INTER |NJD_DSP_640x480_EXT|NJD_PALEXT_HEIGHT_RATIO_1_166)

#define NJD_RESOLUTION_640x248_PALNI_ANTI	(NJD_RESOLUTION_640x248_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x256_PALNI_ANTI	(NJD_RESOLUTION_640x256_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x264_PALNI_ANTI	(NJD_RESOLUTION_640x264_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x272_PALNI_ANTI	(NJD_RESOLUTION_640x272_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x280_PALNI_ANTI	(NJD_RESOLUTION_640x280_PALNI|NJD_DSP_ANTI)

#define NJD_RESOLUTION_640x496_PALNI_ANTI	(NJD_RESOLUTION_640x496_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x512_PALNI_ANTI	(NJD_RESOLUTION_640x512_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x528_PALNI_ANTI	(NJD_RESOLUTION_640x528_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x544_PALNI_ANTI	(NJD_RESOLUTION_640x544_PALNI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x560_PALNI_ANTI	(NJD_RESOLUTION_640x560_PALNI|NJD_DSP_ANTI)

#define NJD_RESOLUTION_640x496_PALI_ANTI	(NJD_RESOLUTION_640x496_PALI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x512_PALI_ANTI	(NJD_RESOLUTION_640x512_PALI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x528_PALI_ANTI	(NJD_RESOLUTION_640x528_PALI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x544_PALI_ANTI	(NJD_RESOLUTION_640x544_PALI|NJD_DSP_ANTI)
#define NJD_RESOLUTION_640x560_PALI_ANTI	(NJD_RESOLUTION_640x560_PALI|NJD_DSP_ANTI)

#define NJD_VIDEO_MASK	(0x00000003)
#define NJD_DSP_MASK	(0x00000FFF)
#define NJD_WIDTH_MASK 	(NJD_DSP_WIDTH_640)
#define NJD_HEIGHT_MASK	(NJD_DSP_HEIGHT_480)
#define NJD_PALEXT_MASK	(0x000E0000)
#define NJD_PALEXT_OFFSET	(17)

/*--------------------------------------*/
/*      FRAME BUFFER MODE               */
/*--------------------------------------*/
#define	NJD_FRAMEBUFFER_MODE_RGB565				0
#define	NJD_FRAMEBUFFER_MODE_RGB555				1
#define	NJD_FRAMEBUFFER_MODE_ARGB1555			3
#define	NJD_FRAMEBUFFER_MODE_RGB888				4
#define	NJD_FRAMEBUFFER_MODE_ARGB8888			5

/*--------------------------------------*/
/*      COLOR MODE		               */
/*--------------------------------------*/
#define NJD_ARC1_NORMAL			(0x00000002)
#define NJD_ARC1_TEXTURE		(0x0000000A)
#define NJD_ARC1_NORMAL_FLAT	(0x00000000)
#define NJD_ARC1_TEXTURE_FLAT	(0x00000008)

#define NJD_ARC1_HIGHLIGHT		(0x00000004)
#define NJD_ARC1_OPAQUE			(0x00080000)
#define NJD_ARC1_TRANS			(0x02100000)
#define NJD_ARC1_INTENSITY		(0x00000020)
#define NJD_ARC1_PACKED			(0x00000000)
#define NJD_ARC1_FLOAT			(0x00000010)

#define NJD_ARC1_UV32_FLOAT		(0x00000000)
#define NJD_ARC1_UV16_FLOAT		(0x00000001)

#define	NJD_COLOR_MODE_NORMAL						(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_TEXTURE						(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_FLAT_NORMAL					(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_FLAT_TEXTURE					(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_NORMAL				(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_TEXTURE			(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_NORMAL		(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_TEXTURE		(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_NORMAL_TRANS					(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_TEXTURE_TRANS				(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_FLAT_NORMAL_TRANS			(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_FLAT_TEXTURE_TRANS			(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_NORMAL_TRANS		(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_TEXTURE_TRANS		(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_NORMAL_TRANS	(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_TEXTURE_TRANS	(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT)

#define	NJD_COLOR_MODE_TEXTURE_UV16							(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_FLAT_TEXTURE_UV16					(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_TEXTURE_UV16				(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_TEXTURE_UV16			(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_TEXTURE_TRANS_UV16					(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_FLAT_TEXTURE_TRANS_UV16				(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_TEXTURE_TRANS_UV16			(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT|NJD_ARC1_UV16_FLOAT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_TEXTURE_TRANS_UV16	(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_FLOAT|NJD_ARC1_HIGHLIGHT|NJD_ARC1_UV16_FLOAT)


#define	NJD_COLOR_MODE_INTENSITY_NORMAL							(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_INTENSITY_TEXTURE						(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_FLAT_INTENSITY_NORMAL					(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_FLAT_INTENSITY_TEXTURE					(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_INTENSITY_NORMAL				(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_INTENSITY_TEXTURE				(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_INTENSITY_NORMAL			(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_INTENSITY_TEXTURE			(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_INTENSITY|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_INTENSITY_NORMAL_TRANS					(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_INTENSITY_TEXTURE_TRANS					(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_FLAT_INTENSITY_NORMAL_TRANS				(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_FLAT_INTENSITY_TEXTURE_TRANS				(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_INTENSITY_NORMAL_TRANS			(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_INTENSITY_TEXTURE_TRANS		(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_INTENSITY_NORMAL_TRANS	(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_INTENSITY_TEXTURE_TRANS	(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_INTENSITY|NJD_ARC1_HIGHLIGHT)

#define	NJD_COLOR_MODE_PACKED_NORMAL						(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_PACKED_TEXTURE						(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_FLAT_PACKED_NORMAL					(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_FLAT_PACKED_TEXTURE					(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_PACKED_NORMAL				(NJD_ARC1_NORMAL|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_PACKED_TEXTURE				(NJD_ARC1_TEXTURE|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_PACKED_NORMAL			(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_PACKED_TEXTURE		(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_OPAQUE|NJD_ARC1_PACKED|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_PACKED_NORMAL_TRANS					(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_PACKED_TEXTURE_TRANS					(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_FLAT_PACKED_NORMAL_TRANS				(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_FLAT_PACKED_TEXTURE_TRANS			(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_PACKED_NORMAL_TRANS		(NJD_ARC1_NORMAL|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_PACKED_TEXTURE_TRANS		(NJD_ARC1_TEXTURE|NJD_ARC1_TRANS|NJD_ARC1_PACKED|NJD_ARC1_HIGHLIGHT)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_PACKED_NORMAL_TRANS	(NJD_ARC1_NORMAL_FLAT|NJD_ARC1_TRANS|NJD_ARC1_PACKED)
#define	NJD_COLOR_MODE_HIGHLIGHT_FLAT_PACKED_TEXTURE_TRANS	(NJD_ARC1_TEXTURE_FLAT|NJD_ARC1_TRANS|NJD_ARC1_PACKED|NJD_ARC1_HIGHLIGHT)

/*--------------------------------------*/
/*      Render Mode Define              */
/*--------------------------------------*/
/* Culling Mode */
#define NJD_POLYGON_NOCULLING        0
#define NJD_POLYGON_CULLINGSMALL     1
#define NJD_POLYGON_CULLINGACW       2
#define NJD_POLYGON_CULLINGCW        3

/* Color Blending Mode */
/* Target */
#define NJD_SOURCE_COLOR                     0
#define NJD_DESTINATION_COLOR                1
/* Mode */
#define NJD_COLOR_BLENDING_BOTHINVALPHA     0
#define NJD_COLOR_BLENDING_BOTHSRCALPHA     1
#define NJD_COLOR_BLENDING_DESTALPHA        2
#define NJD_COLOR_BLENDING_DESTCOLOR        3
#define NJD_COLOR_BLENDING_INVDESTALPHA     4
#define NJD_COLOR_BLENDING_INVDESTCOLOR     5
#define NJD_COLOR_BLENDING_INVSRCALPHA      6
#define NJD_COLOR_BLENDING_INVSRCCOLOR      7
#define NJD_COLOR_BLENDING_SRCALPHA         8
#define NJD_COLOR_BLENDING_SRCCOLOR         9
#define NJD_COLOR_BLENDING_ONE              10
#define NJD_COLOR_BLENDING_ZERO             11

/* Texture Shading Mode */
#define NJD_TEX_SHADING_MODE_MODULATE       0
#define NJD_TEX_SHADING_MODE_DECALALPHA     1
#define NJD_TEX_SHADING_MODE_MODULATEALPHA  2
#define NJD_TEX_SHADING_MODE_DECAL		    3

/* UserClippingMode */
#define NJD_CLIP_DISABLE					0
#define NJD_CLIP_RESERVE					1
#define NJD_CLIP_INSIDE						2
#define NJD_CLIP_OUTSIDE					3

/*--------------------------------------*/
/*      SYSTEM ATTR SIZE                */
/*--------------------------------------*/
#define NJD_SYS_ATTR_SIZE 					192

/*--------------------------------------*/
/*      FATAL ERROR CALL BACK           */
/*--------------------------------------*/
#define NJD_FATAL_ERR_NOQEUE_INTR			(1)
#define NJD_FATAL_ERR_OVERFLW_VTXBUF		(2)
#define NJD_FATAL_ERR_TOO_MANY_VTX			(3)
#define NJD_FATAL_ERR_V_NOT_RESPOND			(4)

/*--------------------------------------*/
/*      Texture Define                  */
/*--------------------------------------*/
/* Texture Size */
#define NJD_TEXSIZE_1					1
#define NJD_TEXSIZE_2					2
#define NJD_TEXSIZE_4					4
#define NJD_TEXSIZE_8					8
#define NJD_TEXSIZE_16					16
#define NJD_TEXSIZE_32					32
#define NJD_TEXSIZE_64					64
#define NJD_TEXSIZE_128					128
#define NJD_TEXSIZE_256					256
#define NJD_TEXSIZE_512					512
#define NJD_TEXSIZE_1024				1024

/* Texture Attr */
/*
|31 -  21|20 - 17|16          |15 - 0|
 texattr reserved texcontinue  pvmentryId
*/
#define NJD_TEXATTR_TYPE_FILE			0
#define NJD_TEXATTR_CACHE				BIT_31
#define NJD_TEXATTR_TYPE_MEMORY			BIT_30
#define NJD_TEXATTR_BOTH				BIT_29
#define NJD_TEXATTR_TYPE_FRAMEBUFFER	BIT_28

#define NJD_TEXATTR_READAREA_MASK		(BIT_31|BIT_29)
#define NJD_TEXATTR_READTYPE_MASK		(BIT_30|BIT_28)

#define NJD_TEXATTR_PALGLOBALINDEX		BIT_24
#define NJD_TEXATTR_GLOBALINDEX			BIT_23
#define NJD_TEXATTR_AUTOMIPMAP			BIT_22
#define	NJD_TEXATTR_AUTODITHER			BIT_21
#define NJD_TEXATTR_MASK				0xFFFE0000

#define NJD_TEXATTR_TEXCONTINUE			BIT_16


/*
	texaddr
	|31 - 26|25   -     0|
	  bank    globalIndex
*/
#define NJD_TEXBANK_SHIFT			(26)
#define NJD_TEXBANK_MASK			(0xFC000000)
#define NJD_TEXPALGLOBAL_MASK		(0x03FFFFFF)

/* Texture Color Format */
#define NJD_TEXFMT_ARGB_1555            (0x00)
#define NJD_TEXFMT_RGB_565              (0x01)
#define NJD_TEXFMT_ARGB_4444            (0x02)
#define NJD_TEXFMT_YUV_422				(0x03)
#define NJD_TEXFMT_BUMP					(0x04)
#define NJD_TEXFMT_RGB_555				(0x05)
#define NJD_TEXFMT_ARGB_8888 			(0x06)
#define NJD_TEXFMT_YUV_420	 			(0x06)
#define NJD_TEXFMT_COLOR_MASK			(0xFF)

#define NJD_TEXFMT_TWIDDLED	            (0x0100)
#define NJD_TEXFMT_TWIDDLED_MM	        (0x0200)
#define NJD_TEXFMT_VQ	                (0x0300)
#define NJD_TEXFMT_VQ_MM	            (0x0400)
#define NJD_TEXFMT_PALETTIZE4	        (0x0500)
#define NJD_TEXFMT_PALETTIZE4_MM	    (0x0600)
#define NJD_TEXFMT_PALETTIZE8	        (0x0700)
#define NJD_TEXFMT_PALETTIZE8_MM	    (0x0800)
#define NJD_TEXFMT_RECTANGLE	        (0x0900)
#define NJD_TEXFMT_STRIDE	            (0x0B00)
#define NJD_TEXFMT_TWIDDLED_RECTANGLE	(0x0D00)
#define NJD_TEXFMT_ABGR					(0x0E00)
#define NJD_TEXFMT_ABGR_MM				(0x0F00)

#define NJD_TEXFMT_SMALLVQ              (0x1000)
#define NJD_TEXFMT_SMALLVQ_MM           (0x1100)

#define NJD_TEXFMT_TYPE_MASK           	(0xFF00)

#define NJD_TEXMEM_FREESIZE				(0x00000000)
#define NJD_TEXMEM_MAXBLOCK				(0x00000001)
#define NJD_TEXMEM_MAXSIZE				(0x00000002)

#define NJD_TEXERR_OTHER				(1)
#define NJD_TEXERR_FILEOPEN				(2)
#define NJD_TEXERR_EXTND				(3)
#define NJD_TEXERR_HEADER				(4)
#define NJD_TEXERR_FILELOAD				(5)
#define NJD_TEXERR_SURFACE				(6)
#define NJD_TEXERR_MAINMEMORY			(7)
#define NJD_TEXERR_TEXMEMLOAD			(8)
#define NJD_TEXERR_GLOBALINDEX			(9)

#define NJD_START_GLOBALINEX 			(0xFFFFFFEF)
#define NJD_GLOBAXINDEX_FIRST_VAL 		(0xFFFFFFFF)
#define NJD_BANK_FIRST_VAL 				(0xFFFFFFFF)

#define NJD_TEXMEMFLAG_CACHE			BIT_0
#define NJD_TEXMEMFLAG_TEXMEM			BIT_1
#define NJD_TEXMEMFLAG_PALSRC			BIT_3
#define NJD_TEXMEMFLAG_LOADAREA_MASK	(BIT_0|BIT_1)


#define NJD_GD_SECT_SIZE 		2048

/* Palette Color bpp */
#define NJD_TEXPALETTE_16BPP	16
#define NJD_TEXPALETTE_32BPP	32

#define NJD_TEX_STAT_READ 	0
#define NJD_TEX_STAT_END   	1
#define NJD_TEX_STAT_OPEN 	2
#define NJD_TEX_STAT_RWAIT 	4
#define NJD_TEX_STAT_LOAD 	5
#define NJD_TEX_STAT_LWAIT 	6
#define NJD_TEX_STAT_CLOSE 	7
#define NJD_TEX_STAT_ERR   -1

/* Pixelformat */
#define NJD_PIXELFORMAT_ARGB1555		(0x00000000)
#define NJD_PIXELFORMAT_RGB565			(0x08000000)
#define NJD_PIXELFORMAT_ARGB4444		(0x10000000)
#define NJD_PIXELFORMAT_YUV422			(0x18000000)
#define NJD_PIXELFORMAT_BUMP			(0x20000000)
#define NJD_PIXELFORMAT_PALETTIZED_4BPP	(0x28000000)
#define NJD_PIXELFORMAT_PALETTIZED_8BPP	(0x30000000)

/* Surfaceflag */
#define NJD_SURFACEFLAGS_MIPMAPED		BIT_0
#define NJD_SURFACEFLAGS_CHROMAKEYFIX	BIT_1
#define NJD_SURFACEFLAGS_TWIDDLED		BIT_2
#define NJD_SURFACEFLAGS_VQ				BIT_3
#define NJD_SURFACEFLAGS_STRIDE			BIT_4
#define NJD_SURFACEFLAGS_PALETTIZED		BIT_5

/*--------------------------------------*/
/*      Model Define                    */
/*--------------------------------------*/
#define NJD_EVAL_UNIT_POS  BIT_0     /* ignore translation */
#define NJD_EVAL_UNIT_ANG  BIT_1     /* ignore rotation */
#define NJD_EVAL_UNIT_SCL  BIT_2     /* ignore scaling */
#define NJD_EVAL_HIDE      BIT_3    /* do not draw model */
#define NJD_EVAL_BREAK     BIT_4     /* terminate tracing children */
#define NJD_EVAL_ZXY_ANG   BIT_5 
#define NJD_EVAL_SKIP		BIT_6
#define NJD_EVAL_SHAPE_SKIP	BIT_7
#define NJD_EVAL_CLIP		BIT_8
#define NJD_EVAL_MODIFIER	BIT_9
#define NJD_EVAL_MASK       0x3ff 

/* SRC Alpha Instr(31-29) */
#define NJD_SA_ZERO			(0) 					 /* 0 zero                */
#define NJD_SA_ONE			(BIT_29)				 /* 1 one   	          */
#define NJD_SA_OTHER		(BIT_30)				 /* 2 Other Color		  */
#define NJD_SA_INV_OTHER	(BIT_30|BIT_29)			 /* 3 Inverse Other Color */
#define NJD_SA_SRC 			(BIT_31)		         /* 4 SRC Alpha	          */
#define NJD_SA_INV_SRC		(BIT_31|BIT_29)	         /* 5 Inverse SRC Alpha   */
#define NJD_SA_DST			(BIT_31|BIT_30)			 /* 6 DST Alpha           */
#define NJD_SA_INV_DST		(BIT_31|BIT_30|BIT_29)   /* 7 Inverse DST Alpha   */
#define NJD_SA_MASK			(BIT_31|BIT_30|BIT_29)	 /* MASK          		  */

/* DST Alpha Instr(28-26) */
#define NJD_DA_ZERO			(0) 					 /* 0 zero                */
#define NJD_DA_ONE			(BIT_26) 				 /* 1 one   	          */
#define NJD_DA_OTHER		(BIT_27)				 /* 2 Other Color		  */
#define NJD_DA_INV_OTHER	(BIT_27|BIT_26)			 /* 3 Inverse Other Color */
#define NJD_DA_SRC 			(BIT_28)				 /* 4 SRC Alpha	          */
#define NJD_DA_INV_SRC		(BIT_28|BIT_26)			 /* 5 Inverse SRC Alpha   */
#define NJD_DA_DST			(BIT_28|BIT_27)			 /* 6 DST Alpha           */
#define NJD_DA_INV_DST		(BIT_28|BIT_27|BIT_26)	 /* 7 Inverse DST Alpha   */
#define NJD_DA_MASK			(BIT_28|BIT_27|BIT_26)	 /* MASK		          */

/* filter mode */
#define NJD_FILTER_POINT	(0)
#define NJD_FILTER_BILINEAR	(BIT_13)
#define NJD_FILTER_TRILINEAR	(BIT_14)
#define NJD_FILTER_BLEND	(BIT_14|BIT_13)
#define NJD_FILTER_MASK		(BIT_14|BIT_13)

/* Mip-Map 'D' adjust */
#define NJD_D_025	(BIT_8)						/* 0.25		*/
#define NJD_D_050	(BIT_9)						/* 0.50		*/
#define NJD_D_075	(BIT_9|BIT_8)				/* 0.75		*/
#define NJD_D_100	(BIT_10)					/* 1.00		*/
#define NJD_D_125	(BIT_10|BIT_8)				/* 1.25		*/
#define NJD_D_150	(BIT_10|BIT_9)				/* 1.50		*/
#define NJD_D_175	(BIT_10|BIT_9|BIT_8)		/* 1.75		*/
#define NJD_D_200	(BIT_11)					/* 2.00		*/
#define NJD_D_225	(BIT_11|BIT_8)				/* 2.25		*/
#define NJD_D_250	(BIT_11|BIT_9)				/* 2.50		*/
#define NJD_D_275	(BIT_11|BIT_9|BIT_8)		/* 2.75		*/
#define NJD_D_300	(BIT_11|BIT_10)		        /* 3.00		*/
#define NJD_D_325	(BIT_11|BIT_10|BIT_8)		/* 3.25		*/
#define NJD_D_350	(BIT_11|BIT_10|BIT_9)		/* 3.50		*/
#define NJD_D_375	(BIT_11|BIT_10|BIT_9|BIT_8)	/* 3.75		*/
#define NJD_D_MASK	(BIT_11|BIT_10|BIT_9|BIT_8)	/* MASK	        */

/* flags */
#define NJD_FLAG_IGNORE_LIGHT		(BIT_25)
#define NJD_FLAG_USE_FLAT			(BIT_24)
#define NJD_FLAG_DOUBLE_SIDE		(BIT_23)
#define NJD_FLAG_USE_ENV			(BIT_22)
#define NJD_FLAG_USE_TEXTURE		(BIT_21)
#define NJD_FLAG_USE_ALPHA			(BIT_20)
#define NJD_FLAG_IGNORE_SPECULAR	(BIT_19)
#define NJD_FLAG_FLIP_U				(BIT_18)
#define NJD_FLAG_FLIP_V				(BIT_17)
#define NJD_FLAG_CLAMP_U			(BIT_16)
#define NJD_FLAG_CLAMP_V			(BIT_15)
#define NJD_FLAG_USE_ANISOTROPIC	(BIT_12)
#define NJD_FLAG_PICK				(BIT_7)

/* Mask for flip and clamp */
#define NJD_FLAG_FLIP_MASK  (NJD_FLAG_FLIP_U| NJD_FLAG_FLIP_V)
#define NJD_FLAG_CLAMP_MASK  (NJD_FLAG_CLAMP_U|NJD_FLAG_CLAMP_V)

/* Mask for the flags evaluated by H/W */
#define NJD_FLAG_HARD_MASK (NJD_FLAG_USE_ALPHA	        \
	| NJD_FLAG_FLIP_MASK|NJD_FLAG_CLAMP_MASK        \
	| NJD_FLAG_USE_ANISOTROPIC)

/* Mask for the flags evaluated by the library */
#define NJD_FLAG_SOFT_MASK ( NJD_FLAG_IGNORE_LIGHT	\
	|NJD_FLAG_PICK| NJD_FLAG_USE_TEXTURE		\
	| NJD_FLAG_DOUBLE_SIDE| NJD_FLAG_USE_ENV	\
	| NJD_FLAG_USE_FLAT)

/* Mask for entire flag */
#define NJD_FLAG_MASK (NJD_FLAG_HARD_MASK		\
	| NJD_FLAG_SOFT_MASK)

/* Default user mask */
#define NJD_DEFAULT_USER_MASK	(BIT_2|BIT_1|BIT_0)

/* Default systm mask */
#define NJD_DEFAULT_SYS_MASK	(~NJD_DEFAULT_USER_MASK)

/* Mask for the field sent direcly to the H/W */
#define NJD_SYS_HARD_MASK (NJD_SA_MASK|NJD_SD_MASK	 \
	|NJD_FOG_MASK|NJD_FLAG_HARD_MASK		 	 \
	|NJD_D_MASK|NJD_FILTER_MASK)
 

/* meshset type bits (type_addrId 14-15 bit) */
#define NJD_MESHSET_3           0x0000  /* polygon3 meshset         */
#define NJD_MESHSET_4           0x4000  /* polygon4 meshset         */
#define NJD_MESHSET_N           0x8000  /* polygonN meshset         */
#define NJD_MESHSET_TRIMESH     0xc000  /* trimesh meshset          */
#define NJD_MESHSET_MASK        0xc000  /* meshset type mask        */

/* meshset trimesh end bit */
#define NJD_TRIMESH_END         0x8000  /* trimesh end bit          */

/* Callback bits (attr_texId 30-31 bit */
#define	NJD_POLYGON_CALLBACK	(BIT_31)			/* polygon callback		*/
#define	NJD_MATERIAL_CALLBACK	(BIT_30)			/* material callback	*/
#define	NJD_CALLBACK_MASK		(BIT_31 | BIT_30)	/* callback mask		*/

/*--------------------------------------*/
/*      Motion define                   */
/*--------------------------------------*/
/* motion type bits */

#define NJD_MTYPE_POS_0			BIT_0
#define NJD_MTYPE_ANG_1			BIT_1
#define NJD_MTYPE_SCL_2			BIT_2
#define NJD_MTYPE_VEC_3			BIT_3
#define NJD_MTYPE_VERT_4		BIT_4
#define NJD_MTYPE_NORM_5		BIT_5
#define	NJD_MTYPE_TARGET_3		BIT_6
#define	NJD_MTYPE_ROLL_6		BIT_7
#define	NJD_MTYPE_ANGLE_7		BIT_8
#define	NJD_MTYPE_RGB_8			BIT_9
#define	NJD_MTYPE_INTENSITY_9	BIT_10
#define	NJD_MTYPE_SPOT_10		BIT_11
#define	NJD_MTYPE_POINT_10		BIT_12
#define NJD_MTYPE_QUAT_1        BIT_13

#define NJD_MTYPE_LINER         0x0000    /* use liner                */
#define NJD_MTYPE_SPLINE        0x0040    /* use spline               */
#define NJD_MTYPE_USER          0x0080    /* use user function        */
#define NJD_MTYPE_MASK          0x00c0    /* motion function mask     */

/*--------------------------------------*/
/*      Light define                    */
/*--------------------------------------*/
/* light type */
#define NJD_CONSTANT            0x00    /* constant                 */
#define NJD_AMBIENT             0x01    /* ambient                  */
#define NJD_DIR_LIGHT           0x02    /* directional light        */
#define NJD_POINT_LIGHT         0x04    /* point light              */
#define NJD_SPOT_LIGHT          0x08    /* spot light               */
#define NJD_SPEC_DIR            0x10    /* dir. light's specular    */
#define NJD_SPEC_POINT          0x20    /* point light's specular   */
#define NJD_USER_LIGHT          0x40    /* user-defined funcion     */
#define NJD_SIMPLE_LIGHT        0x80    /* simplified funcion       */
#define NJD_BLOCK_LIGHT	        0xc0    /* calculated by model      */

#define NJD_LAMBERT_DIR         NJD_AMBIENT|NJD_DIR_LIGHT
										/* dir. light's Lambertian  */
#define NJD_LAMBERT_POINT       NJD_AMBIENT|NJD_POINT_LIGHT
										/* point light's Lambertian */
#define NJD_PHONG_DIR           NJD_AMBIENT|NJD_DIR_LIGHT|NJD_SPEC_DIR
										/* dir. light's Phong       */
#define NJD_PHONG_POINT         NJD_AMBIENT|NJD_POINT_LIGHT|NJD_SPEC_POINT
										/* point light's Phong      */

/* light's ON-OFF flags */
#define NJD_LIGHT_OFF           0       /* turning off a light      */
#define NJD_LIGHT_ON            1       /* turning on a light       */

/* the number of light */
#define NJD_LIGHT_MAX           32      /* the maximum of lights    */
#define NJD_LIGHT_MIN           0       /* the minimum of lights    */


/*--------------------------------------*/
/*      Attribute define                */
/*--------------------------------------*/

#define NJD_DRAW_MASK           0x0000000f  /* Mask for Drawing Attribute   */

#define NJD_DRAW_NORMAL         0x00000000  /* Draw Normally                */
#define NJD_DRAW_FAN            0x00000001  /* Draw Radially                */
#define NJD_DRAW_CONNECTED      0x00000002  /* Draw Continuously            */
#define NJD_DRAW_INSCRIBED      0x00000010  /* Draw Inscribed Circle        */
#define NJD_FILL                0x00000020  /* Fill Polygon                 */
#define NJD_TRANSPARENT         0x00000040  /* Be Transparent               */
#define NJD_CALC_LIGHT          0x40000000  /* Put under the Light Effect   */
#define NJD_USE_TEXTURE         0x80000000  /* Use Texture                  */


/*--------------------------------------*/
/*      Scroll define                   */
/*--------------------------------------*/
/* cell size */
#define NJD_CELL_8              8
#define NJD_CELL_16             16
#define NJD_CELL_32             32
#define NJD_CELL_64             64
#define NJD_CELL_128            128
#define NJD_CELL_256            256

/* the maximum of scroll's value */
#define NJD_CELL_NUM_MAX        0xFFFF  /* the maximum of cell's number */
#define NJD_MAP_W_MAX           0xFF    /* the maximum of map's width   */
#define NJD_MAP_H_MAX           0xFF    /* the muximum of map's height  */
#define NJD_MAP_MAX             (NJD_MAP_W_MAX*NJD_MAP_H_MAX)
										/* the muximum of map           */

/* 3d control */
#define	NJD_CONTROL_3D_DISP_AXIS			BIT_0
#define	NJD_CONTROL_3D_NO_CLIP_CHECK		BIT_1
#define	NJD_CONTROL_3D_CONSTANT_ATTR		BIT_2
#define	NJD_CONTROL_3D_ENABLE_ALPHA			BIT_3
#define	NJD_CONTROL_3D_CONSTANT_MATERIAL	BIT_4
#define	NJD_CONTROL_3D_OFFSET_MATERIAL		BIT_5
#define	NJD_CONTROL_3D_DEPTH_QUEUE			BIT_6
#define	NJD_CONTROL_3D_VERTEX_OFFSET		BIT_7
#define	NJD_CONTROL_3D_MODEL_CLIP			BIT_8
#define	NJD_CONTROL_3D_CONSTANT_TEXTURE_MATERIAL BIT_9
#define	NJD_CONTROL_3D_SHADOW				BIT_10
#define	NJD_CONTROL_3D_CNK_CONSTANT_ATTR	BIT_11
#define	NJD_CONTROL_3D_SHADOW_OPAQUE		BIT_12
#define	NJD_CONTROL_3D_TRANS_MODIFIER		BIT_13
#define	NJD_CONTROL_3D_USE_PUNCHTHROUGH		BIT_14
#define	NJD_CONTROL_3D_CNK_BLEND_MODE		BIT_15
#define	NJD_CONTROL_3D_DISABLE_NEAR_CLIP	BIT_16
#define	NJD_CONTROL_3D_ENV_UV_SCROLL		BIT_17

#define	NJD_CONTROL_3D_MIRROR_MODEL			BIT_31

/* clipping */
#define	NJD_CLIP_NEAR			BIT_0
#define	NJD_CLIP_FAR			BIT_1
#define	NJD_CLIP_LEFT			BIT_2
#define	NJD_CLIP_RIGHT			BIT_3
#define	NJD_CLIP_TOP			BIT_4
#define	NJD_CLIP_BOTTOM			BIT_5
#define	NJD_CLIP_SCREEN			(NJD_CLIP_LEFT|NJD_CLIP_RIGHT|NJD_CLIP_TOP|NJD_CLIP_BOTTOM)

/* sprite */
#define	NJD_SPRITE_ANGLE			BIT_0
#define	NJD_SPRITE_COLOR			BIT_1
#define	NJD_SPRITE_HFLIP			BIT_2
#define	NJD_SPRITE_VFLIP			BIT_3
#define	NJD_SPRITE_HVFLIP			( NJD_SPRITE_HFLIP | NJD_SPRITE_VFLIP )
#define	NJD_SPRITE_SCALE			BIT_4
#define	NJD_SPRITE_ALPHA			BIT_5

/* texture */
#define	NJD_TEXTUREFILTER_POINT_SAMPLE	0
#define	NJD_TEXTUREFILTER_BILINEAR		1
#define	NJD_TEXTUREFILTER_TRILINEAR		2

#define	NJD_TEXTURECLAMP_NOCLAMP		0
#define	NJD_TEXTURECLAMP_CLAMP_V		1
#define	NJD_TEXTURECLAMP_CLAMP_U		2
#define	NJD_TEXTURECLAMP_CLAMP_UV		3

#define	NJD_TEXTUREFLIP_NOFLIP			0
#define	NJD_TEXTUREFLIP_FLIP_V			1
#define	NJD_TEXTUREFLIP_FLIP_U			2
#define	NJD_TEXTUREFLIP_FLIP_UV			3

/*--------------------------------------*/
/*      Matrix                          */
/*--------------------------------------*/
typedef Float *NJS_MATRIX_PTR;

enum {M00, M01, M02, M03,
	  M10, M11, M12, M13,
	  M20, M21, M22, M23,
	  M30, M31, M32, M33};




/*--------------------------------------*/
/*      MACRO                           */
/*--------------------------------------*/
/*
 *  COLOR MACRO
 */
#define NJM_COLOR_A(col)        ((col) >> 24)
#define NJM_COLOR_R(col)        (((col) >> 16) & 0xFF)
#define NJM_COLOR_G(col)        (((col) >> 8) & 0xFF)
#define NJM_COLOR_B(col)        ((col) & 0xFF)

/*
 *  ANGLE MACRO
 */
#define NJD_PI  3.141592

#define NJM_DEG_RAD(n)  ((n) * NJD_PI / 180.0)
															/*  deg ¨ rad  */
#define NJM_DEG_ANG(n)  ((Angle)(((n) * 65536.0) / 360.0))
															/*  deg ¨ ang  */
#define NJM_RAD_ANG(n)  ((Angle)(((n) * 65536.0) / (2 * NJD_PI)))
															/*  rad ¨ ang  */
#define NJM_RAD_DEG(n)  ((n) * 180.0 / NJD_PI)
															/*  rad ¨ deg  */
#define NJM_ANG_DEG(n)  ((n) * 360.0 / 65536.0)
															/*  ang ¨ deg  */
#define NJM_ANG_RAD(n)  ((n) * (2 * NJD_PI) / 65536.0)
															/*  ang ¨ rad  */
/*
 *	MAX MIN MACRO
 */
#define NJM_MAX(a,b) ((a)>(b)?(a):(b))
#define NJM_MIN(a,b) ((a)<(b)?(a):(b))

/*
 *  SCROLL MACRO
 */
#define NJM_MAP(celnum,u,v)	(((celnum) << 16 | (u) << 8 | (v)))
#define NJM_MAPCELL(map)	(((map) >> 16)&0x0000FFFF)
#define NJM_MAPU(map)	(((map) >> 8)&0x000000FF)
#define NJM_MAPV(map)	((map)&0x000000FF)

/*
 *  LIGHT MACRO
 */
#define NJM_LIGHT_INIT_VECTOR(l)	(l)->vctr                /*  NJS_LIGHT *l */
#define NJM_LIGHT_INIT_POINT(l)		(l)->pnt
#define NJM_LIGHT_MATRIX(l)			(l)->mtrx

#define NJM_LIGHT_VECTOR(l)	((l)->ltcal).lvctr
#define NJM_LIGHT_POINT(l)	((l)->ltcal).lpnt
#define NJM_LIGHT_AMB(l)	((l)->ltcal).amb
#define NJM_LIGHT_DIF(l)	((l)->ltcal).dif
#define NJM_LIGHT_SPC(l)	((l)->ltcal).spc
#define NJM_LIGHT_EXP(l)	((l)->ltcal).exp

#define NJM_LIGHT_COLOR(l)	((l)->attr).argb


/*
 *  DEBUG MACRO
 */
#define NJM_LOCATION(x,y)	((x)<<16|(y))
#define NJM_GET_X(n)        ((Sint16)((n)>>16))
#define NJM_GET_Y(n)        ((Sint16)((n)&0x0000FFFF))

/*
 *  TEXTURE MACRO
 */
#define NJM_BANKGLOBALINDEX(bank,globalIndex)	(((bank) << NJD_TEXBANK_SHIFT)|((globalIndex)&NJD_TEXPALGLOBAL_MASK))

#define NJM_TEXTURE_WIDTH(texlist,n) (((NJS_TEXMEMLIST*)(texlist)->textures[(n)].texaddr)->texinfo.texsurface.nWidth)
#define NJM_TEXTURE_HEIGHT(texlist,n) (((NJS_TEXMEMLIST*)(texlist)->textures[(n)].texaddr)->texinfo.texsurface.nHeight)
#define NJM_TEXTURE_GLOBALINDEX(texlist,n) (((NJS_TEXMEMLIST*)(texlist)->textures[(n)].texaddr)->globalIndex)
#define NJM_TEXTURE_SIZE(texlist,n) (((NJS_TEXMEMLIST*)(texlist)->textures[(n)].texaddr)->texinfo.texsurface.TextureSize)

#endif /* _NINJA_DEF_H_ */

#ifndef _NINJA_STR_H_
#define _NINJA_STR_H_

typedef Sint8 NJS_SYS_ATTR[NJD_SYS_ATTR_SIZE];

typedef void (*NJS_INT_FUNC)(void);

typedef struct{
	Uint32 NativeLimit;
	Uint32 NativeCurrent;
	Uint32 NativePeak;
	Uint32 InternalVertexBufSize[5];
	Uint32 MaxVertexBufSize[5];
	Uint32 LastTATime;
	Uint32 LastRenderTime;
	Uint32 CurrentVertexBufSize[5];
	Uint32 reserved0;
	Uint32 reserved1;
	Uint32 reserved2;
	Uint32 reserved3;
	Uint32 reserved4;
	Uint32 reserved5;
	Uint32 reserved6;
	Uint32 reserved7;
	Uint32 reserved8;
	Uint32 reserved9;
	Uint32 reserved10;
	Uint32 reserved11;
} NJS_SYSTEMINFO;

typedef struct {
	Sint16  u;
	Sint16  v;
} NJS_TEX;

typedef	struct {
	Uint8   b;
	Uint8   g;
	Uint8   r;
	Uint8   a;
} NJS_BGRA;

typedef union {
	Uint32  color;
	/*NJS_TEX tex;*/
	NJS_BGRA argb;
} NJS_COLOR;

typedef struct {
	Float   a;
	Float   r;
	Float   g;
	Float   b;
} NJS_ARGB;

typedef struct {
	Sint16  x;
	Sint16  y;
} NJS_POINT2I;

typedef struct {
	Float  x;
	Float  y;
} NJS_POINT2;

typedef struct {
	Float   x;
	Float   y;
	Float   z;
} NJS_POINT3, NJS_VECTOR;

typedef struct {
	NJS_POINT2  *p;
	NJS_COLOR   *col;
	NJS_TEX     *tex;
	Uint32      num;
} NJS_POINT2COL;

typedef struct {
	NJS_POINT3  *p;
	NJS_COLOR   *col;
	NJS_TEX     *tex;
	Uint32      num;
} NJS_POINT3COL;

typedef struct {
	Float   px,py,pz;
	Float   vx,vy,vz;
} NJS_LINE, NJS_PLANE, NJS_PVECTOR;


/* コリジョン用構造体追加 */
typedef struct{
	NJS_POINT3 c;
	Float      r;
} NJS_SPHERE;

typedef struct{
	NJS_POINT3 c1;
	NJS_POINT3 c2;
	Float      r;
} NJS_CAPSULE;

typedef struct{
	NJS_POINT3 v[8];
} NJS_BOX;

typedef struct{
	NJS_POINT3 v[4];
} NJS_RECT;
/* 追加終わり */


typedef struct {
	NJS_MATRIX m;
	Float   px,py,pz;
	Float   vx,vy,vz;
	Angle   roll;
	Float   apx,apy,apz;
	Float   avx,avy,avz;
	Angle   aroll;
} NJS_VIEW;

typedef struct {
	Float   dist;
	Float   w,h;
	Float   cx,cy;
} NJS_SCREEN;


/*--------------------------------------*/
/*      Texture Structure               */
/*--------------------------------------*/
typedef struct{
	Uint32 Type;
	Uint32 BitDepth;
	Uint32 PixelFormat;
	Uint32 nWidth;
	Uint32 nHeight;
	Uint32 TextureSize;
	Uint32 fSurfaceFlags;
	Uint32 *pSurface;
	Uint32 *pVirtual;
	Uint32 *pPhysical;
}NJS_TEXSURFACE;
	
typedef struct{
	void*			   texaddr; /* texture address   			*/
	NJS_TEXSURFACE	texsurface;	/* texture surface 				*/
} NJS_TEXINFO;

typedef struct {
	void            *filename;  /* texture filename strings     */
	Uint32               attr;  /* texture attribute            */
	Uint32            texaddr;  /* texture memory list address  */
} NJS_TEXNAME;

typedef struct {
	NJS_TEXNAME     *textures;  /* texture array                */
	Uint32          nbTexture;  /* texture count                */
} NJS_TEXLIST;

typedef struct {
	Uint32			globalIndex;/* global unique texture ID     */
	Uint32			bank;		/* palette bank 				*/
	Uint32			tspparambuffer;/* TSPParambuffer			*/
	Uint32			texparambuffer;/* TextureParambuffer		*/
	Uint32			texaddr;	/* texture flag					*/
	NJS_TEXINFO		texinfo;	/* texinfo						*/
	Uint16			count;		/* texture count  				*/
	Uint16			dummy;
} NJS_TEXMEMLIST;

typedef struct {
	void 			*palette;	/*	palette pointer				*/
	Uint16 			mode;		/*	color mode					*/
	Sint16 			bank;		/*	bank						*/
	Sint16 			offset;		/*	entry offset				*/
	Sint16 			count;		/*	entry count					*/
}NJS_TEXPALETTE;

typedef struct {
	Uint32	n;
	Uint32	globalIndex;
	Sint32	texerr;
	Sint32	gdstat;
	Sint32	gderr;
	Sint32	reserved0;
	Sint32	reserved1;
	Sint32	reserved2;
}NJS_TEXERRSTAT;

typedef struct{
	Uint32 nTextureType;
	Uint16 nWidth;
	Uint16 nHeight;
}NJS_PVRHEADERDATA;

/*--------------------------------------*/
/*      Model Structure                 */
/*--------------------------------------*/
/*
 * NJS_MODEL
 */
typedef struct {
	NJS_COLOR         diffuse;
	NJS_COLOR        specular;
	Float            exponent;
	Uint32         attr_texId;  /* attribute and texture ID in texlist        */
	Uint32          attrflags;  /* attribute flags */
} NJS_MATERIAL;

typedef struct {
	Uint16          type_matId; /* meshset type and attr index
	                               14-15 : meshset type bits
	                                0-13 : material id(0-4095)  */
	Uint16          nbMesh;     /* mesh count                   */
	Sint16          *meshes;    /* mesh array                   */
	Uint32          *attrs;     /* attribure                    */
	NJS_VECTOR      *normals;   /* mesh normal list             */
	NJS_COLOR       *vertcolor; /* polygon vertex color list    */
	NJS_TEX         *vertuv;    /* polygon vertex uv list       */
} NJS_MESHSET;

typedef struct {
	Uint16          type_matId; /* meshset type and attr index
	                               14-15 : meshset type bits
	                                0-13 : material id(0-4095)  */
	Uint16          nbMesh;     /* mesh count                   */
	Sint16          *meshes;    /* mesh array                   */
	Uint32          *attrs;     /* attribure                    */
	NJS_VECTOR      *normals;   /* mesh normal list             */
	NJS_COLOR       *vertcolor; /* polygon vertex color list    */
	NJS_TEX         *vertuv;    /* polygon vertex uv list       */
	void            *buffer;    /* A pointer to MeshSetBuffer. See definition in direct3d.h. */
} NJS_MESHSET_SADX;

typedef struct {
	NJS_POINT3      *points;    /* vertex list                  */
	NJS_VECTOR      *normals;   /* vertex normal list           */
	Sint32          nbPoint;    /* vertex count                 */
	NJS_MESHSET     *meshsets;  /* meshset list                 */
	NJS_MATERIAL    *mats;      /* material list                */
	Uint16          nbMeshset;  /* meshset count                */
	Uint16          nbMat;      /* material count               */
	NJS_POINT3      center;     /* model center                 */
	Float			r;			/* ??????????? */
} NJS_MODEL;

typedef struct {
	NJS_POINT3      *points;    /* vertex list                  */
	NJS_VECTOR      *normals;   /* vertex normal list           */
	Sint32          nbPoint;    /* vertex count                 */
	NJS_MESHSET_SADX *meshsets;  /* meshset list                 */
	NJS_MATERIAL    *mats;      /* material list                */
	Uint16          nbMeshset;  /* meshset count                */
	Uint16          nbMat;      /* material count               */
	NJS_POINT3      center;     /* model center                 */
	Float			r;			/* ??????????? */
	void            *buffer;
} NJS_MODEL_SADX;

struct struct_0
{
	char DataType;
	char ElementSize;
	__int16 ElementCount;
	int field_4;
	void *Data;
	int DataSize;
};

struct SA2B_Model
{
	struct_0 *Vertices;
	int field_4;
	void *Poly;
	int field_C;
	__int16 PolyCount;
	__int16 field_12;
	NJS_VECTOR Center;
	float Radius;
};

struct NJS_CNK_MODEL;

/*
 * NJS_OBJECT
 */
typedef struct obj {
	Uint32          evalflags;  /* evalation flags              */
	void            *model;     /* model data pointer           */
	Float           pos[3];     /* translation                  */
	Angle           ang[3];     /* rotation                     */
	Float           scl[3];     /* scaling                      */
	struct obj      *child;     /* child object                 */
	struct obj      *sibling;   /* sibling object               */

#ifdef __cplusplus
	NJS_MODEL       *getbasicmodel() const { return (NJS_MODEL*)model; }
	void            putbasicmodel(NJS_MODEL *value) { model = value; }
	NJS_MODEL_SADX  *getbasicdxmodel() const { return (NJS_MODEL_SADX*)model; }
	void            putbasicdxmodel(NJS_MODEL_SADX *value) { model = value; }
	NJS_CNK_MODEL   *getchunkmodel() const { return (NJS_CNK_MODEL*)model; }
	void            putchunkmodel(NJS_CNK_MODEL *value) { model = value; }
	SA2B_Model      *getsa2bmodel() const { return (SA2B_Model*)model; }
	void            putsa2bmodel(SA2B_Model *value) { model = value; }

#ifdef _MSC_VER
	/* MSVC-specific property emulation. */
	__declspec(property(get = getbasicmodel, put = putbasicmodel))
	NJS_MODEL       *basicmodel;
	__declspec(property(get = getbasicdxmodel, put = putbasicdxmodel))
	NJS_MODEL_SADX  *basicdxmodel;
	__declspec(property(get = getchunkmodel, put = putchunkmodel))
	NJS_CNK_MODEL   *chunkmodel;
	__declspec(property(get = getsa2bmodel, put = putsa2bmodel))
	SA2B_Model      *sa2bmodel;
#endif

	int countanimated() const
	{
		int result = (evalflags & NJD_EVAL_SKIP) == NJD_EVAL_SKIP ? 0 : 1;
		if (child != nullptr)
			result += child->countanimated();
		if (sibling != nullptr)
			result += sibling->countanimated();
		return result;
	}

	int countmorph() const
	{
		int result = (evalflags & NJD_EVAL_SHAPE_SKIP) == NJD_EVAL_SHAPE_SKIP ? 0 : 1;
		if (child != nullptr)
			result += child->countmorph();
		if (sibling != nullptr)
			result += sibling->countmorph();
		return result;
	}
#endif /* __cplusplus */

} NJS_OBJECT, NJS_CNK_OBJECT;

/*
 * NJS_MOTION
 */
/* key frame animation */
typedef struct {
	Uint32          keyframe;
	Float           key[3];
} NJS_MKEY_F;

typedef struct {
	Uint32          keyframe;
	Angle           key[3];     /* angle                        */
} NJS_MKEY_A;

typedef struct {
	Uint32          keyframe;
	Angle           angle;      /* angle                        */
	Float           axe[3];     /* axe vector                   */
} NJS_MKEY_AX;

typedef struct 
{
	Uint32          keyframe;
	void            *key;        /* pointer                     */
} NJS_MKEY_P;

typedef struct {
	Uint32          keyframe;
	Float           key;
} NJS_MKEY_F1;

typedef struct {
	Uint32          keyframe;
	Float           key[2];
} NJS_MKEY_F2;

typedef struct {
	Uint32          keyframe;
	Float           nrang;
	Float           frang;
	Angle           iang;
	Angle           oang;
} NJS_MKEY_SPOT;

typedef struct {
	Uint32          keyframe;
	Uint32          key;
} NJS_MKEY_UI32;

typedef struct {
	Uint32          keyframe;
	Sint32          key;
} NJS_MKEY_SI32, NJS_MKEY_A1;

typedef struct {
	Uint32          keyframe;
	Float           key[4];
} NJS_MKEY_QUAT;

/*
 * NJS_MDATA
 */
/* 1 factor : pos or ang or scl or shape or vec */
typedef struct {
	void            *p[1];      /* factor pointer               */
	Uint32          nb[1];      /* factor count                 */
} NJS_MDATA1;

/* 2 factors : pos and ang, pos and vec(light motion), etc      */
typedef struct {
	void            *p[2];      /* factor pointer               */
	Uint32          nb[2];      /* factor count                 */
} NJS_MDATA2;

/* 3 factors : pos and ang and scl, pos and ang and shape, etc  */
typedef struct {
	void            *p[3];      /* factor pointer               */
	Uint32          nb[3];      /* factor count                 */
} NJS_MDATA3;

/* 4 factors : pos and ang and scl and shape, etc               */
typedef struct {
	void            *p[4];      /* factor pointer               */
	Uint32          nb[4];      /* factor count                 */
} NJS_MDATA4;

/* 5 factors : etc               */
typedef struct {
	void            *p[5];      /* factor pointer               */
	Uint32          nb[5];      /* factor count                 */
} NJS_MDATA5;

typedef struct {
	void            *mdata;     /* NJS_MDATA array              */
	Uint32          nbFrame;    /* frame count                  */
	Uint16          type;       /* motion type  NJD_MTYPE_...   */
	Uint16          inp_fn;     /* interpolation & factor count */
} NJS_MOTION;

typedef struct {
	NJS_OBJECT      *object;    /* Object Tree top pointer      */
	NJS_MOTION      *motion;    /* motion                       */
#if 0
	NJS_TEXLIST     *texlist;   /* current texture list         */
#endif
} NJS_ACTION;

typedef struct
{
	NJS_MOTION		*motion[2];		/* motion's pointers */
	Float			frame[2];		/* current frames */
} NJS_MOTION_LINK;

typedef struct
{
	NJS_OBJECT		*object;		/* object's pointer */
	NJS_MOTION_LINK	*motionlink;	/* motion-link's pointer */
} NJS_ACTION_LINK;


/*
 * 3DLIB WORK BUFFER
 */
typedef union {
	Uint32		work;	/* work	*/
	Uint32		clip;	/* clip	*/

} NJS_CLIP_BUF;

typedef struct {
	NJS_CLIP_BUF a;			/* union			: 32		*/
	Float		sx,sy;		/* screen point		: 36 40		*/
	Float		oow;		/* 1/w				: 44		*/
	NJS_POINT3	point;		/* vertex point		:  0  4  8	*/
	Uint32		reserve;	/* clip				: 28		*/
	Float		u,v;		/* U, V				: 48 52		*/
	Float		inten[2];	/* Intensity		: 56 60		*/
	NJS_VECTOR	vector;		/* vertex normal	: 12 16 20	*/
	Uint32		flag;		/* flag				: 24		*/
} NJS_VERTEX_BUF;			/* TotalSize		: 64		*/

typedef struct {
	NJS_CLIP_BUF a;			/* union			: 4	*/
	Float		sx,sy;		/* screen point		:12	*/
	Float		oow;		/* 1/w				:16	*/
	NJS_POINT3	point;		/* vertex point		:44	*/
	Uint32		reserve;	/* clip				:64	*/
	NJS_ARGB	col;
	NJS_VECTOR	vector;		/* vertex normal	:56	*/
	Uint32		flag;		/* flag				:58	*/
} NJS_VERTEX_BUF2;			/* TotalSize		: 64		*/


typedef struct {
	NJS_CLIP_BUF a;			/* union			: 32		*/
	Float		sx,sy;		/* screen point		: 36 40		*/
	Float		oow;		/* 1/w				: 44		*/
	NJS_POINT3	point;		/* vertex point		:  0  4  8	*/
	Uint32		reserve;	/* clip				: 28		*/
	Float		u,v;		/* U, V				: 48 52		*/
	Float		inten[2];	/* Intensity		: 56 60		*/
	NJS_VECTOR	vector;		/* vertex normal	: 12 16 20	*/
	Uint32		flag;		/* flag				: 24		*/
} NJS_VERTEX_BUF_0;

typedef struct {
	NJS_CLIP_BUF a;			/* union			: 4	*/
	Float		sx,sy;		/* screen point		:12	*/
	Float		oow;		/* 1/w				:16	*/
	NJS_POINT3	point;		/* vertex point		:44	*/
	Uint32		reserve;	/* clip				:64	*/
	NJS_ARGB	col;
	NJS_VECTOR	vector;		/* vertex normal	:56	*/
	Uint32		flag;		/* flag				:58	*/
} NJS_VERTEX_BUF_1;

typedef struct {
	NJS_CLIP_BUF a;			/* union			: 4	*/
	Float		sx,sy;		/* screen point		:12	*/
	Float		oow;		/* 1/w				:16	*/
	Float		u,v;		/* U V				:24	*/
	Uint32		col[2];		/* pack col			:32	*/
	NJS_POINT3	point;		/* vertex point		:44	*/
	NJS_VECTOR	vector;		/* vertex normal	:56	*/
	Uint32		flag;		/* flag				:58	*/
	Uint32		reserve;	/* clip				:64	*/
} NJS_VERTEX_BUF_3;

typedef struct {
	NJS_CLIP_BUF a;			/* union			: 32		*/
	Float		sx,sy;		/* screen point		: 36 40		*/
	Float		oow;		/* 1/w				: 44		*/
	NJS_POINT3	point;		/* vertex point		:  0  4  8	*/
	Uint32		reserve;	/* clip				: 28		*/
	Float		u,v;		/* U, V				: 48 52		*/
	Float		inten[2];	/* Intensity		: 56 60		*/
	NJS_VECTOR	vector;		/* vertex normal	: 12 16 20	*/
	Uint32		flag;		/* flag				: 24		*/
} NJS_VERTEX_BUF_7;

typedef struct {
	NJS_VECTOR	vector;		/* vertex normal	:12	*/
	Uint32		clip;		/* clip				:16	*/
	Float		sx,sy;		/* screen point		:24	*/
	Float		oow;		/* 1/w				:28	*/
	Float		inten;		/* intensity		:32	*/
} NJS_VERTEX_BUF_X;


/*--------------------------------------*/
/*      Light Structure                 */
/*--------------------------------------*/
/*
 * NJS_LIGHT_CAL
 */
typedef struct
{
/*************/
	Float       ratten;         /* attenuation rate             */
	Float       ipd;		    /* inner product                */
/*************/
	Float       nrr;            /* limit judgment value         */
	Float       frr;            /* limit judgment value         */
	Float       cosi;           /* limit judgment value         */
	Float       cose;           /* limit judgment value         */
	Float		idev;           /* judgment value of devision   */
	Float		odev;           /* judgment value of devision   */
	Float       rate;           /* attenuation rate(calculated) */
	Float       intns;          /* Intensity (0-1)              */
	Int         exp;            /* exponent(SGI):material       */
/*************/
	Int         reserve;        /*                              */
/*************/
	NJS_POINT3  lpnt;           /* point vector                 */
	NJS_VECTOR  lvctr;          /* directional vector           */
/*************/
	NJS_VECTOR  lmvctr;         /* directional vector           */
/*************/
	NJS_ARGB    atten;          /* intns * argb                 */
	NJS_ARGB    amb;            /* iamb*atten                   */
	NJS_ARGB    dif;            /* idif*atten                   */
	NJS_ARGB    spc;            /* ispc*atten                   */
/*************/
	NJS_ARGB    mamb;           /* amb*material                 */
	NJS_ARGB    mdif;           /* dif*material                 */
	NJS_ARGB    mspc;           /* spc*material                 */
/*************/
} NJS_LIGHT_CAL, *NJS_LIGHT_CAL_PTR;

/*
 * NJS_LIGHT_ATTR
 */
typedef struct {
	Int             lsrc;       /* the kind of light source     */
	Float           iamb;       /* ambient's intensity          */
	Float           idif;       /* deffuse light's intensity    */
	Float           ispc;       /* specular light's intensity   */
	Float           nrang;      /* limit value (nearest)        */
	Float           frang;      /* limit value (farthest)       */
	void*           func;       /* the pointer of Callback func.*/
	Angle           iang;       /* limit value (inside)         */
	Angle           oang;       /* limit value (outside)        */
	NJS_ARGB        argb;       /* light's color                */
} NJS_LIGHT_ATTR, *NJS_LIGHT_ATTR_PTR;

/*
 * NJS_LIGHT
 */
typedef struct {
	NJS_MATRIX      mtrx;       /* matrix                       */
	NJS_POINT3      pnt;        /* point vector                 */
	NJS_VECTOR      vctr;       /* directional vector           */
	Int             stat;       /* status                       */
	Int             reserve;    /* reserve                      */
	NJS_LIGHT_CAL   ltcal;      /* calculation                  */
	NJS_LIGHT_ATTR  attr;       /* attribute                    */
} NJS_LIGHT, *NJS_LIGHT_PTR;

/*
 * NJS_LIGHT_MATERIAL
 */
typedef  struct {
	NJS_ARGB        ambient;    /* Ambient color from Material	*/
	NJS_ARGB        diffuse;    /* Diffuse color from Material	*/
	NJS_ARGB        specular;   /* Specular color from Material	*/
	Float           exponent;   /* Exponent from Material		*/
} NJS_LIGHT_MATERIAL;

/*
 * NJF_LIGHT_FUNC
 */
typedef void (*NJF_LIGHT_FUNC)(NJS_ARGB*, NJS_POINT3*, NJS_VECTOR*, NJS_LIGHT_PTR);

/*--------------------------------------*/
/*      Camera Structure                */
/*--------------------------------------*/

typedef struct {
	Float px, py, pz;
	Float vx, vy, vz;
	Angle roll;
	Angle ang;
	Float n_clip;
	Float f_clip;
	NJS_VECTOR lx,ly;
} NJS_CAMERA;

/*--------------------------------------*/
/*   Light & Camera Motion Structure    */
/*--------------------------------------*/

typedef struct {
	NJS_LIGHT	*light;
	NJS_MOTION	*motion;
} NJS_LACTION;

typedef struct {
	NJS_CAMERA	*camera;
	NJS_MOTION	*motion;
} NJS_CACTION;

typedef struct {
	Float	*pos;
	Angle	*ang;
	Float	*scl;
} NJS_MOTION_DATA;

typedef struct {
	Float	*pos;
	Float	*vect;
	Angle	*roll;
	Angle	*ang;
} NJS_CMOTION_DATA;

typedef struct {
	Float	*pos;
	Float	*vect;
	Float	*rgb;
	Float	*inten;
	Float 	*rang;
	Angle	*ang;
} NJS_LMOTION_DATA;

/*--------------------------------------*/
/*      Scroll Structure                */
/*--------------------------------------*/
/*
 * NJS_SCLATTR
 */
typedef Uint32 NJS_SCLATTR;     /* scroll attribute structure   */

/*
 * NJS_SCLMTRX
 */
typedef Float NJS_SCLMTRX[4];   /* scroll rotation matrix       */

/*
 *NJS_SCROLL
 */
typedef struct {
	Uint16      celps;          /* pixel size of the cells      */
	Uint16      mapw;           /* number of the width of map   */
	Uint16      maph;           /* number of the height of map  */
	Uint16      sw,sh;          /* size of the scroll draw      */
	NJS_TEXLIST* list;			/* texlist ID                   */
	Uint32      *map;           /* map array pointer            */
	Float       px,py;          /* scroll transration           */
	Float       bx,by;          /* map draw scroll              */
	Float       pr;             /* priority                     */
	Uint16      sflag;          /* scale flag                   */
	Float       sx,sy;          /* scale x,y                    */
	Float		spx,spy;		/* center of scale x,y			*/
	Uint16      mflag;          /* matrix flag                  */
	Float       cx,cy;          /* center of rotation x,y       */
	NJS_SCLMTRX m;              /* rotation matrix              */
	Uint32      colmode;        /* color mode                   */
	NJS_POINT2	clip[2];
	NJS_SCLATTR attr;           /* scroll attribute             */
	NJS_COLOR	sclc;
}NJS_SCROLL;

enum{
	_SM00,_SM01,
	_SM10,_SM11
};

/*--------------------------------------*/
/*      Sprite                          */
/*--------------------------------------*/
typedef struct {
	Sint16		sx,sy;			/* size				*/
	Sint16      cx,cy;          /* center           */
	Sint16      u1,v1;          /* upper left       */
	Sint16      u2,v2;          /* lower right      */
	Sint16      texid;          /* texture id       */
	Sint16      attr;           /* attribute        */
} NJS_TEXANIM;

typedef struct {
	NJS_POINT3  p;              /* point            */
	Float       sx,sy;          /* scale            */
	Angle       ang;            /* angle            */
	NJS_TEXLIST *tlist;         /* texture list     */
	NJS_TEXANIM *tanim;         /* animation list   */
} NJS_SPRITE;

/*--------------------------------------*/
/*      Low Level                       */
/*--------------------------------------*/

typedef struct {
	Float	x;
	Float	y;
	Float	z;
	Uint32	col;
} NJS_POLYGON_VTX;

typedef struct {
	Float	x;
	Float	y;
	Float	z;
	Float	u;
	Float	v;
	Uint32	col;
} NJS_TEXTURE_VTX;

typedef struct {
	Float	x;
	Float	y;
	Float	z;
	Float	u;
	Float	v;
	Uint32	bcol;
	Uint32	ocol;
} NJS_TEXTUREH_VTX;

/*--------------------------------------*/
/*      Math                            */
/*--------------------------------------*/
typedef struct {
	int		keys;			/* keyframes between two points which must be interpolated */
	int		*iparam;		/* attribute data (integer) */
	float	*fparam;		/* attribute data (float) */
} NJS_SPLINE;

typedef	struct {
	float	re;					/* real (or scalor) part of quaternion		*/
	float	im[3];				/* imaginary (or vector) part of quaternion	*/
} NJS_QUATERNION;


/*--------------------------------------*/
/*      Multi Pass                      */
/*--------------------------------------*/

typedef struct {
	Bool			bPreSort;
	Sint32			nSize[5];
} NJS_PASS_INFO;

typedef struct {
	Sint32			nNumOfMaxPass;
	NJS_PASS_INFO	sPassInfo[8];
} NJS_VERTEXBUFFER_INFO;


/*--------------------------------------*/
/*      Quad Draw                       */
/*--------------------------------------*/

typedef struct {
	Float	x1,y1,x2,y2;
	Float	u1,v1,u2,v2;
} NJS_QUAD_TEXTURE;

typedef struct {
	Float	x,y,z;
	Float	vx1,vy1;
	Float	vx2,vy2;
	Uint32	dmy;
	Float	u,v;
	Float	vu1,vv1;
	Float	vu2,vv2;
	Uint32	dmy2[2];
} NJS_QUAD_TEXTURE_EX;




#endif /* _NINJA_STR_H_ */


#ifndef _NINJA_CNK_H_
#define _NINJA_CNK_H_

/*======================================*/
/*                                      */
/*      Chunk Table                     */
/*                                      */
/*======================================*/
/* Chunk type offset */
#define NJD_NULLOFF             0 /* null chunk (16 bits size)            */
#define NJD_BITSOFF             1 /* chunk bits offset (16 bits size)     */
#define NJD_TINYOFF             8 /* chunk tiny offset (32 bits size)     */
#define NJD_MATOFF             16 /* chunk material offset (32 bits size) */
#define NJD_VERTOFF            32 /* chunk vertex offset (32 bits size)   */
#define NJD_VOLOFF             56 /* chunk volume offset (32 bits size)   */
#define NJD_STRIPOFF           64 /* chunk strip offset                   */
#define NJD_ENDOFF            255 /* end chunk offset (16 bits size)      */

/*------------*/
/* Chunk Null */
/*------------*/
/* <Format>=[ChunkHead(16)](16 bits chunk) */
#define NJD_CN     (NJD_NULLOFF+0)/* NULL chunk for boundary control      */

/*-----------*/
/* Chunk End */
/*-----------*/
/* <Format>=[ChunkHead(16)](16 bits chunk) */
#define NJD_CE      (NJD_ENDOFF+0)

/*------------*/
/* Chunk Bits */
/*------------*/
/* <Format>=[bits(8)|ChunkHead(8)](16 bits chunk) */
/* BA  : Blend Alpha                      */
/*       13-11 = SRC Alpha Instruction(3) */
/*       10- 8 = DST Alpha Instruction(3) */
/* DA  : Mipmap 'D' adjust                */
/*       11- 8 = Mipmap 'D' adjust(4)     */
/* EXP : Specular Exponent                */
/*       12- 8 = Exponent(5) range:0-16   */
/* CP  : Cache Polygon List               */
/*       15- 8 = Cache Number range:0-255 */
/* DP  : Draw Polygon List                */
/*       15- 8 = Cache Number range:0-255 */
#define NJD_CB_BA   (NJD_BITSOFF+0)
#define NJD_CB_DA   (NJD_BITSOFF+1)
#define NJD_CB_EXP  (NJD_BITSOFF+2)
#define NJD_CB_CP   (NJD_BITSOFF+3)
#define NJD_CB_DP   (NJD_BITSOFF+4)


/*------------*/
/* Chunk Tiny */
/*------------*/
/* <Format>=[headbits(8)|ChunkHead(8)][texbits(3)|TexId(13)] (32 bits chunk) */
/* TID : Set Texture                      */
/*     <headbits>                         */
/*       15-14 = FlipUV(2)                */
/*       13-12 = ClampUV(2)               */
/*       11- 8 = Mipmap 'D' adjust(4)     */
/*     <texbits>                          */
/*       15-14 = Filter Mode(2)           */
/*       13    = Super Sample(1)          */
/*     (TexId Max = 8191)                 */
#define NJD_CT_TID  (NJD_TINYOFF+0)

/* 2para modifier */
#define NJD_CT_TID2 (NJD_TINYOFF+1)

/*----------------*/
/* Chunk Material */
/*----------------*/
/* <Format>=[ChunkHead][Size][Data]                        */
/*       13-11 = SRC Alpha Instruction(3)                  */
/*       10- 8 = DST Alpha Instruction(3)                  */
/* D  : Diffuse (ARGB)                            bit 0    */
/* A  : Ambient (RGB)                             bit 1    */
/* S  : Specular(ERGB) E:exponent(5) range:0-16   bit 2    */
#define NJD_CM_D    (NJD_MATOFF+1)  /* [CHead][4(Size)][ARGB]              */
#define NJD_CM_A    (NJD_MATOFF+2)  /* [CHead][4(Size)][NRGB] N: NOOP(255) */
#define NJD_CM_DA   (NJD_MATOFF+3)  /* [CHead][8(Size)][ARGB][NRGB]        */
#define NJD_CM_S    (NJD_MATOFF+4)  /* [CHead][4(Size)][ERGB] E: Exponent  */
#define NJD_CM_DS   (NJD_MATOFF+5)  /* [CHead][8(Size)][ARGB][ERGB]        */
#define NJD_CM_AS   (NJD_MATOFF+6)  /* [CHead][8(Size)][NRGB][ERGB]        */
#define NJD_CM_DAS  (NJD_MATOFF+7)  /* [CHead][12(Size)][ARGB][NRGB][ERGB] */

/* Bump */
#define NJD_CM_BU   (NJD_MATOFF+8)  /* [CHead][12(Size)][dx(16)][dy(16)][dz(16)]
									   [ux(16)][uy(16)][uz(16)]            */

/* 2para modifier */
#define NJD_CM_D2   (NJD_MATOFF+9)  /* [CHead][4(Size)][ARGB]              */
#define NJD_CM_A2   (NJD_MATOFF+10) /* [CHead][4(Size)][NRGB] N: NOOP(255) */
#define NJD_CM_DA2  (NJD_MATOFF+11) /* [CHead][8(Size)][ARGB][NRGB]        */
#define NJD_CM_S2   (NJD_MATOFF+12) /* [CHead][4(Size)][ERGB] E: Exponent  */
#define NJD_CM_DS2  (NJD_MATOFF+13) /* [CHead][8(Size)][ARGB][ERGB]        */
#define NJD_CM_AS2  (NJD_MATOFF+14) /* [CHead][8(Size)][NRGB][ERGB]        */
#define NJD_CM_DAS2 (NJD_MATOFF+15) /* [CHead][12(Size)][ARGB][NRGB][ERGB] */

/*--------------*/
/* Chunk Vertex */
/*--------------*/
/* <Format>=[headbits(8)|ChunkHead(8)]                                    */
/*          [Size(16)][IndexOffset(16)][nbIndices(16)]                    */
/*     <headbits>(NF only)                                                */
/*        9- 8 = WeightStatus(2) Start, Middle, End                       */
/* VN  : use vertex normal                                                */
/* VNX : 32bits vertex normal  reserved(2)|x(10)|y(10)|z(10)              */
/* SH  : SH4 optimize                                                     */
/* D8  : Diffuse ARGB8888 only                                            */
/* S5  : Diffuse RGB565 and Specular RGB565                               */
/* S4  : Diffuse RGB4444 and Specular RGB565                              */
/* IN  : Diffuse(16)|Specular(16)                                         */
/* NF  : NinjaFlags32 for extention                                       */
/* UF  : UserFlags32                                                      */
 
/* optimize for SH4 */
#define NJD_CV_SH     (NJD_VERTOFF+0)  /* x,y,z,1.0F, ...                 */
#define NJD_CV_VN_SH  (NJD_VERTOFF+1)  /* x,y,z,1.0F,nx,ny,nz,0.0F,...    */

/* chunk vertex */
#define NJD_CV        (NJD_VERTOFF+2)  /* x,y,z, ...                      */
#define NJD_CV_D8     (NJD_VERTOFF+3)  /* x,y,z,D8888,...                 */  
#define NJD_CV_UF     (NJD_VERTOFF+4)  /* x,y,z,UserFlags32, ...          */
#define NJD_CV_NF     (NJD_VERTOFF+5)  /* x,y,z,NinjaFlags32,...          */
#define NJD_CV_S5     (NJD_VERTOFF+6)  /* x,y,z,D565|S565,...             */
#define NJD_CV_S4     (NJD_VERTOFF+7)  /* x,y,z,D4444|S565,...            */
#define NJD_CV_IN     (NJD_VERTOFF+8)  /* x,y,z,D16|S16,...               */
 
#define NJD_CV_VN     (NJD_VERTOFF+9)  /* x,y,z,nx,ny,nz, ...             */
#define NJD_CV_VN_D8  (NJD_VERTOFF+10) /* x,y,z,nx,ny,nz,D8888,...        */
#define NJD_CV_VN_UF  (NJD_VERTOFF+11) /* x,y,z,nx,ny,nz,UserFlags32,...  */
#define NJD_CV_VN_NF  (NJD_VERTOFF+12) /* x,y,z,nx,ny,nz,NinjaFlags32,... */
#define NJD_CV_VN_S5  (NJD_VERTOFF+13) /* x,y,z,nx,ny,nz,D565|S565,...    */
#define NJD_CV_VN_S4  (NJD_VERTOFF+14) /* x,y,z,nx,ny,nz,D4444|S565,...   */
#define NJD_CV_VN_IN  (NJD_VERTOFF+15) /* x,y,z,nx,ny,nz,D16|S16,...      */

#define NJD_CV_VNX    (NJD_VERTOFF+16) /* x,y,z,nxyz32, ...               */
#define NJD_CV_VNX_D8 (NJD_VERTOFF+17) /* x,y,z,nxyz32,D8888,...          */
#define NJD_CV_VNX_UF (NJD_VERTOFF+18) /* x,y,z,nxyz32,UserFlags32,...    */


/*--------------*/
/* Chunk vOlume */
/*--------------*/
/* UserFlags N=0,1(16bit*1),2(16bit*2),3(16bit*3)                         */
/* <Format P3>=[ChunkHead(16)][Size(16)][UserOffset(2)|nbPolygon(14)]     */
/*            i0, i1, i2, UserflagPoly0(*N),                              */
/*            i3, i4, i5, UserflagPoly1(*N), ...                          */
/* <Format P4>=[ChunkHead(16)][Size(16)][UserOffset(2)|nbPolygon(14)]     */
/*            i0, i1, i2, i3, UserflagPoly0(*N),                          */
/*            i4, i5, i6, i7, UserflagPoly1(*N), ...                      */
/* <Format ST>=[ChunkHead(16)][Size(16)][UserOffset(2)|nbStrip(14)]       */
/*          [flag|len, i0, i1, i2, Userflag2(*N), i3, Userflag3(*N), ...  */
/* P3  : Polygon3     (Modifier Volume)                                   */
/* P4  : Polygon4                                                         */
/* ST  : triangle STrip(Trimesh)                                          */

#define NJD_CO_P3       (NJD_VOLOFF+0)
#define NJD_CO_P4       (NJD_VOLOFF+1)
#define NJD_CO_ST       (NJD_VOLOFF+2)

/*-------------*/
/* Chunk Strip */
/*-------------*/
/* UserFlags N=0,1(16bit*1),2(16bit*2),3(16bit*3)                         */
/* <Format1>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15), index0(16), index1(16),                    */
/*            index2, UserFlag2(*N), ...]                                 */
/* <Format2>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), U0(16), V0(16),                                 */
/*            index1, U1, V1,                                             */
/*            index2, U2, V2, UserFlag2(*N), ... ]                        */
/* <Format3>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), vnx0(16), vny0(16), vnz0(16),                   */
/*            index1, vnx1, vny1, vny1,                                   */
/*            index2, vnx2, vny2, vnz2, UserFlag2(*N),                    */
/*            index3, vnx2, vny2, vnz2, UserFlag3(*N), ... ]              */
/* <Format4>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), U0(16), V0(16), vnx0(16), vny0(16), vnz0(16),   */
/*            index1, U1, V1, vnx1, vny1, vny1,                           */
/*            index2, U2, V2, vnx2, vny2, vnz2, UserFlag2(*N),            */
/*            index3, U3, V3, vnx3, vny3, vnz3, UserFlag3(*N), ... ]      */
/* <Format5>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), AR0(16), GB0(16),                               */
/*            index1, AR1, GB1,                                           */
/*            index2, AR2, GB2, UserFlag2(*N),                            */
/*            index3, AR3, GB3, UserFlag3(*N), ... ]                      */
/* <Format6>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), U0(16), V0(16), AR0(16), GB0(16),               */
/*            index1, U1, V1, AR1, GB1,                                   */
/*            index2, U2, V2, AR2, GB2, UserFlag2(*N), ... ]              */
/*                                                                        */
/* << 2para modifiler >>                                                  */
/* <Format7>=[CFlags(8)|CHead(8)][Size(16)][UserOffset(2)|nbStrip(14)]    */
/*           [flag(1)|len(15),                                            */
/*            index0(16), U0a(16), V0a(16), U0b(16), V0b(16),             */
/*            index1, U1a, V1a, U1b, V1b,                                 */
/*            index2, U2a, V2a, U2b, V2b, UserFlag2(*N), ... ]            */
/*                                                                        */
/* UV  : Uv (0-255)                                                       */
/* UVH : Uv (0-1023)                                                      */
/* VN  : Vertex Normal                                                    */
/* D8  : Strip Color ARGB8888                                             */

#define NJD_CS          (NJD_STRIPOFF+0)  /* <Format1>   */
#define NJD_CS_UVN      (NJD_STRIPOFF+1)  /* <Format2>   */
#define NJD_CS_UVH      (NJD_STRIPOFF+2)  /* <Format2>   */

#define NJD_CS_VN       (NJD_STRIPOFF+3)  /* <Format3>   */
#define NJD_CS_UVN_VN   (NJD_STRIPOFF+4)  /* <Format4>   */
#define NJD_CS_UVH_VN   (NJD_STRIPOFF+5)  /* <Format4>   */

#define NJD_CS_D8       (NJD_STRIPOFF+6)  /* <Format5>   */
#define NJD_CS_UVN_D8   (NJD_STRIPOFF+7)  /* <Format6>   */
#define NJD_CS_UVH_D8   (NJD_STRIPOFF+8)  /* <Format6>   */

/* 2para modifier */
#define NJD_CS_2        (NJD_STRIPOFF+9)  /* <Format1>   */
#define NJD_CS_UVN2     (NJD_STRIPOFF+10) /* <Format7>   */
#define NJD_CS_UVH2     (NJD_STRIPOFF+11) /* <Format7>   */

/*======================================*/
/*                                      */
/*      Chunk Flags                     */
/*                                      */
/*======================================*/
/*------------*/
/* Chunk Bits */
/*------------*/

/** Flags Blending SRC **/

/* ZER : Zero                                            */
/* ONE : One                                             */
/* OC  : `Other' Color                                   */
/* IOC : Inverse `Other' Color                           */
/* SA  : Src Alpha                                       */
/* ISA : Inverse SRC Alpha                               */
/* DA  : DST Alpha                                       */
/* IDA : Inverse DST Alpha                               */

/* Flag Blending Src */

#define NJD_FBS_SHIFT     11
#define NJD_FBS_ZER      (0<<NJD_FBS_SHIFT)
#define NJD_FBS_ONE      (1<<NJD_FBS_SHIFT)
#define NJD_FBS_OC       (2<<NJD_FBS_SHIFT)
#define NJD_FBS_IOC      (3<<NJD_FBS_SHIFT)
#define NJD_FBS_SA       (4<<NJD_FBS_SHIFT)
#define NJD_FBS_ISA      (5<<NJD_FBS_SHIFT)
#define NJD_FBS_DA       (6<<NJD_FBS_SHIFT)
#define NJD_FBS_IDA      (7<<NJD_FBS_SHIFT)

#define NJD_FBS_MASK     (0x7<<NJD_FBS_SHIFT)

/* Flag Blending Dst */

#define NJD_FBD_SHIFT     8
#define NJD_FBD_ZER      (0<<NJD_FBD_SHIFT)
#define NJD_FBD_ONE      (1<<NJD_FBD_SHIFT)
#define NJD_FBD_OC       (2<<NJD_FBD_SHIFT)
#define NJD_FBD_IOC      (3<<NJD_FBD_SHIFT)
#define NJD_FBD_SA       (4<<NJD_FBD_SHIFT)
#define NJD_FBD_ISA      (5<<NJD_FBD_SHIFT)
#define NJD_FBD_DA       (6<<NJD_FBD_SHIFT)
#define NJD_FBD_IDA      (7<<NJD_FBD_SHIFT)

#define NJD_FBD_MASK     (0x7<<NJD_FBD_SHIFT)

/** Flag 'D' Adjust **/

#define NJD_FDA_SHIFT     8
#define NJD_FDA_025      (1<<NJD_FDA_SHIFT)      /* 0.25 */
#define NJD_FDA_050      (2<<NJD_FDA_SHIFT)      /* 0.50 */
#define NJD_FDA_075      (3<<NJD_FDA_SHIFT)      /* 0.75 */
#define NJD_FDA_100      (4<<NJD_FDA_SHIFT)      /* 1.00 */
#define NJD_FDA_125      (5<<NJD_FDA_SHIFT)      /* 1.25 */
#define NJD_FDA_150      (6<<NJD_FDA_SHIFT)      /* 1.50 */
#define NJD_FDA_175      (7<<NJD_FDA_SHIFT)      /* 1.75 */
#define NJD_FDA_200      (8<<NJD_FDA_SHIFT)      /* 2.00 */
#define NJD_FDA_225      (9<<NJD_FDA_SHIFT)      /* 2.25 */
#define NJD_FDA_250      (10<<NJD_FDA_SHIFT)     /* 2.25 */
#define NJD_FDA_275      (11<<NJD_FDA_SHIFT)     /* 2.25 */
#define NJD_FDA_300      (12<<NJD_FDA_SHIFT)     /* 3.00 */
#define NJD_FDA_325      (13<<NJD_FDA_SHIFT)     /* 3.25 */
#define NJD_FDA_350      (14<<NJD_FDA_SHIFT)     /* 3.50 */
#define NJD_FDA_375      (15<<NJD_FDA_SHIFT)     /* 3.75 */

#define NJD_FDA_MASK     (0xf<<NJD_FDA_SHIFT)

/** Flag EXPonent range : 0-16 **/

#define NJD_FEXP_SHIFT    8
#define NJD_FEXP_00      (0<<NJD_FEXP_SHIFT)     /*  0.0 */
#define NJD_FEXP_01      (1<<NJD_FEXP_SHIFT)     /*  1.0 */
#define NJD_FEXP_02      (2<<NJD_FEXP_SHIFT)     /*  2.0 */
#define NJD_FEXP_03      (3<<NJD_FEXP_SHIFT)     /*  3.0 */
#define NJD_FEXP_04      (4<<NJD_FEXP_SHIFT)     /*  4.0 */
#define NJD_FEXP_05      (5<<NJD_FEXP_SHIFT)     /*  5.0 */
#define NJD_FEXP_06      (6<<NJD_FEXP_SHIFT)     /*  6.0 */
#define NJD_FEXP_07      (7<<NJD_FEXP_SHIFT)     /*  7.0 */
#define NJD_FEXP_08      (8<<NJD_FEXP_SHIFT)     /*  8.0 */
#define NJD_FEXP_09      (9<<NJD_FEXP_SHIFT)     /*  9.0 */
#define NJD_FEXP_10      (10<<NJD_FEXP_SHIFT)    /* 10.0 */
#define NJD_FEXP_11      (11<<NJD_FEXP_SHIFT)    /* 11.0 */
#define NJD_FEXP_12      (12<<NJD_FEXP_SHIFT)    /* 12.0 */
#define NJD_FEXP_13      (13<<NJD_FEXP_SHIFT)    /* 13.0 */
#define NJD_FEXP_14      (14<<NJD_FEXP_SHIFT)    /* 14.0 */
#define NJD_FEXP_15      (15<<NJD_FEXP_SHIFT)    /* 15.0 */
#define NJD_FEXP_16      (16<<NJD_FEXP_SHIFT)    /* 16.0 */

#define NJD_FEXP_MASK    (0x1f<<NJD_FEXP_SHIFT)


/*--------------------*/
/* Chunk Multi Lights */
/*--------------------*/

#define	NJD_CNK_EASY_MULTILIGHT_1			1
#define	NJD_CNK_EASY_MULTILIGHT_2			2
#define	NJD_CNK_EASY_MULTILIGHT_3			3
#define	NJD_CNK_EASY_MULTILIGHT_4			4
#define	NJD_CNK_EASY_MULTILIGHT_5			5
#define	NJD_CNK_EASY_MULTILIGHT_6			6

#define	NJD_CNK_EASY_MULTILIGHT_ALL			0

#define	NJD_CNK_SIMPLE_MULTILIGHT_1			1
#define	NJD_CNK_SIMPLE_MULTILIGHT_2			2
#define	NJD_CNK_SIMPLE_MULTILIGHT_3			3
#define	NJD_CNK_SIMPLE_MULTILIGHT_4			4
#define	NJD_CNK_SIMPLE_MULTILIGHT_5			5
#define	NJD_CNK_SIMPLE_MULTILIGHT_6			6

#define	NJD_CNK_SIMPLE_MULTILIGHT_ALL		0


/*------------*/
/* Chunk Tiny */
/*------------*/
/** TexId **/
/* Flag FLip <headbits> */
#define NJD_FFL_U         (BIT_15)
#define NJD_FFL_V         (BIT_14)
/* Flag CLamp <headbits> */
#define NJD_FCL_U         (BIT_13)
#define NJD_FCL_V         (BIT_12)

  /* Look at Chunk Bits about 'D' adust */

/* Flag Filter Mode<texbits> */
/* PS   : Point Sampled                   */
/* BF   : Bilinear Filter                 */
/* TF_A : Tri-liner Filter A              */
/* TF_B : Tri-liner Filter B              */
#define NJD_FFM_SHIFT      14
#define NJD_FFM_PS        (0<<NJD_FFM_SHIFT)
#define NJD_FFM_BF        (1<<NJD_FFM_SHIFT)
#define NJD_FFM_TF_A      (2<<NJD_FFM_SHIFT)
#define NJD_FFM_TF_B      (3<<NJD_FFM_SHIFT)

#define NJD_FFM_MASK      (0x3<<NJD_FFM_SHIFT)

/* Flag Super Sample<texbits> */
#define NJD_FSS           (BIT_13)

#define NJD_TID_MASK      (~(NJD_FSS|NJD_FFM_MASK))

/*--------------*/
/* Chunk Vertex */
/*--------------*/
/* Flag Weight status (NF only) */
#define NJD_FW_SHIFT     8
#define NJD_FW_START    (0<<NJD_FW_SHIFT)      /* Start  */
#define NJD_FW_MIDDLE   (1<<NJD_FW_SHIFT)      /* Middle */
#define NJD_FW_END      (2<<NJD_FW_SHIFT)      /* End    */

#define NJD_FW_MASK     (0x2<<NJD_FW_SHIFT)

/* Flag Vertex */
#define NJD_FV_SHIFT     8
#define NJD_FV_CONT     (0x80<<NJD_FV_SHIFT)   /* vertex calculation continue */

/*-------------*/
/* Chunk Strip */
/*-------------*/
/** Flag STrip **/
/* <Format>=[ChunkFlags(8)|ChunkHead(8)](16 bits size)   */
/* IL  : Ignore light                                    */
/* IS  : Ignore specular                                 */
/* IA  : Ignore ambient                                  */
/* UA  : Use alpha                                       */
/* DB  : Double side                                     */
/* FL  : Flat shading                                    */
/* ENV : Environment                                     */
#define NJD_FST_SHIFT      8
#define NJD_FST_IL        (0x01<<NJD_FST_SHIFT)
#define NJD_FST_IS        (0x02<<NJD_FST_SHIFT)
#define NJD_FST_IA        (0x04<<NJD_FST_SHIFT)
#define NJD_FST_UA        (0x08<<NJD_FST_SHIFT)
#define NJD_FST_DB        (0x10<<NJD_FST_SHIFT)
#define NJD_FST_FL        (0x20<<NJD_FST_SHIFT)
#define NJD_FST_ENV       (0x40<<NJD_FST_SHIFT)
#define NJD_FST_MASK      (0xFF<<NJD_FST_SHIFT)

/** UserFlag Offset Mask **/
#define NJD_UFO_SHIFT      14
#define NJD_UFO_0         (0<<NJD_UFO_SHIFT)
#define NJD_UFO_1         (1<<NJD_UFO_SHIFT)
#define NJD_UFO_2         (2<<NJD_UFO_SHIFT)
#define NJD_UFO_3         (3<<NJD_UFO_SHIFT)
#define NJD_UFO_MASK      (0x3<<NJD_UFO_SHIFT) /* 0 - 3 */


/*------------------*/
/*	Structure		*/
/*------------------*/
/*
 * NJS_CNK_MODEL
 */
struct NJS_CNK_MODEL {
	Sint32            *vlist;   /* vertex list                  */
	Sint16            *plist;   /* polygon list                 */
	NJS_POINT3        center;   /* model center                 */
	Float                  r;   /* radius                       */
};


/** Vertex Buffer    **/
typedef    struct    {
	Sint32    clip;             /* clip flag        00:04*/
	Float    sx;                /* Screen X         04:08*/
	Float    sy;                /* Screen Y         08:12*/
	Float    ooz;               /* One over Z       12:16*/
	NJS_POINT3    p;            /* 3D Point         16:28*/
	Uint32    dmy1;             /* dummy            28:32*/
	Sint32    vtype;            /* vetex type       32:36*/
	NJS_VECTOR    v;            /* Normal vector    36:48*/
	union  {                    /*                  48:56*/
		Uint32    argb[2];      /* Packed Color     48:56*/
		Float    inten[2];      /* intensity        48:56*/
	} col;                      /* color            48:56*/
	Sint32    NinjaFlag;        /* Ninja Flag       56:60*/
	Sint32    UserFlag;         /* User Flag        60:64*/
} NJS_CNK_VERTEX_BUF;

typedef    struct    {
	Float    x;                /* Screen X                */
	Float    y;                /* Screen Y                */
	Float    ooz;              /* One over Z              */
	Float    inten;            /* intensity               */
} NJS_CNK_EASY_BUF;

typedef    struct    {
	Float    z;                /* 3D Z                    */
	Float    x;                /* Screen X                */
	Float    y;                /* Screen Y                */
	Float    ooz;              /* One over Z              */
	Float    inten;            /* Intensity               */
	Float    spec;             /* Specular                */
	Float    nx;               /* Normal X                */
	Float    ny;               /* Normal Y                */
} NJS_CNK_SIMPLE_BUF;

typedef    struct    {
	Float    z;                /* 3D Z                    */
	Float    x;                /* Screen X                */
	Float    y;                /* Screen Y                */
	Float    ooz;              /* One over Z              */
	Float    a;                /* Alpha                   */
	Float    r;                /* Red                     */
	Float    g;                /* Green                   */
	Float    b;                /* Blue                    */
} NJS_CNK_EASYMULTI_BUF;

#endif /* _NINJA_CNK_H_ */

#ifndef _NINJA_DIR_H_
#define _NINJA_DIR_H_

typedef struct {
	Float		x,y,z;
	Float		r;
	void		*next;
	Uint32		nOP,nTR;
	Uint32		mode;					/* 0:normal 1:D8				*/
} NJS_DIRECT_HEAD;

typedef struct {
	Uint32		GP;
	Uint32		mode;					/* 0:normal 1:environment map	*/
	Float		sg,sb,sa,sr;
	Float		dg,db,da,dr;
	Uint32		TSPPARAM,TEXTUREADDR;
	Uint32		GLOBALPARAM,ISPPARAM;
	Uint32		num;
	Uint32		texid;
} NJS_DIRECT_GLOBAL;

typedef struct {
	Uint32		GP;
	Uint32		mode;
	Uint32		TSPPARAM,TEXTUREADDR;
	Uint32		GLOBALPARAM,ISPPARAM;
	Uint32		num;
	Uint32		texid;
} NJS_DIRECT_GLOBAL_D8;

typedef struct {
	Float		px,py,pz;
	Float		nz,nx,ny;
	Uint32		uv;
	Uint32		param;
} NJS_DIRECT_VERTEX;

typedef struct {
	Float		px,py,pz,one;
	Uint32		base,off;
	Uint32		param;
	Uint32		uv;
} NJS_DIRECT_VERTEX_D8;

typedef struct {
	Float		px,py,pz;
	Float		nx,ny,nz;
	Uint32		col;
	Uint32		dmy;
} NJS_DIRECT_BUF;

typedef struct {
	Float	x,y,z;
	Float	inten,ambient;
	Float	r,g,b;
} NJS_DIRECT_COMPILE_LIGHT;

#endif /* _NINJA_DIR_H_ */

static inline Sint16 *NextChunk(Sint16 *chunk)
{
	unsigned char v5 = (unsigned char)*chunk++;
	if (v5 == NJD_CE)
		return nullptr;
	if (v5 >= NJD_TINYOFF)
	{
		if (v5 >= NJD_MATOFF)
			return chunk + *chunk + 1;
		else
			return chunk + 1;
	}
	return chunk;
}

static inline Sint16 *FindChunk(Sint16 *chunk, unsigned char type)
{
	chunk = NextChunk(chunk);
	while (chunk != nullptr)
		if ((unsigned char)*chunk == type)
			return chunk;
		else
			chunk = NextChunk(chunk);
	return chunk;
}

static inline Sint32 *NextChunk(Sint32 *chunk)
{
	unsigned char v5 = (unsigned char)*chunk++;
	if (v5 == NJD_CE)
		return nullptr;
	if (v5 >= NJD_TINYOFF)
	{
		if (v5 >= NJD_MATOFF)
			return chunk + *chunk + 1;
		else
			return chunk + 1;
	}
	return chunk;
}

static inline Sint32 *FindChunk(Sint32 *chunk, unsigned char type)
{
	chunk = NextChunk(chunk);
	while (chunk != nullptr)
		if ((unsigned char)*chunk == type)
			return chunk;
		else
			chunk = NextChunk(chunk);
	return chunk;
}

/*
 * End Of File
 */

#endif /* SEGA_NINJA_H */
