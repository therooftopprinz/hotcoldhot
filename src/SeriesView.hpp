#ifndef __SERIESVIEW_HPP__
#define __SERIESVIEW_HPP__

#include <string>
#include <cstdio>
#include <limits>
#include <vector>

template <typename T>
class SeriesView
{
public:
    static const int LAST = std::numeric_limits<int>::max();
    static const int FIRST = std::numeric_limits<int>::min();

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
        fileOffset = fileSize -  window.size()*sizeof(T);
        windowA = 0;

        if (fileOffset < 0)
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
            windowB = 0;
        }

        fseek(fp, fileOffset, SEEK_SET);
        fread(window.data(), sizeof(int), window.size(), fp);
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

    // void setWindowSize(unsigned size)
    // {
    //     window.resize(size);
    //     // reload
    // }

    void scrollBy(int64_t rpos)
    {
        if (rpos > isViewEnd())
        {
            return;
        }
    }

    void push(T t)
    {
        bool isEnd = isViewEnd();
        fseek(fp, 0, SEEK_END);
        fwrite(&t, sizeof(t), 1, fp);
        fileSize += sizeof(T);

        if (isEnd)
        {
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
    int64_t windowA = 0;
    // @note : exclusive
    int64_t windowB;
    int64_t fileOffset = 0;
    int64_t fileSize = 0;
};


#endif // __SERIESVIEW_HPP__