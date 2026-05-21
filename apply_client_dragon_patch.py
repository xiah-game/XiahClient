import os

filePath = r"d:\xiahold\XiahClient\XiahGameObject.cpp"

with open(filePath, 'rb') as f:
    content = f.read()

try:
    text = content.decode('cp949')
    encoding = 'cp949'
except Exception:
    text = content.decode('utf-8', errors='replace')
    encoding = 'utf-8'

# Normalize any line endings for search and replace, then restore them
has_crlf = "\r\n" in text
text_normalized = text.replace("\r\n", "\n")

def replace_block(txt, start_marker, end_marker, replacement):
    start_marker = start_marker.replace("\r\n", "\n")
    end_marker = end_marker.replace("\r\n", "\n")
    replacement = replacement.replace("\r\n", "\n")
    
    start_idx = txt.find(start_marker)
    if start_idx == -1:
        print(f"ERROR: Cannot find start marker: {repr(start_marker)}")
        return txt
    end_idx = txt.find(end_marker, start_idx)
    if end_idx == -1:
        print(f"ERROR: Cannot find end marker: {repr(end_marker)}")
        return txt
    
    actual_end_idx = end_idx + len(end_marker)
    print(f"Found block: starting at index {start_idx}, ending at index {actual_end_idx}")
    return txt[:start_idx] + replacement + txt[actual_end_idx:]

# BING (Ice)
bing_start = "if( m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON ) )"
bing_end = """\t\t\t\t\tm_pBing_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

bing_replacement = """if( m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON ) )\t//?? ?? ??
\t\t\t{
\t\t\t\tif( !m_pBing_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

\t\t\t\t\t_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eBing_Dragon );
\t\t\t\t\t// matrix
\t\t\t\t\tif( pEffectPackage )
\t\t\t\t\t{
\t\t\t\t\t\tm_pBing_DragonPP = g_EffectManager.GetCurEffectPackagePair();

\t\t\t\t\t\tm_pBing_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
\t\t\t\t\t\t
\t\t\t\t\t\tm_pBing_DragonPP->dwTotalTime = 3000;

\t\t\t\t\t\tm_pBing_DragonPP->dwElapsedTime = 0;
\t\t\t\t\t}

\t\t\t\t\tg_EffectManager.OffSharedPackagePair();
\t\t\t\t}
\t\t\t\telse\t// ???? ??? ?? ?? ??.
\t\t\t\t{
\t\t\t\t\tif(m_pBing_DragonPP->dwElapsedTime >= m_pBing_DragonPP->dwTotalTime)
\t\t\t\t\t{
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( BING_DRAGONSINJANG);
\t\t\t\t\t\t
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( BING_DRAGONSUNGCHEON);

\t\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pBing_DragonPP );
\t\t\t\t\t\tm_pBing_DragonPP = NULL;
\t\t\t\t\t}
\t\t\t\t\telse
\t\t\t\t\t{
\t\t\t\t\t\tm_pBing_DragonPP->dwElapsedTime += fLocalFrameScale;
\t\t\t\t\t\tm_pBing_DragonPP->bIsVisible = true;
\t\t\t\t\t}
\t\t\t\t}
\t\t\t}
\t\t\telse\t// ??? ???? ???.
\t\t\t{
\t\t\t\tif( m_pBing_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pBing_DragonPP );
\t\t\t\t\tm_pBing_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

# DOK (Poison)
dok_start = "if( m_KeepUpMugongList.IsExist(DOK_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(DOK_DRAGONSUNGCHEON ) )"
dok_end = """\t\t\t\t\tm_pDok_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

dok_replacement = """if( m_KeepUpMugongList.IsExist(DOK_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(DOK_DRAGONSUNGCHEON ) )\t//?? ?? ??
\t\t\t{
\t\t\t\tif( !m_pDok_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

\t\t\t\t\t_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eDok_Dragon );
\t\t\t\t\t// matrix
\t\t\t\t\tif( pEffectPackage )
\t\t\t\t\t{
\t\t\t\t\t\tm_pDok_DragonPP = g_EffectManager.GetCurEffectPackagePair();

\t\t\t\t\t\tm_pDok_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();

\t\t\t\t\t\tm_pDok_DragonPP->dwTotalTime = 3000;

\t\t\t\t\t\tm_pDok_DragonPP->dwElapsedTime = 0;
\t\t\t\t\t}

\t\t\t\t\tg_EffectManager.OffSharedPackagePair();
\t\t\t\t}
\t\t\t\telse\t// ???? ??? ?? ?? ??.
\t\t\t\t{
\t\t\t\t\tif(m_pDok_DragonPP->dwElapsedTime >= m_pDok_DragonPP->dwTotalTime)
\t\t\t\t\t{
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(DOK_DRAGONSINJANG))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( DOK_DRAGONSINJANG);
\t\t\t\t\t\t
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(DOK_DRAGONSUNGCHEON))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( DOK_DRAGONSUNGCHEON);

\t\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pDok_DragonPP );
\t\t\t\t\t\tm_pDok_DragonPP = NULL;
\t\t\t\t\t}
\t\t\t\t\telse
\t\t\t\t\t{
\t\t\t\t\t\tm_pDok_DragonPP->dwElapsedTime += fLocalFrameScale;
\t\t\t\t\t\tm_pDok_DragonPP->bIsVisible = true;
\t\t\t\t\t}
\t\t\t\t}
\t\t\t}
\t\t\telse\t// ??? ???? ???.
\t\t\t{
\t\t\t\tif( m_pDok_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pDok_DragonPP );
\t\t\t\t\tm_pDok_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

# NOI (Thunder)
noi_start = "if( m_KeepUpMugongList.IsExist(NOI_DRAGONSINJANG) || m_KeepUpMugongList.IsExist(NOI_DRAGONSUNGCHEON ) )"
noi_end = """\t\t\t\t\tm_pNoi_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

noi_replacement = """if( m_KeepUpMugongList.IsExist(NOI_DRAGONSINJANG) || m_KeepUpMugongList.IsExist(NOI_DRAGONSUNGCHEON ) )\t//?? ?? ??
\t\t\t{
\t\t\t\tif( !m_pNoi_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

\t\t\t\t\t_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eNoi_Dragon );
\t\t\t\t\t// matrix
\t\t\t\t\tif( pEffectPackage )
\t\t\t\t\t{
\t\t\t\t\t\tm_pNoi_DragonPP = g_EffectManager.GetCurEffectPackagePair();

\t\t\t\t\t\tm_pNoi_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
\t\t\t\t\t\t
\t\t\t\t\t\tm_pNoi_DragonPP->dwTotalTime = 3000;

\t\t\t\t\t\tm_pNoi_DragonPP->dwElapsedTime = 0;
\t\t\t\t\t}

\t\t\t\t\tg_EffectManager.OffSharedPackagePair();
\t\t\t\t}
\t\t\t\telse\t// ???? ??? ?? ?? ??.
\t\t\t\t{
\t\t\t\t\tif(m_pNoi_DragonPP->dwElapsedTime >= m_pNoi_DragonPP->dwTotalTime)
\t\t\t\t\t{
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(NOI_DRAGONSINJANG))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( NOI_DRAGONSINJANG);
\t\t\t\t\t\t
\t\t\t\t\t\tif( m_KeepUpMugongList.IsExist(NOI_DRAGONSUNGCHEON))
\t\t\t\t\t\t\tm_KeepUpMugongList.Delete( NOI_DRAGONSUNGCHEON);

\t\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pNoi_DragonPP );
\t\t\t\t\t\tm_pNoi_DragonPP = NULL;
\t\t\t\t\t}
\t\t\t\t\telse
\t\t\t\t\t{
\t\t\t\t\t\tm_pNoi_DragonPP->dwElapsedTime += fLocalFrameScale;
\t\t\t\t\t\tm_pNoi_DragonPP->bIsVisible = true;
\t\t\t\t\t}
\t\t\t\t}
\t\t\t}
\t\t\telse\t// ??? ???? ???.
\t\t\t{
\t\t\t\tif( m_pNoi_DragonPP )
\t\t\t\t{
\t\t\t\t\tg_EffectManager.DeqEffectPackagePair( m_pNoi_DragonPP );
\t\t\t\t\tm_pNoi_DragonPP = NULL;
\t\t\t\t}
\t\t\t}//?? ?? ??"""

# Apply replacements
text_normalized = replace_block(text_normalized, bing_start, bing_end, bing_replacement)
text_normalized = replace_block(text_normalized, dok_start, dok_end, dok_replacement)
text_normalized = replace_block(text_normalized, noi_start, noi_end, noi_replacement)

# Restore line endings
if has_crlf:
    final_text = text_normalized.replace("\n", "\r\n")
else:
    final_text = text_normalized

with open(filePath, 'wb') as f:
    f.write(final_text.encode(encoding))

print("SUCCESS: Client-side XiahGameObject.cpp successfully patched with new dragon duration & cleanup logic!")
