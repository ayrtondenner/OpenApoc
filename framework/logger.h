#pragma once

/* The logger is global state as we want it to be available even if the framework hasn't been successfully initialised */

namespace OpenApoc {
/* MSVC doesn't ahve __PRETTY_FUNCTION__ but __FUNCSIG__? */
//FIXME: !__GNUC__ isn't the same as MSVC
#ifndef __GNUC__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if defined(__GNUC__)
#define PRINTF_FORMAT(x,y) __attribute__((format(printf,x,y)))
#else
#define PRINTF_FORMAT
#endif

	enum class LogLevel
	{
		Info,
		Warning,
		Error,
	};

	//TODO: ostream log versions?
	PRINTF_FORMAT(3,4) void Log(LogLevel level, const char *prefix, const char* format, ...);


}; //namespace OpenApoc

#if defined(__GNUC__)
//GCC has an extension if __VA_ARGS__ are not supplied to 'remove' the precending comma
#define LogInfo(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Info, __PRETTY_FUNCTION__, f, ##__VA_ARGS__)
#define LogWarning(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Warning, __PRETTY_FUNCTION__, f, ##__VA_ARGS__)
#define LogError(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Error, __PRETTY_FUNCTION__, f, ##__VA_ARGS__)
#else
//At least msvc automatically removes the comma
#define LogInfo(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Info, __PRETTY_FUNCTION__, f, __VA_ARGS__)
#define LogWarning(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Warning, __PRETTY_FUNCTION__, f, __VA_ARGS__)
#define LogError(f, ...) OpenApoc::Log(OpenApoc::LogLevel::Error, __PRETTY_FUNCTION__, f, __VA_ARGS__)
#endif
