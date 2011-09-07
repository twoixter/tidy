/*
 * Copyright (c) 2011 Jose Miguel Pérez, Twoixter S.L.
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

#ifndef TIDY_HTMLTOKEN_H
#define TIDY_HTMLTOKEN_H

#include <string>

using namespace std;

class htmlToken {
public:
    typedef enum {
        text,
        tag,
        doctype,
        comment
    } tokenType_t;

    htmlToken(tokenType_t _t = text)
        : m_type(_t)
    {}

    htmlToken(const string &_str, tokenType_t _t = text)
        : m_type(_t)
        , name(_str)
    {}

    // Yep... This structure sounds silly, let's see if I come up with a better one. :-D
    string name;
    string extra;
    const string content() const
    {
        return name;
    }

    tokenType_t type()
    {
        return m_type;
    }

    ostream &serialize(ostream &_out) const;

private:
    tokenType_t m_type;
};

inline ostream & operator << (ostream &_out, const htmlToken &_tok)
{
    return _tok.serialize(_out);
}

#endif  // TIDY_HTMLTOKEN_H
