#ifndef TRANSLATIONS_CHINESE_H
#define TRANSLATIONS_CHINESE_H

#include <string>

namespace translations {

// 中文 UI 翻译
// 品牌名和命令名保持英文，只翻译界面文字

inline const std::string UI_LOADING = "加载中...";
inline const std::string UI_OK_START = "按 OK 开始";
inline const std::string UI_OK_QUIT = "按 OK 退出";
inline const std::string UI_OK = "OK";
inline const std::string UI_SAVE = "保存";
inline const std::string UI_BACK = "<";
inline const std::string UI_NO_RESULTS = "无结果";

// 扫描说明
inline const std::string UI_SCAN_TITLE = "关于扫描";
inline const std::string UI_SCAN_DESC = "扫描该品牌所有遥控配置";
inline const std::string UI_SCAN_TEXT1 = "当设备对某个遥控有反应时";
inline const std::string UI_SCAN_TEXT2 = " 按空格键 ";
inline const std::string UI_SCAN_TEXT3 = "加入收藏夹";
inline const std::string UI_SCAN_TEXT4 = "你需要先选择一个品牌";

// 扫描完成
inline const std::string UI_SCAN_COMPLETE = "扫描完成";
inline const std::string UI_SCAN_ALL_TRIED = "已尝试该品牌所有遥控";
inline const std::string UI_SCAN_NOT_FOUND = "如果没找到合适的遥控";
inline const std::string UI_SCAN_TRY_FAV = "你可以试试收藏夹里的";

// 文件说明
inline const std::string UI_FILE_TITLE = "关于文件";
inline const std::string UI_FILE_DESC = "你可以从 SD 卡读取 .ir 文件";
inline const std::string UI_FILE_TEXT = "访问 GitHub 获取这些文件";

// 扫描中提示
inline const std::string UI_SCAN_PRESS_SPACE = "按空格键";
inline const std::string UI_SCAN_ADD_FAV = "加入收藏夹";

// 搜索栏
inline const std::string UI_TYPE_SEARCH = "输入以搜索";

// 收藏夹提示
inline const std::string UI_ADD_TO_FAV = "加入收藏夹";
inline const std::string UI_ADD_TO_FAV2 = "添加到收藏夹";

}

#endif
