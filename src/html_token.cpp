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

#include "html_token.h"

#include <iostream>

ostream &htmlToken::display(ostream &_out) const
{
    switch (m_type) {
        case text:
            _out << data();
            break;

        case tag:
            _out << "<";
            if (isClose()) _out << "/";
            _out << name();
            if (m_attrs.size()) {
                for (htmlAttributes::const_iterator iter = m_attrs.begin()
                    ; iter != m_attrs.end()
                    ; ++iter)
                {
                    _out << " " << iter->first << "=\"" << iter->second << "\"";
                }
            }
            if (isSelfClose()) _out << "/";
            _out << ">";
            break;

        case doctype:
            _out << "<!DOCTYPE " << data() << ">";
            break;

        case comment:
            _out << "<!-- " << data() << " -->";
            break;

        case cdata:
            _out << "<![CDATA[" << data() << "]]>";
            break;
    }
    return _out;
}
