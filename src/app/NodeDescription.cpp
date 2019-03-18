#include "NodeDescription.h"

#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static NodeDescriptor::DataType DataTypeFromString(const QString & str)
{
	if (str == "Texture")
	{
		return(NodeDescriptor::Texture);
	}
	else if (str == "Float")
	{
		return(NodeDescriptor::Float);
	}
	else if (str == "Boolean")
	{
		return(NodeDescriptor::Boolean);
	}

	return(NodeDescriptor::UnknownDataType);
}

static bool ImportInputs(const QJsonArray & array, std::vector<NodeDescriptor::Input> & inputs)
{
	for (QJsonValue object : array)
	{
		if (!object.isObject())
		{
			return(false);
		}

		QJsonObject node = object.toObject();

		NodeDescriptor::Input input;

		for (QJsonObject::const_iterator it = node.begin(); it != node.end(); it++)
		{
			QString key = it.key();
			QJsonValue value = it.value();

			if (key == "id")
			{
				input.identifier = value.toString();
			}
			else if (key == "name")
			{
				input.name = value.toString();
			}
			else if (key == "type")
			{
				input.type = DataTypeFromString(value.toString());
			}
			else
			{
				return(false);
			}
		}

		inputs.push_back(input);
	}

	return(true);
}

static bool ImportOutputs(const QJsonArray & array, std::vector<NodeDescriptor::Output> & outputs)
{
	for (QJsonValue object : array)
	{
		if (!object.isObject())
		{
			return(false);
		}

		QJsonObject node = object.toObject();

		NodeDescriptor::Output output;

		for (QJsonObject::const_iterator it = node.begin(); it != node.end(); it++)
		{
			QString key = it.key();
			QJsonValue value = it.value();

			if (key == "name")
			{
				output.name = value.toString();
			}
			else if (key == "type")
			{
				output.type = DataTypeFromString(value.toString());
			}
			else
			{
				return(false);
			}
		}

		outputs.push_back(output);
	}

	return(true);
}

/**
 * @brief NodeDescriptor::NodeDescriptor
 */
NodeDescriptor::NodeDescriptor(void) : name("empty")
{

}

/**
 * @brief NodeDescriptor::loadFromFile
 * @param filename
 * @return
 */
bool NodeDescriptor::loadFromFile(const QString & filename)
{
	QFile loadFile(filename);

	if (!loadFile.open(QIODevice::ReadOnly))
	{
		qWarning("Couldn't open node file.");
		return(false);
	}

	QByteArray fileContent = loadFile.readAll();

	QJsonDocument doc(QJsonDocument::fromJson(fileContent));

	QJsonObject root = doc.object();

	for (QJsonObject::const_iterator it = root.begin(); it != root.end(); it++)
	{
		QString key = it.key();
		QJsonValue value = it.value();

		if (key == "id")
		{
			if (!value.isString())
			{
				return(false);
			}

			identifier = value.toString();
		}
		else if (key == "name")
		{
			if (!value.isString())
			{
				return(false);
			}

			name = value.toString();
		}
		else if (key == "inputs")
		{
			if (!value.isArray())
			{
				return(false);
			}

			ImportInputs(value.toArray(), inputs);
		}
		else if (key == "outputs")
		{
			if (!value.isArray())
			{
				return(false);
			}

			ImportOutputs(value.toArray(), outputs);
		}
		else
		{
			return(false);
		}
	}

	return(true);
}
