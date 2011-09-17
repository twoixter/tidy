/*
 * Copyright (c) 2011 Jose Miguel Pérez, Twoixter S.L.
 *
 * Standard disclaimer follows:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef TIDY_COLORSTREAMS_H
#define TIDY_COLORSTREAMS_H

#include <iostream>
#include <unistd.h>   // For isatty

#define IMPLEMENT(color) \
    inline std::ostream & (color)(ostream &_out) \
    { if (can_colorize) _out << _##color; return _out; }

namespace ansi {

    /**
     * Since we are using color_streams.h as a header to be inlined, we need
     * to declare can_colorize as a weak symbol so that there is no complains
     * by the linker. However, __attribute__ is a GNU extension.
     */
    bool can_colorize __attribute__((weak)) = isatty(STDOUT_FILENO);

    const char _reset[]   = "\x1b[0m";

    const char _red[]     = "\x1b[31m";
    const char _green[]   = "\x1b[32m";
    const char _yellow[]  = "\x1b[33m";
    const char _blue[]    = "\x1b[34m";
    const char _magenta[] = "\x1b[35m";
    const char _cyan[]    = "\x1b[36m";
    const char _white[]   = "\x1b[37m";

    IMPLEMENT(reset)

    IMPLEMENT(red)
    IMPLEMENT(green)
    IMPLEMENT(yellow)
    IMPLEMENT(blue)
    IMPLEMENT(magenta)
    IMPLEMENT(cyan)
    IMPLEMENT(white)
}

#endif
