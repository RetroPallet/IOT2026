/**
 * @file fonts.h
 * @brief Bitmap font descriptors and dimensions.
 */
/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   Header for fonts.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

/* Max size of bitmap will based on a font24 (17x24) */
/**
 * @brief Maximum bundled glyph height in pixels.
 */
#define MAX_HEIGHT_FONT         24
/**
 * @brief Maximum bundled glyph width in pixels.
 */
#define MAX_WIDTH_FONT          17
/**
 * @brief Bitmap file pixel-data offset in bytes.
 */
#define OFFSET_BITMAP           54

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * @brief Bitmap font data and glyph dimensions.
 */
typedef struct _tFont
{    
  const uint8_t *table;
  /**
   * @brief Glyph width in pixels.
   */
  uint16_t Width;
  /**
   * @brief Glyph height in pixels.
   */
  uint16_t Height;
  
} sFONT;

/** @brief Font descriptor for glyphs 24 pixels high. */
extern sFONT Font24;
/** @brief Font descriptor for glyphs 20 pixels high. */
extern sFONT Font20;
/** @brief Font descriptor for glyphs 16 pixels high. */
extern sFONT Font16;
/** @brief Font descriptor for glyphs 12 pixels high. */
extern sFONT Font12;
/** @brief Font descriptor for glyphs 8 pixels high. */
extern sFONT Font8;

#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
