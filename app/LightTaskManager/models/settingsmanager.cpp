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
        settings->setValue("TodoListBinPath", "todolist");
    settings->endGroup();

    settings->beginGroup("Statuses");
        settings->setValue("Count", 4);
        settings->setValue("Status0", "ToDo");
        settings->setValue("Status1", "InProgress");
        settings->setValue("Status2", "Testing");
        settings->setValue("Status3", "Done");
    settings->endGroup();

    settings->beginGroup("Users");
        settings->setValue("Count", 2);
        settings->setValue("User0", "Xtail");
        settings->setValue("User1", "Valeria");
    settings->endGroup();

    settings->beginGroup("Tags");
        settings->setValue("Count", 3);
        settings->setValue("Tag0", "Important");
        settings->setValue("Tag1", "Task");
        settings->setValue("Tag2", "Bug");
    settings->endGroup();

    saveSettings();
}

QStringList SettingsManager::settingsKeys()
{
    return settings->allKeys();
}
