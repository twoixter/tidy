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

#include <iostream>
#include "tidy_parser.h"
#include "color_streams.h"

using namespace std;

tidyParser::tidyParser()
    : htmlParser()
    , m_level(0)
    , m_inScript(false)
    , m_inStyle(false)
{
}

tidyParser::~tidyParser()
{
}

void tidyParser::startDocument()
{
    m_level = 0;
    m_inScript = false;
    m_inStyle = false;
}

void tidyParser::endDocument()
{
}

void tidyParser::openingTag(const htmlToken &_token)
{
    if (_token == "script") m_inScript = true;
    if (_token == "style")  m_inStyle = true;
    cout << levelstr() << ansi::white << _token << ansi::reset << endl;
    if (_token.type() == htmlToken::tag) m_level++;
}

void tidyParser::textNode(const htmlToken &_token)
{
    if (m_inScript || m_inStyle) {
        cout << levelstr(1) << _token.name() << endl;
    } else {
        cout << levelstr(1) << _token << endl;
    }
}

void tidyParser::closingTag(const htmlToken &_token)
{
    m_level--;
    if (!_token.isSelfClose()) {
        cout << levelstr() << ansi::white << _token << ansi::reset << endl;
    }
    if (_token == "script") m_inScript = false;
    if (_token == "style")  m_inStyle = false;
}
