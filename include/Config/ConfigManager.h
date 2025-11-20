#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#pragma once
#include <QObject>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

/**
 * @file ConfigManager.h
 * @brief Definition of the ConfigManager class for application configuration management
 * @author Miguel Fernández Lorenzo
 * @version 1.0
 * @date November 2025
 */

/**
 * @brief The ConfigManager class manages the application configuration.
 *
 * Manages the application configuration from a config.ini file.
 * Applies the **Singleton** pattern to ensure only one globally accessible
 * instance exists, providing a centralized point to access program options.
 */
class ConfigManager final : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Gets the unique instance of ConfigManager (Singleton).
     *
     * This is the global access point to the configuration manager.
     *
     * @return Reference to the unique ConfigManager object.
     */
    static ConfigManager& instance();

    /**
     * @brief Loads the configuration file from the specified path.
     *
     * If no path is specified, it will look for a `config.ini` file in the
     * current application directory. Initializes the internal QSettings object.
     *
     * @param filePath Complete path to the .ini file. If empty, uses the default value.
     */
    void load(const QString& filePath = QString());

    /**
     * @brief Returns a value from the INI file as QString.
     *
     * Reads a value from the associated QSettings. If the key doesn't exist, returns the default value.
     *
     * @param key Configuration key (e.g., "network/ip").
     * @param defaultValue Default value if the key is not found.
     * @return Value associated with the key as QString.
     */
    QString getValue(const QString& key, const QString& defaultValue = QString()) const;

    /**
     * @brief Returns a value from the INI file as integer.
     *
     * Converts the key value to an integer.
     *
     * @param key Configuration key (e.g., "audio/samplerate").
     * @param defaultValue Default value if the key is not found.
     * @return Value associated with the key as int.
     */
    int getInt(const QString& key, int defaultValue = 0) const;

    /**
     * @brief Returns a value from the INI file as boolean.
     *
     * Converts the key value to a boolean (true if it's "true", "on", "1", etc.).
     *
     * @param key Configuration key (e.g., "ai/enabled").
     * @param defaultValue Default value if the key is not found.
     * @return Value associated with the key as bool.
     */
    bool getBool(const QString& key, bool defaultValue = false) const;

    /**
     * @brief Resolves a relative path using the application base directory.
     *
     * Uses the base directory established during loading to convert a
     * relative path (e.g., "data/model.dat") to an absolute path.
     *
     * @param relativePath The relative path to resolve.
     * @return The calculated absolute path.
     */
    QString resolvePath(const QString& relativePath) const;

private:
    /**
     * @brief Private constructor.
     *
     * Private to ensure the Singleton pattern.
     *
     * @param parent Pointer to the parent object (QObject).
     */
    explicit ConfigManager(QObject *parent = nullptr);

    /**
     * @brief Destructor.
     *
     * Uses `= default` for standard implementation.
     */
    ~ConfigManager() override = default;

    // Block copy and assignment
    ConfigManager(const ConfigManager&) = delete;            ///< Copy constructor blocked.
    ConfigManager& operator=(const ConfigManager&) = delete; ///< Assignment operator blocked.

    QSettings* settings_ = nullptr; ///< Pointer to the QSettings object that manages the INI file.
    QString baseDir_;               ///< The base directory of the loaded configuration file.
};

#endif // CONFIGMANAGER_H