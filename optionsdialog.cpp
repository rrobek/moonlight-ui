#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QSettings>

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
    sbBitRate->setValue(s.value(OPTION_BITRATE).toInt());
    cbBitRateAuto->setChecked(sbBitRate->value() <= 0);
    comboSelect(cbCodec, s.value(OPTION_CODEC, OPTION_CODEC_DEFAULT).toString());
    rbSysOpsYes->setChecked(s.value(OPTION_SYSOPS).toBool());
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

}

void OptionsDialog::on_cbBitRateAuto_clicked(bool checked)
{

}

void OptionsDialog::done(int result)
{
    if(result == QDialog::Accepted) {
        // TODO save settings
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
