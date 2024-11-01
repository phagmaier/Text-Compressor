#ifndef CONSTANTS_H
#define CONSTANTS_H

inline constexpr unsigned int CODE_VALUE_BITS = 20;
inline constexpr unsigned int ONE = 1;
inline constexpr unsigned int MAX= (ONE << CODE_VALUE_BITS) - 1;
inline constexpr unsigned int BOTTOM = 0;
inline constexpr unsigned int QUARTER = ONE << (CODE_VALUE_BITS - 2);     
inline constexpr unsigned int HALF = 2 * QUARTER;
inline constexpr unsigned int THREE_QUARTERS = 3 * QUARTER;

#endif
