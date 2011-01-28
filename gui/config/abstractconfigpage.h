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
#ifndef ABSTRACTCONFIGPAGE_H
#define ABSTRACTCONFIGPAGE_H

#include <QWidget>

#include "configwidget.h"
#include <QIcon>

class QDialogButtonBox;


/**
 * @class AbstractConfigPage
 * @brief The AbstractConfigPage class provides a common interface for configuration pages.
 *
 * @see ConfigWidget
 * @see ConfigDialog
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class AbstractConfigPage : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractConfigPage)

    public:
        enum ErrorMessageType
        {
            InfoMessage,
            WarningMessage,
            ErrorMessage
        };

    protected:
        /**
         * Constructs a new AbstractConfigPage
         */
        explicit AbstractConfigPage(const QString &label, const QString &title, const QIcon &icon, ConfigWidget *parent);

        /**
         * Constructs a new AbstractConfigPage
         */
        explicit AbstractConfigPage(const QString &label, const QIcon &icon, ConfigWidget *parent);

        /**
         * Constructs a new AbstractConfigPage
         */
        explicit AbstractConfigPage(const QString &label, ConfigWidget *parent);

    public:
        /**
         * Destructs the config page.
         */
        virtual ~AbstractConfigPage();


        /**
         * Returns whether the current state of the page is different
         * from the current configuration.
         */
        virtual bool isModified() const = 0;


        /**
         * Returns whether the current state of the page is valid.
         * This base implementation returns true.
         *
         * @see errorMessageEnabled
         * @see errorMessageType
         * @see errorMessageTitle
         * @see errorMessageDescription
         * @see errorMessageChanged
         */
        virtual bool isValid() const;


        /**
         *
         * @see errorMessageType
         * @see errorMessageTitle
         * @see errorMessageDescription
         * @see errorMessageChanged
         */
        bool errorMessageEnabled() const;


        /**
         *
         */
        void setErrorMessageEnabled(bool b);


        /**
         *
         * @see errorMessageEnabled
         * @see errorMessageTitle
         * @see errorMessageDescription
         * @see errorMessageChanged
         */
        ErrorMessageType errorMessageType() const;


        /**
         *
         */
        void setErrorMessageType(ErrorMessageType type);


        /**
         *
         * @see errorMessageEnabled
         * @see errorMessageType
         * @see errorMessageDescription
         * @see errorMessageChanged
         */
        QString errorMessageTitle() const;


        /**
         *
         */
        void setErrorMessageTitle(const QString &str);


        /**
         *
         * @see errorMessageEnabled
         * @see errorMessageType
         * @see errorMessageTitle
         * @see errorMessageChanged
         */
        QString errorMessageDescription() const;


        /**
         *
         */
        void setErrorMessageDescription(const QString &str);


        /**
         *
         */
        ConfigWidget* configWidget() const;


        /**
         *
         */
        void setConfigWidget(ConfigWidget *widget);


        /**
         *
         */
        QString pageLabel() const;

        /**
         *
         */
        void setPageLabel(const QString &str);


        /**
         * Returns the title of this page
         *
         * @see setPageTitle()
         */
        QString pageTitle() const;


        /**
         * Sets the @param title of this page
         *
         * @see pageTitle()
         */
        void setPageTitle(const QString &str);


        /**
         *
         */
        QString pageDescription() const;


        /**
         *
         */
        void setPageDescription(const QString &str);


        /**
         * Returns the icon of this page
         *
         * @see setPageIcon()
         */
        QIcon pageIcon() const;


        /**
         * Sets the @param icon of this page
         *
         * @see pageIcon()
         */
        void setPageIcon(const QIcon &icon);


        /**
         * Returns the tooltip of this page
         *
         * @see setPageToolTip()
         */
        QString pageToolTip() const;


        /**
         * Sets the @param tooltip of this page
         *
         * @see pageToolTip()
         */
        void setPageToolTip(const QString &str);


        /**
         * Returns the what's this of this page
         *
         * @see setPageWhatsThis()
         */
        QString pageWhatsThis() const;


        /**
         * Sets the @param whatsthis of this page
         *
         * @see pageWhatsThis()
         */
        void setPageWhatsThis(const QString &str);


    public slots:

        /**
         * Apply all changes.
         *
         * This slot is called when the ConfigWidget's commit() slot is called
         * or when the Ok button of the ConfigDialog is clicked.
         */
        virtual bool commit() = 0;


        /**
         * Revert all changes and reload the initial data.
         *
         * This slot is called when the ConfigWidget's revert() slot is called
         * or when the Cancel button of the ConfigDialog is clicked.
         */
        virtual void revert();

    signals:
        /**
         * Emit this signal whenever a value in this widget was changed.
         */
        void pageModified();

        /**
         *
         * @see errorMessageEnabled
         * @see errorMessageType
         * @see errorMessageTitle
         * @see errorMessageDescription
         */
        void errorMessageChanged();

    private:
        ConfigWidget *m_configWidget;
        bool m_errorMessageEnabled;
        ErrorMessageType m_errorMessageType;
        QString m_errorMessageTitle;
        QString m_errorMessageDescription;
        QString m_label;
        QString m_title;
        QString m_description;
        QIcon m_icon;
        QString m_toolTip;
        QString m_whatsThis;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
