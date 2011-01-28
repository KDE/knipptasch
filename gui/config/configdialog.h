/*
 * Copyright 2011  Stefan Böhmann <kde@hilefoks.org>
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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QVBoxLayout;

class ConfigWidget;


/**
 * @class ConfigDialog
 * @brief The ConfigDialog class provides a common interface for configuration dialogs.
 *
 * @see ConfigWidget
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class ConfigDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ConfigDialog)

    public:
        /**
         * Constructs a new ConfigDialog
         */
        explicit ConfigDialog(QWidget *parent = 0);


        /**
         * Destructs the config dialog.
         */
        virtual ~ConfigDialog();


        /**
         * Returns the dialog button box.
         */
        QDialogButtonBox* dialogButtonBox() const;


        /**
         * Sets the @param box.
         * The previous dialog button box will be deleted if this dialog is the parent of it.
         */
        void setDialogButtonBox(QDialogButtonBox *box);


        /**
         * Returns the config widget.
         */
        ConfigWidget* configWidget() const;


        /**
         * Sets the @param widget.
         * The previous widget will be deleted if this dialog is the parent of it.
         */
        void setConfigWidget(ConfigWidget *widget);

    public slots:
        /**
         *
         */
        virtual bool commit();


        /**
         *
         */
        virtual void revert();

        /**
         *
         */
        virtual void accept();


        /**
         *
         */
        virtual void reject();

    signals:
        void committed();
        void reverted();

    private slots:
        /**
         *
         */
        void checkButtonState();

    private:
        ConfigWidget *m_configWidget;
        QDialogButtonBox *m_buttonBox;
        QVBoxLayout *m_layout;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
