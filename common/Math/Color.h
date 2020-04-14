/*
    MIT License

    Copyright (c) 2018-2020 NovusCore

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#pragma once

struct Color
{
    Color(float inR, float inG, float inB, float inA = 1.0f)
        : r(inR)
        , g(inG)
        , b(inB)
        , a(inA)
    {
        
    }

    Color()
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , a(1.0f)
    {

    }

    float r;
    float g;
    float b;
    float a;

    static Color Black;
    static Color Blue;
    static Color Clear;
    static Color Cyan;
    static Color Gray;
    static Color Green;
    static Color Grey;
    static Color Magenta;
    static Color Red;
    static Color White;
    static Color Yellow;
};