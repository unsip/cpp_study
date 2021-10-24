#pragma once

class Renderer
{
public:
    virtual void render(double point) = 0;
    virtual ~Renderer() = default;
};

