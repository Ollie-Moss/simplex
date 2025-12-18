#pragma once

#include "AssetLoader.h"
#include "assets/HasLoaderConfig.h"
#include <string>
#include <unordered_map>

class IAssetProvider
{
  public:
};

template <HasLoaderConfig TAsset>
class AssetProvider : public IAssetProvider
{
  public:
    const TAsset &Load(const std::string &name, const TAsset::LoaderConfig &config)
    {
        auto [it, inserted] =
            m_assets.try_emplace(name, AssetLoader<TAsset>::Load(config));
        return it->second;
    }

    const TAsset &Get(const std::string &name)
    {
        return m_assets.at(name);
    }

  private:
    std::unordered_map<std::string, TAsset> m_assets;
};
