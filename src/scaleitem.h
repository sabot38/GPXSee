#ifndef SCALEITEM_H
#define SCALEITEM_H

#include <QGraphicsItem>
#include "units.h"

class ScaleItem : public QGraphicsItem
{
public:
	ScaleItem(QGraphicsItem *parent = 0);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	  QWidget *widget);

	void setLatitude(qreal lat);
	void setZoom(int z);
	void setUnits(enum Units units);

private:
	void computeScale();
	QString units() const;

	int _zoom;
	qreal _lat;
	qreal _width;
	qreal _length;
	Units _units;
	bool _scale;
};

#endif // SCALEITEM_H
