#ifndef _SPLITTOOL_H_
#define _SPLITTOOL_H_
#include "../../lib/cppjieba/include/cppjieba/Jieba.hpp"
#include "../myhead.h"

class SplitTool { // 分词工具基类 纯虚类 起接口作用
public:
  virtual vector<string> cut(const string &sentence, vector<string> &vec) = 0;
  virtual ~SplitTool() {}
};

class Cppjieba
    : public SplitTool { // Cppjieba类继承SplitTool 对cppjieba库进行封装
public:
  static SplitTool *getSplitTool(); // 单例模式 获取唯一的Cppjieba对象
  vector<string> cut(const string &sentence, vector<string> &vec);
  static void destroy(); // 销毁单例对象

private:
  class AutoRelease {
  public:
    ~AutoRelease() {
      if (splittool)
        delete splittool;
    }
  };
  Cppjieba();
  ~Cppjieba() {}
  map<string, string> &_configs;
  cppjieba::Jieba _jieba;
  static SplitTool *splittool;
  static pthread_mutex_t _mutex;
};

#endif
