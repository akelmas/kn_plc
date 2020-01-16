#include "settingsform.h"
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>

SettingsForm::SettingsForm(QWidget *parent) : QDialog(parent)
{

    settingsResult="";
    TIM1ON=new QCheckBox;
    TIM1DURATION=new QLineEdit;
    QValidator *validator = new QIntValidator(1, 9999, this);
    TIM1DURATION->setValidator(validator);


    CNT1ON=new QCheckBox;
    CNT1EDGE=new QComboBox;
    CNT1RELOAD=new QLineEdit;
    CNT2ON=new QCheckBox;
    CNT2EDGE=new QComboBox;
    CNT2RELOAD=new QLineEdit;

    CNT1RELOAD->setValidator(validator);
    CNT2RELOAD->setValidator(validator);

    CNT1EDGE->addItem("RISING");
    CNT1EDGE->addItem("FALLING");
    CNT2EDGE->addItem("RISING");
    CNT2EDGE->addItem("FALLING");


    TIM1DURATION->setEnabled(0);

    CNT1EDGE->setEnabled(0);
    CNT1RELOAD->setEnabled(0);

    CNT2EDGE->setEnabled(0);
    CNT2RELOAD->setEnabled(0);


    layout = new QFormLayout;
    layout->addRow(tr("&TIM1ON"), TIM1ON);
    layout->addRow(tr("&TIM1DURATION"), TIM1DURATION);
    layout->addRow(tr("&CNT1ON"), CNT1ON);
    layout->addRow(tr("&CNT1EDGE"),CNT1EDGE);
    layout->addRow(tr("&CNT1RELOAD"), CNT1RELOAD);
    layout->addRow(tr("&CNT2ON"), CNT2ON);
    layout->addRow(tr("&CNT2EDGE"),CNT2EDGE);
    layout->addRow(tr("&CNT2RELOAD"), CNT2RELOAD);
    connect(TIM1ON,SIGNAL(stateChanged(int)),this,SLOT(setTIM1(int)));
    connect(CNT1ON,SIGNAL(stateChanged(int)),this,SLOT(setCNT1(int)));
    connect(CNT2ON,SIGNAL(stateChanged(int)),this,SLOT(setCNT2(int)));





    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(tr("Apply"),
                         QDialogButtonBox::AcceptRole);
    buttonBox->addButton(tr("Close"),
                         QDialogButtonBox::RejectRole);
    buttonBox->addButton(QDialogButtonBox::Reset);
    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));
    connect(buttonBox->button(QDialogButtonBox::Reset),
            SIGNAL(clicked()), this, SLOT(reset()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);


    setWindowTitle(tr("Settings"));

}

void SettingsForm::reset()
{
    TIM1ON->setCheckState(Qt::CheckState::Unchecked);
    CNT1ON->setCheckState(Qt::CheckState::Unchecked);
    CNT2ON->setCheckState(Qt::CheckState::Unchecked);
    TIM1DURATION->setEnabled(0);
    TIM1DURATION->clear();
    CNT1EDGE->setEnabled(0);
    CNT1EDGE->setCurrentIndex(0);
    CNT1RELOAD->setEnabled(0);
    CNT1RELOAD->clear();
    CNT2EDGE->setCurrentIndex(0);
    CNT2EDGE->setEnabled(0);
    CNT2RELOAD->setEnabled(0);
    CNT2RELOAD->clear();

}

void SettingsForm::setTIM1(int state)
{
    TIM1DURATION->setEnabled(state);
}

void SettingsForm::setCNT1(int state)
{
    CNT1EDGE->setEnabled(state);
    CNT1RELOAD->setEnabled(state);

}

void SettingsForm::setCNT2(int state)
{

    CNT2EDGE->setEnabled(state);
    CNT2RELOAD->setEnabled(state);
}

void SettingsForm::accept()
{
    settingsResult.clear();
    if(TIM1ON->checkState()==Qt::CheckState::Checked){

        settingsResult.append("#define TIM1ON ");
        settingsResult.append("1\n");
        settingsResult.append("#define TIM1DURATION ");
        settingsResult.append(TIM1DURATION->text());
        settingsResult.append("\n");
    }
    if(CNT1ON->checkState()==Qt::CheckState::Checked){

        settingsResult.append("#define CNT1ON ");
        settingsResult.append("1\n");
        settingsResult.append("#define CNT1EDGE ");
        settingsResult.append(CNT1EDGE->currentText());
        settingsResult.append("\n");
        settingsResult.append("#define CNT1RELOAD ");
        settingsResult.append(CNT1RELOAD->text());
        settingsResult.append("\n");

    }
    if(CNT2ON->checkState()==Qt::CheckState::Checked){

        settingsResult.append("#define CNT2ON ");
        settingsResult.append("1\n");
        settingsResult.append("#define CNT2EDGE ");
        settingsResult.append(CNT2EDGE->currentText());
        settingsResult.append("\n");
        settingsResult.append("#define CNT2RELOAD ");
        settingsResult.append(CNT2RELOAD->text());
        settingsResult.append("\n");

    }
    qDebug()<<settingsResult<<endl;



}
