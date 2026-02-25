#include "memory.h"



class Memory {
    private:
    Client client;
    #ifdef _WIN32
    auto get_pid() -> void {
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);


        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hsnap, &pe)) {
            do {
                if (_stricmp(client.name_.c_str(), pe.szExeFile) == 0) {
                    client.set_id(pe.th32ProcessID);
                }
            } while (Process32Next(hsnap, &pe));
        }

        CloseHandle(hsnap);
        return;
    }

    auto get_base(DWORD pid, const std::string& module_name = std::string()) -> void {
        if (!pid) {
            return;
        }

        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

        MODULEENTRY32 me{};
        me.dwSize = sizeof(MODULEENTRY32);

        uintptr_t found_base = 0;

        if (Module32First(hsnap, &me)) {
            if (module_name.empty()) {
                client.set_base(me.modBaseAddr);
            }

            do {
                if (_stricmp(module_name.c_str(), me.szModule) == 0) {
                    client.set_base(me.modBaseAddr);
                }
            } while (Module32Next(hsnap, &me));
        }

        CloseHandle(hsnap);

        return;
    }
    #else
    auto get_pid() -> void {
        DIR* proc = opendir("/proc");
        if (!proc) {
            client.set_id(0);
            return;
        }

        dirent* entry;
        while ((entry = readdir(proc)) != nullptr) {
            if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
                pid_t pid = std::stoi(entry->d_name);

                std::string path = "/proc/" + std::string(entry->d_name) + "/comm";
                std::ifstream comm_file(path);
                std::string name;
                if (comm_file) {
                    std::getline(comm_file, name);
                }

                // std::cout << "PID: " << pid << ", Name: " << name << std::endl;
                if (name == client.get_name()) {
                    closedir(proc);
                    client.set_id(pid);
                }
            }
        }
        closedir(proc);
        client.set_id(0);
    }
    #endif







    public:
    Memory(const std::string& name, const std::string& module_name = std::string()) {
        client = Client(name);
#ifdef _WIN32
        get_pid();
        if (client.get_id()) {
            HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
            if (hProc) {
                client.set_handle(hProc);
            }

            uintptr_t base = get_base(pid, module_name);
            client.set_base_address(base);
            if (!module_name.empty() && base) {
                client.set_module_name(module_name);
            }
        }
#else
        client.set_id(client.get_id());
#endif
    }
    ~Memory() {

    }
    auto get_client() const -> const Client& {
        return client;
    }

    template<typename T>
    T read(uintptr_t addy) {
        T result{};

        iovec local;
        local.iov_base = &result;
        local.iov_len = sizeof(T);

        iovec remote;
        remote.iov_base = reinterpret_cast<void*>(addy);
        remote.iov_len = sizeof(T);


        ssize_t vread = process_vm_readv(client.get_id(), &local, 1, &remote, 1, 0);
        return result;
    }

    template<typename T>
    bool write(uintptr_t addy, T data) {
        iovec local;
        local.iov_base = &data;
        local.iov_len = sizeof(T);

        iovec remote;
        remote.iov_base = reinterpret_cast<void*>(addy);
        remote.iov_len = sizeof(T);


        ssize_t vwrite = process_vm_writev(client.get_id(), &local, 1, &remote, 1, 0);
        return vwrite == sizeof(T);
    }

};
