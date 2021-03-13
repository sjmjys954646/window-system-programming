#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s'은 실행할수있는 프로그램이 아닙니다.\n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

int CmdProcessing(int);
int CmdReadTokenize(void);
TCHAR *StrLower(TCHAR *);

int tmain(int argc, TCHAR *argv[])
{
    /*** 한글 입력 하게하기위해***/
    _tsetlocale(LC_ALL, _T("Korean"));

    /*** 매개변수 전달인자가 있는경우 처리***/
    if (argc > 2)
    {
        for (int i = 1; i < argc; i++)
            _tcscpy(cmdTokenList[i - 1], argv[i]);
        CmdProcessing(argc - 1);
    }

    DWORD isExit = NULL;

    while (1)
    {
        int tokenNum = CmdReadTokenize();

        if (tokenNum == 0)
            continue;

        isExit = CmdProcessing(tokenNum);
        if (isExit == TRUE)
        {
            _fputts(_T("명령어 처리를 종료합니다.\n"), stdout);
            break;
        }
    }

    return 0;
}

int CmdReadTokenize(void)
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
    return tokenNum;
}

int CmdProcessing(int tokenNum)
{
    BOOL isRun;
    STARTUPINFO si = {
        0,
    };
    PROCESS_INFORMATION pi;

    TCHAR cmdStringWithOptions[STR_LEN] = {
        0,
    };
    TCHAR optString[STR_LEN] = {
        0,
    };
    if (!_tcscmp(cmdTokenList[0], _T("exit")))
    {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("start")))
    {
        if (tokenNum > 1)
        {
            for (int i = 1; i < tokenNum; i++)
            {
                _stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
            }
            _stprintf(cmdStringWithOptions, _T("%s %s"), _T("CmdProject.exe"), optString);
        }
        else
            _stprintf(cmdStringWithOptions, _T("%s"), _T("CmdProject.exe"));

        isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else if (!_tcscmp(cmdTokenList[0], _T("echo")))
    {
        for (int i = 1; i < tokenNum; i++)
        {
            _stprintf(optString, _T("%s %s", optString, cmdTokenList[i]));
        }

        _tprintf(_T("echo message : %s \n"), optString);
    }
    else
    {
        _tcscpy(cmdStringWithOptions, cmdTokenList[0]);
        for (int i = 1; i < tokenNum; i++)
        {
            _stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);
        }
        BOOL isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

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