#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "../lib/nc_string.h"

/*
 * Markdown Grammer
 * Document     := [ Block ]
 *
 * Block        := Header | EmptyLine | Paragraph
 *
 * Header       := HeaderLevel Space Formatted Text EmptyLine
 * HeaderLevel := '#' | '##' | '###' | '####' | '#####' | '######'
 *
 * Paragraph    := [ Formatted Text ]
 *
 * FormattedText := [Emphasis] Text [Emphasis]
 * FormattedText := Text 
 *
 * Text         := Text
 *
 * Emphasis     := '*' Text '*'
 *
 * EmptyLine   := '\n'
 * EndOfLine  := '\0'
 *
 * # Header
 * Paragraph with *emphasis* in it!
 *
 *                          Document
 *                             |
 *                  |--------------------|
 *                  |                    |
 *              Heading              Paragraph 
 *                                   Formatted text
 *              "#"                  "Paragraph with "
 *              "Header"             Emphasis
 *                                   "emphasis"
 *                                   "in it!"
 */


#endif /*parser_h*/
