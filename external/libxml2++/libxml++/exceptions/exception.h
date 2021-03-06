// -*- C++ -*-

/* exception.h
 *
 * Copyright (C) 2002 The libxml++ development team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __LIBXMLPP_EXCEPTION_H
#define __LIBXMLPP_EXCEPTION_H

#include <exception>
#include <string> //#include <glibmm/ustring.h>

#include <libxml++config.h>

extern "C" {
  struct _xmlError;
  struct _xmlParserCtxt;
}

namespace xmlpp
{

/** Base class for all xmlpp exceptions.
 */
class LIBXMLPP_API exception: public std::exception
{
public:
  explicit exception(const std::string& message);
  virtual ~exception() throw();

  virtual const char* what() const throw();
  virtual void Raise() const;
  virtual exception * Clone() const;

private:
  std::string message_;
};

/** Format an _xmlError struct into a text string, suitable for printing.
 *
 * @newin{2,36}
 *
 * @param error Pointer to an _xmlError struct or <tt>0</tt>. If <tt>0</tt>,
 *              the error returned by xmlGetLastError() is used.
 * @returns A formatted text string. If the error struct does not contain an
 *          error (error->code == XML_ERR_OK), an empty string is returned.
 */
std::string format_xml_error(const _xmlError* error = 0);

/** Format a parser error into a text string, suitable for printing.
 *
 * @newin{2,36}
 *
 * @param parser_context Pointer to an _xmlParserCtxt struct.
 * @returns A formatted text string. If the parser context does not contain an
 *          error (parser_context->lastError.code == XML_ERR_OK), an empty
 *          string is returned.
 */
std::string format_xml_parser_error(const _xmlParserCtxt* parser_context);

} // namespace xmlpp

#endif // __LIBXMLPP_EXCEPTION_H
