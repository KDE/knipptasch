#include "scrollpanner.h"


namespace QSint
{


// constructor

ScrollPanner::ScrollPanner(QWidget *parent) :
    QWidget(parent),
    m_panning(true),
    m_sliderX(0),
    m_sliderY(0)
{
    m_valueXpan = m_valueYpan = 0;
    m_sizeXpan = m_sizeYpan = 1;

    m_minX = m_maxX = m_valueX = 0;
    m_minY = m_maxY = m_valueY = 0;

    m_sizeX = m_sizeY = 1;
    m_viewSize = QSize(1,1);

    m_opacity = 1;
    m_panOpacity = 1;

    m_oldX = m_oldY = 0;
}

ScrollPanner::~ScrollPanner()
{
//    disconnectSource();
}

// properties

void ScrollPanner::setBackgroundPen(const QPen& pen)
{
    m_pen = pen;
    update();
}

void ScrollPanner::setBackgroundBrush(const QBrush& brush)
{
    m_brush = brush;
    update();
}

void ScrollPanner::setSliderPen(const QPen& pen)
{
    m_panPen = pen;
    update();
}

void ScrollPanner::setSliderBrush(const QBrush& brush)
{
    m_panBrush = brush;
    update();
}

void ScrollPanner::setOpacity(double opacity)
{
    m_opacity = opacity;
    update();
}

void ScrollPanner::setSliderOpacity(double opacity)
{
    m_panOpacity = opacity;
    update();
}

// source connection

void ScrollPanner::connectSource(QAbstractSlider *horizontal, QAbstractSlider *vertical)
{
    disconnectSource();

    connectH(horizontal);
    connectV(vertical);
}

void ScrollPanner::connectSource(QAbstractScrollArea *area)
{
    connectSource(area->horizontalScrollBar(), area->verticalScrollBar());
}

void ScrollPanner::connectH(QAbstractSlider *sl)
{
    m_sliderX = sl;

    if (m_sliderX)
    {
        connect(m_sliderX, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setHorizontalRange(int,int)));

        connect(m_sliderX, SIGNAL(valueChanged(int)),
                this, SLOT(setHorizontalValue(int)));

        connect(this, SIGNAL(scrollHorizontal(int)),
                m_sliderX, SLOT(setValue(int)));

        m_sliderX->installEventFilter(this);
    }
}

void ScrollPanner::connectV(QAbstractSlider *sl)
{
    m_sliderY = sl;

    if (m_sliderY)
    {
        connect(m_sliderY, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setVerticalRange(int,int)));

        connect(m_sliderY, SIGNAL(valueChanged(int)),
                this, SLOT(setVerticalValue(int)));

        connect(this, SIGNAL(scrollVertical(int)),
                m_sliderY, SLOT(setValue(int)));

        m_sliderY->installEventFilter(this);
    }
}

void ScrollPanner::disconnectSource()
{
    if (m_sliderX)
    {
        disconnect(m_sliderX, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setHorizontalRange(int,int)));

        disconnect(m_sliderX, SIGNAL(valueChanged(int)),
                this, SLOT(setHorizontalValue(int)));

        disconnect(this, SIGNAL(scrollHorizontal(int)),
                m_sliderX, SLOT(setValue(int)));

        m_sliderX->removeEventFilter(this);

        m_sliderX = 0;
    }

    if (m_sliderY)
    {
        disconnect(m_sliderY, SIGNAL(rangeChanged(int,int)),
                this, SLOT(setVerticalRange(int,int)));

        disconnect(m_sliderY, SIGNAL(valueChanged(int)),
                this, SLOT(setVerticalValue(int)));

        disconnect(this, SIGNAL(scrollVertical(int)),
                m_sliderY, SLOT(setValue(int)));

        m_sliderY->removeEventFilter(this);

        m_sliderY = 0;
    }
}

void ScrollPanner::setHorizontalRange(int min, int max)
{
    m_minX = min;
    m_maxX = max;
    m_sizeX = max - min + m_viewSize.width();

    update();
}

void ScrollPanner::setHorizontalValue(int value)
{
    double d = double(width()) / double(m_sizeX);
    int v = value - m_minX;
    double dv = v * d;
    m_valueXpan = dv;

    update();
}

void ScrollPanner::setHorizontalViewSize(int size)
{
    m_viewSize.setWidth(size);

    calculateXsize(size);

    update();
}

void ScrollPanner::setVerticalRange(int min, int max)
{
    m_minY = min;
    m_maxY = max;
    m_sizeY = max - min + m_viewSize.height();

    update();
}

void ScrollPanner::setVerticalValue(int value)
{
    double d = double(height()) / double(m_sizeY);
    int v = value - m_minY;
    double dv = v * d;
    m_valueYpan = dv;

    update();
}

void ScrollPanner::setVerticalViewSize(int size)
{
    m_viewSize.setHeight(size);

    calculateYsize(size);

    update();
}

void ScrollPanner::setViewSize(const QSize &size)
{
    m_viewSize = size;

    calculateXsize(m_viewSize.width());
    calculateYsize(m_viewSize.height());

    update();
}

// panning methods

void ScrollPanner::enablePanning(bool on)
{
    m_panning = on;
}

void QSint::ScrollPanner::mousePressEvent(QMouseEvent *event)
{
    if (m_panning && event->buttons() & Qt::LeftButton)
    {
        m_oldX = m_valueXpan - event->pos().x();
        m_oldY = m_valueYpan - event->pos().y();
    }
}

void QSint::ScrollPanner::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning && event->buttons() & Qt::LeftButton)
    {
        m_valueXpan = event->pos().x() + m_oldX;
        if (m_valueXpan < 0)
            m_valueXpan = 0;
        else
        if (m_valueXpan >= (width()-m_sizeXpan))
            m_valueXpan = width()-m_sizeXpan;
        double dx = double(width()) / double(m_sizeX);
        int viewX = double(m_valueXpan) / dx + m_minX;
        emit scrollHorizontal(viewX);

        m_valueYpan = event->pos().y() + m_oldY;
        if (m_valueYpan < 0)
            m_valueYpan = 0;
        else
        if (m_valueYpan >= (height()-m_sizeYpan))
            m_valueYpan = height()-m_sizeYpan;
        double dy = double(height()) / double(m_sizeY);
        int viewY = double(m_valueYpan) / dy + m_minY;
        emit scrollVertical(viewY);
    }
}

// painting methods

void ScrollPanner::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(m_pen);
    p.setBrush(m_brush);
    p.setOpacity(m_opacity);

    drawBackground(p, rect().adjusted(0,0,-1,-1));

    p.setPen(m_panPen);
    p.setBrush(m_panBrush);
    p.setOpacity(m_panOpacity);

    drawSlider(p, QRect(m_valueXpan, m_valueYpan,
                        m_sizeXpan, m_sizeYpan).adjusted(0,0,-1,-1));
}

void ScrollPanner::drawBackground(QPainter& p, QRect r)
{
    p.drawRect(r);
}

void ScrollPanner::drawSlider(QPainter& p, QRect r)
{
    p.drawRect(r);
}

// helper methods

void ScrollPanner::calculateXsize(int size)
{
    m_sizeX = m_maxX - m_minX + m_viewSize.width();
    double d = double(width()) / double(m_sizeX);
    m_sizeXpan = qMax(int(size * d), 1);
}

void ScrollPanner::calculateYsize(int size)
{
    m_sizeY = m_maxY - m_minY + m_viewSize.height();
    double d = double(height()) / double(m_sizeY);
    m_sizeYpan = qMax(int(size * d), 1);
}

// event filter

bool ScrollPanner::eventFilter(QObject *obj, QEvent *event)
{
    bool b = QObject::eventFilter(obj, event);

    if (event->type() == QEvent::Resize)
    {
        QAbstractSlider *sl = dynamic_cast<QAbstractSlider*>(obj);
        if (sl)
        {
            if (sl == m_sliderX)
            {
                setHorizontalViewSize(sl->pageStep());
            } else
            if (sl == m_sliderY)
            {
                setVerticalViewSize(sl->pageStep());
            }
        }
    }

    return b;
}

 bool ScrollPanner::event(QEvent *e)
 {
     if (e->type() == QEvent::Paint)
     {
         if (m_sliderX && m_sliderX->pageStep() != m_viewSize.width())
         {
             setHorizontalRange(m_sliderX->minimum(), m_sliderX->maximum());
             setHorizontalViewSize(m_sliderX->pageStep());
             setHorizontalValue(m_sliderX->value());
         }

         if (m_sliderY && m_sliderY->pageStep() != m_viewSize.height())
         {
             setVerticalRange(m_sliderY->minimum(), m_sliderY->maximum());
             setVerticalViewSize(m_sliderY->pageStep());
             setVerticalValue(m_sliderY->value());
         }
     }

     return QWidget::event(e);
 }


} // namespace QSint
