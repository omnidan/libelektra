#ifndef ELEKTRA_KDB_EXCEPT_HPP
#define ELEKTRA_KDB_EXCEPT_HPP

/*
 * @brief See examples/cpp_example_userexception.cpp for how to use
 * USER_DEFINED_EXCEPTIONS.
 */
#ifndef USER_DEFINED_EXCEPTIONS

#include <keyexcept.hpp>

#include <string>
#include <sstream>

#include <kdbio.hpp>

namespace kdb
{

class KDBException : public Exception
{
public:
	KDBException (Key key) :
		m_key(key),
		m_str()
	{}

	virtual ~KDBException() throw()
	{}

	virtual const char* what() const throw()
	{
		if (m_str.empty())
		{
			// note that the code will be re-evaluated
			// if it prints nothing, but an expensive
			// function not printing anything seems
			// to be unlikely.
			//
			// note that printError/printWarning will be
			// used either from namespace kdb or global
			// namespace.
			std::stringstream ss;
			printError(ss, m_key);
			printWarnings(ss, m_key);
			m_str = ss.str();
		}
		return m_str.c_str();
	}
private:
	Key m_key;
	mutable std::string m_str;
};

}

#endif

#endif
