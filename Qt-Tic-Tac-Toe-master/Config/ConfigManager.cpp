#include "ConfigManager.h"

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    // No cargamos aquí directamente, porque queremos permitir
    // especificar la ruta desde fuera si se desea.
}

void ConfigManager::load(const QString& filePath)
{
    QString configPath;

    if (!filePath.isEmpty()) {
        configPath = filePath;
    } else {
        // Busca "config.ini" en el directorio actual de ejecución
        QString localPath = QDir::currentPath() + QDir::separator() + "config.ini";
        if (QFile::exists(localPath)) {
            configPath = localPath;
        } else {
            // Busca también en el mismo directorio que el ejecutable
            QString execDir = QCoreApplication::applicationDirPath() + QDir::separator() + "config.ini";
            if (QFile::exists(execDir))
                configPath = execDir;
            else {
                qWarning() << "[ConfigManager] No se encontró config.ini. Se usará configuración por defecto.";
                return;
            }
        }
    }

    if (settings_)
        delete settings_;

    settings_ = new QSettings(configPath, QSettings::IniFormat);
    qDebug() << "[ConfigManager] Archivo de configuración cargado desde:" << configPath;
}

QString ConfigManager::getValue(const QString& key, const QString& defaultValue) const
{
    if (!settings_) {
        qWarning() << "[ConfigManager] getValue(): Configuración no cargada aún.";
        return defaultValue;
    }

    QVariant value = settings_->value(key, defaultValue);
    return value.toString();
}

int ConfigManager::getInt(const QString& key, int defaultValue) const
{
    if (!settings_) {
        qWarning() << "[ConfigManager] getInt(): Configuración no cargada aún.";
        return defaultValue;
    }

    return settings_->value(key, defaultValue).toInt();
}

bool ConfigManager::getBool(const QString& key, bool defaultValue) const
{
    if (!settings_) {
        qWarning() << "[ConfigManager] getBool(): Configuración no cargada aún.";
        return defaultValue;
    }

    return settings_->value(key, defaultValue).toBool();
}
