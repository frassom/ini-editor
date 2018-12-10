#ifndef PARSEEXCEPTION__H
#define PARSEEXCEPTION__H

#include <exception>

namespace ini {

	class ParseException : public std::exception {
	public:
		ParseException(const char* msg, int line) : mErrorMsg(msg), mErrorLine(line) {}

		const char* what() const noexcept override {
			return mErrorMsg;
		}

		int line() const {
			return mErrorLine;
		}

		void setLine(int line) {
			mErrorLine = line;
		}

	private:
		const char* mErrorMsg;
		int mErrorLine;
	};

}	// namespace ini

#endif
