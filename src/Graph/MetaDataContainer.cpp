#include "MetaDataContainer.h"

/**
 * @brief MetaDataContainer::addMetaData
 * @param szKey
 * @param szValue
 */
void MetaDataContainer::addMetaData(const char * szKey, const char * szValue)
{
	std::pair<std::string, std::string> pair(szKey, szValue);
	metada.insert(pair);
}

/**
 * @brief MetaDataContainer::addMetaData
 * @param szKey
 * @param fValue
 */
void MetaDataContainer::addMetaData(const char * szKey, float fValue)
{
	char szFloatValue [16];
	sprintf(szFloatValue, "%f", fValue);
	std::pair<std::string, std::string> pair(szKey, szFloatValue);
	metada.insert(pair);
}

/**
 * @brief MetaDataContainer::addMetaData
 * @param szKey
 * @param fValue
 */
void MetaDataContainer::addMetaData(const char * szKey, double fValue)
{
	char szFloatValue [16];
	sprintf(szFloatValue, "%f", fValue);
	std::pair<std::string, std::string> pair(szKey, szFloatValue);
	metada.insert(pair);
}

/**
 * @brief MetaDataContainer::addMetaData
 * @param szKey
 * @param iValue
 */
void MetaDataContainer::addMetaData(const char * szKey, int iValue)
{
	char szIntValue [16];
	sprintf(szIntValue, "%d", iValue);
	std::pair<std::string, std::string> pair(szKey, szIntValue);
	metada.insert(pair);
}

/**
 * @brief MetaDataContainer::addMetaData
 * @param szKey
 * @param iValue
 */
void MetaDataContainer::addMetaData(const char * szKey, long long iValue)
{
	char szIntValue [16];
	sprintf(szIntValue, "%lld", iValue);
	std::pair<std::string, std::string> pair(szKey, szIntValue);
	metada.insert(pair);
}

/**
 * @brief MetaDataContainer::getMetaData
 * @param szKey
 * @return
 */
const std::string & MetaDataContainer::getMetaData(const char * szKey)
{
	return(metada[szKey]);
}

/**
 * @brief MetaDataContainer::begin
 * @return
 */
MetaDataContainer::map::iterator MetaDataContainer::begin(void)
{
	return(metada.begin());
}

/**
 * @brief MetaDataContainer::end
 * @return
 */
MetaDataContainer::map::iterator MetaDataContainer::end(void)
{
	return(metada.end());
}
