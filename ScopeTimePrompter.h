#pragma once
#include <iostream>
#include <string>
#include <chrono>

class ScopeTimePrompter
{
	std::ostream& out;
	std::string _line;
	const std::chrono::system_clock::time_point _time;
	static int &instanceCount()
	{
		static int ___ = 0;
		return ___;
	}
	static void incInstance()
	{
		++instanceCount();
	}
	static void decInstance()
	{
		--instanceCount();
	}
public:
	ScopeTimePrompter(const std::string&line, std::ostream& os = std::cout)
		: out(os), _line(line)
		, _time(std::chrono::system_clock::now())
	{
		
		incInstance();
		out.clear();
		std::string indent(instanceCount() - 1, '\t');
		out << indent << "START:" << _line << std::endl;
	}
	~ScopeTimePrompter()
	{
		std::string indent(instanceCount() - 1, '\t');
		const auto nano = std::chrono::system_clock::now() - _time;
		const auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(nano);

		out << indent << "FINISH:" << _line << ". TAKES:" << (milli.count() / 1000.0f )<< " SECONDS" << std::endl;
		decInstance();
	}
};