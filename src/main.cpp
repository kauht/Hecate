#include <cstring>
#include <sys/types.h>
#include <string>
#include <fstream>
#include <stdint.h>
#ifdef _WIN32
#include <Windows.h>
#include <TlHelp32.h>
#else
#include <dirent.h>
#include <sys/uio.h>
#endif

struct Client {
    public:
    #ifdef _WIN32
    Client(const std::string& name, int id, HANDLE handle, const std::string& module_name) : name_(name), id_(id), handle_(handle), module_name_(module_name) {}
    const HANDLE get_handle() const {
        return handle_;
    }
    const std::string& get_module_name() const {
        return module_name_;
    }
    #else
    Client(const std::string& name, int id, HANDLE handle) : name_(name), id_(id) {}
    #endif

    const std::string& get_name() const {
        return name_;
    }
    const int get_id() const {
        return id_;
    }
private:
    std::string name_;
    int id_;

    #ifdef _WIN32
    HANDLE handle_;
    std::string module_name_;
    #endif
};

#ifdef _WIN32


auto get_pid(const std::string& process_name) -> DWORD {

}

auto get_handle(DWORD pid, const std::string& module_name = std::string()) -> HANDLE {
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    LPMODULEENTRY32 lpme{};
    lpme->dwSize = sizeof(LPMODULEENTRY32);

    if (Module32First(hsnap, lpme)) {
        if (module_name == std::string()) {
            return lpme->modBaseAddr;
        }
        do {
            if (std::string(lpme->szModule) == module_name) {
                return lpme->modBaseAddr;
            }
        } while(Module32Next(hsnap, lpme));
    }
    return NULL;
}
#else
auto get_pid(const std::string& process_name) -> pid_t {
    DIR* proc = opendir("/proc");

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
            if (name == process_name) {
                return pid;
            }
        }
    }
    closedir(proc);
    return 0;
}
#endif

auto main() -> int {
    pid_t pid = get_pid("electron");

}


class Memory {
    private:
    Client client;

    public:
    Memory(std::string name) : client(name, get_pid("name")){}
    ~Memory() {

    }
    const Client get_client() const {
        return client;
    }

    template<typename T>
    T read(uintptr_t addy) {
        T result = NULL;

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


        ssize_t vread = process_vm_writev(client.get_id(), &local, 1, &remote, 1, 0);
        return vread == sizeof(T);
    }

};
