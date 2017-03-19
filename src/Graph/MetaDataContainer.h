#pragma once

#include <string>
#include <map>

class MetaDataContainer
{
public:

	void					addMetaData		(const char * szKey, const char * szValue);
	void					addMetaData		(const char * szKey, float fValue);
	void					addMetaData		(const char * szKey, double fValue);
	void					addMetaData		(const char * szKey, int iValue);
	void					addMetaData		(const char * szKey, long long iValue);

	const std::string &		getMetaData		(const char * szKey);

private:

	std::map<std::string, std::string> metada;

};
