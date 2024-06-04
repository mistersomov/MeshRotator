#ifndef MESH_ROTATOR_CACHE_HPP
#define MESH_ROTATOR_CACHE_HPP

#include "AndroidOut.hpp"
#include <map>
#include <memory>
#include <iostream>

namespace ndk_helper {
    namespace utils {
        template<typename Key, typename Value>
        class Cache {
        public:
            void add(const Key& key, const Value& value) {
                auto element = std::make_shared<Value>(value);
                cache_[key] = element;
            }

            Value get(const Key& key) const {
                auto it = cache_.find(key);
                if (it != cache_.end()) {
                    return it->second;
                } else {
                    aout << "Element not found in the cache." << std::endl;
                    throw std::out_of_range("Element not found in the cache.");
                }
            }

        private:
            std::map<Key, std::shared_ptr<Value>> cache_;
        };
    }
}

#endif //MESH_ROTATOR_CACHE_HPP
