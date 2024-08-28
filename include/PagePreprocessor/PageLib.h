#ifndef _PAGELIB_H
#define _PAGELIB_H
#include "../../lib/simhash/Simhasher.hpp"
#include "../SearchEngineServer/Configuration.h"
#include "DirScanner.h"

struct HammingDistance_3 {
  bool operator()(const uint64_t &lhs, const uint64_t &rhs) const {
    return simhash::Simhasher::isEqual(lhs, rhs);
  }
};

class PageLib {
public:
  PageLib();
  void create();
  void store();

private:
  Configuration &_config;          // 配置文件对象
  DirScanner _dirScanner;          // 目录扫描器
  std::vector<std::string> _pages; // 存放所有网页的路径 ？
  simhash::Simhasher _simhasher;   // simhash算法对象
  std::unordered_set<uint64_t, std::hash<uint64_t>, HammingDistance_3>
      _simhashset; // 存放所有网页的simhash值
  // std::map<int, std::pair<int, int>> _offsetLib; //
  // 存放网页在网页库中的偏移位置
};
#endif //_PAGELIB_H
