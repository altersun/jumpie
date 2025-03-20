#include <functional>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <unordered_map>
#include <vector>

// Is it a function type?
template <typename T>
concept FunctionType = std::is_same_v<T, std::function<typename T::result_type(typename T::argument_type)>>;

// Is it an integral type?
template <typename T>
concept IsInteger = std::is_integral_v<T>;

// MIGHT AS WELL JUMP
template <typename K, FunctionType T>
class JumpTable {
    public:
        using Container_t = std::conditional_t<IsInteger<K>, std::vector<T>, std::unordered_map<K, T>>;

        // Implicit integral index constructor
        //JumpTable(std::initializer_list<T> init_list) requires IsInteger<T> {
        //    for (const auto& value : init_list) {
        //        container.push_back(value);
        //    }
        //}

        // Explicit integral index constructor
        JumpTable(std::initializer_list<std::pair<const K, T>> init_list) {
            if constexpr (IsInteger<K>) {
                size_t last_index = std::size_t(std::ssize(*std::prev(init_list.end())));
                container.resize(last_index, nullptr);
                for (const auto& [index, value] : init_list) {
                    container.at(index) = value;
                }
            } else {
                for (const auto& [key, value] : init_list) {
                    container.emplace(key, value);
                }
            }
        }
        

        // Non-integral key constructor
       // JumpTable(std::initializer_list<std::pair<const T, K>> init_list) requires (!IsInteger<T>) {
       //     for (const auto& [key, value] : init_list) {
       //         container.emplace(key, value);
       //     }
       // }

        //JumpTable(std::initializer_list<std::pair<const T, K>> init_list) = default;

        // Random access operator overload
        auto& operator[](const K& key) {
            if constexpr (IsInteger<K>) {
                if (key >= 0 && static_cast<size_t>(key) < container.size()) {
                    return container[key];
                } else {
                    throw std::out_of_range("Index out of bounds");
                }
            } else {
                return container.at(key);
            }
        }
        
        std::vector<K> GetAllKeys() {
            std::vector<K> keys;
            if constexpr (IsInteger<K>) {
                for (K key : std::ranges::iota_view(0, container.size())) {
                    keys.push_back(key);
                }
            } else {
                for (const auto& key : std::views::keys(container)) {
                    keys.push_back(key);
                }
            }
            return keys;
        }

        

    private:
        
        Container_t container;
};


// Deduction guide for implicit int keys constructor
//template <typename T>
//JumpTable(std::initializer_list<T> init_list) -> JumpTable<T, T>;

// Deduction guide 
template <typename K, FunctionType T>
JumpTable(std::initializer_list<std::pair<const K, T>> init_list) -> JumpTable<K, T>;

