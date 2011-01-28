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

#include "compat/iconloader.h"

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



QuickReportPopup::QuickReportPopup(AccountSortFilterProxyModel *proxy, QWidget *parent)
  : QMenu( parent )
{
    QuickReportWidget *widget1 = new QuickReportWidget( proxy, this );
    widget1->setCurrentDate( QDate::currentDate().addMonths( -1 ) );
    addAction( new QuickReportAction( widget1, this ) );
    addSeparator();

    QuickReportWidget *widget2 = new QuickReportWidget( proxy, this );
    addAction( new QuickReportAction( widget2, this ) );
    addSeparator();

    QuickReportWidget *widget3 = new QuickReportWidget( proxy, this );
    widget3->setCurrentDate( QDate::currentDate().addMonths( 1 ) );
    addAction( new QuickReportAction( widget3, this ) );
}




// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
