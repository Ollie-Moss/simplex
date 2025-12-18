#pragma once

#include "AssetLoader.h"
#include <string>
#include <unordered_map>

class IAssetProvider
{
  public:
};

template <typename TAsset>
class AssetProvider : public IAssetProvider
{
  public:
    template <typename TConfig>
    const TAsset &Load(const std::string &name, const TConfig &config)
    {
        auto [it, inserted] =
            m_assets.try_emplace(name, AssetLoader<TAsset>::template Load<TConfig>(config));
        return it->second;
    }

    const TAsset &Get(const std::string &name)
    {
        return m_assets.at(name);
    }

  private:
    std::unordered_map<std::string, TAsset> m_assets;
};
