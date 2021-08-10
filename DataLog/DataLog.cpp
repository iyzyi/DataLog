#include "stdafx.h"

#include "DataLog.h"
//#pragma warning(disable : 4996)






CDataLog::CDataLog(PCHAR szLogFilePath) {
	//fpLog = _fsopen(szLogFilePath, "a+b", _SH_DENYNO);
	//if (fpLog == NULL) {
	//	MessageBoxA(NULL, "无法打开log文件", "错误", NULL);
	//}
	strcpy(this->szLogFilePath, szLogFilePath);
	bBuffer = new BYTE[BUFFER_MAX_LENGTH];			// 千万不要写成 new BYTE(BUFFER_MAX_LENGTH);这样写会crash.

	//InitializeCriticalSection(&m_cs);
}

CDataLog::~CDataLog() {
	//fclose(fpLog);
	CloseLogFile();
	delete[] bBuffer;
	//DeleteCriticalSection(&m_cs);
}


VOID CDataLog::OpenLogFile() {
	if (fpLog == NULL) {
		fpLog = _fsopen(szLogFilePath, "a+b", _SH_DENYNO);
	}
}


VOID CDataLog::CloseLogFile() {
	if (fpLog != NULL) {
		fclose(fpLog);
	}
	fpLog = NULL;
}


// piblic的log函数需要OpenLogFile()和CloseLogFile()

VOID CDataLog::LogString(PCHAR szData) {
	//EnterCriticalSection(&m_cs);
	OpenLogFile();
	fwrite(szData, strlen(szData), 1, fpLog);
	CloseLogFile();
	//LeaveCriticalSection(&m_cs);
}


VOID CDataLog::LogHexData(PCHAR szPreString, PBYTE pbData, DWORD dwDataLen) {
	//EnterCriticalSection(&m_cs);
	OpenLogFile();

	fwrite(szPreString, strlen(szPreString), 1, fpLog);

	//system("pause");

	DWORD dwRow = 0, dwColumn = 0;
	for (dwRow = 0; dwRow < dwDataLen / 16 + 1; dwRow++) {
		for (dwColumn = 0; (dwRow * 16 + dwColumn < dwDataLen) && (dwColumn < 16); dwColumn++) {
			sprintf((PCHAR)bBuffer, "0x%02x ", pbData[dwRow * 16 + dwColumn]);
			__LogString((PCHAR)bBuffer);
		}

		if (dwColumn != 16) {
			while (dwColumn < 16) {
				__LogString("     ");
				dwColumn++;
			}
		}
		__LogChar("\t");

		for (dwColumn = 0; (dwRow * 16 + dwColumn < dwDataLen) && (dwColumn < 16); dwColumn++) {
			DWORD dwIndex = dwRow * 16 + dwColumn;
			if (pbData[dwIndex] >= 32 && pbData[dwIndex] <= 126) {
				__LogChar(pbData[dwIndex]);
			}
			else {
				__LogChar(".");
			}
		}
		__LogEnter();
	}

	CloseLogFile();
	//LeaveCriticalSection(&m_cs);
}



// private的log函数不能OpenLogFile()和CloseLogFile()

VOID CDataLog::__LogString(PCHAR szData) {
	fwrite(szData, strlen(szData), 1, fpLog);
}


VOID CDataLog::__LogEnter() {
	fwrite("\n", 1, 1, fpLog);
}


VOID CDataLog::CDataLog::__LogChar(PCHAR pbOneChar) {
	fwrite(pbOneChar, 1, 1, fpLog);
}


VOID CDataLog::__LogChar(CHAR cData) {
	sprintf((PCHAR)bBuffer, "%c", cData);
	fwrite((PCHAR)bBuffer, 1, 1, fpLog);
}



int main()
{
	CHAR szLogFilePath[] = "d:\\桌面\\test.log";
	CDataLog m_DataLog = CDataLog((PCHAR)szLogFilePath);


	sprintf((PCHAR)m_DataLog.bBuffer, "我的数据是%d，你的数据是0x%llx\n", 123, 0x123456789abc);
	m_DataLog.LogString((PCHAR)m_DataLog.bBuffer);

	system("pause");

	CHAR szPreString[] = "接下来的数据是十六进制形式：\n";
	CHAR szData[] = "对于互斥锁我们要先知道为什么要用互斥锁？它能解决什么问题？根据这两个问题，可以来举个例子说明一下，假如现在我们要求1 - 10000的和，然后我们为了提高效率，我们建立两个线程同时去计算[1, 5000)的和以及[5000, 10001)的和，那么用于计算和的变量都用相同的ans来获取结果，代码如下";
	DWORD dwDataLen = strlen(szData);
	m_DataLog.LogHexData(szPreString, (PBYTE)szData, dwDataLen);

	
    return 0;
}