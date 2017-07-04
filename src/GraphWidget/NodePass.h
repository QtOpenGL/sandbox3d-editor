#pragma once

#include "Node.h"

namespace GraphWidget
{

class NodePass : public Node
{
public:

	//
	// Constructor / Destructor
    explicit			NodePass		(void);
	virtual				~NodePass		(void);

	//
	// API
	void				setTitle		(const QString & strTitle);
	const QString &		getTitle		(void) const;

	void				addInput		(const QString & strName);
	void				addOutput		(const QString & strName);

	virtual ConnectorInput *	getInputConnector	(unsigned int index) override;
	virtual ConnectorOutput *	getOutputConnector	(unsigned int index) override;

	virtual qreal				getWidth			(void) const override;
	virtual qreal				getHeight			(void) const override;

protected:

	//
	// Override QGraphicsItem
	QRectF				boundingRect	(void) const override;
	QPainterPath		shape			(void) const override;
    void				paint			(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:

	QString						title;
	QList<ConnectorInput*>		inputList;
	QList<QString>				inputNameList;
	QList<ConnectorOutput*>		outputList;
	QList<QString>				outputNameList;

public:

	static const qreal width;
	static const qreal height;
};

}
