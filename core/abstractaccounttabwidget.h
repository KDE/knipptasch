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
#ifndef KNIPPTASCH_CORE_ABSTRACTACCOUNTTABWIDGET_H
#define KNIPPTASCH_CORE_ABSTRACTACCOUNTTABWIDGET_H

#include "knipptasch_core_export.h"

#include <QWidget>

class AccountModel;
class QModelIndex;


/**
 * @class AbstractAccountTabWidget
 * @ingroup Core
 * @brief Abstract base class that provides an interface for account tab widgets.
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class KNIPPTASCH_CORE_EXPORT AbstractAccountTabWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit AbstractAccountTabWidget( const QString &title, QWidget *parent = 0 );
        AbstractAccountTabWidget( const QString &title, const QIcon &icon, QWidget *parent = 0 );

        /**
         * Default Destructor
         */
        virtual ~AbstractAccountTabWidget();

        AccountModel *accountModel();
        const AccountModel *accountModel() const;
        void setAccountModel( AccountModel *model );

        QModelIndex currentSelectedIndex() const;

        /**
         * Returns a icon for the tab.
         * Calls virtual tabIcon() if the user wants a icon,
         * else returns a empty icon.
         */
        QIcon icon() const;
        QString label() const;
        QString tabToolTip() const;
        QString tabWhatsThis() const;

    signals:
        void updateTabView( AbstractAccountTabWidget *widget );

    public slots:
        void setCurrentSelectedIndex( const QModelIndex &index );

    protected slots:
        void setIcon( const QIcon &icon );
        void setLabel( const QString &str );
        void setToolTip( const QString &str );
        void setWhatsThis( const QString &str );

    protected:
        virtual void accountModelAboutToBeChanged();
        virtual void accountModelChanged( AccountModel *model ) = 0;
        virtual void currentSelectedIndexAboutToBeChanged();
        virtual void currentSelectedIndexChanged( const QModelIndex &index ) = 0;

        void changeEvent( QEvent *event );

    private:
        class Private;
        Private *const d;
};



#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
