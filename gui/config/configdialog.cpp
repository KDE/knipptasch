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
#include "configdialog.h"
#include "configwidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QDebug>



ConfigDialog::ConfigDialog(QWidget* parent)
  : QDialog( parent ),
    m_configWidget( new ConfigWidget( this ) ),
    m_buttonBox( 0 ),
    m_layout( 0 )
{
    m_buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this );

    connect( m_buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( m_buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
    connect( m_buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked(bool) ), this, SLOT( commit() ) );

    connect( m_configWidget, SIGNAL( pageModified(AbstractConfigPage*) ), this, SLOT( checkButtonState() ) );
    connect( m_configWidget, SIGNAL( committed() ), this, SIGNAL( committed() ) );
    connect( m_configWidget, SIGNAL( reverted() ), this, SIGNAL( reverted() ) );

    m_layout = new QVBoxLayout( this );
    m_layout->addWidget( m_configWidget );
    m_layout->addWidget( m_buttonBox );

    setLayout( m_layout );

    checkButtonState();
}


ConfigDialog::~ConfigDialog()
{
}


void ConfigDialog::checkButtonState()
{
    if( !m_configWidget->isValid() ) {
        m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
        m_buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    }
    else {
        m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
        m_buttonBox->button( QDialogButtonBox::Apply )
               ->setEnabled( m_configWidget->isModified() );
    }
}


QDialogButtonBox* ConfigDialog::dialogButtonBox() const
{
    return m_buttonBox;
}


void ConfigDialog::setDialogButtonBox(QDialogButtonBox *box)
{
    if( box == m_buttonBox ) {
        return;
    }

    if( m_buttonBox ) {
        m_layout->removeWidget( m_buttonBox );

        if( m_buttonBox->parent() == this ) {
            m_buttonBox->deleteLater();
        }
    }

    m_buttonBox = box;

    if( m_buttonBox ) {
        m_layout->addWidget( m_buttonBox );
    }
}


ConfigWidget* ConfigDialog::configWidget() const
{
    return m_configWidget;
}


void ConfigDialog::setConfigWidget(ConfigWidget *cw)
{
    if( cw == m_configWidget ) {
        return;
    }

    if( m_configWidget ) {
        disconnect( m_configWidget, 0, this, 0 );
        m_layout->removeWidget( m_configWidget );

        if( m_configWidget->parent() == this ) {
            m_configWidget->deleteLater();
        }
    }

    m_configWidget = cw;

    if( m_configWidget ) {
        connect( m_configWidget, SIGNAL( pageModified(AbstractConfigPage*) ), this, SLOT( checkButtonState() ) );
        connect( m_configWidget, SIGNAL( committed() ), this, SIGNAL( committed() ) );
        connect( m_configWidget, SIGNAL( reverted() ), this, SIGNAL( reverted() ) );

        m_layout->insertWidget( 0, m_configWidget );
    }
}


bool ConfigDialog::commit()
{
    bool b = m_configWidget->commit();

    checkButtonState();

    emit committed();

    return b;
}


void ConfigDialog::revert()
{
    m_configWidget->revert();
    checkButtonState();

    emit reverted();
}


void ConfigDialog::accept()
{
    if( !commit() ) {
        //TODO do something when commit fails.
    }

    QDialog::accept();
}


void ConfigDialog::reject()
{
    revert();
    QDialog::reject();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
