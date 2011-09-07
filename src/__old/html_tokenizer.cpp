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

#include "html_tokenizer.h"
#include "html_escaped_string.h"

using namespace std;

/*
 * Reads (or tries to) a full token from the input stream.
 *
 * This could be an HTML tag or other kind of "nodes", like text nodes or
 * CDATA sections, etc.
 */
istream &htmlToken::read(istream &in)
{
	unsigned char c = '\0';

	m_content.clear();

	in >> skipws >> c;

	if (c == '<') {
		return read_tag(in);
	} else {
		m_content = c;
		return read_text(in);
	}
}


istream &htmlToken::read_text(istream &in)
{
	unsigned char c = '\0';

	cout << "--- TEXT STATE ---" << endl;
	in >> noskipws;

	while ((in >> c) && (c != '<')) {
		m_content.push_back(c);
	}
	return in;
}

istream &htmlToken::read_tag(istream &in)
{
	unsigned char c = '\0';

	cout << "--- TAG STATE ---" << endl;
}


/*
 * Some debugging aids...
 */
void htmlToken::debug()
{
	cout << "----------------------------------------" << endl;
	cout << "Type: " << m_type << endl;
}
