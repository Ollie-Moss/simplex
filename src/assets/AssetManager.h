#pragma once

#include "AssetProvider.h"
#include "assets/HasLoaderConfig.h"
#include "core/Types.h"
#include <cassert>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>

class AssetManager
{
  public:
    AssetManager() : m_Index(0) {}

    // Load default assets
    bool Init();

    // Ideally names should not conflict
    template <HasLoaderConfig TAsset>
    const TAsset &Get(const std::string &name)
    {
        std::shared_ptr<AssetProvider<TAsset>> provider = GetAssetProvider<TAsset>();

        return provider->Get(name);
    }

    template <HasLoaderConfig TAsset>
    const TAsset &Load(const std::string &name, const TAsset::LoaderConfig &config)
    {
        std::shared_ptr<AssetProvider<TAsset>> provider = GetAssetProvider<TAsset>();

        return provider->Load(name, config);
    };

  public:
  private:
    template <HasLoaderConfig TAsset>
    std::shared_ptr<AssetProvider<TAsset>> GetAssetProvider()
    {
        std::type_index type = typeid(TAsset);

        // Already exists?
        auto it = m_DataTypeToAssetProviderIndex.find(type);
        if(it != m_DataTypeToAssetProviderIndex.end())
        {
            uint32_t index = it->second;
            return std::static_pointer_cast<AssetProvider<TAsset>>(m_AssetProviders[index]);
        }

        // Create new provider
        assert(m_Index < MAX_ASSET_PROVIDERS && "Too many asset types");

        uint32_t newIndex = m_Index++;
        auto provider = std::make_shared<AssetProvider<TAsset>>();

        m_AssetProviders[newIndex] = provider;
        m_DataTypeToAssetProviderIndex[type] = newIndex;

        return provider;
    }

  private:
    std::array<std::shared_ptr<IAssetProvider>, MAX_ASSET_PROVIDERS> m_AssetProviders;

    std::unordered_map<std::type_index, uint32_t> m_DataTypeToAssetProviderIndex;

    uint32_t m_Index;
};
