#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;
using std::exception;
using std::string;

class Logger
{
protected:
	bool debug_mode_ = false;
	std::ostream* pOstream_;

public:
	Logger() { pOstream_ = &cout; }
	virtual ~Logger() {}

	bool get_debug_mode()  const { return debug_mode_; }
	void set_debug_mode( const bool value ) { debug_mode_ = value; }

	virtual void info( string const& msg ) const { *pOstream_ << msg << endl;  }
	virtual void debug( string const& msg ) const { if (  debug_mode_  ) info( msg ); }
	virtual void error( string const& msg, exception const& ex ) const { info( msg + "=>" + ex.what() ); }
};

