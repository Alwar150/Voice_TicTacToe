#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#pragma once
#include <QObject>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

/**
 * @brief La clase ConfigManager gestiona la configuración de la aplicación.
 *
 * Gestiona la configuración de la aplicación desde un archivo config.ini.
 * Aplica el patrón **Singleton** para asegurar que solo existe una instancia
 * accesible globalmente, proporcionando un punto centralizado para acceder
 * a las opciones del programa.
 */
class ConfigManager final : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Obtiene la instancia única del ConfigManager (Singleton).
     *
     * Este es el punto de acceso global al gestor de configuración.
     *
     * @return Referencia al único objeto ConfigManager.
     */
    static ConfigManager& instance();

    /**
     * @brief Carga el archivo de configuración desde la ruta indicada.
     *
     * Si no se especifica ruta, buscará un archivo `config.ini` en el directorio
     * actual de la aplicación. Inicializa el objeto QSettings interno.
     *
     * @param filePath Ruta completa al archivo .ini. Si está vacío, usa el valor por defecto.
     */
    void load(const QString& filePath = QString());

    /**
     * @brief Devuelve un valor del archivo INI como QString.
     *
     * Lee un valor del QSettings asociado. Si la clave no existe, devuelve el valor por defecto.
     *
     * @param key Clave de la configuración (por ejemplo, "network/ip").
     * @param defaultValue Valor por defecto si no se encuentra la clave.
     * @return Valor asociado a la clave como QString.
     */
    QString getValue(const QString& key, const QString& defaultValue = QString()) const;

    /**
     * @brief Devuelve un valor del archivo INI como entero.
     *
     * Convierte el valor de la clave a un entero.
     *
     * @param key Clave de la configuración (por ejemplo, "audio/samplerate").
     * @param defaultValue Valor por defecto si no se encuentra la clave.
     * @return Valor asociado a la clave como int.
     */
    int getInt(const QString& key, int defaultValue = 0) const;

    /**
     * @brief Devuelve un valor del archivo INI como booleano.
     *
     * Convierte el valor de la clave a un booleano (true si es "true", "on", "1", etc.).
     *
     * @param key Clave de la configuración (por ejemplo, "ai/enabled").
     * @param defaultValue Valor por defecto si no se encuentra la clave.
     * @return Valor asociado a la clave como bool.
     */
    bool getBool(const QString& key, bool defaultValue = false) const;

    /**
     * @brief Resuelve una ruta relativa utilizando el directorio base de la aplicación.
     *
     * Utiliza el directorio base establecido durante la carga para convertir una
     * ruta relativa (ej. "data/model.dat") en una ruta absoluta.
     *
     * @param relativePath La ruta relativa a resolver.
     * @return La ruta absoluta calculada.
     */
    QString resolvePath(const QString& relativePath) const;

private:
    /**
     * @brief Constructor privado.
     *
     * Privado para asegurar el patrón Singleton.
     *
     * @param parent Puntero al objeto padre (QObject).
     */
    explicit ConfigManager(QObject *parent = nullptr);

    /**
     * @brief Destructor.
     *
     * Uso de `= default` para la implementación estándar.
     */
    ~ConfigManager() override = default;

    // Bloquear copia y asignación
    ConfigManager(const ConfigManager&) = delete;            ///< Constructor de copia bloqueado.
    ConfigManager& operator=(const ConfigManager&) = delete; ///< Operador de asignación bloqueado.

    QSettings* settings_ = nullptr; ///< Puntero al objeto QSettings que maneja el archivo INI.
    QString baseDir_;               ///< El directorio base del archivo de configuración cargado.
};

#endif // CONFIGMANAGER_H
