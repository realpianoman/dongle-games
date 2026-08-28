#pragma once

namespace constants {
inline constexpr uint32_t TARGET_FPS(12);
inline constexpr uint32_t TARGET_FRAME_TIME(1000 / TARGET_FPS);

// Rows and cols should both be divisible by CELL_SIZE
inline constexpr int CELL_SIZE(2);
inline constexpr int ROWS(40);
inline constexpr int COLS(80);
} // namespace constants
