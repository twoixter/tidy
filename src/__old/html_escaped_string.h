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

#ifndef TIDY_ESCAPEDSTRING_H
#define TIDY_ESCAPEDSTRING_H

#include <iostream>

using namespace std;

class escaped_string {
public:
	
	typedef enum {
		alpha,
		alnum,
		html
	} delim_type_t;

	escaped_string(string &_output, delim_type_t _type = alpha)
		: m_string(_output)
		, m_type(_type)
		, m_separators(",<>/=")
	{}

	istream & operator() (istream &in)
	{
		const istream::sentry ok(in);
		if (ok) {
			unsigned char c = in.get();

			cout << "Initial char is : " << c << endl;

			m_string.clear();
			while (in && is_valid(c)) {
				
				cout << "* Pushing: " << c << endl;
				
				m_string.push_back(c);
				c = in.get();
			}

			if (in) in.putback(c);
			if (!m_string.size()) {
				in.setstate(ios::failbit);
			}
		}
		return in;
	}

	friend ostream & operator << (ostream &, escaped_string &);

private:
	delim_type_t m_type;
	string &m_string;
	string m_separators;

	inline bool is_valid(const char c)
	{
		switch (m_type) {
		case alpha:
			return ::isalpha(c);
		case alnum:
			return ::isalnum(c);
		case html:
			return c != '<';
		}

		// return find(m_separators.begin(), m_separators.end(), c) == m_separators.end();
	}
};

inline istream & operator >> (istream &in, escaped_string str)
{
	return str(in);
}

inline ostream & operator << (ostream &out, escaped_string &estr)
{
	out << estr.m_string;
}

#endif // TIDY_ESCAPEDSTRING_H
