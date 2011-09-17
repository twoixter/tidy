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

#ifndef TIDY_PARSER_H
#define TIDY_PARSER_H

#include "html_stream_parser.h"

class tidyParser : public htmlParser {

public:
    tidyParser();
    virtual ~tidyParser();

private:
    virtual void startDocument();
    virtual void openingTag(const htmlToken &_token);
    virtual void textNode(const htmlToken &_token);
    virtual void closingTag(const htmlToken &_token);
    virtual void endDocument();

    inline const string levelstr(int _plus = 0)
    {
        return m_level + _plus > 0 ? string((m_level + _plus - 1) * 4, ' ') : string();
    }

    int m_level;
    bool m_inScript;
    bool m_inStyle;
};

#endif
