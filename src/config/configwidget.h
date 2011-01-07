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
#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>

class QListWidgetItem;
class QListWidget;
class QSplitter;
class QStackedWidget;

class AbstractConfigPage;



/**
 * @class ConfigWidget
 * @brief The ConfigWidget class provides a common interface for configuration widgets.
 *
 * @see ConfigDialog
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class ConfigWidget : public QWidget
{
    Q_OBJECT

    Q_DISABLE_COPY( ConfigWidget )

    Q_PROPERTY(int countPages READ countPages)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

    public:
        /**
         * Constructs a new ConfigWidget
         */
        explicit ConfigWidget(QWidget* parent = 0);


        /**
         * Destructs the config widget.
         */
        virtual ~ConfigWidget();


        /**
         * Returns whether the current state of the widget is different
         * from the current configuration.
         */
        virtual bool isModified() const;


        /**
         * Returns whether the current state of the widget is valid
         */
        virtual bool isValid() const;


        /**
         *
         */
        QSize iconSize() const;


        /**
         *
         */
        void setIconSize(const QSize& size);


        /**
         * Adds a @param page.
         *
         * @return index of added page
         *
         * @see insertPage()
         * @see takePage()
         */
        int addPage(AbstractConfigPage *page);


        /**
         * Inserts a @param page at @param index.
         *
         * @return index of added page
         *
         * @see addPage()
         * @see takePage()
         */
        int insertPage(int index, AbstractConfigPage *page);


        /**
         * Removes the page at @param index and returns it.
         *
         * @return the page
         *
         * @see addPage()
         * @see insertPage()
         *
         * @note Does not delete the page widget.
         */
        AbstractConfigPage* takePage(int index);


        /**
         * The number of pages
         *
         * @property ConfigWidget::countPages
         *
         * @return the number of pages
         */
        int countPages() const;


        /**
         * The index of current page
         *
         * @property ConfigWidget::currentIndex
         *
         * @see currentPage()
         * @see setCurrentIndex()
         *
         * @return the index of current page
         */
        int currentIndex() const;


        /**
         * Returns the current page.
         *
         * @see currentIndex()
         * @see setCurrentPage()
         *
         * @return the current page
         */
        AbstractConfigPage* currentPage() const;


        /**
         * Returns the index of @param page or @c -1 if the page is unknown.
         *
         * @return the index of the @param page
         */
        int indexOf(AbstractConfigPage *page) const;


        /**
         * Returns the page at @param index or @c 0 if the @param index is out of range.
         *
         * @return the page at  @param index
         */
        AbstractConfigPage* page(int index) const;

    public slots:
        /**
         *
         */
        void setCurrentIndex(int index);


        /**
        * Sets the current @param page.
        *
        * @see currentPage()
        * @see currentIndex()
        */
        void setCurrentPage(AbstractConfigPage *page);

        /**
         *
         */
        virtual bool commit();


        /**
         *
         */
        virtual void revert();

    signals:
        /**
         *
         */
        void currentIndexChanged(int index);


        /**
         * A page in the widget was modified.
         */
        void pageModified(AbstractConfigPage *page);

        void committed();
        void reverted();

    protected:
        /**
         * Returns the internal splitter
         *
         * @see tableWidget()
         * @see stackedWidget()
         */
        QSplitter* splitter() const;


        /**
         * Returns the internal table widget used for showing page icons.
         *
         * @see splitter() stackedWidget()
         */
        QListWidget* listWidget() const;


        /**
         * Returns the internal stacked widget used for stacking pages.
         *
         * @see splitter()
         * @see tableWidget()
         */
        QStackedWidget* stackedWidget() const;

    private slots:
        /**
         *
         */
        void onPageModified();

    private:
        QSplitter *m_splitter;
        QStackedWidget *m_stack;
        QListWidget *m_view;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
