#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <numeric>
#include <algorithm>
#include <cassert>

#include "xxhash.h"


#define STRINGIFY_UTIL_(s) #s
#define STRINGIFY(s) STRINGIFY_UTIL_(s)

namespace fs = std::filesystem;

template <typename T>
using MyArray = std::vector<T>;
template <typename K, typename T>
using MyDict = std::unordered_map<K, T>;
using CharString = std::string;
using StringArray = MyArray<CharString>;
using BytesArray = MyArray<uint8_t>;


#define rcast reinterpret_cast
#define scast static_cast

#ifdef __GNUC__
#define PACKED_STRUCT_BEGIN
#define PACKED_STRUCT_END __attribute__((__packed__))
#else
#define PACKED_STRUCT_BEGIN __pragma(pack(push, 1))
#define PACKED_STRUCT_END __pragma(pack(pop))
#endif


struct RefString {
    static const uint32_t InvalidRef = ~0u;

    RefString() : ref(InvalidRef) {}
    RefString(const RefString& other) : ref(other.ref), str(other.str) {}

    inline bool IsValidRef() const {
        return this->ref != InvalidRef;
    }

    bool operator ==(const RefString& other) const {
        if (this->IsValidRef()) {
            return ref == other.ref;
        } else {
            return str == other.str;
        }
    }

    uint32_t    ref;
    CharString  str;
};

struct Hasher {
    static uint32_t FromData(const void* data, const size_t length) {
        return XXH32(data, length, 0);
    }

    static uint32_t FromString(const CharString& str) {
        return XXH32(str.data(), str.length(), 0);
    }
};


struct HashString {
    HashString() : hash(0u) {}
    HashString(const HashString& other) : hash(other.hash), str(other.str) {}
    HashString(const CharString& other) { *this = other; }

    inline void operator =(const HashString& other) { hash = other.hash; str = other.str; }
    inline void operator =(const CharString& other) { str = other; hash = Hasher::FromString(other); }

    inline bool operator ==(const HashString& other) const { return hash == other.hash; }
    inline bool operator !=(const HashString& other) const { return hash != other.hash; }

    inline bool operator <(const HashString& other) const { return hash < other.hash; }
    inline bool operator >(const HashString& other) const { return hash > other.hash; }

    uint32_t    hash;
    CharString  str;
};

namespace std {
    template <> struct hash<HashString> {
        size_t operator()(const HashString& s) const {
            return scast<size_t>(s.hash);
        }
    };
}


inline bool StrEndsWith(const CharString& str, const CharString& ending) {
    return str.size() >= ending.size() && str.compare(str.size() - ending.size(), ending.size(), ending) == 0;
}


class MemStream {
public:
    MemStream(const void* _data, const size_t _size)
        : data(rcast<const uint8_t*>(_data))
        , length(_size)
        , cursor(0) {
    }
    ~MemStream() {}

    inline bool Ended() const {
        return this->cursor == this->length;
    }

    inline size_t Remains() const {
        return this->length - this->cursor;
    }

    void ReadToBuffer(void* buffer, const size_t bytesToRead) {
        if (this->cursor + bytesToRead <= this->length) {
            std::memcpy(buffer, this->data + this->cursor, bytesToRead);
            this->cursor += bytesToRead;
        }
    }

    void SkipBytes(const size_t bytesToSkip) {
        if (this->cursor + bytesToSkip <= this->length) {
            this->cursor += bytesToSkip;
        } else {
            this->cursor = this->length;
        }
    }

    template <typename T>
    T ReadTyped() {
        T result = T(0);
#if 0
        this->ReadToBuffer(&result, sizeof(T));
#else
        if (this->cursor + sizeof(T) <= this->length) {
            result = *rcast<const T*>(this->data + this->cursor);
            this->cursor += sizeof(T);
        }
#endif
        return result;
    }

    template <typename T>
    void ReadStruct(T& s) {
        this->ReadToBuffer(&s, sizeof(T));
    }

    CharString ReadStringZ() {
        CharString result;
#if 0
        for (char ch = this->ReadTyped<char>(); ch != 0; ch = this->ReadTyped<char>()) {
            result.push_back(ch);
        }
#else
        size_t i = this->cursor;
        const char* ptr = rcast<const char*>(this->data);
        for (; i < this->length; ++i) {
            if (!ptr[i]) {
                break;
            }
        }

        result.assign(ptr + this->cursor, i - this->cursor);
        this->cursor = i + 1;
#endif
        return result;
    }

    inline size_t GetCursor() const {
        return this->cursor;
    }

    void SetCursor(const size_t pos) {
        this->cursor = std::min<size_t>(pos, this->length);
    }

    inline const uint8_t* GetDataAtCursor() const {
        return this->data + this->cursor;
    }

    MemStream Substream(const size_t subStreamLength) {
        const size_t allowedLength = ((this->cursor + subStreamLength) > this->length) ? (this->length - this->cursor) : subStreamLength;
        return MemStream(this->GetDataAtCursor(), allowedLength);
    }

private:
    const uint8_t*  data;
    size_t          length;
    size_t          cursor;
};


template <typename T>
constexpr T SetBit(const T& v, const T& bit) {
    return v |= bit;
}
template <typename T>
constexpr T RemoveBit(const T& v, const T& bit) {
    return v &= ~bit;
}
template <typename T>
constexpr bool TestBit(const T& v, const T& bit) {
    return 0 != (v & bit);
}

inline uint32_t CountBitsU32(uint32_t x) {
    x = (x & 0x55555555) + ((x >>  1) & 0x55555555);
    x = (x & 0x33333333) + ((x >>  2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >>  4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >>  8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
    return x;
}

PACKED_STRUCT_BEGIN
struct Bitset256 {
    uint32_t dwords[8];

    inline size_t CountOnes() const {
        size_t result = 0;
        for (uint32_t x : dwords) {
            result += CountBitsU32(x);
        }
        return result;
    }

    inline bool IsPresent(const size_t idx) const {
        const size_t i = idx >> 5;
        assert(i <= 7);
        const uint32_t mask = 1 << (idx & 0x1F);
        return (dwords[i] & mask) == mask;
    }
} PACKED_STRUCT_END;


#ifndef MySafeRelease
#define MySafeRelease(ptr)  \
    if (ptr) {              \
        (ptr)->Release();   \
        (ptr) = nullptr;    \
    }
#endif

#ifndef MySafeDelete
#define MySafeDelete(ptr)   \
    if (ptr) {              \
        delete (ptr);       \
        (ptr) = nullptr;    \
    }
#endif
