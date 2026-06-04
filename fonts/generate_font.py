#!/usr/bin/env python3
"""
VLW 字体生成脚本 — 为 M5Cardputer Ultimate-Remote 生成中英混合字体

用法:
  python3 generate_font.py

依赖:
  pip install Pillow fonttools requests

生成:
  fonts/main.vlw  (放入 SD 卡根目录 /fonts/main.vlw)

字体来源:
  Noto Sans (Google 开源字体)
"""

import os
import struct
import requests
from PIL import Image, ImageFont, ImageDraw
import tempfile

# ── 配置 ──────────────────────────────────────────────
FONT_URL = "https://github.com/googlefonts/noto-fonts/raw/main/hinted/ttf/NotoSans/NotoSans-Regular.ttf"
FONT_SIZE = 20  # 基准字号 pt
OUTPUT_DIR = "fonts"
OUTPUT_FILE = os.path.join(OUTPUT_DIR, "main.vlw")

# 需要包含的 Unicode 字符
# ASCII 可打印字符 + 中文 UI 用字
CHINESE_CHARS = (
    "加载中按OK开始退保存收藏夹"
    "扫描完成无果已尝试所有遥控"
    "选择品牌反应到你的设备空格键"
    "访问获取文件等关于该配置"
    "搜索加入收藏夹如果没找到合适"
    "你可以试试里的"
)

ASCII_CHARS = "".join(chr(i) for i in range(32, 127))

# ── 下载字体 ──────────────────────────────────────────
def download_font():
    print("下载 Noto Sans 字体...")
    resp = requests.get(FONT_URL, timeout=30)
    resp.raise_for_status()
    tmp = tempfile.NamedTemporaryFile(suffix=".ttf", delete=False)
    tmp.write(resp.content)
    tmp.close()
    print(f"  → {tmp.name}")
    return tmp.name

# ── 收集需要的字符 ───────────────────────────────────
def collect_chars():
    chars = set(ASCII_CHARS)
    chars.update(CHINESE_CHARS)
    return sorted(chars)

# ── VLW 格式写入 ─────────────────────────────────────
def write_vlw(font_path, output_path, font_size, chars):
    font = ImageFont.truetype(font_path, font_size)

    # 预渲染所有字形，收集信息
    glyphs = []
    for ch in chars:
        # 使用 Pillow 获取字形边界框
        bbox = font.getbbox(ch)
        if bbox is None:
            continue
        left, top, right, bottom = bbox
        w = right - left
        h = bottom - top
        if w == 0 or h == 0:
            continue

        # 渲染到图像
        img = Image.new("L", (w, h), 0)
        draw = ImageDraw.Draw(img)
        draw.text((-left, -top), ch, font=font, fill=255)

        # 获取 advance
        advance = font.getlength(ch)

        glyphs.append({
            'char': ch,
            'unicode': ord(ch),
            'width': w,
            'height': h,
            'xOffset': left,
            'yOffset': top,
            'xAdvance': int(advance),
            'yAdvance': 0,
            'bitmap': img.tobytes(),
        })

    # VLW 文件头
    # 格式版本: 1
    # 字体名称: "NotoSans"
    font_name = b"NotoSans"
    num_glyphs = len(glyphs)

    with open(output_path, 'wb') as f:
        # Header
        f.write(struct.pack('>I', 1))              # version
        f.write(struct.pack('>I', len(font_name))) # font name length
        f.write(font_name)                          # font name
        f.write(struct.pack('>f', float(font_size))) # point size
        f.write(struct.pack('>I', 1))               # smooth (1 = anti-aliased)
        f.write(struct.pack('>I', 1))               # charset count
        f.write(struct.pack('>H', 0))               # charset range start
        f.write(struct.pack('>H', 0xFFFF))          # charset range end
        f.write(struct.pack('>I', num_glyphs))      # glyph count

        # Glyphs
        for g in glyphs:
            f.write(struct.pack('>I', g['unicode']))
            f.write(struct.pack('>h', g['width']))
            f.write(struct.pack('>h', g['height']))
            f.write(struct.pack('>h', g['xAdvance']))
            f.write(struct.pack('>h', g['yAdvance']))
            f.write(struct.pack('>h', g['xOffset']))
            f.write(struct.pack('>h', g['yOffset']))
            # Bitmap: 1 byte per pixel (grayscale)
            f.write(g['bitmap'])

    print(f"  → 共 {num_glyphs} 个字形")
    file_size = os.path.getsize(output_path)
    print(f"  → 文件大小: {file_size:,} bytes ({file_size/1024:.1f} KB)")

# ── 主流程 ───────────────────────────────────────────
def main():
    print("=" * 50)
    print("  M5Cardputer Ultimate-Remote 字体生成")
    print("=" * 50)

    os.makedirs(OUTPUT_DIR, exist_ok=True)

    font_path = download_font()
    chars = collect_chars()
    print(f"  → 共 {len(chars)} 个字符")

    write_vlw(font_path, OUTPUT_FILE, FONT_SIZE, chars)

    # 清理临时文件
    os.unlink(font_path)

    print("\n完成! 将 fonts/main.vlw 放入 SD 卡 /fonts/ 目录即可。")

if __name__ == "__main__":
    main()
