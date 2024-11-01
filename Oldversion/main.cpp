#include <iostream>
#include <utility>
#include <map>
#include <fstream>
#include <string>
#include <stdint.h>

const unsigned int CODE_VALUE_BITS = 20;
const unsigned int ONE = 1;
const unsigned int TOP = (ONE << CODE_VALUE_BITS) - 1;
const unsigned int BOTTOM = 0;
const unsigned int QUARTER = ONE << (CODE_VALUE_BITS - 2);     
const unsigned int HALF = 2 * QUARTER;
const unsigned int THREE_QUARTERS = 3 * QUARTER;

using Rpair = std::pair<unsigned int, unsigned int>;




class BitIO {
    static const int CHAR_BITS = 8;
    unsigned char buffer;
    int bits_in_buffer;
    std::fstream& stream;
    bool is_reading;

public:
    BitIO(std::fstream& fs, bool reading) : 
        buffer(0), bits_in_buffer(0), stream(fs), is_reading(reading) {}
    
    ~BitIO() {
        if (!is_reading && bits_in_buffer > 0) {
            buffer <<= (CHAR_BITS - bits_in_buffer);
            stream.write(reinterpret_cast<char*>(&buffer), 1);
        }
    }

    bool read_bit() {
        if (bits_in_buffer == 0) {
            stream.read(reinterpret_cast<char*>(&buffer), 1);
            if (stream.eof()) return false;
            bits_in_buffer = CHAR_BITS;
        }
        bool bit = (buffer & 0x80) != 0;
        buffer <<= 1;
        bits_in_buffer--;
        return bit;
    }

    void write_bit(bool bit) {
        buffer = (buffer << 1) | (bit ? 1 : 0);
        bits_in_buffer++;
        
        if (bits_in_buffer == CHAR_BITS) {
            stream.write(reinterpret_cast<char*>(&buffer), 1);
            buffer = 0;
            bits_in_buffer = 0;
        }
    }
};

std::pair<std::map<char, Rpair>, unsigned int> get_ranges(const std::string& text) {
    // This function remains the same
    std::map<char, unsigned int> freq;
    unsigned int total = 0;
    
    for (char c : text) {
        freq[c]++;
        total++;
    }
    
    std::map<char, Rpair> ranges;
    unsigned int cumulative = 0;
    for (const auto& [c, count] : freq) {
        ranges[c] = {cumulative, cumulative + count};
        cumulative += count;
    }
    
    return {ranges, total};
}


void encoder(const std::string& filename, const std::string& text, 
    std::map<char, Rpair>& ranges, const unsigned int total) {
    std::fstream file(filename, std::ios::binary | std::ios::out);
    if (!file) {
        std::cerr << "Failed to open output file\n";
        return;
    }

    BitIO bit_io(file, false);
    unsigned int lower = BOTTOM;
    unsigned int upper = TOP;
    size_t pending_bits = 0;

    for (char c : text) {
        unsigned int range = upper - lower + 1;
        const auto& [low_count, high_count] = ranges[c];
        
        // Key fix 1: More precise range calculations without changing types
        unsigned int temp_upper = lower + (range * high_count - 1) / total;
        unsigned int temp_lower = lower + (range * low_count) / total;
        
        upper = temp_upper;
        lower = temp_lower;

        // Output bits
        for (;;) {
            if (upper < HALF) {
                bit_io.write_bit(0);
                while (pending_bits > 0) {
                    bit_io.write_bit(1);
                    pending_bits--;
                }
            }
            else if (lower >= HALF) {
                bit_io.write_bit(1);
                while (pending_bits > 0) {
                    bit_io.write_bit(0);
                    pending_bits--;
                }
            }
            else if (lower >= QUARTER && upper < THREE_QUARTERS) {
                pending_bits++;
                lower -= QUARTER;
                upper -= QUARTER;
            }
            else break;

            lower = (lower << 1) & TOP;
            upper = ((upper << 1) & TOP) | 1;
        }
    }

    // Write final bits
    pending_bits++;
    bit_io.write_bit(1);
    while (pending_bits > 0) {
        bit_io.write_bit(0);
        pending_bits--;
    }
}

// Key fix 2: Improved symbol finding function
//char find_symbol(const std::map<char, Rpair>& ranges, unsigned int value, unsigned int total) {
char find_symbol(const std::map<char, Rpair>& ranges, unsigned int value) {
    for (const auto& [c, range] : ranges) {
        if (value >= range.first && value < range.second) {
            return c;
        }
    }
    std::cerr << "Error: No symbol found for value " << value << std::endl;
    return '\0';
}

void decoder(const std::string& filename, std::map<char, Rpair>& ranges, 
            unsigned int total, size_t text_length) {
    std::fstream file(filename, std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "Failed to open input file\n";
        return;
    }

    BitIO bit_io(file, true);
    unsigned int value = 0;
    unsigned int lower = BOTTOM;
    unsigned int upper = TOP;

    // Initialize value register
    for (unsigned int i = 0; i < CODE_VALUE_BITS; i++) {
        value = (value << 1) | bit_io.read_bit();
    }

    std::string result;
    
    for (size_t i = 0; i < text_length; i++) {
        unsigned int range = upper - lower + 1;
        
        // Key fix 3: Simplified value scaling
        unsigned int scaled_value = ((value - lower) * total) / range;
        
        //char c = find_symbol(ranges, scaled_value, total);
        char c = find_symbol(ranges, scaled_value);
        if (c == '\0') {
            std::cerr << "Decoding failed at position " << i << std::endl;
            return;
        }
        
        result += c;

        // Key fix 4: Matching encoder's range calculations
        unsigned int temp_upper = lower + (range * ranges[c].second - 1) / total;
        unsigned int temp_lower = lower + (range * ranges[c].first) / total;
        
        upper = temp_upper;
        lower = temp_lower;

        // Rescale and read new bits
        for (;;) {
            if (upper < HALF) {
                // Do nothing
            }
            else if (lower >= HALF) {
                value -= HALF;
                lower -= HALF;
                upper -= HALF;
            }
            else if (lower >= QUARTER && upper < THREE_QUARTERS) {
                value -= QUARTER;
                lower -= QUARTER;
                upper -= QUARTER;
            }
            else break;

            lower = (lower << 1) & TOP;
            upper = ((upper << 1) & TOP) | 1;
            value = ((value << 1) & TOP) | bit_io.read_bit();
        }
    }

    std::cout << "Decoded text: " << result << std::endl;
}

// Helper function to validate ranges
void validate_ranges(const std::map<char, Rpair>& ranges, unsigned int total) {
    std::cout << "Validating ranges:\n";
    for (const auto& [c, range] : ranges) {
        unsigned int range_size = range.second - range.first;
        double probability = static_cast<double>(range_size) / total;
        std::cout << "Symbol '" << c << "': "
                 << "range [" << range.first << ", " << range.second << ") "
                 << "size = " << range_size 
                 << " (p = " << probability << ")\n";
    }
}

int main() {
    //std::string text = "Hello World This is my Arithmetic coder";
    std::string text = "Text is: hello world my name is parker hagmaier";
    std::cout << "Original text: " << text << std::endl;
    
    auto [ranges, total] = get_ranges(text);
    //validate_ranges(ranges, total);
    
    std::string filename = "compressed.bin";
    encoder(filename, text, ranges, total);
    decoder(filename, ranges, total, text.length());
    
    return 0;
}
