#include "settingsmanager.h"

SettingsManager::SettingsManager() : SettingsManager(DEFAULT_SETTINGS_PATH)
{

}

SettingsManager::SettingsManager(QString settingsPath)
{
    if (!QFileInfo::exists(settingsPath))
    {
        QMessageBox(QMessageBox::Warning, "Ошибка",
                    "Файл с настройками не найден. Используем настройки по умолчанию").exec();

        settings = std::shared_ptr<QSettings>( new QSettings(DEFAULT_SETTINGS_PATH, QSettings::IniFormat) );
        settings->setIniCodec("UTF-8");
        generateDefaultSettings();
    }
    else
    {
        settings = std::shared_ptr<QSettings>( new QSettings(settingsPath, QSettings::IniFormat) );
        settings->setIniCodec("UTF-8");
    }
}

SettingsManager::~SettingsManager()
{
    saveSettings();
}

void SettingsManager::saveSettings()
{
    settings->sync();
}

void SettingsManager::exportSettings(QString path)
{
    QSettings expSettings(path, QSettings::IniFormat);
    for (QString key : settings->allKeys())
    {
        expSettings.setValue(key, settings->value(key));
    }
    expSettings.sync();
}

void SettingsManager::importSettings(QString settingsPath)
{
    if (QFileInfo::exists(settingsPath))
    {
        QSettings importSettings(settingsPath, QSettings::IniFormat);

        for (QString key : importSettings.allKeys())
        {
            settings->setValue(key, importSettings.value(key));
        }

        saveSettings();

        QMessageBox(QMessageBox::Information, "Информация",
                    "Настройки импортированы. Перезапустите приложение для их применения.").exec();
    }
}

QVariant SettingsManager::get(QString group, QString key) const
{
    QVariant value;
    settings->beginGroup(group);
    if (settings->contains(key))
    {
        value = settings->value(key);
    }
    else
    {
        settings->endGroup();
        throw std::invalid_argument("Отсутствует ключ " + key.toStdString());
    }
    settings->endGroup();
    return value;
}

void SettingsManager::set(QString group, QString key, QVariant value)
{
    settings->beginGroup(group);
        settings->setValue(key, value);
    settings->endGroup();
}

void SettingsManager::generateDefaultSettings()
{
    settings->beginGroup("General");
        settings->setValue("TodoListBinPath", "/usr/local/opt/todolist/bin/todolist");
    settings->endGroup();

    saveSettings();
}

QStringList SettingsManager::settingsKeys()
{
    return settings->allKeys();
}
