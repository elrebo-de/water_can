#include "ssid_manager.h"

#include <algorithm>
#include <esp_log.h>

#include "generic_nvsflash.hpp"

#define TAG "SsidManager"
#define MAX_WIFI_SSID_COUNT 10

SsidManager::SsidManager() {
    LoadFromNvs();
}

SsidManager::~SsidManager() {
}

void SsidManager::Clear() {
    ssid_list_.clear();
    SaveToNvs();
}

void SsidManager::LoadFromNvs() {
    ssid_list_.clear();

    // Load ssid and password from NVS from namespace "wifi"
    // ssid, ssid1, ssid2, ... ssid9
    // password, password1, password2, ... password9
    GenericNvsFlash nvsWifi(std::string("nvsWifi"), std::string("wifi"), NVS_READONLY);

   if (!nvsWifi.NvsNamespaceOpen()) {
        // The namespace doesn't exist, just return
        ESP_LOGW(TAG, "NVS namespace %s doesn't exist", std::string("wifi"));
        return;
    }
    esp_err_t ret = ESP_OK;

    for (int i = 0; i < MAX_WIFI_SSID_COUNT; i++) {
        std::string ssid_key = "ssid";
        if (i > 0) {
            ssid_key += std::to_string(i);
        }
        std::string password_key = "password";
        if (i > 0) {
            password_key += std::to_string(i);
        }

        std::string ssid = nvsWifi.GetStr(ssid_key, &ret);
        if (ret != ESP_OK) {
            continue;
        }

        std::string password = nvsWifi.GetStr(password_key, &ret);
        if (ret != ESP_OK) {
            continue;
        }
        ssid_list_.push_back({ssid.c_str(), password.c_str()});
    }
}

void SsidManager::SaveToNvs() {
    GenericNvsFlash nvsWifi(std::string("nvsWifi"), std::string("wifi"), NVS_READWRITE);

    if (!nvsWifi.NvsNamespaceOpen()) {
        // simulate ESP_ERR_INVALID_ARG error
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    }
    esp_err_t ret = ESP_OK;

    for (int i = 0; i < MAX_WIFI_SSID_COUNT; i++) {
        std::string ssid_key = "ssid";
        if (i > 0) {
            ssid_key += std::to_string(i);
        }
        std::string password_key = "password";
        if (i > 0) {
            password_key += std::to_string(i);
        }
        
        if (i < ssid_list_.size()) {
            ret = nvsWifi.SetStr(ssid_key, ssid_list_[i].ssid);
            ret = nvsWifi.SetStr(password_key, ssid_list_[i].password);
        } else {
            ret = nvsWifi.EraseKey(ssid_key);
            ret = nvsWifi.EraseKey(password_key);
        }
    }
}

void SsidManager::AddSsid(const std::string& ssid, const std::string& password) {
    for (auto& item : ssid_list_) {
        ESP_LOGI(TAG, "compare [%s:%d] [%s:%d]", item.ssid.c_str(), item.ssid.size(), ssid.c_str(), ssid.size());
        if (item.ssid == ssid) {
            ESP_LOGW(TAG, "SSID %s already exists, overwrite it", ssid.c_str());
            item.password = password;
            SaveToNvs();
            return;
        }
    }

    if (ssid_list_.size() >= MAX_WIFI_SSID_COUNT) {
        ESP_LOGW(TAG, "SSID list is full, pop one");
        ssid_list_.pop_back();
    }
    // Add the new ssid to the front of the list
    ssid_list_.insert(ssid_list_.begin(), {ssid, password});
    SaveToNvs();
}

void SsidManager::RemoveSsid(int index) {
    if (index < 0 || index >= ssid_list_.size()) {
        ESP_LOGW(TAG, "Invalid index %d", index);
        return;
    }
    ssid_list_.erase(ssid_list_.begin() + index);
    SaveToNvs();
}

void SsidManager::SetDefaultSsid(int index) {
    if (index < 0 || index >= ssid_list_.size()) {
        ESP_LOGW(TAG, "Invalid index %d", index);
        return;
    }
    // Move the ssid at index to the front of the list
    auto item = ssid_list_[index];
    ssid_list_.erase(ssid_list_.begin() + index);
    ssid_list_.insert(ssid_list_.begin(), item);
    SaveToNvs();
}
