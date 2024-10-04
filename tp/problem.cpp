#include <iostream>
#include <Windows.h>

int main() {
    HANDLE hFile = CreateFile(
        L"\\\\.\\PhysicalMemory",  // Physical memory device
        GENERIC_READ,              // Read access
        FILE_SHARE_READ,           // Share mode
        NULL,                      // Security attributes
        OPEN_EXISTING,             // Open existing file
        0,                         // File attributes
        NULL                       // Template file
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open physical memory device." << std::endl;
        return 1;
    }

    // Map a view of the physical memory into the process's address space
    PVOID addr = MapViewOfFile(
        hFile,               // Handle to the physical memory device
        FILE_MAP_READ,       // Read-only access
        0,                   // High-order DWORD of the file offset
        0,                   // Low-order DWORD of the file offset
        4096                // Mapping size (e.g., one page)
    );

    if (addr == nullptr) {
        std::cerr << "Failed to map physical memory." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // Read an integer value from the mapped memory address 0x3BC (example address)
    int* ptr = reinterpret_cast<int*>(static_cast<char*>(addr) + 0x3BC);
    int value = *ptr;

    std::cout << "Value at address 0x3BC: " << value << std::endl;

    // Unmap the view of the physical memory
    UnmapViewOfFile(addr);

    // Close the handle to the physical memory device
    CloseHandle(hFile);

    return 0;
}
