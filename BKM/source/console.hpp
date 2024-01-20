#ifndef _CONSOLE_HPP
#define _CONSOLE_HPP

#include "coloredstream.hpp"
#include <iostream>

namespace BKM {

	namespace Console {
		using namespace colored_stream;

		struct StandardPrinter {
			template<class T>
			StandardPrinter& operator << (T&& t) {
				std::cout << ColoredStream{ YELLOW, std::forward<T>(t) };
				return *this;
			}

			// Here we make sure that we can use std::endl;
			StandardPrinter& operator << (std::ostream& (*pManip)(std::ostream&)) {
				std::cout << (*pManip);
				return *this;
			}
		};

		struct DebugPrinter {
			template<class T>
			DebugPrinter& operator << (T&& t) {
				std::cout << ColoredStream{ CYAN, std::forward<T>(t) };
				return *this;
			}

			// Here we make sure that we can use std::endl;
			DebugPrinter& operator << (std::ostream& (*pManip)(std::ostream&)) {
				std::cout << (*pManip);
				return *this;
			}
		};

		struct WarnPrinter {
			template<class T>
			WarnPrinter& operator << (T&& t) {
				std::cout << ColoredStream{ MAGENTA, std::forward<T>(t) };
				return *this;
			}

			// Here we make sure that we can use std::endl;
			WarnPrinter& operator << (std::ostream& (*pManip)(std::ostream&)) {
				std::cout << (*pManip);
				return *this;
			}
		};

		struct ErrorPrinter {
			template<class T>
			ErrorPrinter& operator << (T&& t) {
				std::cout << ColoredStream{ RED, std::forward<T>(t) };
				return *this;
			}

			// Here we make sure that we can use std::endl;
			ErrorPrinter& operator << (std::ostream& (*pManip)(std::ostream&)) {
				std::cout << (*pManip);
				return *this;
			}
		};

		inline StandardPrinter Print;
		inline DebugPrinter Debug;
		inline WarnPrinter Warn;
		inline ErrorPrinter Error;

	}
}
#endif