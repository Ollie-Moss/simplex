#pragma once

class IRendererBase
{
  public:
    virtual ~IRendererBase() = default;
    virtual void SubmitVoid(const void *data) {}
    virtual void Render() = 0;
};

template <typename T>
class IRenderer : public IRendererBase
{
  public:
    virtual void Submit(const T &data) = 0;

    void SubmitVoid(const void *data) override
    {
        Submit(*static_cast<const T *>(data));
    }
};
