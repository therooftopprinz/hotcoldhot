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
    createSample<short>(10);
    SeriesView<short> ser("test.ser", 5);
    printSer(ser);
    ser.push(10);
    ser.push(11);
    ser.push(12);
    printSer(ser);
}


TEST(SeriesView, shouldLoadPartAndPush)
{
    createSample<short>(3);
    SeriesView<short> ser("test.ser", 5);
    printSer(ser);
    ser.push(3);
    ser.push(4);
    ser.push(5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldCreateAndPush)
{
    createSample<short>(0);
    SeriesView<short> ser("test.ser", 5);
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
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(-5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftNonWrapping)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(-5); // wrapping
    ser.scrollBy(-1); // non wrapping
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftFullLoad)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(-20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollLeftOutOfBound)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollNotEnoughSample)
{
    createSample<short>(6);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    // @todo : expectations
}

/////////////

TEST(SeriesView, shouldScrollRightWrapping)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
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
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(5); // non wrapping
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollRightFullLoad)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldScrollRightOutOfBound)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    ser.scrollBy(ser.LAST);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldViewExpandRightWall)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    printSer(ser);
    ser.setWindowSize(20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldViewShrinkRightWall)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    printSer(ser);
    ser.setWindowSize(5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldViewExpandLefWall)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    ser.setWindowSize(20);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldViewShrinkLefWall)
{
    createSample<short>(100);
    SeriesView<short> ser("test.ser", 10);
    ser.scrollBy(ser.FIRST);
    printSer(ser);
    ser.setWindowSize(5);
    printSer(ser);
    // @todo : expectations
}

TEST(SeriesView, shouldViewExpandToFileSize)
{
    createSample<short>(20);
    SeriesView<short> ser("test.ser", 10);
    printSer(ser);
    ser.setWindowSize(30);
    printSer(ser);
    // @todo : expectations
}

#include <cmath>

TEST(SeriesView, shouldNotUpdateViewWhenNotAtEnd)
{

    size_t count = 60*60;
    auto fp1 = fopen("target.ser", "w+");
    auto fp2 = fopen("actual.ser", "w+");

    for (size_t t = 0; t<count; t++)
    {
        short x = (sin(2*t*3.14/600)*0.25+0.5)*sin(2*t*3.14/60)*5000+5000;
        short y = (sin(2*t*3.14/600)*0.25+0.5)*cos(2*t*3.14/60)*5000+5000;

        fwrite(&x, sizeof(short), 1, fp1);
        fwrite(&y, sizeof(short), 1, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}
