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
void printSer(T& ser)
{
    const auto data = ser.getBuffer();
    size_t sz = ser.getWindowSize();

    std::stringstream ss;
    for (size_t i=0; i<sz; i++)
    {
        ss << (size_t) data[i] << " ";
    }

    auto str = ss.str();
    if (str.size()) str.pop_back();
    printf("ser[%lu]@%lu: %s\n", sz, ser.getOffset(), str.c_str());
}

TEST(SeriesView, shouldLoadFullAndPush)
{
    createSample<uint8_t>(10);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser);
    ser.push(10);
    ser.push(11);
    ser.push(12);
    printSer(ser);
}


TEST(SeriesView, shouldLoadPartAndPush)
{
    createSample<uint8_t>(3);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser);
    ser.push(3);
    ser.push(4);
    ser.push(5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldCreateAndPush)
{
    createSample<uint8_t>(0);
    SeriesView<uint8_t> ser("test.ser", 5);
    printSer(ser);
    ser.push(1);
    ser.push(2);
    ser.push(3);
    ser.push(4);
    ser.push(5);
    ser.push(6);
    ser.push(7);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftWrapping)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(-5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftNonWrapping)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(-5); // wrapping
    ser.scrollBy(-1); // non wrapping
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftFullLoad)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(-20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftOutOfBound)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollNotEnoughSample)
{
    createSample<uint8_t>(6);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    // @todo : expectations
}

/////////////

TEST(SeriesView, shouldScrollRightWrapping)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    ser.scrollBy(5); // non wrapping
    printSer(ser);
    ser.scrollBy(3); // wrapping
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollRightNonWrapping)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(5); // non wrapping
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollRightFullLoad)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollRightOutOfBound)
{
    createSample<uint8_t>(100);
    SeriesView<uint8_t> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(ser.LAST);
    printSer(ser);
    // @todo : expectations
}
