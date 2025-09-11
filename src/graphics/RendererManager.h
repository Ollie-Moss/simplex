#pragma once

#include "core/Types.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "graphics/IRenderer.h"

class RendererManager
{
  public:
    template <typename TData, typename TRenderer>
    void Register()
    {
        assert(!HasRenderer<TData>() && "Registering renderer more than once!");

        const char *typeName = typeid(TData).name();
        std::shared_ptr<IRendererBase> renderer = std::make_shared<TRenderer>();
        m_Renderers[m_Index] = renderer;
        m_DataTypes[typeName] = m_Index;
        ++m_Index;
    }

    template <typename TData>
    void Submit(TData *data)
    {
        assert(HasRenderer<TData>() && "No renderer found for this data type!");

        std::shared_ptr<IRendererBase> renderer = GetRenderer<TData>();
        renderer->SubmitVoid(data);
    }

  public:
  private:
    template <typename TData>
    bool HasRenderer()
    {
        const char *typeName = typeid(TData).name();
        return m_DataTypes.find(typeName) != m_DataTypes.end();
    }

    template <typename TData>
    std::shared_ptr<IRendererBase> GetRenderer()
    {
        const char *typeName = typeid(TData).name();
        uint32_t id = m_DataTypes.find(typeName)->second;
        return m_Renderers[id];
    }

  private:
    std::array<std::shared_ptr<IRendererBase>, MAX_RENDERERS> m_Renderers;
    std::unordered_map<const char *, uint32_t> m_DataTypes;

    uint32_t m_Index;
};
