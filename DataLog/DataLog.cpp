#include "stdafx.h"
#include "windows.h"
#pragma warning(disable : 4996)


#define BUFFER_MAX_LENGTH 65536


class CDataLog {

public:

	FILE *fpLog = NULL;
	PBYTE bBuffer;
	CRITICAL_SECTION m_cs;

	CDataLog(PCHAR szLogFilePath) {
		fpLog = fopen(szLogFilePath, "a+b");
		if (fpLog == NULL) {
			MessageBoxA(NULL, "�޷���log�ļ�", "����", NULL);
		}

		bBuffer = new BYTE[BUFFER_MAX_LENGTH];			// ǧ��Ҫд�� new BYTE(BUFFER_MAX_LENGTH);����д��crash.

		InitializeCriticalSection(&m_cs);
	}

	~CDataLog() {
		fclose(fpLog);
		delete[] bBuffer;
		DeleteCriticalSection(&m_cs);
	}

	VOID LogEnter() {
		fwrite("\n", 1, 1, fpLog);
	}

	VOID LogChar(PCHAR pbOneChar) {
		fwrite(pbOneChar, 1, 1, fpLog);
	}

	VOID LogChar(CHAR cData) {
		sprintf((PCHAR)bBuffer, "%c", cData);
		fwrite((PCHAR)bBuffer, 1, 1, fpLog);
	}

	VOID LogString(PCHAR szData) {
		EnterCriticalSection(&m_cs);
		fwrite(szData, strlen(szData), 1, fpLog);
		LeaveCriticalSection(&m_cs);
	}

	VOID LogHexString(PCHAR szPreString, PBYTE pbData, DWORD dwDataLen) {
		EnterCriticalSection(&m_cs);

		fwrite(szPreString, strlen(szPreString), 1, fpLog);

		DWORD dwRow = 0, dwColumn = 0;
		for (dwRow = 0; dwRow < dwDataLen / 16 + 1; dwRow++) {
			for (dwColumn = 0; (dwRow * 16 + dwColumn < dwDataLen) && (dwColumn < 16); dwColumn++) {
				sprintf((PCHAR)bBuffer, "0x%02x ", pbData[dwRow * 16 + dwColumn]);
				LogString((PCHAR)bBuffer);
			}

			if (dwColumn != 16) {
				while (dwColumn < 16) {
					LogString("     ");
					dwColumn++;
				}
			}
			LogChar("\t");

			for (dwColumn = 0; (dwRow * 16 + dwColumn < dwDataLen) && (dwColumn < 16); dwColumn++) {
				DWORD dwIndex = dwRow * 16 + dwColumn;
				if (pbData[dwIndex] >= 32 && pbData[dwIndex] <= 126) {
					LogChar(pbData[dwIndex]);
				}
				else {
					LogChar(".");
				}
			}
			LogEnter();
		}

		LeaveCriticalSection(&m_cs);
	}
};



int main()
{
	CHAR szLogFilePath[] = "d:\\����\\test.log";
	CDataLog m_DataLog = CDataLog((PCHAR)szLogFilePath);


	sprintf((PCHAR)m_DataLog.bBuffer, "�ҵ�������%d�����������0x%llx\n", 123, 0x123456789abc);
	m_DataLog.LogString((PCHAR)m_DataLog.bBuffer);


	CHAR szPreString[] = "��������������ʮ��������ʽ��\n";
	CHAR szData[] = "���ڻ���������Ҫ��֪��ΪʲôҪ�û����������ܽ��ʲô���⣿�������������⣬�������ٸ�����˵��һ�£�������������Ҫ��1 - 10000�ĺͣ�Ȼ������Ϊ�����Ч�ʣ����ǽ��������߳�ͬʱȥ����[1, 5000)�ĺ��Լ�[5000, 10001)�ĺͣ���ô���ڼ���͵ı���������ͬ��ans����ȡ�������������";
	DWORD dwDataLen = strlen(szData);
	m_DataLog.LogHexString(szPreString, (PBYTE)szData, dwDataLen);


    return 0;
}