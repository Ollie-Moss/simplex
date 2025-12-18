#pragma once

template <typename T>
concept HasLoaderConfig = requires {
    typename T::LoaderConfig;
};
