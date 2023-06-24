//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {}

LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool {
  std::scoped_lock<std::mutex> lock(data_latch_);
  if (cache_map_.empty()) {
    return false;
  }
  // 把最前头的删掉
  *frame_id = cache_list_.front();
  cache_list_.pop_front();
  cache_map_.erase(*frame_id);

  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::scoped_lock<std::mutex> lock(data_latch_);
  auto it = cache_map_.find(frame_id);
  if (it != cache_map_.end()) {
    cache_list_.erase(cache_map_[frame_id]);
    cache_map_.erase(it);
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::scoped_lock<std::mutex> lock(data_latch_);
  auto it = cache_map_.find(frame_id);
  if (it == cache_map_.end()) {
    cache_list_.push_back(frame_id);
    cache_map_[frame_id] = prev(cache_list_.end());
  }
  // 在的话，就不用管了
}

auto LRUReplacer::Size() -> size_t {
  std::scoped_lock<std::mutex> lock(data_latch_);
  auto ret = cache_map_.size();
  return ret;
}

}  // namespace bustub
