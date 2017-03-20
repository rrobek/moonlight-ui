#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QSettings>
#include <QFileDialog>

/*
 *
 *     QLineEdit *editAppName;
    QLineEdit *editServer;
    QComboBox *cbResolution;
    QComboBox *cbFrameRate;
    QCheckBox *cbBitRateAuto;
    QSpinBox *sbBitRate;
    QComboBox *cbCodec;
    QRadioButton *rbSysOpsYes;
    QRadioButton *rbSysOpsNo;
    QComboBox *cbAudio;
 * */
#define OPTION_GROUP "Options"
#define OPTION_APPNAME "AppName"
#define OPTION_APPNAME_DEFAULT "moonlight"
#define OPTION_SERVER "Server"
#define OPTION_RESOLUTION "Resolution"
#define OPTION_FRAMERATE "FrameRate"
#define OPTION_BITRATE "BitRate"
#define OPTION_CODEC "Codec"
#define OPTION_CODEC_DEFAULT "-h264"
#define OPTION_SYSOPS "SysOps"
#define OPTION_AUDIO "Audio"

Options OptionsDialog::getOptions(bool forStreamCommand)
{
    Options r;

    QSettings s;
    s.beginGroup(OPTION_GROUP);
    r.executable = s.value(OPTION_APPNAME, OPTION_APPNAME_DEFAULT).toString();
    r.server = s.value(OPTION_SERVER).toString();
    if(forStreamCommand) {
        r.args << s.value(OPTION_RESOLUTION).toString();
        r.args << s.value(OPTION_FRAMERATE).toString();
        int kbps = s.value(OPTION_BITRATE).toInt() * 1000;
        if(kbps > 0) r.args << "-bitrate" << QString::number(kbps);
        r.args << s.value(OPTION_CODEC, OPTION_CODEC_DEFAULT).toString();
        bool sops = s.value(OPTION_SYSOPS).toBool();
        if(!sops) r.args << "-nosops";
        r.args << s.value(OPTION_AUDIO).toString();
    }
    return r;
}

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    // Fill combo boxes
    addComboItem(cbResolution, "HD (1280 x 720)", "");
    addComboItem(cbResolution, "Full HD (1920 x 1080)", "-1080");
    addComboItem(cbResolution, "Ultra HD (3840 x 2160)", "-width 3840 -height 2160");

    addComboItem(cbFrameRate, "30 fps", "-30fps");
    addComboItem(cbFrameRate, "60 fps", "");

    addComboItem(cbCodec, "AVC (H.264)", "-h264");
    addComboItem(cbCodec, "HEVC (H.265)", "-hevc");

    addComboItem(cbAudio, "None (Play locally)", "-localaudio");
    addComboItem(cbAudio, "Stereo", "");
    addComboItem(cbAudio, "Surround", "-surround");


    // Select current options
    QSettings s;
    s.beginGroup(OPTION_GROUP);
    editAppName->setText(s.value(OPTION_APPNAME, OPTION_APPNAME_DEFAULT).toString());
    editServer->setText(s.value(OPTION_SERVER).toString());
    comboSelect(cbResolution, s.value(OPTION_RESOLUTION).toString());
    comboSelect(cbFrameRate, s.value(OPTION_FRAMERATE).toString());
    sbBitRate->setValue(abs(s.value(OPTION_BITRATE).toInt()));
    cbBitRateAuto->setChecked(sbBitRate->value() <= 0);
    sbBitRate->setEnabled(!cbBitRateAuto->isChecked());
    comboSelect(cbCodec, s.value(OPTION_CODEC, OPTION_CODEC_DEFAULT).toString());
    bool sops = s.value(OPTION_SYSOPS).toBool();
    rbSysOpsYes->setChecked(sops); rbSysOpsNo->setChecked(!sops);
    comboSelect(cbAudio, s.value(OPTION_AUDIO).toString());

    // Set size
    QSize sz = sizeHint();
    resize(sz);
    setFixedHeight(sz.height());
}

OptionsDialog::~OptionsDialog()
{

}

void OptionsDialog::on_btnAppName_clicked()
{
    QFileInfo fi(editAppName->text());
    QString dir = fi.dir().dirName();
    if(dir.length() <= 1) dir = "/usr/bin";
    QFileDialog fd(this, "Select Moonlight executable");
    fd.setFilter(QDir::Executable | QDir::Files);
    fd.setFileMode(QFileDialog::ExistingFile);
    int r = fd.exec();
    if(r == QDialog::Accepted)
        editAppName->setText(fd.selectedFiles().first());
}

void OptionsDialog::on_cbBitRateAuto_clicked(bool checked)
{
    sbBitRate->setEnabled(!checked);
}

void OptionsDialog::done(int result)
{
    if(result == QDialog::Accepted) {
        // TODO save settings
        QSettings s;
        s.beginGroup(OPTION_GROUP);

        s.setValue(OPTION_APPNAME, editAppName->text());
        s.setValue(OPTION_SERVER, editServer->text());
        s.setValue(OPTION_RESOLUTION, comboGetValue(cbResolution));
        s.setValue(OPTION_FRAMERATE, comboGetValue(cbFrameRate));
        int vbit = sbBitRate->value();
        if(cbBitRateAuto->isChecked()) vbit *= -1;
        s.setValue(OPTION_BITRATE, vbit);
        s.setValue(OPTION_CODEC, comboGetValue(cbCodec));
        s.setValue(OPTION_SYSOPS, rbSysOpsYes->isChecked());
        s.setValue(OPTION_AUDIO, comboGetValue(cbAudio));

    }
    // End dialog
    QDialog::done(result);
}

void OptionsDialog::addComboItem(QComboBox *cb, const QString &title, const QString &value)
{
    cb->addItem(title, value);
}

void OptionsDialog::comboSelect(QComboBox *cb, const QString &value)
{
    int r = cb->findData(value);
    if(r < 0) r = 0;
    cb->setCurrentIndex(r);
}

QString OptionsDialog::comboGetValue(QComboBox *cb)
{
    QVariant v = cb->currentData();
    return v.toString();
}
