#include <internal/LogData.hpp>

#include <ctime>
#include <mutex>

namespace LogSystem
{
	std::string debugPath;
	bool append;
	bool initialized;
	int outputLevelCutoff;

	std::mutex fileMutex;
}

struct LogInitializer
{
	LogInitializer()
	{
		LogSystem::debugPath = "";
		LogSystem::append = true;
		LogSystem::initialized = false;
		LogSystem::outputLevelCutoff = 0;
	}

} initializer;


void LogSystem::PrintToConsole( const char* message, const char* prefix )
{
	std::cout << message;
}

void LogSystem::PrintToClop( const char* message, const char* prefix )
{
}


void LogSystem::PrintToFile( const char* message, const char* prefix, bool close )
{
	std::lock_guard<std::mutex> guard( LogSystem::fileMutex );

	static std::fstream ff;
	if( !ff.is_open() && LogSystem::initialized )
	{
		ff.open( debugPath, std::ios::out | (append ? std::ios::app : std::ios::trunc) );
		if( ff.is_open() == false )
		{
			initialized = false;
			std::cout << "Could not open log file" << std::endl;
		}
		else
		{
			std::time_t timev;
			std::time( &timev );
			ff << std::endl << "========= " << std::asctime(std::localtime(&timev));
		}
	}
		
	if ( initialized && ff.is_open() )
	{
		if( close )
		{
			ff.close();
			initialized = false;
		}
		else
		{
			ff << message;
		}
	}
}

void LogSystem::InitializeLogFile( std::string path, bool appendToFile )
{
	debugPath = path;
	append = appendToFile;
	initialized = true;
}

void LogSystem::SetOutputLevel( int levelCutoff )
{
	LogSystem::outputLevelCutoff = levelCutoff;
}

int LogSystem::GetOutputLevel()
{
	return LogSystem::outputLevelCutoff;
}