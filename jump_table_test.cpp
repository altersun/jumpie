#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <variant>
#include <vector>

#include "./jump_table.hpp"

using ByteSpan_t = std::vector<std::byte>;
using DataParser_t = std::function<int(ByteSpan_t)>;
using IntParserTable_t = JumpTable<int, DataParser_t>;
using FloatParserTable_t = JumpTable<float, DataParser_t>;
using StringParserTable_t = JumpTable<std::string, DataParser_t>;




static const ByteSpan_t test_data {
    std::byte{0xAA}, std::byte{0xFE}, std::byte{0x23},
    std::byte{0x4D}, std::byte{0x44}
};


int PrintForward(ByteSpan_t bytes);
int PrintBackward(ByteSpan_t bytes);
int SumAll(ByteSpan_t bytes);


/*K size = static_cast<K>(container.size());
                std::copy(
                    std::views::iota(static_cast<K>(0), size).begin(), 
                    std::views::iota(static_cast<K>(0), size).end(), 
                    std::back_inserter(keys)
                );
*/


JumpTable<int, DataParser_t> IntParsers{{
    {3, PrintForward},
    {5, PrintBackward},
    {8, SumAll}
}};


JumpTable<float, DataParser_t> floatParsers{{
    {4.6, PrintForward},
    {5.66666, PrintBackward},
    {7890.2, SumAll}
}};


JumpTable<std::string, DataParser_t> stringParsers{{
    {"forward", PrintForward},
    {"backward", PrintBackward},
    {"sum", SumAll}
}};


int main()
{
    using ParserTableVariant = std::variant<
        IntParserTable_t,
        FloatParserTable_t,
        StringParserTable_t
    >;

    using KeysVariant = std::variant<
        std::vector<int>,
        std::vector<float>,
        std::vector<std::string>
    >;
    
    std::vector<ParserTableVariant> parserTests {
        IntParsers,
        floatParsers,
        stringParsers
    };

    for (auto& key : IntParsers.GetAllKeys()) { 
        if (IntParsers[key] != nullptr) {
            IntParsers[key](test_data);
        } else {
            std::cout << "No function for key " << key << std::endl;
        }
    }

    return 0;
}


int PrintForward(ByteSpan_t bytes)
{
    std::ostringstream oss;
    int count = 0;
    bool first = true;
    for (const auto& b : bytes) {
        if (!first) {
            oss << ", ";
        }
        first = false;
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(b);
        count++;
    }
    std::cout << oss.str() << std::endl;
    return count;
}


int PrintBackward(ByteSpan_t bytes)
{
    std::ostringstream oss;
    int count = 0;
    bool first = true;
    for (const auto& b : std::views::reverse(bytes)) {
        if (!first) {
            oss << ", ";
        }
        first = false;
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(b);
        count++;
    }
    std::cout << oss.str() << std::endl;
    return count;
}


int SumAll(ByteSpan_t bytes)
{
    int sum = 0;
    for (const auto& b : bytes) {  
        sum += static_cast<int>(b);
    }
    std::cout << "Sum: " << sum << std::endl;
    return sum;
}