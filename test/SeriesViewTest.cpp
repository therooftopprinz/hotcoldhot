#include <sstream>
#include <gtest/gtest.h>

#include <SeriesView.hpp>

template<typename T>
void createSample(size_t count)
{
    auto fp = fopen("test.ser", "w+");
    for (T i = 0; i<count; i++)
    {
        fwrite(&i, sizeof(i), 1, fp);
    }
    fclose(fp);
}

template<typename T>
void printSer(const T* data, size_t sz)
{
    std::stringstream ss;
    for (size_t i=0; i<sz; i++)
    {
        ss << (size_t) data[i] << " ";
    }

    auto str = ss.str();
    if (str.size()) str.pop_back();
    printf("ser[%ld]: %s\n", sz, str.c_str());
}

TEST(SeriesView, shouldLoadFullAndPush)
{
    createSample<uint8_t>(10);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
    ser.push(10);
    ser.push(11);
    ser.push(12);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
}


TEST(SeriesView, shouldLoadPartAndPush)
{
    createSample<uint8_t>(3);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
    ser.push(3);
    ser.push(4);
    ser.push(5);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
}

TEST(SeriesView, shouldCreateAndPush)
{
    createSample<uint8_t>(0);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
    ser.push(1);
    ser.push(2);
    ser.push(3);
    ser.push(4);
    ser.push(5);
    ser.push(6);
    ser.push(7);
    printSer(ser.getBuffer(), ser.getWindowSize());
    printf("off: %lu\n", ser.getOffset());
}