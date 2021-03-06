#pragma once

// Forward declaration.
class IBestiaryVisitor;

class Applier
{
public:
    virtual void apply(const IBestiaryVisitor&) = 0;
};
