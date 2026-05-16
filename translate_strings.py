import re
import os
from concurrent.futures import ThreadPoolExecutor, as_completed
from deep_translator import GoogleTranslator

translator = GoogleTranslator(source='ko', target='zh-CN')

def translate_text(text):
    if not text.strip():
        return text
    
    placeholders = []
    def repl(m):
        placeholders.append(m.group(0))
        return f"[{len(placeholders)-1}]"
        
    safe_text = re.sub(r'(%[0-9]*[a-zA-Z])|(\\n)|(\\|)', repl, text)
    
    try:
        translated = translator.translate(safe_text)
    except Exception as e:
        return text
        
    for i, p in enumerate(placeholders):
        translated = translated.replace(f"[{i}]", p)
        translated = translated.replace(f"[ {i} ]", p)
        
    return translated

def process_file(filepath, src_enc, dst_enc):
    with open(filepath, 'r', encoding=src_enc, errors='replace') as f:
        lines = f.readlines()

    pattern = re.compile(r'(_T\(")(.*?)("\))')
    
    tasks = []
    for i, line in enumerate(lines):
        match = pattern.search(line)
        if match:
            korean_text = match.group(2)
            # Find Korean chars or missing translations
            if re.search(r'[\uac00-\ud7a3]', korean_text):
                tasks.append((i, korean_text))
                
    print(f"Found {len(tasks)} strings to translate in {filepath}")
    
    results = {}
    with ThreadPoolExecutor(max_workers=20) as executor:
        future_to_i = {executor.submit(translate_text, txt): i for i, txt in tasks}
        for future in as_completed(future_to_i):
            i = future_to_i[future]
            try:
                results[i] = future.result()
            except Exception:
                results[i] = tasks[i][1]

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
        
    os.rename(filepath, filepath + '.bak2')
    
    with open(filepath, 'w', encoding=dst_enc, errors='replace') as f:
        f.writelines(new_lines)
        
    print(f"Finished {filepath}")

process_file(r'D:\xiahold\XiahClient\StringDefine.h', 'euc-kr', 'gbk')
process_file(r'D:\xiahold\XiahClient\StringDefine_utf8.h', 'utf-8', 'utf-8')
