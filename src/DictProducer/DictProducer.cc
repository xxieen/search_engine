#include "../../include/DictProducer/DictProducer.h"
#include "../../include/SearchEngineServer/Configuration.h"

DictProducer::DictProducer()
    : _config(Configuration::getInstance()->getConfigMap()), _cuttor(nullptr),
      _dict(), _dict_index(), _index() {
  buildEnDict();
  createIndex();
}

DictProducer::DictProducer(SplitTool *cuttor)
    : _config(Configuration::getInstance()->getConfigMap()), _cuttor(cuttor),
      _dict(), _dict_index(), _index() {
  buildCnDict();
  createIndex();
}

void DictProducer::buildEnDict() {
  unordered_set<string> stop_word;
  Configuration *config = Configuration::getInstance();
  stop_word = config->getStopWordList(); // 获取配置文件中的停用词列表

  string englishPage(_config["englishpage"]); // 英文语料的路径
  ifstream str(englishPage.c_str());          // 打开读取
  if (!str.is_open()) {
    cerr << "englishPath Open Error !" << endl;
  }
  str.seekg(std::ios_base::beg); // 确保指针从文件头部开始
  string sentence;
  while (getline(str, sentence)) {
    istringstream ss(sentence);
    string words;
    while (ss >> words) { // 逐词读取
      for (int c = 0; c < words.size(); ++c) {
        if (words[c] >= 'A' && words[c] <= 'Z') {
          words[c] = tolower(words[c]);
        } // 将所有单词中的大写字母转换为小写字母

        if (!isalpha(words[c])) {
          words.erase(c);
        } // 删除非字母字符
      }
      auto stop = stop_word.find(words); // 判断单词是否为停用词
      if (stop != stop_word.end()) {
        continue;
      } else {                                // 若不是停用词
        auto exist = _dict_index.find(words); // 是否在词典中
        if (exist != _dict_index.end()) {
          ++(_dict[exist->second]
                 .second); // 如果在，就将词典中的对应索引上的值加1
        } else {           // 如果不在
          _dict.push_back(pair<string, int>(words, 1)); // 在字典中添加该单词
          _dict_index.insert(pair<string, int>(
              words, _dict.size())); // 并记录其索引，方便快速查找
        }
      }
    }
  }
}

void DictProducer::buildCnDict() {
  // map<string, string> &config = Configuration::getInstance()->getConfigMap();
  string pagePath(_config["ripepage"]); // 获取中文语料文件的路径
  ifstream ifs(pagePath);
  if (!ifs.is_open()) {
    cerr << "pagePath Open error !" << endl;
  }
  ifs.seekg(std::ios_base::beg); // 将文件游标指向文件开头
  string sentence;
  vector<string>
      words; // 一个句子经过cuttor中文分词工具后生成的若干个中文词的数组
  while (getline(ifs, sentence)) {
    _cuttor->cut(sentence, words);
    for (auto &it : words) {             // 遍历每一个中文词
      auto exist = _dict_index.find(it); // 判断该词是否在词典中
      if (exist != _dict_index.end()) {
        ++(_dict[exist->second].second); // 在，增加频率
      } else { // 不在，在字典中新建，并在索引表中记录该词的位置，方便快速查找
        _dict.push_back(pair<string, int>(it, 1));
        _dict_index.insert(pair<string, int>(it, _dict.size()));
      }
    }
  }
}

void DictProducer::createIndex() { // 创建反向索引
  string letter;
  int index = 0; // 用于追踪当前处理的单词的索引

  if (_cuttor == nullptr) // 此标志说明是英文
  {
    for (auto &it : _dict) {    // 遍历字典中每一个单词
      for (char c : it.first) { // 遍历每个单词中的每一个字母
        letter = c;
        auto map_it = _index.find(letter); // 查找该字母是否已经在反向索引表中了
        if (map_it != _index.end()) { // 如果在
          map_it->second.insert(index); // 直接将该单词所对应的索引加入set集合中
        } else {                        // 如果不在
          set<int> nset;
          nset.insert(index);
          _index.insert(pair<string, set<int>>(
              letter, nset)); // 创建set 并插入反向索引表中
        }
      }
      ++index; // 索引递增 处理下一个单词
    }
  } else // 中文
  {
    for (auto &it : _dict) { // 遍历词典中每一个词
      int i = 0;             // 记录当前处理的词在词典中的下标
      while (i < it.first.size()) { // 遍历词条中的每个字符（字节）
        unsigned char c = it.first[i]; // 获取当前字符
        if (c < 128) { // 如果字符是 ASCII 字符（即英文字符或符号）
          ++i;         // 直接跳过该字符
          continue;
        }
        // 处理 UTF-8 编码的中文字符
        int cnt = 1; // 初始化字节数（每个中文字符在 UTF-8 中占多个字节）
        for (; cnt < 6; ++cnt) { // 计算该中文字符在UTF-8中占几个字节
          unsigned char b =
              c << cnt; // 将第一个字节左移 cnt 位,如果移位后的值小于
                        // 128，说明到了 UTF-8 编码的结束部分
          if (b < 128) {
            break;
          }
        }

        // 获取完整的中文字符子串
        letter = it.first.substr(i, cnt); // 获取cnt个字节
        i += cnt; // 移动索引到下一个字符的位置
        auto map_it = _index.find(letter); // 在索引表中查找当前字符是否已经存在
        if (map_it != _index.end()) { // 如果已经存在
          map_it->second.insert(index); // 将当前词条的位置添加到集合中
        } else {                        // 如果不存在
          set<int> nset;
          nset.insert(index);
          _index.insert(pair<string, set<int>>(
              letter, nset)); // 将新字符和位置集合插入索引表
        }
      }
      ++index; // 更新词条在词典中的索引
    }
  }
}

// 将生成好的字典存放在对应的文件目录下
void DictProducer::storeDict(const char *path) {
  ofstream wfile(path);
  if (!wfile.good()) {
    cout << "storeDict Error!" << endl;
    return;
  }
  for (auto &it : _dict) {
    wfile << it.first << " " << it.second << endl;
  }
  wfile.close();
}

// 将生成好的反向索引表存放在对应的文件目录下
void DictProducer::storeIndex(const char *path) {
  ofstream wfile(path);
  if (!wfile.good()) {
    cout << "storeIndex Error!" << endl;
    return;
  }
  for (auto &it : _index) {
    wfile << it.first << " ";
    for (auto &set_it : it.second) {
      wfile << set_it << " ";
    }
    wfile << endl;
  }
  wfile.close();
}
