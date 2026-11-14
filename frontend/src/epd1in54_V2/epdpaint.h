/**
 * @file epdpaint.h
 * @brief Monochrome bitmap drawing primitives.
 */
/**
 *  File   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     July 28 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDPAINT_H
#define EPDPAINT_H

// Display orientation
/**
 * @brief Drawing orientation constant.
 */
#define ROTATE_0            0
/**
 * @brief Drawing orientation constant.
 */
#define ROTATE_90           1
/**
 * @brief Drawing orientation constant.
 */
#define ROTATE_180          2
/**
 * @brief Drawing orientation constant.
 */
#define ROTATE_270          3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
/**
 * @brief Invert the stored bit for a colored pixel when enabled.
 */
#define IF_INVERT_COLOR     1

#include "fonts.h"

/**
 * @brief Drawing operations on a caller-owned one-bit image buffer.
 */
class Paint {
public:
    /**
     * @brief Initialize the Paint object.
     * @param image Caller-owned packed one-bit image buffer.
     * @param width Width in pixels.
     * @param height Height in pixels.
     * @note The image buffer remains owned by the caller and must fit the requested dimensions.
     */
    Paint(unsigned char* image, int width, int height);
    /**
     * @brief Release the Paint object.
     */
    ~Paint();
    /**
     * @brief Fill the image buffer with the requested color.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void Clear(int colored);
    /**
     * @brief Read the drawing-buffer width.
     * @return The width in pixels.
     */
    int  GetWidth(void);
    /**
     * @brief Set the drawing-buffer width, rounding up to a multiple of eight pixels.
     * @param width Width in pixels.
     * @note The caller must provide enough image-buffer storage.
     */
    void SetWidth(int width);
    /**
     * @brief Read the drawing-buffer height.
     * @return The height in pixels.
     */
    int  GetHeight(void);
    /**
     * @brief Set the drawing-buffer height in pixels.
     * @param height Height in pixels.
     * @note The caller must provide enough image-buffer storage.
     */
    void SetHeight(int height);
    /**
     * @brief Read the drawing rotation.
     * @return The current ROTATE_* orientation value.
     */
    int  GetRotate(void);
    /**
     * @brief Set the drawing orientation.
     * @param rotate One of ROTATE_0, ROTATE_90, ROTATE_180 or ROTATE_270.
     */
    void SetRotate(int rotate);
    /**
     * @brief Access the caller-owned monochrome image buffer.
     * @return A pointer to the image buffer.
     */
    unsigned char* GetImage(void);
    /**
     * @brief Set one pixel using physical buffer coordinates without rotation.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawAbsolutePixel(int x, int y, int colored);
    /**
     * @brief Set one pixel after applying the configured rotation.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawPixel(int x, int y, int colored);
    /**
     * @brief Draw one ASCII character into the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param ascii_char ASCII character to render.
     * @param font Font descriptor used for rendering.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
    /**
     * @brief Draw a null-terminated text string into the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param text Text to draw.
     * @param font Font descriptor used for rendering.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored);
    /**
     * @brief Draw a line between two endpoints in the image buffer.
     * @param x0 Horizontal coordinate of the first endpoint.
     * @param y0 Vertical coordinate of the first endpoint.
     * @param x1 Horizontal coordinate of the second endpoint.
     * @param y1 Vertical coordinate of the second endpoint.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawLine(int x0, int y0, int x1, int y1, int colored);
    /**
     * @brief Draw a horizontal line into the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param line_width Width in pixels.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawHorizontalLine(int x, int y, int line_width, int colored);
    /**
     * @brief Draw a vertical line into the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param line_height Height in pixels.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawVerticalLine(int x, int y, int line_height, int colored);
    /**
     * @brief Draw a rectangle outline in the image buffer.
     * @param x0 Horizontal coordinate of the first endpoint.
     * @param y0 Vertical coordinate of the first endpoint.
     * @param x1 Horizontal coordinate of the second endpoint.
     * @param y1 Vertical coordinate of the second endpoint.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
    /**
     * @brief Draw a filled rectangle in the image buffer.
     * @param x0 Horizontal coordinate of the first endpoint.
     * @param y0 Vertical coordinate of the first endpoint.
     * @param x1 Horizontal coordinate of the second endpoint.
     * @param y1 Vertical coordinate of the second endpoint.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
    /**
     * @brief Draw a circle outline in the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param radius Circle radius in pixels.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawCircle(int x, int y, int radius, int colored);
    /**
     * @brief Draw a filled circle in the image buffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param radius Circle radius in pixels.
     * @param colored Pixel color selector interpreted using IF_INVERT_COLOR.
     */
    void DrawFilledCircle(int x, int y, int radius, int colored);

private:
    /**
     * @brief Packed monochrome drawing buffer.
     */
    unsigned char* image;
    /**
     * @brief Image or display width in pixels.
     */
    int width;
    /**
     * @brief Image or display height in pixels.
     */
    int height;
    /**
     * @brief Current ROTATE_* drawing orientation.
     */
    int rotate;
};

#endif

/* END OF FILE */

