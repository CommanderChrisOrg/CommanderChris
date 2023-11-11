
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

int main() {
    HKEY hUninstKey = NULL;
    HKEY hAppKey = NULL;
    char sAppKeyName[1024];
    char sSubKey[1024];
    char sDisplayName[1024];
    char sDisplayVersion[1024];
    long lResult = ERROR_SUCCESS;
    DWORD dwType = KEY_ALL_ACCESS;
    DWORD dwBufferSize = 0;

    // Open the "Uninstall" key.
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),
        0, KEY_READ, &hUninstKey) != ERROR_SUCCESS) {
        std::cerr << "Failed to open uninstall key." << std::endl;
        return 1;
    }

    // Enumerate all subkeys of the uninstall key.
    for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++) {
        dwBufferSize = sizeof(sAppKeyName);
        lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName, &dwBufferSize, NULL, NULL, NULL, NULL);
        if (lResult == ERROR_SUCCESS) {
            // Open the subkey.
            strcpy(sSubKey, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
            strcat(sSubKey, sAppKeyName);
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_READ, &hAppKey) != ERROR_SUCCESS) {
                std::cerr << "Failed to open app key." << std::endl;
                continue;
            }

            // Try to read the display name value.
            dwBufferSize = sizeof(sDisplayName);
            if (RegQueryValueEx(hAppKey, "DisplayName", NULL, NULL, (LPBYTE)sDisplayName, &dwBufferSize) == ERROR_SUCCESS) {
                // Try to read the display version value.
                dwBufferSize = sizeof(sDisplayVersion);
                if (RegQueryValueEx(hAppKey, "DisplayVersion", NULL, NULL, (LPBYTE)sDisplayVersion, &dwBufferSize) != ERROR_SUCCESS) {
                    strcpy(sDisplayVersion, "Unknown");
                }
                
                // Write to file
                std::ofstream outfile("installed_programs.txt", std::ios::app);
                if (outfile.is_open()) {
                    outfile << sDisplayName << " - Version: " << sDisplayVersion << std::endl;
                    outfile.close();
                }
            }
            RegCloseKey(hAppKey);
        }
    }

    RegCloseKey(hUninstKey);

    return 0;
}
