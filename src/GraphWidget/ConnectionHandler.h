#pragma once

class QGraphicsLineItem;
class QGraphicsSceneMouseEvent;

namespace GraphWidget
{

class View;
class Connector;

class ConnectionHandler
{
public:

	static ConnectionHandler & instance(void);

	void setView(View * view);
	void start(Connector * connector);
	void move(float x, float y);
	void stop(float x, float y);

protected:

	ConnectionHandler(void);
	~ConnectionHandler(void);

private:

	View * m_pView;

	QGraphicsLineItem * m_pLine;

	Connector * m_pStartConnector;
};

}
