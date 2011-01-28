/*
 * Copyright 2011 by Stefan Böhmann <kde@hilefoks.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef APPEARANCECOLORCHOOSERWIDGET_H
#define APPEARANCECOLORCHOOSERWIDGET_H

#include <QWidget>


namespace Ui
{
    class AppearanceColorChooserWidget;
}

class QColor;


/**
 * @class AppearanceColorChooserWidget
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class AppearanceColorChooserWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(AppearanceColorChooserWidget)

    public:
        /**
         * Constructs a new AppearanceColorChooserWidget
         */
        explicit AppearanceColorChooserWidget(QWidget *parent = 0);

        /**
         * Destructs the config page.
         */
        virtual ~AppearanceColorChooserWidget();

        bool editable();

        QColor color() const;

        /**
         * Returns the recommended size for this widget.
         */
        QSize sizeHint() const;

    public slots:
        void setColor(const QColor &color);
        void setEditable(bool b);

    signals:
        void colorChanged(const QColor &color);

    private slots:
        void onColorButtonValueChanged();
        void onSpinBoxValueChanged();

    private:
        void updateColorWidgets();

    private:
        Ui::AppearanceColorChooserWidget *ui;
        QColor m_color;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
