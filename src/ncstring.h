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

#ifndef __NCSTRING_H
#define __NCSTRING_H

#include <string>
#include <iostream>

//------------------------------------------------------------------------------
/**
 * ignorecase_traits define los rasgos (char_traits) de caracter para usar con
 * las cadenas STL de tal forma que podemos crear una cadena "case insensitive".
 */
struct ignorecase_traits : public std::char_traits<char> {
    /**
	 * eq(c1, c2) operador de comparación, devuelve true si c1 == c2
	 */
    static bool eq(const char& c1, const char& c2) {
        return std::toupper(c1)==std::toupper(c2);
    }

    /**
	 * ne(c1, c2) operador de comparación, devuelve true si c1 != c2
	 */
    static bool ne(const char& c1, const char& c2) {
        return std::toupper(c1)!=std::toupper(c2);
    }

    /**
	 * lt(c1, c2) operador de comparación, devuelve true si c1 < c2
	 */
    static bool lt(const char& c1, const char& c2) {
        return std::toupper(c1)<std::toupper(c2);
    }

    /**
	 * compare(s1, s2, n) comparar dos cadenas, semántica igual a strncmp.
	 */
    static int compare(const char* s1, const char* s2, std::size_t n)
        {
            return strncasecmp( s1, s2, n );
            /*
				¿Comparación alternativa? ¿Más o menos eficiente?
            for (std::size_t i=0; i<n; ++i) {
                if (!eq(s1[i],s2[i])) {
                    return lt(s1[i],s2[i])?-1:1;
                }
            }
            return 0;
            */
        }

    /**
	 * search(s, n, c) búsqueda de un caracter "c" en la cadena "s"
	 */
    static const char* find(const char* s, std::size_t n, const char& c)
        {
            for (std::size_t i=0; i<n; ++i) {
                if (eq(s[i],c)) {
                    return &(s[i]);
                }
            }
            return 0;
        }
};

/**
 * Definimos el tipo de dato "ncstring" que usa los rasgos anteriores.
 */
typedef std::basic_string<char, ignorecase_traits> ncstring;

inline std::ostream & operator << (std::ostream &_out, const ncstring &_str)
{
    return _out << _str.c_str();
}

#endif  // __NCSTRING_H

