#pragma once

#include "gui/utility/ChildSpecificiation.h"
#include "gui/utility/UISpecification.h"
#include <functional>

template <typename T>
inline std::function<std::vector<UISpecification>()> Map(std::vector<T> dataSource, std::function<UISpecification(T &)> transform)
{
    return [dataSource, transform]() {
        std::vector<UISpecification> specs;
        for(auto data : dataSource)
        {
            specs.push_back(transform(data));
        }
        return specs;
    };
}
