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

#include "html_stream_parser.h"

#include <cctype>

using namespace std;

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateData()
{
    cout << "STATE: stateData" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
    
    switch (m_current) {
        case '&':
            changeState(&htmlParser::stateCharacterReferenceInData);
            break;

        case '<':
            changeState(&htmlParser::stateTagOpen);
            break;

        case '\0':
            // TODO: Should be error
        default:
            emitCharacter();
            break;
    }
}

//---------------------------------------------------------------------------
//! PARTIAL, TODO
void htmlParser::stateCharacterReferenceInData()
{
    cout << "STATE: stateCharacterReferenceInData" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    // TODO: Correctly parse a character entity reference.
    // For now, just emit the ampersand token and swith to the Data state
    emitCharacter();
    changeState(&htmlParser::stateData);
}

//---------------------------------------------------------------------------
void htmlParser::stateRCDATA()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRCDATACharacterReference()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRAWTEXT()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptData()
{
}

//---------------------------------------------------------------------------
void htmlParser::statePlainText()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateTagOpen()
{
    cout << "STATE: stateTagOpen" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    if (::isalpha(m_current)) {
        m_characterContent = ::tolower(m_current);
        m_token = htmlToken(htmlToken::tag);
        changeState(&htmlParser::stateTagName);
        return;
    }

    switch (m_current) {
        case '!':
            changeState(&htmlParser::stateMarkupDeclarationOpen);
            break;

        case '/':
            changeState(&htmlParser::stateEndTagOpen);
            break;

        case '?':
            // TODO: Should be PARSE ERROR.
            changeState(&htmlParser::stateBogusComment);
            break;

        default:
            // TODO: Should be PARSE ERROR.
            emitCharacter('<');
            changeState(&htmlParser::stateData);
            processCurrent();
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateEndTagOpen()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateTagName()
{
    cout << "STATE: stateTagName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    if (::isspace(m_current)) {
        changeState(&htmlParser::stateBeforeAttributeName);
        return;
    }

    switch (m_current) {
        case '/':
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;

        case '>':
            // TODO: EMIT OPEN TAG...
            //    cout << "***** OPEN TAG ****** [" << m_token << "]" << endl;

            m_characterContent = "";
            changeState(&htmlParser::stateData);
            break;

        case '\0':
            // TODO: Should be Parse Error
            emitCharacter(HTML_INVALID_CHAR);
            break;

        default:
            emitCharacter(::tolower(m_current));
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateRCDATALessThanSign()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRCDATAEndTagOpen()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRCDATAEndTagName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRAWTEXTLessThanSign()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRAWTEXTEndTagOpen()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateRAWTEXTEndTagName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataLessThanSign()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEndTagOpen()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEndTagName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapeStart()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapeStartDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscaped()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapedDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapedDashDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapedLessThanSign()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapedEndTagOpen()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataEscapedEndTagName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscapeStart()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscaped()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscapedDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscapedDashDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscapedLessThanSign()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateScriptDataDoubleEscapeEnd()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBeforeAttributeName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterAttributeName()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBeforeAttributeValue()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeValueDoubleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeValueSingleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeValueUnquoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCharacterReferenceInAttributeValue()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterAttributeValueQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateSelfClosingStartTag()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBogusComment()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateMarkupDeclarationOpen()
{
    string _low;
    
    cout << "STATE: stateMarkupDeclarationOpen" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    switch (::tolower(m_current)) {
        case '-':
            if (peekNext(1) == "-") {
                m_characterContent = "";
                changeState(&htmlParser::stateCommentStart);
                return;
            }
            break;

        case 'd':
            _low = peekNext(6);
            transform(_low.begin(), _low.end(), _low.begin(), ::tolower);
            if (_low == "octype") {
                m_characterContent = "";
                changeState(&htmlParser::stateDOCTYPE);
                return;
            }
            break;

        case '[':
            // For a CDATA section to be valid, the "current node", acording
            // to the W3C documentation, should not be in the HTML namespace.
            // Hopefully, this will be catched by the SAX parser, since we
            // are now dealing with pure tokens, not building a DOM tree.
            if (peekNext(6) == "CDATA[") {
                m_characterContent = "";
                changeState(&htmlParser::stateCDATASection);
                return;
            }
            break;
    }

    // TODO: Should be PARSE ERROR.
    changeState(&htmlParser::stateBogusComment);
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentStart()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentStartDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentState()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentEndDash()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentEnd()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCommentEndBang()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateDOCTYPE()
{
    cout << "STATE: stateDOCTYPE" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    // We are going to the next state anyway, but if we have a non-space
    // character, we need to put it back into the stream or reprocess the
    // next state as we do here using "processCurrent".
    //
    // The W3C parser state machine was slightly different, because they take
    // an EOF character into account which we don't have to here.
    changeState(&htmlParser::stateBeforeDOCTYPEName);

    if (!::isspace(m_current)) {
        processCurrent();
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateBeforeDOCTYPEName()
{
    cout << "STATE: stateBeforeDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (m_current == '>') {
        // TODO: Emit the htmlToken as a quircks-mode empty doctype and raising an error.
        changeState(&htmlParser::stateData);
    } else {
        emitCharacter( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
        changeState(&htmlParser::stateDOCTYPEName);
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateDOCTYPEName()
{
    cout << "STATE: stateDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    if (::isspace(m_current)) {
        changeState(&htmlParser::stateAfterDOCTYPEName);
        return;
    }
    
    if (m_current == '>') {
        // TODO: Emit the htmlToken as a quircks-mode DOCTYPE
        changeState(&htmlParser::stateData);
        return;
    }

    emitCharacter( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPEName()
{
    cout << "STATE: stateAfterDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (m_current == '>') {
        m_token = htmlToken(htmlToken::doctype);
        m_token.extra = "m_characterContent";
        openingTag(m_token);

        changeState(&htmlParser::stateData);
        return;
    }

    emitCharacter( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPEPublicKeyword()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBeforeDOCTYPEPublicIdentifier()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateDOCTYPEPublicIdentifierDoubleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateDOCTYPEPublicIdentifierSingleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPEPublicIdentifier()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBetweenDOCTYPEPublicAndSystemIdentifiers()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPESystemKeyword()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBeforeDOCTYPESystemIdentifier()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateDOCTYPESystemIdentifierDoubleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateDOCTYPESystemIdentifierSingleQuoted()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPESystemIdentifier()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateBogusDOCTYPE()
{
}

//---------------------------------------------------------------------------
void htmlParser::stateCDATASection()
{
}
