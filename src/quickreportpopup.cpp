/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
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
#include "quickreportpopup.h"
#include "quickreportwidget.h"

#include <QWidgetAction>


class QuickReportAction : public QWidgetAction
{
    public:
        QuickReportAction(QuickReportWidget *widget, QObject *parent)
          : QWidgetAction( parent ),
            m_report( widget ),
            m_originalparent( widget->parentWidget() )
        {
        }

    protected:
        QWidget *createWidget(QWidget *parent)
        {
            m_report->setParent( parent );
            return m_report;
        }


        void deleteWidget(QWidget *widget)
        {
            if( widget != m_report ) {
                return;
            }

            m_report->setParent( m_originalparent );
        }

    private:
        QuickReportWidget *m_report;
        QWidget *m_originalparent;
};



QuickReportPopup::QuickReportPopup(QWidget *parent)
  : QMenu( parent )
{
    m_report = new QuickReportWidget( this );

    addAction( new QuickReportAction( m_report, this ) );
}


void QuickReportPopup::updateView()
{
    m_report->updateView();
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
