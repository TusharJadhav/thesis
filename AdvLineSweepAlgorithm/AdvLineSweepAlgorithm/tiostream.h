#ifndef TIOSTREAM_H
#define TIOSTREAM_H

#include <istream> // for std::basic_istream
#include <ostream> // for std::basic_ostream
// We won't put our references into the std namespace because that isn't nice
// and we'll leave on a .h extension to indicate that we aren't a standard
// library file

typedef std::basic_ostream<TCHAR> tostream;
typedef std::basic_istream<TCHAR> tistream;

extern tostream &tcout;
extern tistream &tcin;

#endif