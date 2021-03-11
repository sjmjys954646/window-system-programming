#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s'은 실행할수있는 프로그램이 아닙니다.\n");

int CmdProcessing();
TCHAR *StrLower(TCHAR *);

int tmain(int agrc, TCHAR *argv[])
{
    _tsetlocale(LC_ALL, _T("Korean"));

    DWORD isExit;

    while (1)
    {
        isExit = CmdProcessing();
        if (isExit == TRUE)
        {
            _fputts(_T("명령어 처리를 종료합니다.\n"), stdout);
            break;
        }
    }

    return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

int CmdProcessing()
{
    _fputts(_T("Best command prompt>> "), stdout);
    _getts(cmdString);

    TCHAR *token = _tcstok(cmdString, seps);
    int tokenNum = 0;

    while (token != NULL)
    {
        _tcscpy(cmdTokenList[tokenNum++], StrLower(token));
        token = _tcstok(NULL, seps);
    }

    if (!_tcscmp(cmdTokenList[0], _T("exit")))
    {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("추가되는명령어 1")))
    {
    }
    else if (!_tcscmp(cmdTokenList[0], _T("추가되는명령어 2")))
    {
    }
    else
    {
        STARTUPINFO si = {
            0,
        };
        PROCESS_INFORMATION pi;
        si.cb = sizeof(si);
        BOOL isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        if (isRun == FALSE)
            _tprintf(ERROR_CMD, cmdTokenList[0]);
    }
    return 0;
}

TCHAR *StrLower(TCHAR *pStr)
{
    TCHAR *ret = pStr;

    while (*pStr)
    {
        if (_istupper(*pStr))
            *pStr = _totlower(*pStr);
        pStr++;
    }
    return ret;
}