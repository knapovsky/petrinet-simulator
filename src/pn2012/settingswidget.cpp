/**
 * @file
 * @author Martin Knapovsky <xknapo02@stud.fit.vutbr.cz>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2012, Martin Knapovsky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *   must display the following acknowledgement:
 *    This product includes software developed by the Martin Knapovsky.
 * 4. Neither the name of the Martin Knapovsky nor the
 *    names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MARTIN KNAPOVSKY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MARTIN KNAPOVSKY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 *
 * Implementace tridy, ktera je custom widgetem zobrazujicim nastaveni programu
 * a umoznujicim toto nastaveni zmenit
 *
 */

#include "settingswidget.h"

SettingsWidget::SettingsWidget(Settings *settingsIn, QDialog *dialogIn)
{
    // Ukazatele
    settings = settingsIn;
    dialog = dialogIn;

    // Labely
    //QHBoxLayout *address = new QHBoxLayout;
    QLabel *ipL = new QLabel("IP", this);
    QLabel *portL = new QLabel("Port", this);
    QLabel *usernameL = new QLabel("Username", this);
    QLabel *passwordL = new QLabel("Password", this);
    QLabel *configFileL = new QLabel("Config", this);

    // Line Edits
    ipE = new QLineEdit(this);
    ipE->setInputMask("000.000.000.000;_");
    portE = new QLineEdit(this);
    portE->setInputMask("00000");
    usernameE = new QLineEdit(this);
    passwordE = new QLineEdit(this);
    passwordE->setEchoMode(QLineEdit::Password);
    configFileE = new QLineEdit(this);

    ipL->setBuddy(ipE);
    portL->setBuddy(portE);
    usernameL->setBuddy(usernameE);
    passwordL->setBuddy(passwordE);
    configFileL->setBuddy(configFileE);

    // Browse Button
    QPushButton *browseConfig = new QPushButton("...", this);
    connect(browseConfig, SIGNAL(clicked()), this, SLOT(getConfigFile()));

    QHBoxLayout *configFileLayout = new QHBoxLayout;
    configFileLayout->addWidget(configFileE);
    configFileLayout->addWidget(browseConfig);

    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(ipL, 0, 0);
    widgetLayout->addWidget(ipE, 0, 1);
    widgetLayout->addWidget(portL, 1, 0);
    widgetLayout->addWidget(portE, 1, 1);
    widgetLayout->addWidget(usernameL, 2, 0);
    widgetLayout->addWidget(usernameE, 2, 1);
    widgetLayout->addWidget(passwordL, 3, 0);
    widgetLayout->addWidget(passwordE, 3, 1);
    widgetLayout->addWidget(configFileL, 4, 0);
    widgetLayout->addLayout(configFileLayout, 4, 1);

    this->setParent(settings->mainWindowPointer);

    // Widget vytvoren, zobrazi se informace
    showSettings();

    this->setLayout(widgetLayout);
}

void SettingsWidget::showSettings()
{
    ipE->setText(settings->ip);
    ipE->setCursorPosition(0);
    portE->setText(settings->port);
    portE->setCursorPosition(0);
    usernameE->setText(settings->username);
    passwordE->setText(settings->password);
    configFileE->setText(settings->configFile);
}

void SettingsWidget::setSettings()
{
    settings->ip = ipE->text();

    if(portE->text().toInt() < 65536 && portE->text().toInt() > 1024){

        settings->port = portE->text();
        settings->username = usernameE->text();
        settings->password = passwordE->text();

        // Nacteni souboru s nastavenim
        dialog->accept();
        if(!configFileE->text().isEmpty() && (settings->configFile != configFileE->text())){
            if(settings->loadSettingsFromXML(configFileE->text())){
                settings->configFile = configFileE->text();
            }
        }
    }
    else{

        // Port neni ve spravnem rozsahu
        settings->getMainWindowPointer()->statusBar()->showMessage(QString("Port must be lower than %1 and higher than 1024").arg(65536));
        settings->showAlert("Port must be lower than 65536 and higher than 1024");
    }
}

void SettingsWidget::getConfigFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open pn2012 config file"),
                                                    ".",
                                                    tr("pn2012 config files (*.xml)")
                                                    );
    if (fileName.isEmpty()){

        // Pouziti puvodniho nastaveni
        settings->getMainWindowPointer()->statusBar()->showMessage("Default configuration will be used");
        //settings->showAlert("Port must be lower than 65536 and higher than 1024");
    }
    else{

        // Nastaveni programu dle souboru s nastavenim
        configFileE->setText(fileName);
        settings->setConfigFile(fileName);
        this->showSettings();
    }
}
