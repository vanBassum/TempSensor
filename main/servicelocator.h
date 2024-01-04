#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include "esp_log.h"
#include "servicetags.h"

class ServiceContext {
    constexpr static const char* TAG = "ServiceContext";
    std::unordered_map<std::string, std::shared_ptr<void>> services;

public:

    // Add service without interface using a custom tag
    template <typename Concrete, typename... Args>
    void addService(const char* tag, Args&&... args) {
        ESP_LOGI(TAG, "Adding service '%s'", tag);
        if (services.find(tag) != services.end()) {
            ESP_LOGE(TAG, "Error: Service with tag '%s' already exists in ServiceFactory", tag);
            return;
        }

        auto concreteInstance = std::make_shared<Concrete>(std::forward<Args>(args)...);
        services[tag] = std::static_pointer_cast<void>(concreteInstance);
    }



    // Add service with interface and custom tag
    template <typename Interface, typename Concrete, typename... Args>
    void addService(const char* tag, Args&&... args) {
        ESP_LOGI(TAG, "Adding service '%s'", tag);
        if (services.find(tag) != services.end()) {
            ESP_LOGE(TAG, "Error: Service with tag '%s' already exists in ServiceFactory", tag);
            return;
        }

        auto concreteInstance = std::make_shared<Concrete>(std::forward<Args>(args)...);
        services[tag] = std::static_pointer_cast<void>(concreteInstance);
    }

#ifdef CONFIG_COMPILER_CXX_RTTI
    // Add service without interface and without tag
    template <typename Concrete, typename... Args>
    void addService(Args&&... args) {
        const std::string tag = typeid(Concrete).name();

        if (services.find(tag) != services.end()) {
            ESP_LOGE(TAG, "Error: Service of type '%s' already exists in ServiceFactory", tag.c_str());
            return;
        }

        auto concreteInstance = std::make_shared<Concrete>(std::forward<Args>(args)...);
        services[tag] = std::static_pointer_cast<void>(concreteInstance);
    }

    // Add service with interface without tag
    template <typename Interface, typename Concrete, typename... Args>
    void addService(Args&&... args) {
        const std::string tag = typeid(Interface).name();
        if (services.find(tag) != services.end()) {
            ESP_LOGE(TAG, "Error: Service with interface '%s' already exists in ServiceFactory", tag.c_str());
            return;
        }

        auto concreteInstance = std::make_shared<Concrete>(std::forward<Args>(args)...);
        services[tag] = std::static_pointer_cast<void>(concreteInstance);
    }

    // Get service without custom tag
    template <typename TYPE>
    std::shared_ptr<TYPE> getService() const {
        const std::string tag = typeid(TYPE).name();
        auto it = services.find(tag);

        if (it == services.end()) {
            ESP_LOGE(TAG, "Error: Service of type '%s' not found", tag.c_str());
            return nullptr;
        }

        return std::static_pointer_cast<TYPE>(it->second);
    }
#endif

    // Get service using custom tag
    template <typename TYPE>
    std::shared_ptr<TYPE> getService(const char* tag) const {
        auto it = services.find(tag);

        if (it == services.end()) {
            ESP_LOGE(TAG, "Error: Service with tag '%s' not found", tag);
            return nullptr;
        }

        return std::static_pointer_cast<TYPE>(it->second);
    }
};


class Builder {
public:
    ServiceContext Services;

    // Build the application
    template <typename T, typename... Args>
    std::shared_ptr<T> build(Args&&... args) const {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
};
