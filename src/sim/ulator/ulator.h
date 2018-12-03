#pragma once


class Ulator
{
public:
    virtual ~Ulator() {}
    virtual bool start() = 0;
    virtual void stop() = 0;
};