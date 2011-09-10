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

#include <map>
#include "ncstring.h"

using namespace std;

typedef map<ncstring, ncstring> htmlAttributes;

class htmlToken {
public:

    typedef enum {
        text,
        tag,
        doctype,
        cdata,
        comment
    } tokenType_t;

    htmlToken(tokenType_t _t = text)
        : m_type(_t)
        , m_isOpen(true)
        , m_isClose(false)
    {}

    htmlToken(const ncstring &_str, tokenType_t _t = text)
        : m_type(_t)
        , m_data(_str)
        , m_isOpen(true)
        , m_isClose(false)
    {}


    tokenType_t type() const { return m_type; }

    const ncstring &name() const
    {
        if (m_type == tag) return m_data;
        return m_dummy = (m_type == doctype) ? "doctype" : "";
    }

    const ncstring &data() const
    {
        if ((m_type == tag) || (m_type == doctype)) {
            return m_dummy = "";
        }
        return m_data;
    }

    template <typename _T>
    void setName(const _T &_name)
    {
        m_data.assign(_name.begin(), _name.end());
    }

    void setClose(bool _isClose = true) { m_isClose = _isClose; }
    void setOpen(bool _isOpen = true) { m_isOpen = _isOpen; }

    bool isClose() const { return m_isClose; }
    bool isOpen() const  { return m_isOpen; }
    bool isSelfClose() const { return m_isOpen && m_isClose; }

    void addData(char _c)
    {
        m_data.push_back(_c);
    }

    template <typename _T>
    void addData(const _T &_data)
    {
        m_data.append(_data.begin(), _data.end());
    }

    void addAttribute(const ncstring &name, const ncstring &value)
    {
        m_attrs[name] = value;
    }

    ostream &display(ostream &_out) const;

private:
    tokenType_t m_type;
    ncstring m_data;
    htmlAttributes m_attrs;
    bool m_isOpen, m_isClose;

    mutable ncstring m_dummy;
};

inline ostream & operator << (ostream &_out, const htmlToken &_tok)
{
    return _tok.display(_out);
}

#endif  // TIDY_HTMLTOKEN_H
