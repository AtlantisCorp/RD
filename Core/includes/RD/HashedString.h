//
//  HashedString.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef HashedString_h
#define HashedString_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Hashed string, based on entt::HashString.
     */
    class HashedString final
    {
        struct ConstCharWrapper final {
            // non-explicit constructor on purpose
            constexpr ConstCharWrapper(const char *str) noexcept: str{str} {}
            const char *str;
        };
        
        //! @brief Offset for the hash compute.
        static constexpr uint64_t offset = 14695981039346656037ull;
        //! @brief Prime for the hash computation.
        static constexpr uint64_t prime = 1099511628211ull;
        
        /*! @brief Fowler-Noll-Vo 1a hash function. */
        static constexpr std::uint64_t helper(uint64_t partial, const char *str) noexcept {
            return str[0] == 0 ? partial : helper((partial^str[0])*prime, str+1);
        }
        
    public:
        
        /*! @brief Unsigned integer type. */
        using hash_type = std::uint64_t;
        
        /**
         * @brief Constructs a hashed string from an array of const chars.
         *
         * Forcing template resolution avoids implicit conversions. An
         * human-readable identifier can be anything but a plain, old bunch of
         * characters.<br/>
         * Example of use:
         * @code{.cpp}
         * HashedString sh{"my.png"};
         * @endcode
         *
         * @tparam N Number of characters of the identifier.
         * @param str Human-readable identifer.
         */
        template <std::size_t N>
        constexpr HashedString(const char (&str)[N]) noexcept
        : hash{helper(offset, str)}, str{str}
        {}
        
        /**
         * @brief Explicit constructor on purpose to avoid constructing a hashed
         * string directly from a `const char *`.
         *
         * @param wrapper Helps achieving the purpose by relying on overloading.
         */
        explicit constexpr HashedString(ConstCharWrapper wrapper) noexcept
        : hash{helper(offset, wrapper.str)}, str{wrapper.str}
        {}
        
        /**
         * @brief Returns the human-readable representation of a hashed string.
         * @return The string used to initialize the instance.
         */
        constexpr operator const char *() const noexcept { return str; }
        
        /**
         * @brief Returns the numeric representation of a hashed string.
         * @return The numeric representation of the instance.
         */
        constexpr operator hash_type() const noexcept { return hash; }
        
        /**
         * @brief Compares two hashed strings.
         * @param other Hashed string with which to compare.
         * @return True if the two hashed strings are identical, false otherwise.
         */
        constexpr bool operator==(const HashedString &other) const noexcept {
            return hash == other.hash;
        }
        
    private:
        const hash_type hash;
        const char *str;
    };
}

#endif /* HashedString_h */
