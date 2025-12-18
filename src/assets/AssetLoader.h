#pragma once

#include "assets/HasLoaderConfig.h"

template <HasLoaderConfig TAsset>
struct AssetLoader
{
    static TAsset Load(const TAsset::LoaderConfig &config);
};
