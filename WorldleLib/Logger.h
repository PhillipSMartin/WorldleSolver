#pragma once

#include <exception>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::exception;
using std::string;

class Logger
{
private:
	bool debug_mode_ = false;

public:
	bool const get_debug_mode()  { return debug_mode_; }
	void set_debug_mode( const bool value ) { debug_mode_ = value; }

	virtual void const info( string const& msg ) { cout << msg << endl;  }
	virtual void const debug( string const& msg ) { if (  debug_mode_  ) info( msg ); }
	virtual void const error( string const& msg, exception const& ex ) { info( msg + "=>" + ex.what() ); }
};

