# 字体文件 (Fonts)

## 使用说明

将 `main.vlw` 文件放入 SD 卡的 `/fonts/` 目录：

```
SD Card Root/
├── fonts/
│   └── main.vlw      ← 中英混合字体 (Noto Sans)
├── IRDB/             ← 可选: Flipper-IRDB 文件
└── ...
```

固件启动时会自动检测并加载该字体。如果 SD 卡中没有字体文件，将回退到内置的 `efontCN_24` 中文字体。

## 字体特点

| 特性 | 说明 |
|---|---|
| 字体 | Noto Sans Regular |
| 字号 | 20pt |
| 包含 | ASCII 全字符 + 中文 UI 用字 |
| 格式 | VLW (M5GFX 原生支持) |
| 大小 | ~30 KB (仅包含需要的字符) |

## 自定义字体

如果想更换字体或添加更多中文字符，可以运行生成脚本：

```bash
cd fonts/
pip install Pillow fonttools requests
python3 generate_font.py
```

生成的 `main.vlw` 放入 SD 卡即可生效。

## 编辑 `generate_font.py` 中的 `CHINESE_CHARS` 变量来添加更多汉字。
