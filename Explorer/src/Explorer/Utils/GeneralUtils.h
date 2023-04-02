#pragma once

template<typename T1, typename T2>
struct IsSameType
{
    operator bool()
    {
        return false;
    }
};

template<typename T1>
struct IsSameType<T1, T1>
{
    operator bool()
    {
        return true;
    }
};