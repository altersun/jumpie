#include <algorithm>
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

        // Explicit index constructor
        explicit JumpTable(std::initializer_list<std::pair<const K, T>> init_list) {
            if constexpr (IsInteger<K>) {
                size_t last_index = std::size_t((*std::prev(init_list.end())).first);
                container.resize(last_index + 1, nullptr);
                for (const auto& [index, value] : init_list) {
                    container.at(index) = value;
                }
            } else {
                for (const auto& [key, value] : init_list) {
                    container.emplace(key, value);
                }
            }
        }
        
         
        // throws std::out_of_range on bad key
        // Returns function on good key
        T operator[](const K& key) {     
            try {
                return container[key];
            } catch (const std::out_of_range& f) {
                return nullptr;
            }
        }

        // Returns nullptr on bad key
        // Returns function on good key
        T& at(const K& key) { 
            if constexpr (IsInteger<K>) {
                if (key >= 0 && static_cast<size_t>(key) < container.size() && container[key]) {
                    return container[key];
                } else {
                    throw std::out_of_range("Invalid key");
                }
            } else {
                return container.at(key);
            }
        }

        auto Size() {
            return container.size();
        }



        
        std::vector<K> GetAllKeys() {
            std::vector<K> keys;
            if constexpr (IsInteger<K>) {
                K size = static_cast<K>(container.size());
                std::copy(
                    std::views::iota(static_cast<K>(0), size).begin(), 
                    std::views::iota(static_cast<K>(0), size).end(), 
                    std::back_inserter(keys)
                );

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
explicit JumpTable(std::initializer_list<std::pair<const K, T>> init_list) -> JumpTable<K, T>;

