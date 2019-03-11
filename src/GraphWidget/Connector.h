#pragma once

#include <QGraphicsItem>

#define READ_BIT 1
#define WRITE_BIT 2

#define TYPE_TEXTURE_BIT 4
#define TYPE_FLOAT_BIT 8
#define TYPE_INT_BIT 16
#define TYPE_BOOL_BIT 32

namespace GraphWidget
{

class Node;
class Edge;

class Connector : public QGraphicsItem
{
public:

	enum EType
	{
		Input,
		Output
	};

	//
	// Constructor / Destructor
    explicit	Connector			(Node * parent, unsigned int index);
	virtual		~Connector			(void);

	//
	// Override Type
	enum { Type = UserType + 3 };
    int type(void) const override { return Type; }

	//
	// API
	unsigned int			getIndex			(void) const;

	virtual void			setMask				(unsigned int mask);
	virtual unsigned int 	getMask				(void) const;

	virtual EType			getConnectorType	(void) const = 0;

protected:

	//
	// Override QGraphicsItem
	void			mousePressEvent		(QGraphicsSceneMouseEvent * event) override;
    void			mouseMoveEvent		(QGraphicsSceneMouseEvent * event) override;
    void			mouseReleaseEvent	(QGraphicsSceneMouseEvent * event) override;

	void			hoverEnterEvent		(QGraphicsSceneHoverEvent * event) override;
	void			hoverLeaveEvent		(QGraphicsSceneHoverEvent * event) override;
	void			hoverMoveEvent		(QGraphicsSceneHoverEvent * event) override;

private:

	unsigned int	m_index;

	unsigned int	m_mask;

	bool m_bDisconnecting;

public:

	static const qreal radius;
};

}
