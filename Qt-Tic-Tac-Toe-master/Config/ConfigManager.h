#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#pragma once
#include <QObject>
#include <QSettings>
#include <QDir>
#include <QDebug>

/**
 * @brief The ConfigManager class
 * 
 * Gestiona la configuración de la aplicación desde un archivo config.ini.
 * Aplica el patrón Singleton para asegurar que solo existe una instancia
 * accesible globalmente.
 */
class ConfigManager final : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Obtiene la instancia única del ConfigManager (Singleton)
     * 
     * @return Referencia al único ConfigManager.
     */
    static ConfigManager& instance();

    /**
     * @brief Carga el archivo de configuración desde la ruta indicada.
     * Si no se especifica ruta, buscará config.ini en el directorio actual.
     * 
     * @param filePath Ruta al archivo .ini
     */
    void load(const QString& filePath = QString());

    /**
     * @brief Devuelve un valor del archivo INI como QString.
     * 
     * @param key Clave (por ejemplo, "network/ip")
     * @param defaultValue Valor por defecto si no se encuentra la clave
     * @return Valor asociado
     */
    QString getValue(const QString& key, const QString& defaultValue = QString()) const;

    /**
     * @brief Devuelve un valor como entero.
     */
    int getInt(const QString& key, int defaultValue = 0) const;

    /**
     * @brief Devuelve un valor como booleano.
     */
    bool getBool(const QString& key, bool defaultValue = false) const;

private:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager() override = default;

    // Bloquear copia y asignación
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    QSettings* settings_ = nullptr;
};

#endif // CONFIGMANAGER_H
