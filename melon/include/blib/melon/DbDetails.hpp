#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <filesystem>
#include <blib/catch/catch.hpp>
#include <blib/fmt/format.hpp>
#include <blib/rapidjson/rapidjson.h>
#include <blib/spdlog/spdlog.h>

namespace blib {
  namespace melon {
    using Byte = std::uint8_t;
    /////////////////////////////////////////////////
    /// @brief Converts bytes to and fro.
    ///        
    /////////////////////////////////////////////////


    /////////////////////////////////////////////////
    /// @brief Default Options for the database.
    ///        
    /////////////////////////////////////////////////
    struct DefaultOptions {
      static const std::size_t kPageSize = 1024;
      static const std::size_t kBufferPoolSize = 1024;
      static const std::size_t kDbSize = 10000;
      static const std::size_t kMaxTransacionSize = 100;
      static const std::size_t kDefaultSharedMemSize = 1000;
    };

    /////////////////////////////////////////////////
    /// @brief Options to control the behavior of a database.
    ///        
    /////////////////////////////////////////////////
    struct Options {
      // If true, the database will be created if it is missing.
      // Default: false
      bool create_if_missing;

      // If true, an error is raised if the database already exists.
      // Default: false
      bool error_if_exists;

      // -------------------
      // Parameters that affect performance

      // Amount of data to build up in memory (backed by an unsorted log
      // on disk) before converting to a sorted on-disk file.
      //
      // Larger values increase performance, especially during bulk loads.
      // Up to two write buffers may be held in memory at the same time,
      // so you may wish to adjust this parameter to control memory usage.
      // Also, a larger write buffer will result in a longer recovery time
      // the next time the database is opened.
      //
      // Default: 4MB
      std::size_t write_buffer_size;

      // Approximate size of user data packed per block.  Note that the
      // block size specified here corresponds to uncompressed data.  The
      // actual size of the unit read from disk may be smaller if
      // compression is enabled.  This parameter can be changed dynamically.
      //
      // Default: 4K
      std::size_t block_size;
      Options() :create_if_missing( true ), error_if_exists( true ), write_buffer_size( 4194304 ), block_size( 4096 ) {};
    };

    /////////////////////////////////////////////////
    /// @brief This class will be used to carry the status of the functions.
    ///        
    /////////////////////////////////////////////////
    class Status {
    public:
      enum class Code : std::uint32_t {
        kOK = 0,
        kNotFound = 1,
        kDeleteOrder = 2,
        kInvalidArgument = 3,
        kIOError = 4,
        kDone = 5,
        kMultipartRequired = 6,
        kUserDefinedError = 7,
        kUnknownError = 666
      };

    private:
      std::string _message;
      Code _code;

    public:
      Status() : _code( Code::kOK ) {}

      Status( const Code in_code, std::string const& in_message ) : _code( Code::kOK ), _message( in_message ) {}

      ~Status() = default;

      bool ok() const {
        return Code::kOK == _code;
      }

      bool done() const {
        return Code::kDone == _code;
      }

      bool notFound() const {
        return Code::kNotFound == _code;
      }

      bool ioError() const {
        return Code::kIOError == _code;
      }

      bool userDefinedError() const {
        return Code::kUserDefinedError == _code;
      }

      bool invalidArgument() const {
        return Code::kInvalidArgument == _code;
      }

      std::string const& msg() {
        if (_message.empty()) {
          _message = Status::toString( _code );
        }
        return _message;
      }

      Code code() const {
        return _code;
      }

      void code( const Code in_code ) {
        _code = in_code;
      }

      static std::string toString( const Code in_code ) {
        std::string ret = "OK";
        switch (in_code) {
        case Code::kDone:
          ret = "DONE";
          break;
        case Code::kInvalidArgument:
          ret = "INVALID ARGUMENTS";
          break;

        case Code::kNotFound:
          ret = "NOT FOUND";
          break;

        case Code::kIOError:
          ret = "IO ERROR";
          break;
        }
        return std::move( ret );
      }

    };
  }
}

