#include <stdio.h>
#include <windows.h>

typedef void (__stdcall *lpOut32)(short port, short data);
typedef short (__stdcall *lpInp32)(short port);

lpOut32 out32;
lpInp32 inp32;

int load_inpoutdll() {
    HINSTANCE hLib = LoadLibrary(TEXT("inpoutx64.dll"));
    if (!hLib) {
        printf("Greska pri ucitavanju inpoutx64.dll; Kod greske %ld\n", GetLastError());
        return -1;
    }

    out32 = (lpOut32)GetProcAddress(hLib, "Out32");
    inp32 = (lpInp32)GetProcAddress(hLib, "Inp32");

    if (!out32 || !inp32) {
        printf("Greska pri preuzimanju funkcijskih pokazivaca; Kod greske %ld\n", GetLastError());
        return -1;
    }
    return 0;
}

void write_to_parallel_port(short port, unsigned char data) {
    out32(port, data);
    printf("Uspjesno slanje!\n");
}

int main() {
    if (load_inpoutdll() != 0) return 1;

    unsigned char podaci = 0xFF; // primjer podatka za poslati (neki osmobitni broj u hex) 
    short DATA_PORT = 0x3BC;   // adresa porta

    write_to_parallel_port(DATA_PORT, podaci);

    return 0;
}
