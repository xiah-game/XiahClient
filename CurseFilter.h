#pragma once

// 욕설 필터링

// 욕설 정의 파일 로드
extern bool LoadCurses(const char *filename);
extern bool UnloadCurses();
extern bool IsCurse(const char *str);
extern char *ConvertString(char *str, int max_len);
