// IMPORTED FROM https://github.com/therooftopprinz/bfc

/*
 * Copyright (C) 2023 Prinz Rainer Buyo <mynameisrainer@gmail.com>
 *
 * MIT License 
 * 
 */

#ifndef __BFC_BUFFER_HPP__
#define __BFC_BUFFER_HPP__

#include <type_traits>
#include <cstdint>
#include <functional>

namespace bfc
{

template<typename T>
class BufferImpl
{
    static_assert(sizeof(T)==1);
public:
    template <typename U>
    BufferImpl(U* pData, size_t pSize, std::function<void(T*)> pDeleter = [](T* pPtr){delete[] (T*)pPtr;})
        : mSize(pSize)
        , mData(pData)
        , mDeleter(std::move(pDeleter))
    {
        static_assert(sizeof(U)==1);
    }

    ~BufferImpl()
    {
        reset();
    }

    BufferImpl() = default;
    BufferImpl(const BufferImpl&) = delete;
    void operator=(const BufferImpl&) = delete;

    BufferImpl(BufferImpl&& pOther) noexcept
    {
        reset();
        transferOwnership(std::move(pOther));
    }

    BufferImpl& operator=(BufferImpl&& pOther) noexcept
    {
        reset();
        transferOwnership(std::move(pOther));
        return *this;
    }

    T* data() const
    {
        return mData;
    }

    size_t size() const
    {
        return mSize;
    }

    void reset() noexcept
    {
        if (mData)
        {
            mDeleter((T*)mData);
        }
        clear(std::move(*this));
    }

private:
    static void clear(BufferImpl&& pOther) noexcept
    {
        pOther.mData = nullptr;
        pOther.mSize = 0;
        pOther.mDeleter = nullptr;
    }

    void transferOwnership(BufferImpl&& pOther) noexcept
    {
        mData = pOther.mData;
        mSize = pOther.mSize;
        mDeleter = std::move(pOther.mDeleter);
        clear(std::move(pOther));
    };

    size_t mSize = 0;
    T* mData = nullptr;
    std::function<void(T*)>  mDeleter;
};

using Buffer = BufferImpl<uint8_t>;
using ConstBuffer = BufferImpl<const uint8_t>;

template<typename T>
class BufferViewImpl
{
    using NCT = typename std::remove_const<T>::type;
    using CT = typename std::add_const<NCT>::type;
    static constexpr bool IS_T_CONST = std::is_const<T>::value;
    static_assert(sizeof(T)==1);
public:
    /* Constructors
    ** NCT -> CT  OK
    ** NCT -> NCT OK
    ** CT  -> CT  OK
    ** CT  -> NCT NOK
    */ 
    template<template<class> class U, class V>
    BufferViewImpl(const U<V>& pBuffer,
        typename std::enable_if<std::is_same<V,NCT>::value||std::is_same<V,T>::value>::type* = 0)
        : mSize(pBuffer.size())
        , mData(pBuffer.data())
    {}

    template <typename U>
    BufferViewImpl(U* data, size_t size,
        typename std::enable_if<(std::is_const<U>::value && IS_T_CONST) || !std::is_const<U>::value>::type* = 0)
        : mSize(size)
        , mData(data)
    {
        static_assert(sizeof(U)==1);
    }

    template<template<class> class U, class V>
    typename std::enable_if<std::is_same<V,NCT>::value||std::is_same<V,T>::value,BufferViewImpl>::type&
        operator=(U<V> pBuffer)
    {
        mSize = pBuffer.size();
        mData = pBuffer.data();
        return *this;
    }

    T* data() const
    {
        return mData;
    }

    size_t size() const
    {
        return mSize;
    }

private:
    size_t mSize = 0;
    T* mData = nullptr;
};

using BufferView = BufferViewImpl<uint8_t>;
using ConstBufferView = BufferViewImpl<const uint8_t>;

} // namespace bfc

#endif // __BFC_BUFFER_HPP__