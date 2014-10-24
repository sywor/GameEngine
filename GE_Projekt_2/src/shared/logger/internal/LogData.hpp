#ifndef LOGDATA_HPP
#define LOGDATA_HPP


#ifdef _WIN32
	#ifdef LOGGER_DLL_EXPORT
		#define DLLSETTING __declspec(dllexport)
	#else 
		#define DLLSETTING __declspec(dllimport)
	#endif
#else 
	#define DLLSETTING
#endif

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cstring>


namespace LogSystem
{
	/*!
		Description needed.
	*/
	DLLSETTING void InitializeLogFile( std::string path, bool appendToFile );
	
	/*!
		Description needed.
	*/
	DLLSETTING void PrintToConsole( const char* message, const char* prefix );
	
	/*!
		Description needed.
	*/
	DLLSETTING void PrintToClop( const char* message, const char* prefix );
	
	/*!
		Description needed.
	*/
	DLLSETTING void PrintToFile( const char* message, const char* prefix, bool close );

	/*!
		Description needed.
	*/
	DLLSETTING void SetOutputLevel( int levelCutoff );

	/*!
		Description needed.
	*/
	DLLSETTING int GetOutputLevel();


	/*!
		Description needed.
	*/
	static void CloseLogFile()
	{
		PrintToFile( "", "", true );
	};

	enum LogType : unsigned int
	{
		LogType_ToConsole	= 1U << 0, 
		LogType_ToFile		= 1U << 1,
		LogType_ToClop		= 1U << 2,
	};

	struct LogData
	{
		std::stringstream message;
		std::string message_prefix;
		unsigned int mask;
		int logLevel;

		LogData( unsigned int output_mask, int outputLevel, std::string prefix)
		{
			mask = output_mask;
			message << "[" << prefix << "] ";
			message_prefix = prefix;
			logLevel = outputLevel;
		}

		~LogData()
		{
			if( logLevel < LogSystem::GetOutputLevel() )
				return;

			if( LogType::LogType_ToConsole & mask )
			{
				LogSystem::PrintToConsole( message.str().c_str(), message_prefix.c_str() );
			}

			if( LogType::LogType_ToClop & mask )
			{
				int o = 0;
			}

			if( LogType::LogType_ToFile & mask )
			{
				LogSystem::PrintToFile( message.str().c_str(), message_prefix.c_str(), false );
			}
		}		
	};
}

typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);
static LogSystem::LogData& operator<< ( const LogSystem::LogData& data, StandardEndLine obj )
{
	LogSystem::LogData& temp = (LogSystem::LogData&)data;
	obj( temp.message );
	return temp;
}
	
template < class T >
LogSystem::LogData& operator<< ( const LogSystem::LogData& data, const T& obj )
{
	LogSystem::LogData& temp = (LogSystem::LogData&)data;
	temp.message << obj;
	return temp;
}

/****** Add Logger channels below ******/
#define LOG_DEBUG		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 1,		"debug" )
#define LOG_CONTENT		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 1,		"contentHandler    " )
#define LOG_LEVEL		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 1,		"levelHandler  " )

#define LOG_WARNING		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 10,	"warning" )
#define LOG_ERROR		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 20,	"error" )
#define LOG_FATAL		LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 30,	"fatal" )

#define LOG_CONTENT_SYS	LogSystem::LogData( LogSystem::LogType::LogType_ToConsole | LogSystem::LogType::LogType_ToClop | LogSystem::LogType::LogType_ToFile, 15,	"error" )


#endif