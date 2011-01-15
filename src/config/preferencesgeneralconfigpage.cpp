/*
 * Copyright 2010,2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "preferencesgeneralconfigpage.h"
#include "ui_preferencesgeneralconfigpage.h"

#include "compat/iconloader.h"

#include "preferences.h"

#include <QApplication>
#include <QFileInfo>
#include <QUrl>

#include <QDebug>
#include <qfilesystemmodel.h>
#include <QCompleter>
#include <compat/utils.h>
#include <backend/storage.h>


/**
 *
 */
class FileSystemModel : public QFileSystemModel
{
    public:
        FileSystemModel(QObject *parent = 0)
          : QFileSystemModel(parent)
        {

        }

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
        {
            if( role == Qt::DisplayRole && index.column() == 0 ) {
                QString path  = QDir::toNativeSeparators( filePath( index ) );
                if( path.endsWith( QDir::separator() ) ) {
                    path.chop( 1 );
                }

                return path;
            }

            return QFileSystemModel::data( index, role );
        }
};


PreferencesGeneralConfigPage::PreferencesGeneralConfigPage(ConfigWidget* parent)
  : AbstractConfigPage( tr( "General" ), tr( "General Options" ), DesktopIcon("go-home"), parent ),
    ui( new Ui::PreferencesGeneralConfigPage )
{
    ui->setupUi( this );

    setErrorMessageTitle( tr( "Invalid Default Account File" ) );
    setErrorMessageType( AbstractConfigPage::ErrorMessage );

    //Knipptasch doesn't have a splash screen...
    ui->splashScreen->setVisible( false );

#if not defined(HAVE_KDE) // only available in KTabWidget, not in QTabWidget
    ui->hideTabBarIfEmpty->setVisible( false );
#endif

    ui->startupCombo->clear();
    ui->startupCombo->addItem( tr( "Show Welcome Page" ), Preferences::WelcomePage );
    ui->startupCombo->addItem( tr( "Open Blank File" ), Preferences::BlankFile );
    ui->startupCombo->addItem( tr( "Open Last-Used File" ), Preferences::LastFile );
    ui->startupCombo->addItem( tr( "Open My Default Account File" ), Preferences::DefaultFile );

    ui->defaultAccountFileRequesterButton->setIcon( BarIcon("document-open") );

    FileSystemModel *model = new FileSystemModel( this );
    model->setRootPath( "" );
    model->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

    QCompleter *completer = new QCompleter( this );
    completer->setModel( model );

    ui->defaultAccountFileRequesterLineEdit->setCompleter( completer );

    connect( ui->startupCombo, SIGNAL( currentIndexChanged(int) ), this, SLOT( onStartupComboIndexChanged() ) );
    connect( ui->defaultAccountFileRequesterLineEdit, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->defaultAccountFileRequesterButton, SIGNAL( clicked(bool) ), this, SLOT( onFileChooserButtonClicked() ) );
    connect( ui->splashScreen, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->hideTabBarIfEmpty, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->showCloseButtonOnTabs, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->showCornerCloseButton, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->middleClickCloseTab, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->movableTabs, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesGeneralConfigPage::~PreferencesGeneralConfigPage()
{
    delete ui;
}


bool PreferencesGeneralConfigPage::isValid() const
{
    PreferencesGeneralConfigPage *self = const_cast<PreferencesGeneralConfigPage*>( this );
    self->setErrorMessageEnabled( false );
    self->setErrorMessageType( AbstractConfigPage::ErrorMessage );

    int onStartupActionValue = ui->startupCombo->itemData(
                                    ui->startupCombo->currentIndex() ).toInt();

    if( onStartupActionValue != Preferences::DefaultFile ) {
        return true;
    }

    //TODO fix all setErrorMessageDescription strings here

    if( ui->defaultAccountFileRequesterLineEdit->text().trimmed().isEmpty() ) {
        self->setErrorMessageDescription( tr( "No default file provided" ) );
        self->setErrorMessageEnabled( true );

        return false;
    }

    ui->defaultAccountFileRequesterLineEdit->setStyleSheet( "color:red;" );

    QUrl url( QUrl::fromLocalFile( ui->defaultAccountFileRequesterLineEdit->text() ) );
    if( !url.isValid() ) {
        self->setErrorMessageDescription( tr( "Invalid file" ) );
        self->setErrorMessageEnabled( true );
        return false;
    }

    QFileInfo info( url.toLocalFile() );

    if( !info.exists() ) {
        self->setErrorMessageDescription( tr( "File does not exists" ) );
        self->setErrorMessageEnabled( true );
        return false;
    }

    if( !info.isFile() ) {
        self->setErrorMessageDescription( tr( "Is not a file" ) );
        self->setErrorMessageEnabled( true );
        return false;
    }

    if( !info.isReadable() ) {
        self->setErrorMessageDescription( tr( "File is not readable" ) );
        self->setErrorMessageEnabled( true );
        return false;
    }

    ui->defaultAccountFileRequesterLineEdit->setStyleSheet( "" );
    return true;
}


bool PreferencesGeneralConfigPage::isModified() const
{
    Preferences *p = Preferences::self();

    int onStartupActionValue = ui->startupCombo->itemData(
                                    ui->startupCombo->currentIndex() ).toInt();

    if( onStartupActionValue != p->onStartupAction() ) {
        return true;
    }

    if( ui->splashScreen->isChecked() != p->splashScreenEnabled() ) {
        return true;
    }

    if( ui->hideTabBarIfEmpty->isChecked() != p->hideEmptyTabBar() ) {
        return true;
    }

    if( ui->showCloseButtonOnTabs->isChecked() != p->closeButtonOnTabs() ) {
        return true;
    }

    if( ui->showCornerCloseButton->isChecked() != p->tabCornerCloseButton() ) {
        return true;
    }

    if( ui->middleClickCloseTab->isChecked() != p->middleClickCloseTab() ) {
        return true;
    }

    if( ui->movableTabs->isChecked() != p->movableTabs() ) {
        return true;
    }

    return false;
}


bool PreferencesGeneralConfigPage::commit()
{
    Preferences *p = Preferences::self();

    int onStartupActionValue = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    p->setOnStartupAction( onStartupActionValue );
    QString str;
    if( onStartupActionValue == Preferences::DefaultFile ) {
        str = ui->defaultAccountFileRequesterLineEdit->text().trimmed();
    }
    p->setOnStartupActionDefaultFile( str );
    p->setSplashScreenEnabled( ui->splashScreen->isChecked() );
    p->setHideEmptyTabBar( ui->hideTabBarIfEmpty->isChecked() );
    p->setCloseButtonOnTabs( ui->showCloseButtonOnTabs->isChecked() );
    p->setTabCornerCloseButton( ui->showCornerCloseButton->isChecked() );
    p->setMiddleClickCloseTab( ui->middleClickCloseTab->isChecked() );
    p->setMovableTabs( ui->movableTabs->isChecked() );

    pageModified();

    return true;
}


void PreferencesGeneralConfigPage::revert()
{
    bool block = blockSignals( true );

    Preferences *p = Preferences::self();
    ui->startupCombo->setCurrentIndex( ui->startupCombo->findData( p->onStartupAction() ) );

    int onStartupActionValue = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    if( !p->onStartupActionDefaultFile().isEmpty() || onStartupActionValue == Preferences::DefaultFile ) {
        ui->defaultAccountFileRequesterLineEdit->setText( p->onStartupActionDefaultFile() );
    }
    ui->splashScreen->setChecked( p->splashScreenEnabled() );
    ui->hideTabBarIfEmpty->setChecked( p->hideEmptyTabBar() );
    ui->showCloseButtonOnTabs->setChecked( p->closeButtonOnTabs() );
    ui->showCornerCloseButton->setChecked( p->tabCornerCloseButton() );
    ui->middleClickCloseTab->setChecked( p->middleClickCloseTab() );
    ui->movableTabs->setChecked( p->movableTabs() );

    blockSignals( block );

    onStartupComboIndexChanged();
}


void PreferencesGeneralConfigPage::onStartupComboIndexChanged()
{
    int value = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    ui->defaultAccountFileLabel->setVisible( value == Preferences::DefaultFile );
    ui->defaultAccountFileRequesterLineEdit->setVisible( value == Preferences::DefaultFile );
    ui->defaultAccountFileRequesterButton->setVisible( value == Preferences::DefaultFile );

    pageModified();
}


void PreferencesGeneralConfigPage::onFileChooserButtonClicked()
{
    QString str = getOpenFileName( this, QString(),
                                ui->defaultAccountFileRequesterLineEdit->text(),
                                Storage::filterStringQt(),
                                Storage::filterStringKDE() );

    if( !str.isEmpty() ) {
        ui->defaultAccountFileRequesterLineEdit->setText( str );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
