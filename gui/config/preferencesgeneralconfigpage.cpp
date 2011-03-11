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

#include <Knipptasch/Preferences>

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
        FileSystemModel( QObject *parent = 0 )
            : QFileSystemModel( parent ) {

        }

        QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const {
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


PreferencesGeneralConfigPage::PreferencesGeneralConfigPage( Knipptasch::Preferences *pref, ConfigWidget *parent )
    : AbstractConfigPage( tr( "General" ), tr( "General Options" ), DesktopIcon( "go-home" ), parent ),
      ui( new Ui::PreferencesGeneralConfigPage ),
      m_preferences( pref )
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
    ui->startupCombo->addItem( tr( "Show Welcome Page" ), Knipptasch::Preferences::WelcomePage );
    ui->startupCombo->addItem( tr( "Open Blank File" ), Knipptasch::Preferences::BlankFile );
    ui->startupCombo->addItem( tr( "Open Last-Used File" ), Knipptasch::Preferences::LastFile );
    ui->startupCombo->addItem( tr( "Open My Default Account File" ), Knipptasch::Preferences::DefaultFile );

    ui->defaultAccountFileRequesterButton->setIcon( BarIcon( "document-open" ) );

    FileSystemModel *model = new FileSystemModel( this );
    model->setRootPath( "" );
    model->setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

    QCompleter *completer = new QCompleter( this );
    completer->setModel( model );

    ui->defaultAccountFileRequesterLineEdit->setCompleter( completer );

    connect( ui->startupCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onStartupComboIndexChanged() ) );
    connect( ui->defaultAccountFileRequesterLineEdit, SIGNAL( textChanged( QString ) ), this, SIGNAL( pageModified() ) );
    connect( ui->defaultAccountFileRequesterButton, SIGNAL( clicked( bool ) ), this, SLOT( onFileChooserButtonClicked() ) );
    connect( ui->splashScreen, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );
    connect( ui->hideTabBarIfEmpty, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );
    connect( ui->showCloseButtonOnTabs, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );
    connect( ui->showCornerCloseButton, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );
    connect( ui->middleClickCloseTab, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );
    connect( ui->movableTabs, SIGNAL( stateChanged( int ) ), this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesGeneralConfigPage::~PreferencesGeneralConfigPage()
{
    delete ui;
}


bool PreferencesGeneralConfigPage::isValid() const
{
    PreferencesGeneralConfigPage *self = const_cast<PreferencesGeneralConfigPage *>( this );
    self->setErrorMessageEnabled( false );
    self->setErrorMessageType( AbstractConfigPage::ErrorMessage );

    int onStartupActionValue = ui->startupCombo->itemData(
                                   ui->startupCombo->currentIndex() ).toInt();

    if( onStartupActionValue != Knipptasch::Preferences::DefaultFile ) {
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
    int onStartupActionValue = ui->startupCombo->itemData(
                                   ui->startupCombo->currentIndex() ).toInt();

    if( onStartupActionValue != m_preferences->onStartupAction() ) {
        return true;
    }

    if( ui->splashScreen->isChecked() != m_preferences->splashScreenEnabled() ) {
        return true;
    }

    if( ui->hideTabBarIfEmpty->isChecked() != m_preferences->hideEmptyTabBar() ) {
        return true;
    }

    if( ui->showCloseButtonOnTabs->isChecked() != m_preferences->closeButtonOnTabs() ) {
        return true;
    }

    if( ui->showCornerCloseButton->isChecked() != m_preferences->tabCornerCloseButton() ) {
        return true;
    }

    if( ui->middleClickCloseTab->isChecked() != m_preferences->middleClickCloseTab() ) {
        return true;
    }

    if( ui->movableTabs->isChecked() != m_preferences->movableTabs() ) {
        return true;
    }

    return false;
}


bool PreferencesGeneralConfigPage::commit()
{
    int onStartupActionValue = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    m_preferences->setOnStartupAction( static_cast<Knipptasch::Preferences::EnumOnStartupAction>( onStartupActionValue ) );
    QString str;
    if( onStartupActionValue == Knipptasch::Preferences::DefaultFile ) {
        str = ui->defaultAccountFileRequesterLineEdit->text().trimmed();
    }
    m_preferences->setOnStartupActionDefaultFile( str );
    m_preferences->setSplashScreenEnabled( ui->splashScreen->isChecked() );
    m_preferences->setHideEmptyTabBar( ui->hideTabBarIfEmpty->isChecked() );
    m_preferences->setCloseButtonOnTabs( ui->showCloseButtonOnTabs->isChecked() );
    m_preferences->setTabCornerCloseButton( ui->showCornerCloseButton->isChecked() );
    m_preferences->setMiddleClickCloseTab( ui->middleClickCloseTab->isChecked() );
    m_preferences->setMovableTabs( ui->movableTabs->isChecked() );

    pageModified();

    return true;
}


void PreferencesGeneralConfigPage::revert()
{
    bool block = blockSignals( true );

    ui->startupCombo->setCurrentIndex( ui->startupCombo->findData( m_preferences->onStartupAction() ) );

    int onStartupActionValue = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    if( !m_preferences->onStartupActionDefaultFile().isEmpty() || onStartupActionValue == Knipptasch::Preferences::DefaultFile ) {
        ui->defaultAccountFileRequesterLineEdit->setText( m_preferences->onStartupActionDefaultFile() );
    }
    ui->splashScreen->setChecked( m_preferences->splashScreenEnabled() );
    ui->hideTabBarIfEmpty->setChecked( m_preferences->hideEmptyTabBar() );
    ui->showCloseButtonOnTabs->setChecked( m_preferences->closeButtonOnTabs() );
    ui->showCornerCloseButton->setChecked( m_preferences->tabCornerCloseButton() );
    ui->middleClickCloseTab->setChecked( m_preferences->middleClickCloseTab() );
    ui->movableTabs->setChecked( m_preferences->movableTabs() );

    blockSignals( block );

    onStartupComboIndexChanged();
}


void PreferencesGeneralConfigPage::onStartupComboIndexChanged()
{
    int value = ui->startupCombo->itemData( ui->startupCombo->currentIndex() ).toInt();
    ui->defaultAccountFileLabel->setVisible( value == Knipptasch::Preferences::DefaultFile );
    ui->defaultAccountFileRequesterLineEdit->setVisible( value == Knipptasch::Preferences::DefaultFile );
    ui->defaultAccountFileRequesterButton->setVisible( value == Knipptasch::Preferences::DefaultFile );

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
