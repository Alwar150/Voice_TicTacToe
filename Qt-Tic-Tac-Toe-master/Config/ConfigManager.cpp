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
        configPath = QDir::cleanPath(filePath);
    } else {
        // Si existe variable de entorno CONFIG_PATH, usarla
        QString envPath = qEnvironmentVariable("CONFIG_PATH");
        if (!envPath.isEmpty() && QFile::exists(envPath)) {
            configPath = QDir::cleanPath(envPath);
        } else {
            // Busca config.ini en el directorio actual
            QDir currentDir(QDir::currentPath());
            QString localPath = currentDir.filePath("config.ini");

            if (QFile::exists(localPath)) {
                configPath = QDir::cleanPath(localPath);
            } else {
                // Busca también en el mismo directorio del ejecutable
                QDir execDir(QCoreApplication::applicationDirPath());
                QString execPath = execDir.filePath("config.ini");

                if (QFile::exists(execPath))
                    configPath = QDir::cleanPath(execPath);
                else {
                    qWarning() << "[ConfigManager] No se encontró config.ini.";
                    return;
                }
            }
        }
    }

    if (settings_)
        delete settings_;

    settings_ = new QSettings(configPath, QSettings::IniFormat);

    // ⚙️ Guardamos la ruta base para resolver rutas relativas más adelante
    baseDir_ = QFileInfo(configPath).absolutePath();

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

QString ConfigManager::resolvePath(const QString& relativePath) const {
    if (QDir(relativePath).isAbsolute())
        return relativePath; // ya es absoluta
    return QDir(baseDir_).filePath(relativePath);
}
