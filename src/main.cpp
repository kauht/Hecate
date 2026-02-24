#include <cstring>
#include <exception>
#include <stdexcept>
#include <sys/types.h>
#include <sys/uio.h>
#include <string>
#include <dirent.h>
#include <cctype>
#include <iostream>
#include <print>
#include <format>
#include <vector>
#include <cctype>
#include <fstream>
#include <stdint.h>

struct Client {
public:
    Client(const std::string& name, int id) : name_(name), id_(id) {}
    const std::string& get_name() const {
        return name_;
    }
    const int get_id() const {
        return id_;
    }
private:
    std::string name_;
    int id_;
};

auto get_pid(const std::string& n) -> pid_t {
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
            if (name == n) {
                return pid;
            }
        }
    }
    closedir(proc);
    return 0;
}

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
