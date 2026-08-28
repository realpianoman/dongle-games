#!/bin/bash

# Slopped together, use at your own discression

INPUT="$1"
OUTPUT="$2"

WIDTH=$(magick identify -format "%w" "$INPUT")
HEIGHT=$(magick identify -format "%h" "$INPUT")

echo "#pragma once" > "$OUTPUT"
echo "" >> "$OUTPUT"
echo "#include <stdint.h>" >> "$OUTPUT"
echo "" >> "$OUTPUT"
echo "#define IMAGE_WIDTH $WIDTH" >> "$OUTPUT"
echo "#define IMAGE_HEIGHT $HEIGHT" >> "$OUTPUT"
echo "" >> "$OUTPUT"
echo "const uint16_t image[] = {" >> "$OUTPUT"

magick "$INPUT" -depth 8 RGB:- | \
python3 -c '
import sys

data = sys.stdin.buffer.read()

for i in range(0, len(data), 3):
    r = data[i]
    g = data[i+1]
    b = data[i+2]

    rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

    print(f"0x{rgb565:04X},", end=" ")
    if (i // 3 + 1) % 12 == 0:
        print()
' >> "$OUTPUT"

echo "" >> "$OUTPUT"
echo "};" >> "$OUTPUT"

echo "Created $OUTPUT ($WIDTH x $HEIGHT)"
