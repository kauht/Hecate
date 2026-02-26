#include "memory.h"

#ifdef _WIN32
#include <TlHelp32.h>
#include <algorithm>
#else
#include <cerrno>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <sys/uio.h>
#endif

Memory::Memory(const std::string& name, const std::string& module_name) {
    client.set_name(name);
    get_pid();

#ifdef _WIN32
    auto pid = client.get_id();
    if (pid != 0) {
        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hProc) {
            client.set_handle(hProc);
        }

        get_base(client.get_id(), module_name);

        if (!module_name.empty() && client.get_base()) {
            client.set_module_name(module_name);
        }
    }
#endif
}

Memory::~Memory() {
#ifdef _WIN32
    if (client.get_handle()) {
        CloseHandle(client.get_handle());
    }
#endif
}

auto Memory::get_client() const -> const Client& { return client; }

void Memory::get_pid() {
#ifdef _WIN32
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hsnap, &pe)) {
        do {
            if (_stricmp(client.get_name().c_str(), pe.szExeFile) == 0) {
                client.set_id(pe.th32ProcessID);
                break;
            }
        } while (Process32Next(hsnap, &pe));
    }

    CloseHandle(hsnap);
#else
    DIR* proc = opendir("/proc");
    if (!proc) {
        client.set_id(0);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(proc)) != nullptr) {
        const char* name = entry->d_name;
        if (!std::isdigit(name[0]))
            continue;

        pid_t pid = 0;

        std::string comm_path = std::string("/proc/") + name + "/comm";
        std::ifstream comm_file(comm_path);
        if (!comm_file.is_open())
            continue;

        std::string proc_name;
        std::getline(comm_file, proc_name);
        comm_file.close();

        if (proc_name == client.get_name()) {
            client.set_id(pid);
            closedir(proc);
            return;
        }
    }

    closedir(proc);
    client.set_id(0);
#endif
}

#ifdef _WIN32
void Memory::get_base(DWORD pid, const std::string& module_name) {
    if (!pid) {
        client.set_base(0);
        return;
    }

    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

    MODULEENTRY32 me;
    me.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hsnap, &me)) {
        if (module_name.empty()) {
            client.set_base(me.modBaseAddr);
        }

        do {
            if (!module_name.empty() && _stricmp(module_name.c_str(), me.szModule) == 0) {
                client.set_base(me.modBaseAddr);
                break;
            }
        } while (Module32Next(hsnap, &me));
    }

    CloseHandle(hsnap);
}
#endif
