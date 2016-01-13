#include <QPainter>
#include "config.h"
#include "ll.h"
#include "nicenum.h"
#include "scaleitem.h"


#define SCALE_WIDTH  132
#define SCALE_HEIGHT 5
#define SEGMENTS 3

#define PADDING 4


ScaleItem::ScaleItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
	_units = Metric;
}

QRectF ScaleItem::boundingRect() const
{
	QFont font;
	font.setPixelSize(FONT_SIZE);
	font.setFamily(FONT_FAMILY);
	QFontMetrics fm(font);
	QRect ss, es, us;

	ss = fm.tightBoundingRect(QString::number(0));
	es = fm.tightBoundingRect(QString::number(_length * SEGMENTS));
	us = fm.tightBoundingRect(units());

	return QRectF(-ss.width()/2, -(PADDING + ss.height() + fm.descent()),
	  _width * SEGMENTS + ss.width()/2 + qMax(us.width() + PADDING, es.width()/2),
	  SCALE_HEIGHT + PADDING + ss.height() + 2*fm.descent());
}

void ScaleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
  QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QFont font;
	font.setPixelSize(FONT_SIZE);
	font.setFamily(FONT_FAMILY);
	QFontMetrics fm(font);


	painter->setFont(font);

	for (int i = 0; i <= SEGMENTS; i++) {
		QString label = QString::number(_length * i);
		QRect br = fm.tightBoundingRect(label);
		painter->drawText(_width * i - br.width()/2, -PADDING, label);
	}
	painter->drawText(_width * SEGMENTS + PADDING, SCALE_HEIGHT + fm.descent(),
	  units());

	painter->drawRect(QRectF(0, 0, SEGMENTS * _width, SCALE_HEIGHT));
	for (int i = 0; i < SEGMENTS; i += 2)
		painter->fillRect(QRectF(i * _width, 0, _width, SCALE_HEIGHT),
		  Qt::black);

/*
	painter->setPen(Qt::red);
	painter->drawRect(boundingRect());
*/
}

QString ScaleItem::units() const
{
	if (_units == Imperial)
		return _scale ? QObject::tr("mi") : QObject::tr("ft");
	else
		return _scale ? QObject::tr("km") : QObject::tr("m");
}

void ScaleItem::computeScale()
{
	qreal res = zoom2resolution(_zoom, _lat);

	if (_units == Imperial) {
		_length = niceNum((res * M2FT * SCALE_WIDTH) / SEGMENTS, 1);
		if (_length >= MIINFT) {
			_length = niceNum((res * M2FT * FT2MI * SCALE_WIDTH) / SEGMENTS, 1);
			_width = (_length / (res * M2FT * FT2MI));
			_scale = true;
		} else {
			_width = (_length / (res * M2FT));
			_scale = false;
		}
	} else {
		_length = niceNum((res * SCALE_WIDTH) / SEGMENTS, 1);
		if (_length >= KMINM) {
			_length *= M2KM;
			_width = (_length / (res * M2KM));
			_scale = true;
		} else {
			_width = (_length / res);
			_scale = false;
		}
	}
}

void ScaleItem::setLatitude(qreal lat)
{
	_lat = lat;
	computeScale();
	prepareGeometryChange();
}

void ScaleItem::setZoom(int z)
{
	_zoom = z;
	computeScale();
	prepareGeometryChange();
}

void ScaleItem::setUnits(enum Units units)
{
	_units = units;
	computeScale();
	prepareGeometryChange();
}
