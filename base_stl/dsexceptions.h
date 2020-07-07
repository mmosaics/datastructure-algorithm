#ifndef DS_EXCEPTIONS_H
#define DS_EXCEPTIONS_H

#include <exception>

class UnderflowException: public std::exception { };
class IllegalArgumentException: public std::exception { };
class ArrayIndexOutOfBoundsException: public std::exception { };
class IteratorOutOfBoundsException: public std::exception { };
class IteratorMismatchException: public std::exception { };
class IteratorUninitializedException: public std::exception{ };
class CycleFoundException: public std::exception{};

#endif