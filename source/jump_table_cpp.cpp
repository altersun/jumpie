#include <iomanip>
#include <iostream>
#include <sstream>
#include <variant>
#include <vector>

#include <jump_table.hpp>

// Some aliases just for less typing later
using ByteSpan_t = std::vector<std::byte>;
using DataParser_t = std::function<int(ByteSpan_t)>;
using IntParserTable_t = JumpTable<int, DataParser_t>;
using FloatParserTable_t = JumpTable<float, DataParser_t>;
using StringParserTable_t = JumpTable<std::string, DataParser_t>;

// These two aliases allow for looping over jump tables
// with the same test function for each of them
using ParserTableVariant = std::variant<
    IntParserTable_t,
    FloatParserTable_t,
    StringParserTable_t
>;
using NamedParserTableVariant = std::pair<
    std::string,
    ParserTableVariant
>;

// Our test data. Simulates an incoming bytestream like
// one would frequently see in embedded systems (but shorter).
static const ByteSpan_t test_data {
    std::byte{0xAA}, std::byte{0xFE}, std::byte{0x23},
    std::byte{0x4D}, std::byte{0x44}
};

// Our functions, which will take the above test data
int PrintForward(ByteSpan_t bytes);
int PrintBackward(ByteSpan_t bytes);
int SumAll(ByteSpan_t bytes);

// Keys are implicitly assumed to be contiguous integers
// by the table constructor. Vector under the hood.
IntParserTable_t ImpIntParsers{{
    PrintForward,
    PrintBackward,
    SumAll
}};

// Keys here are explicitly integral. Could be constant values, 
// enums, chars, or any other integral type. Vector under the hood.
IntParserTable_t ExpIntParsers{{
    {3, PrintForward},
    {5, PrintBackward},
    {8, SumAll}
}};

// Keys are floating point numbers. Don't know why anyone would want
// this, but you *could* do it. Map under the hood.
FloatParserTable_t FloatParsers{{
    {4.6, PrintForward},
    {5.66666, PrintBackward},
    {7890.2, SumAll}
}};

// Keys are strings. Map under the hood. A strong benefit over using a Map
// directly is that the values *must* be functions or it won't compile.
StringParserTable_t StringParsers{{
    {"forward", PrintForward},
    {"backward", PrintBackward},
    {"sum", SumAll}
}};

// Test runner. Executes all functions in the table. Templated
// to work with all kinds of keys and function prototypes.
template<typename K, FunctionType T>
void JumpTableTester(JumpTable<K, T> jump, std::string name)
{
    std::cout << name << std::endl;
    for (K& key : jump.GetAllKeys()) { 
        std::cout << key << ": ";
        if (jump[key] != nullptr) {
            jump[key](test_data);
        } else {
            std::cout << "No function" << std::endl;
        }
    }
}


int main()
{
    // Make a handy table to iterate over
    std::vector<NamedParserTableVariant> parser_tables {
        {"Implicit integral keys", ImpIntParsers},
        {"Explicit integral keys", ExpIntParsers},
        {"Float keys", FloatParsers},
        {"String keys", StringParsers}
    };

    // Do it!
    for (const auto& [name, parser_table] : parser_tables) {
        std::visit([&name](const auto& table) {
            JumpTableTester(table, name);
        }, parser_table);
        std::cout << std::endl;
    }

    return 0;
}

// Simple function for testing
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

// Simple function for testing
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

// Simple function for testing
int SumAll(ByteSpan_t bytes)
{
    int sum = 0;
    for (const auto& b : bytes) {  
        sum += static_cast<int>(b);
    }
    std::cout << sum << std::endl;
    return sum;
}