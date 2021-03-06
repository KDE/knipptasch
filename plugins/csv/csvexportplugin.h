/*
 * Copyright 2010, 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef CSVEXPORTPLUGIN_H
#define CSVEXPORTPLUGIN_H

#include <Knipptasch/ExportPlugin>


/**
 * @class CsvExportPlugin
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class CsvExportPlugin : public QObject, public Knipptasch::ExportPlugin
{
        Q_OBJECT
        Q_INTERFACES( Knipptasch::Plugin )
        Q_INTERFACES( Knipptasch::ExportPlugin )

    public:
        CsvExportPlugin( QObject *parent = 0 );

        bool isEnabledByDefault() const;

        QByteArray identifier() const;
        QString shortName() const;
        QString name() const;
        QPixmap icon() const;
        QString description() const;
        QString author() const;
        QByteArray version() const;

        QString exportActionName() const;
        QPixmap exportActionIcon() const;

        void exportAccount( const Account *account, const QList<const Posting *> &selected, QWidget *parent = 0 ) const;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
