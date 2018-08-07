#ifndef TOOL_FUNC_H
#define TOOL_FUNC_H

bool Tool_AppenTextToFile(const char* fileName, const char* szText);

const TCHAR* Tool_GetCurrentPath();

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize);

#endif