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

#ifndef TIDY_HTMLSTREAMPARSER_H
#define TIDY_HTMLSTREAMPARSER_H

#define HTML_INVALID_CHAR    '?'

#include "html_sax_emitter.h"
#include "ncstring.h"
#include <iostream>

using namespace std;

class htmlParser : public htmlSAXEmitter {
public:

    htmlParser()
        : m_state(&htmlParser::stateData)
        , m_currentStream(NULL)
    {}

    void parse(istream &in);

private:

    // Events for the SAX Emitter
    virtual void startDocument()
    {
        cout << "**** STARTING DOCUMENT PARSING ****" << endl;
    }

    virtual void openingTag(const htmlToken &_token)
    {
        cout << _token << endl;
    }

    virtual void textNode(const htmlToken &_token)
    {
        cout << _token << endl;
    }

    virtual void closingTag(const htmlToken &_token)
    {
        cout << _token << endl;
    }

    virtual void endDocument()
    {
        cout << "**** CLOSING DOCUMENT PARSING ****" << endl;
    }


    void emitTag();
    void emitAttribute();

    typedef void (htmlParser::*stateFunc_t)();
    stateFunc_t  m_state;

    istream     *m_currentStream;
    char         m_current;

    ncstring     m_characterContent;
    ncstring     m_currentTagName;
    ncstring     m_currentAttrName;
    ncstring     m_currentAttrValue;
    htmlToken    m_token;

    inline void changeState(stateFunc_t newState)
    {
        m_state = newState;
    }

    inline void process()
    {
        (this->*m_state)();
    }

    inline void addToContent(char _c = 0, bool _clear = false)
    {
        if (_clear) m_characterContent.clear();
        m_characterContent.push_back(_c ? _c : m_current);
    }

    // WARNING: Curves ahead... :-)
    inline ncstring peekNext(int _count)
    {
        char *_str = new char[_count];
        m_currentStream->read(_str, _count);
        for (int i=_count-1; i>=0; i--) m_currentStream->putback(_str[i]);
        ncstring _temp(_str, _count);
        delete _str;
        return _temp;
    }

    inline void consume(int _count)
    {
        m_currentStream->ignore(_count);
    }

    void stateData();
    void stateCharacterReferenceInData();
    void stateRCDATA();
    void stateRCDATACharacterReference();
    void stateRAWTEXT();
    void stateScriptData();
    void statePlainText();
    void stateTagOpen();
    void stateEndTagOpen();
    void stateTagName();
    void stateRCDATALessThanSign();
    void stateRCDATAEndTagOpen();
    void stateRCDATAEndTagName();
    void stateRAWTEXTLessThanSign();
    void stateRAWTEXTEndTagOpen();
    void stateRAWTEXTEndTagName();
    void stateScriptDataLessThanSign();
    void stateScriptDataEndTagOpen();
    void stateScriptDataEndTagName();
    void stateScriptDataEscapeStart();
    void stateScriptDataEscapeStartDash();
    void stateScriptDataEscaped();
    void stateScriptDataEscapedDash();
    void stateScriptDataEscapedDashDash();
    void stateScriptDataEscapedLessThanSign();
    void stateScriptDataEscapedEndTagOpen();
    void stateScriptDataEscapedEndTagName();
    void stateScriptDataDoubleEscapeStart();
    void stateScriptDataDoubleEscaped();
    void stateScriptDataDoubleEscapedDash();
    void stateScriptDataDoubleEscapedDashDash();
    void stateScriptDataDoubleEscapedLessThanSign();
    void stateScriptDataDoubleEscapeEnd();
    void stateBeforeAttributeName();
    void stateAttributeName();
    void stateAfterAttributeName();
    void stateBeforeAttributeValue();
    void stateAttributeValueDoubleQuoted();
    void stateAttributeValueSingleQuoted();
    void stateAttributeValueUnquoted();
    void stateCharacterReferenceInAttributeValue();
    void stateAfterAttributeValueQuoted();
    void stateSelfClosingStartTag();
    void stateBogusComment();
    void stateMarkupDeclarationOpen();
    void stateCommentStart();
    void stateCommentStartDash();
    void stateCommentState();
    void stateCommentEndDash();
    void stateCommentEnd();
    void stateCommentEndBang();
    void stateDOCTYPE();
    void stateBeforeDOCTYPEName();
    void stateDOCTYPEName();
    void stateAfterDOCTYPEName();
    void stateAfterDOCTYPEPublicKeyword();
    void stateBeforeDOCTYPEPublicIdentifier();
    void stateDOCTYPEPublicIdentifierDoubleQuoted();
    void stateDOCTYPEPublicIdentifierSingleQuoted();
    void stateAfterDOCTYPEPublicIdentifier();
    void stateBetweenDOCTYPEPublicAndSystemIdentifiers();
    void stateAfterDOCTYPESystemKeyword();
    void stateBeforeDOCTYPESystemIdentifier();
    void stateDOCTYPESystemIdentifierDoubleQuoted();
    void stateDOCTYPESystemIdentifierSingleQuoted();
    void stateAfterDOCTYPESystemIdentifier();
    void stateBogusDOCTYPE();
    void stateCDATASection();
};


inline istream & operator >> (istream &in, htmlParser &parser)
{
    parser.parse(in >> noskipws);
}

#endif 	// TIDY_HTMLSTREAMPARSER_H
