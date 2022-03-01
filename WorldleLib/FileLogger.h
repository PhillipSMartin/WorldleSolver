#pragma once
#include "Logger.h"

#include <fstream>
#include <string>

using std::string;

class FileLogger :
    public Logger
{
private:
	std::ofstream file_;

public:
	FileLogger() { pOstream_ = &file_; }
	FileLogger(string  const& file_name, bool append = true) : FileLogger() { file_.open( file_name, append ? std::ios_base::app : std::ios_base::out ); }
	~FileLogger() { file_.close();  }

//	FileLogger(const FileLogger& file_logger) = default;

};

