
#if defined(_MSC_VER)
#define RESTRICT __restrict

#elif defined(__GNUC__) || defined(__clang__)
#define RESTRICT __restrict__

#else
#define RESTRICT

#endif

// C++20 (and later)
#if __cplusplus >= 202002L

#define likely_branch [[likely]]
#define unlikely_branch [[unlikely]]

#else

#define likely_branch
#define unlikely_branch

#endif