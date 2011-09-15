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

// #define DEBUG

//---------------------------------------------------------------------------
void htmlParser::parse(istream &in)
{
    m_currentStream = &in;
    startDocument();
    while (in >> m_current) (this->*m_state)();
    endDocument();
}

//---------------------------------------------------------------------------
void htmlParser::emitText()
{
    if (!m_characterContent.empty()) {
        m_token = htmlToken(m_characterContent, htmlToken::text);
        m_characterContent.clear();
        textNode(m_token);
    }
}

//---------------------------------------------------------------------------
void htmlParser::emitTag()
{
    emitText();

    if (!m_currentTagName.empty()) {
        m_token = htmlToken(m_currentTagName, htmlToken::tag, m_currentTagKind);
        m_token.setAttributes(m_currentAttributes);
        m_currentTagName.clear();
        m_currentAttributes.clear();

        switch (m_token.kind()) {
            case htmlToken::open:
                openingTag(m_token);
                break;

            case htmlToken::close:
                closingTag(m_token);
                break;

            case htmlToken::selfClose:
                openingTag(m_token);
                closingTag(m_token);
                break;
        }

#ifdef DEBUG
    cout << "Emiting tag.";
    cout << " currentTagName = '" << m_currentTagName << "'";
    cout << " currentTagKind = '" << m_currentTagKind << "'";
    cout << " isOpen = '" << m_token.isOpen() << "'";
    cout << " isClose = '" << m_token.isClose() << "'";
    cout << " isSelfClose = '" << m_token.isSelfClose() << "'";
    cout << endl;
#endif

    }
}

//---------------------------------------------------------------------------
void htmlParser::emitDoctype()
{
    emitText();

    m_token = htmlToken("doctype", htmlToken::doctype, htmlToken::selfClose);
    openingTag(m_token);

    // Not sure if we must emit a close tag for a DOCTYPE ??
    // closingTag(m_token);
}

//---------------------------------------------------------------------------
void htmlParser::emitAttribute()
{
    if (!m_currentAttrName.empty()) {
        m_currentAttributes[m_currentAttrName] = m_currentAttrValue;
        m_currentAttrName.clear();
        m_currentAttrValue.clear();
    }
}

//---------------------------------------------------------------------------
void htmlParser::emitAttributeWithoutValue()
{
    if (!m_currentAttrName.empty()) {
        m_currentAttributes[m_currentAttrName] = m_currentAttrName;
        m_currentAttrName.clear();
        m_currentAttrValue.clear();
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateData()
{
#ifdef DEBUG
    cout << "STATE: stateData" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

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
            addToContent();
            break;
    }
}

//---------------------------------------------------------------------------
//! PARTIAL, TODO
void htmlParser::stateCharacterReferenceInData()
{
#ifdef DEBUG
    cout << "STATE: stateCharacterReferenceInData" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // TODO: Correctly parse a character entity reference.
    // For now, just emit the ampersand token and swith to the Data state
    addToContent('&');
    addToContent();
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
#ifdef DEBUG
    cout << "STATE: stateTagOpen" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isalpha(m_current)) {
        m_currentTagName = ::tolower(m_current);
        m_currentTagKind = htmlToken::open;

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
            addToContent('<');
            changeStateReconsuming(&htmlParser::stateData);
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateEndTagOpen()
{
#ifdef DEBUG
    cout << "STATE: stateEndTagOpen" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isalpha(m_current)) {
        m_currentTagName = ::tolower(m_current);
        m_currentTagKind = htmlToken::close;

        changeState(&htmlParser::stateTagName);
        return;
    }

    switch (m_current) {
        case '>':
            changeState(&htmlParser::stateData);
            break;

        default:
            changeState(&htmlParser::stateBogusComment);
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateTagName()
{
#ifdef DEBUG
    cout << "STATE: stateTagName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isspace(m_current)) {
        changeState(&htmlParser::stateBeforeAttributeName);
        return;
    }

    switch (m_current) {
        case '/':
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;

        case '>':
            emitTag();
            changeState(&htmlParser::stateData);
            break;

        case '\0':
            // TODO: Should be Parse Error
            m_currentTagName += HTML_INVALID_CHAR;
            break;

        default:
            m_currentTagName += ::tolower(m_current);
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
//! OK
void htmlParser::stateBeforeAttributeName()
{
#ifdef DEBUG
    cout << "STATE: stateBeforeAttributeName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (::isalpha(m_current)) {
        m_currentAttrName = ::tolower(m_current);
        changeState(&htmlParser::stateAttributeName);
        return;
    }

    switch (m_current) {
        case '/':
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;

        case '>':
            emitTag();
            changeState(&htmlParser::stateData);
            break;


        case '\0':
            // TODO: Should be Parse Error
            m_currentAttrName = HTML_INVALID_CHAR;
            changeState(&htmlParser::stateAttributeName);
            break;

        case '\'':
        case '"':
        case '<':
        case '=':
            // TODO: This should be a Parse error as the W3C spec...

        default:
            m_currentAttrName = m_current;
            changeState(&htmlParser::stateAttributeName);
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateAttributeName()
{
#ifdef DEBUG
    cout << "STATE: stateAttributeName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isspace(m_current)) {
        changeState(&htmlParser::stateAfterAttributeName);
        return;
    }

    if (::isalpha(m_current)) {
        m_currentAttrName += ::tolower(m_current);
        return;
    }

    switch (m_current) {
        case '/':
            emitAttributeWithoutValue();
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;

        case '=':
            changeState(&htmlParser::stateBeforeAttributeValue);
            break;

        case '>':
            emitAttributeWithoutValue();
            emitTag();
            changeState(&htmlParser::stateData);
            break;

        case '\0':
            // TODO: Should be Parse Error
            m_currentAttrName += HTML_INVALID_CHAR;
            break;

        case '\'':
        case '"':
        case '<':
            // TODO: This should be a Parse error as the W3C spec...

        default:
            m_currentAttrName += m_current;
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateAfterAttributeName()
{
#ifdef DEBUG
    cout << "STATE: stateAttributeName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (::isalpha(m_current)) {
        emitAttributeWithoutValue();
        m_currentAttrName = ::tolower(m_current);
        changeState(&htmlParser::stateAttributeName);
        return;
    }

    switch (m_current) {
        case '/':
            emitAttributeWithoutValue();
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;

        case '=':
            changeState(&htmlParser::stateBeforeAttributeValue);
            break;

        case '>':
            emitAttributeWithoutValue();
            emitTag();
            changeState(&htmlParser::stateData);
            break;

        case '\0':
            emitAttributeWithoutValue();
            m_currentAttrName = HTML_INVALID_CHAR;
            changeState(&htmlParser::stateAttributeName);
            break;

        case '\'':
        case '"':
        case '<':
            // TODO: This should be a Parse error as the W3C spec...

        default:
            emitAttributeWithoutValue();
            m_currentAttrName = m_current;
            changeState(&htmlParser::stateAttributeName);
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateBeforeAttributeValue()
{
#ifdef DEBUG
    cout << "STATE: stateBeforeAttributeValue" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // Ignore whitespace
    if (::isspace(m_current)) return;

    switch (m_current) {
        case '"':
            changeState(&htmlParser::stateAttributeValueDoubleQuoted);
            break;

        case '\'':
            changeState(&htmlParser::stateAttributeValueSingleQuoted);
            break;
        
        case '\0':
            m_currentAttrValue = HTML_INVALID_CHAR;
            changeState(&htmlParser::stateAttributeValueUnquoted);
            break;

        case '&':
            changeStateReconsuming(&htmlParser::stateAttributeValueUnquoted);
            break;

        case '>':
            // TODO: Should be parse error
            emitAttributeWithoutValue();
            emitTag();
            changeState(&htmlParser::stateData);
            break;

        case '<':
        case '=':
        case '`':
            // TODO: Should be parse error

        default:
            m_currentAttrValue = m_current;
            changeState(&htmlParser::stateAttributeValueUnquoted);
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateAttributeValueDoubleQuoted()
{
#ifdef DEBUG
    cout << "STATE: stateAttributeValueDoubleQuoted" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    switch (m_current) {
        case '"':
            changeState(&htmlParser::stateAfterAttributeValueQuoted);
            break;

        // TODO: Character entities inside values
        // case '&':
        //     changeState(&htmlParser::stateCharacterReferenceInAttributeValue);
        //     break;

        case '\0':
            // TODO: Should be parse error
            m_currentAttrValue += HTML_INVALID_CHAR;
            break;

        default:
            m_currentAttrValue += m_current;
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeValueSingleQuoted()
{
#ifdef DEBUG
    cout << "STATE: stateAttributeValueSingleQuoted" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    switch (m_current) {
        case '\'':
            changeState(&htmlParser::stateAfterAttributeValueQuoted);
            break;

        // TODO: Character entities inside values
        // case '&':
        //     changeState(&htmlParser::stateCharacterReferenceInAttributeValue);
        //     break;

        case '\0':
            // TODO: Should be parse error
            m_currentAttrValue += HTML_INVALID_CHAR;
            break;

        default:
            m_currentAttrValue += m_current;
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateAttributeValueUnquoted()
{
#ifdef DEBUG
    cout << "STATE: stateAttributeValueUnquoted" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isspace(m_current)) {
        emitAttribute();
        changeState(&htmlParser::stateBeforeAttributeName);
        return;
    }

    switch (m_current) {
        case '>':
            emitAttribute();
            emitTag();
            changeState(&htmlParser::stateData);
            break;
        
        // TODO: Character entities inside values
        // case '&':
        //     changeState(&htmlParser::stateCharacterReferenceInAttributeValue);
        //     break;

        case '\0':
            // TODO: Should be parse error
            m_currentAttrValue += HTML_INVALID_CHAR;
            break;

        case '"':
        case '\'':
        case '<':
        case '=':
        case '`':
            // TODO: Should be parse error

        default:
            m_currentAttrValue += m_current;
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateCharacterReferenceInAttributeValue()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateAfterAttributeValueQuoted()
{
#ifdef DEBUG
    cout << "STATE: stateAfterAttributeValueQuoted" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isspace(m_current)) {
        emitAttribute();
        changeState(&htmlParser::stateBeforeAttributeName);
        return;
    }
    
    switch (m_current) {
        case '/':
            emitAttribute();
            changeState(&htmlParser::stateSelfClosingStartTag);
            break;
        
        case '>':
            emitAttribute();
            emitTag();
            changeState(&htmlParser::stateData);
            break;
        
        default:
            emitAttribute();
            changeStateReconsuming(&htmlParser::stateBeforeAttributeName);
            break;
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateSelfClosingStartTag()
{
#ifdef DEBUG
    cout << "STATE: stateSelfClosingStartTag" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    switch (m_current) {
        case '>':
            m_currentTagKind = htmlToken::selfClose;
            emitTag();
            changeState(&htmlParser::stateData);
            break;

        default:
            changeStateReconsuming(&htmlParser::stateBeforeAttributeName);
            break;
    }
}

//---------------------------------------------------------------------------
void htmlParser::stateBogusComment()
{
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateMarkupDeclarationOpen()
{
#ifdef DEBUG
    cout << "STATE: stateMarkupDeclarationOpen" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    switch (::tolower(m_current)) {
        case '-':
            if (peekNext(1) == "-") {
                consume(1);
                m_token = htmlToken(htmlToken::comment);
                changeState(&htmlParser::stateCommentStart);
                return;
            }
            break;

        case 'd':
            if (peekNext(6) == "octype") {
                consume(6);
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
#ifdef DEBUG
    cout << "STATE: stateDOCTYPE" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // We are going to the next state anyway, but if we have a non-space
    // character, we need to put it back into the stream or reprocess the
    // next state as we do here using "processCurrent".
    //
    // The W3C parser state machine was slightly different, because they take
    // an EOF character into account which we don't have to here.
    changeState(&htmlParser::stateBeforeDOCTYPEName);

    if (!::isspace(m_current)) reconsume();
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateBeforeDOCTYPEName()
{
#ifdef DEBUG
    cout << "STATE: stateBeforeDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (m_current == '>') {
        // TODO: Emit the htmlToken as a quircks-mode empty doctype and raising an error.
        emitDoctype();
        changeState(&htmlParser::stateData);
    } else {
        // Ignoring the DOCTYPE declaration
        // m_token.addName( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
        changeState(&htmlParser::stateDOCTYPEName);
    }
}

//---------------------------------------------------------------------------
//! OK
void htmlParser::stateDOCTYPEName()
{
#ifdef DEBUG
    cout << "STATE: stateDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    if (::isspace(m_current)) {
        changeState(&htmlParser::stateAfterDOCTYPEName);
        return;
    }
    
    if (m_current == '>') {
        // TODO: Emit the htmlToken as a quircks-mode DOCTYPE
        emitDoctype();
        changeState(&htmlParser::stateData);
        return;
    }

    // Ignoring the DOCTYPE declaration
    // addToContent( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
}

//---------------------------------------------------------------------------
void htmlParser::stateAfterDOCTYPEName()
{
#ifdef DEBUG
    cout << "STATE: stateAfterDOCTYPEName" << endl;
    cout << "CURRENT CHAR: " << m_current << endl;
#endif

    // Ignore whitespace
    if (::isspace(m_current)) return;

    if (m_current == '>') {
        emitDoctype();
        changeState(&htmlParser::stateData);
        return;
    }

    // Ignoring the DOCTYPE declaration
    // addToContent( (m_current == '\0') ? HTML_INVALID_CHAR : ::tolower(m_current) );
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
