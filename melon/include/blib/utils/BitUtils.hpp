#pragma once

#include <cstdint>
#include <cstddef>

namespace blib {
  namespace utils {
    struct B {};

    struct KB {};

    struct MB {};

    template<typename BytesType = B>
    struct Bytes {
      /// Number of bytes
      const static std::size_t bytes = 1;
    };

    template<>
    struct Bytes<KB> {
      /// Number of bytes
      const static std::size_t bytes = 1024;
    };

    template<>
    struct Bytes<MB> {
      /// Number of bytes
      const static std::size_t bytes = 1024 * Bytes<KB>::bytes;
    };

    template<typename From, typename To, std::size_t N>
    struct ByteConverter;

    template<std::size_t N>
    struct ByteConverter<B, KB, N> {
      const static std::size_t value = N * Bytes<KB>::bytes;
    };

    template<std::size_t N>
    struct ByteConverter<B, MB, N> {
      const static std::size_t value = N * Bytes<MB>::bytes;
    };

    template<std::size_t N>
    struct ByteConverter<KB, MB, N> {
      const static std::size_t value = N * Bytes<KB>::bytes;
    };
  }
}