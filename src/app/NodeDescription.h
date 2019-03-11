#pragma once

#include <QString>
#include <vector>

struct NodeDescriptor
{
	NodeDescriptor(void);
	bool loadFromFile(const QString & filename);

	enum DataType
	{
		UnknownDataType,
		Texture,
		Float,
	};

	struct Input
	{
		QString identifier;
		QString name;
		DataType type;
	};

	struct Output
	{
		QString name;
		DataType type;
	};

	bool operator == (const char * strId) const
	{
		return(identifier == strId);
	}

	QString identifier;
	QString name;

	std::vector<Input> inputs;
	std::vector<Output> outputs;

};
