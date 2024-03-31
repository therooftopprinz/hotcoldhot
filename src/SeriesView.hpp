#ifndef __SERIESVIEW_HPP__
#define __SERIESVIEW_HPP__

#include <string>
#include <cstdio>
#include <limits>
#include <vector>
#include <cmath>

#include "lvgl.h"

template <typename T>
class SeriesView
{
public:
    static constexpr auto LAST = std::numeric_limits<int64_t>::max();
    static constexpr auto FIRST = std::numeric_limits<int64_t>::min()+1;

    SeriesView(std::string path, size_t sz)
        : fp(fopen(path.c_str(), "a+"))
        , window(sz)
    {
        if (nullptr == fp)
        {
            return;
        }

        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        windowA = 0;

        if (window.size()*sizeof(T) > fileSize)
        {
            fileOffset = 0;
            if (fileSize)
            {
                windowB = windowIndex(offsetToIndex(fileSize));
            }
            else
            {
                windowB = 0;
            }
        }
        else
        {
            fileOffset = fileSize -  window.size()*sizeof(T);
            windowB = 0;
        }

        load(fileOffset, 0, window.size()*sizeof(T));
    }

    bool isViewBegin()
    {
        return fileOffset == 0;
    }

    bool isViewEnd()
    {
        return (fileOffset + window.size()*sizeof(T)) >= fileSize;
    }

    T* getBuffer()
    {
        return window.data();
    }

    size_t getWindowSize()
    {
        return fileSize ? windowRDist(windowA, windowB) : 0;
    }

    size_t getOffset()
    {
        return windowA;
    }

    int64_t getFileOffset()
    {
        return fileOffset;
    }

    void setWindowSize(unsigned size)
    {
        if (window.size() == size)
        {
            return;
        }

        if (fileSize == 0)
        {
            fileOffset = 0;
            window.resize(sizeof(T)*4);
            windowA = 0;
            windowB = 0;
            return;
        }

        if (size*sizeof(T) > fileSize)
        {
            fileOffset = 0;
            window.resize(fileSize/sizeof(T));
            load(fileOffset, 0, window.size()*sizeof(T));
            windowA = 0;
            windowB = windowIndex(windowA + window.size());
            return;
        }

        auto oldSize = window.size();
        window.resize(size);
        auto scroll = int64_t(oldSize) - size;
        load(fileOffset, 0, window.size()*sizeof(T));
        windowA = 0;
        windowB = 0;
        scrollBy(scroll);
    }

    void scrollBy(int64_t rpos)
    {
        if (!rpos || windowA != windowB)
        {
            return;
        }

        if (rpos>0)
        {
            scrollRight(rpos);
            return;
        }
        scrollLeft(llabs(rpos));
    }

    void load(int64_t fpos, size_t offset, size_t size)
    {
        if (!size)
        {
            return;
        }

        fseek(fp, fpos, SEEK_SET);
        fread(window.data()+offset, size, 1, fp);
    }

    void scrollLeft(int64_t rpos)
    {
        if (isViewBegin())
        {
            return;
        }

        size_t newFO;
        if (rpos*sizeof(T) > fileOffset)
        {
            newFO = 0;
        }
        else
        {
            newFO = fileOffset - rpos*sizeof(T);
        }

        auto diff = (fileOffset - newFO)/sizeof(T);
        fileOffset = newFO;
        // case: full load
        if (diff > window.size())
        {
            load(newFO, 0, window.size()*sizeof(T));
            windowA = 0;
            windowB = 0;
            return;
        }

        // case: wrapping
        if (diff > windowA)
        {
            auto newWO = window.size() + windowA - diff;
            auto size1 = window.size() - newWO;
            auto size2 = windowA;
            load(newFO, newWO, size1*sizeof(T));
            load(newFO+size1*sizeof(T), 0, size2*sizeof(T));
            windowA = newWO;
            windowB = newWO;
            return;
        }

        // case: non-wrapping
        load(newFO, windowA-diff, diff*sizeof(T));
        windowA -= diff;
        windowB -= diff;
    }

    void scrollRight(int64_t rpos)
    {
        if (isViewEnd())
        {
            return;
        }

        auto newFO = fileOffset+rpos*sizeof(T);
        if ((newFO+window.size()*sizeof(T)) > fileSize || rpos == LAST)
        {
            newFO = fileSize - window.size()*sizeof(T);
        }

        auto diff = (newFO - fileOffset)/sizeof(T);
        fileOffset = newFO;
        // case: full load
        if (diff > window.size())
        {
            load(newFO, 0, window.size()*sizeof(T));
            windowA = 0;
            windowB = 0;
            return;
        }

        // case: wrapping
        if (windowA + diff >= window.size())
        {
            auto size1 = window.size() - windowA;
            auto size2 = windowIndex(windowA + diff);
            auto off1 = (window.size() - diff);
            auto off2 = (window.size() - diff) + size1;
            load(newFO + off1*sizeof(T), windowA, size1*sizeof(T));
            load(newFO + off2*sizeof(T), 0, size2*sizeof(T));
            windowA = size2;
            windowB = size2;
            return;
        }

        // case: non-wrapping
        auto off = (window.size()-diff);
        load(newFO + off*sizeof(T), windowA, diff*sizeof(T));
        windowA += diff;
        windowB += diff;
    }

    void push(T t)
    {
        bool isEnd = isViewEnd();
        fseek(fp, 0, SEEK_END);
        fwrite(&t, sizeof(T), 1, fp);
        fileSize = ftell(fp);

        if (!isEnd)
        {
            return;
        }

        window[windowB] = t;
        if (windowLDist(windowB, windowA) == window.size())
        {
            windowB = windowIndex(windowB+1);
            windowA = windowIndex(windowA+1);
            fileOffset += sizeof(T);
        }
        else
        {
            windowB = windowIndex(windowB+1);
        }
    }

private:

    int64_t offsetToIndex(int64_t o)
    {
        return o/sizeof(T);
    }

    int64_t windowIndex(int64_t i)
    {
        return i % window.size();
    }

    int64_t windowRDist(int64_t a, int64_t b)
    {
        a = windowIndex(a);
        b = windowIndex(b);
        if (b > a)
        {
            return b - a;
        }
        return window.size() - a + b;
    }

    int64_t windowLDist(int64_t a, int64_t b)
    {
        a = windowIndex(a);
        b = windowIndex(b);
        if (a > b)
        {
            return a - b;
        }
        return window.size() - b + a;
    }

    FILE* fp = nullptr;
    std::vector<T> window;
    // @note : inclusive
    size_t windowA = 0;
    // @note : exclusive
    size_t windowB;
    size_t fileOffset = 0;
    size_t fileSize = 0;
};


#endif // __SERIESVIEW_HPP__