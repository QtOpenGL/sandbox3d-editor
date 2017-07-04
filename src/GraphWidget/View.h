#pragma once

#include <QGraphicsView>

#include <QStaticText>

#include <QGraphicsDropShadowEffect>

class QGraphicsScene;

namespace GraphWidget
{

class View : public QGraphicsView
{
	Q_OBJECT

public:

	//
	// Constructor / Destructor
    explicit	View				(QGraphicsScene * pScene, QWidget *parent = 0);
	virtual		~View				(void);

	//
	// API
	void		setTitle			(const QString & strTitle);

protected:

	//
	// Events
    void		keyPressEvent		(QKeyEvent * event) override;
	void		mousePressEvent		(QMouseEvent * event) override;
	void		mouseReleaseEvent	(QMouseEvent * event) override;
	void		mouseMoveEvent		(QMouseEvent * event) override;
#ifndef QT_NO_WHEELEVENT
    void		wheelEvent			(QWheelEvent * event) override;
#endif

	//
	// Draw
    void		drawBackground		(QPainter *painter, const QRectF &rect) override;

private:

	void		scaleView			(qreal scaleFactor);


protected:

	QStaticText title;

};

}
