#include "../../include/DictProducer/DictProducer.h"
#include "../../include/DictProducer/SplitTool.h"
#include "../../include/SearchEngineServer/Configuration.h"

int main() {
  SplitTool *stl = Cppjieba::getSplitTool(); // 静态函数创建单例对象分词工具
  auto &configs = *Configuration::getInstance(); // 获取单例对象Configuration
  DictProducer dp(stl); // 创建DictProducer对象进行中文分词
                        // 将生成好的词典和反向索引表存在对应目录下
  dp.storeDict(configs["xmldict_path"].c_str());
  dp.storeIndex(configs["index_path"].c_str());
  // dp.storeIndex("../data/dict/dictindex(1).dat");
  DictProducer dp1; // 默认构造函数用于英文分词
  dp1.storeDict(configs["engdict_path"].c_str());
  dp1.storeIndex(configs["en_index_path"].c_str());

  return 0;
}
