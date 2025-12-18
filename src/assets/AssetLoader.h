#pragma once

template <typename T>
struct AssetLoader
{
    template <typename TConfig>
    static T Load(const TConfig &config);
};
