#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QObject>

class SettingsForm : public QDialog
{
    Q_OBJECT
public:

    SettingsForm(QWidget *parent = nullptr);
    QString settingsResult;

signals:

public slots:
    void reset();
    void setTIM1(int state);
    void setCNT1(int state);
    void setCNT2(int state);
    void accept() override;
private:
    QCheckBox *TIM1ON;
    QLineEdit *TIM1DURATION;
    QCheckBox *CNT1ON;
    QComboBox *CNT1EDGE;
    QLineEdit *CNT1RELOAD;
    QCheckBox *CNT2ON;
    QComboBox *CNT2EDGE;
    QLineEdit *CNT2RELOAD;
    QFormLayout *layout ;
    QDialogButtonBox *buttonBox;

};

#endif // SETTINGSFORM_H
