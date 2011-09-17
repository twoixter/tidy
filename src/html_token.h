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


class htmlToken {
public:
    enum tokenType { text, tag, doctype, cdata, comment };
    enum tokenKind { open, close, selfClose };
    typedef map<ncstring, ncstring> htmlAttributes;

    htmlToken(tokenType _t = text)
        : m_type(_t)
        , m_kind(open)
    {}

    htmlToken(const ncstring &_str, tokenType _t = text, tokenKind _k = open)
        : m_type(_t)
        , m_kind(_k)
        , m_data(_str)
    {}

    tokenType type() const { return m_type; }
    tokenKind kind() const { return m_kind; }

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

    bool operator == (const ncstring &_str) const
    {
        return m_data == _str;
    }

    template <typename _T>
    void setName(const _T &_name)
    {
        m_data.assign(_name.begin(), _name.end());
    }

    bool isClose() const     { return m_kind == close; }
    bool isOpen() const      { return m_kind == open; }
    bool isSelfClose() const { return m_kind == selfClose; }

    void addData(char _c)
    {
        m_data.push_back(_c);
    }

    template <typename _T>
    inline void addData(const _T &_data)
    {
        m_data.append(_data.begin(), _data.end());
    }

    inline void addAttribute(const ncstring &name, const ncstring &value)
    {
        m_attrs[name] = value;
    }

    inline void setAttributes(const htmlAttributes &_attr)
    {
        m_attrs = _attr;
    }

    ostream &display(ostream &_out) const;

private:
    tokenType m_type;
    tokenKind m_kind;
    ncstring m_data;
    htmlAttributes m_attrs;

    mutable ncstring m_dummy;
};

inline ostream & operator << (ostream &_out, const htmlToken &_tok)
{
    return _tok.display(_out);
}

#endif  // TIDY_HTMLTOKEN_H
