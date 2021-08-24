#pragma once

namespace core
{

class SystemInterface
{
public:
    virtual ~SystemInterface() = default;
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
};

}