import os
import re
from concurrent.futures import ThreadPoolExecutor, as_completed
from deep_translator import GoogleTranslator

translator = GoogleTranslator(source='ko', target='zh-CN')

def translate_text(text):
    if not text.strip():
        return text
        
    # Hardcoded crucial strings
    if text == '굴림': return '宋体'
    if text == '전': return '两'
    if '%s 획득' in text: return '获得物品 [ %s ]'
    if '금전 획득 [ %d전 ]' in text: return '获得金币 [ %d两 ]'
    if '경험치 획득 [ %d ]' in text: return '获得经验值 [ %d ]'
    if '경험치 획득 [ %d + %d ]' in text: return '获得经验值 [ %d + %d ]'

    # Protect formatting strings
    placeholders = []
    def repl(m):
        placeholders.append(m.group(0))
        return f" X{len(placeholders)-1}X "
        
    # Find %d, %s, %c, \n
    safe_text = re.sub(r'(%[0-9]*[a-zA-Z])|(\\n)|(\\|)', repl, text)
    
    try:
        translated = translator.translate(safe_text)
    except Exception as e:
        return text
        
    # Restore formatting strings
    for i, p in enumerate(placeholders):
        translated = translated.replace(f"X{i}X", p)
        translated = translated.replace(f" X{i}X ", p)
        
    # cleanup spaces around formats
    translated = translated.replace(" [ % s ]", " [%s]").replace(" [ % d ]", " [%d]")
    
    return translated

def process_file(filepath):
    # The backup files are UTF-8 without BOM
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()

    pattern = re.compile(r'(_T\(")(.*?)("\))')
    
    tasks = []
    for i, line in enumerate(lines):
        match = pattern.search(line)
        if match:
            korean_text = match.group(2)
            # Only translate if there is korean text or it's a known macro
            if re.search(r'[\uac00-\ud7a3]', korean_text) or korean_text == '전':
                tasks.append((i, korean_text))
                
    print(f"Found {len(tasks)} strings to translate in {filepath}")
    
    results = {}
    with ThreadPoolExecutor(max_workers=20) as executor:
        future_to_i = {executor.submit(translate_text, txt): i for i, txt in tasks}
        completed = 0
        for future in as_completed(future_to_i):
            i = future_to_i[future]
            try:
                results[i] = future.result()
            except Exception:
                results[i] = tasks[i][1]
                
            completed += 1
            if completed % 100 == 0:
                print(f"Translated {completed}/{len(tasks)}...")

    new_lines = []
    for i, line in enumerate(lines):
        if i in results:
            match = pattern.search(line)
            if match:
                original = match.group(0)
                new_str = f'_T("{results[i]}")'
                new_lines.append(line.replace(original, new_str))
                continue
        new_lines.append(line)
        
    # Save as UTF-8 with BOM so Chinese VS reads it correctly without warnings
    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.writelines(new_lines)
        
    print(f"Finished {filepath}")

process_file(r'D:\xiahold\XiahClient\StringDefine.h')
process_file(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
