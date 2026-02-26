#pragma once

#include <cctype>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#ifdef _WIN32
#include <TlHelp32.h>
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/uio.h>
#endif

struct Client {
  public:
#ifdef _WIN32
    using id_t = DWORD;
#else
    using id_t = pid_t;
#endif

    Client()
        : name_(), id_(0), base_(0)
#ifdef _WIN32
          ,
          handle_(NULL), module_name_()
#endif
    {
    }

    Client(const std::string& name, id_t id = 0)
        : name_(name), id_(id), base_(0)
#ifdef _WIN32
          ,
          handle_(NULL), module_name_()
#endif
    {
    }

#ifdef _WIN32
    Client(const std::string& name, id_t id, HANDLE handle, const std::string& module_name)
        : name_(name), id_(id), handle_(handle), module_name_(module_name), base_(0) {}
#endif

    auto get_name() const -> const std::string& { return name_; }
    auto get_id() const -> id_t { return id_; }
    auto get_base() const -> uintptr_t { return base_; }

#ifdef _WIN32
    auto get_handle() const -> HANDLE { return handle_; }
    auto get_module_name() const -> const std::string& { return module_name_; }
#endif

    auto set_name(const std::string& name) -> void { name_ = name; }
    auto set_id(id_t id) -> void { id_ = id; }
    auto set_base(uintptr_t addr) -> void { base_ = addr; }

#ifdef _WIN32
    auto set_handle(HANDLE h) -> void { handle_ = h; }
    auto set_module_name(const std::string& m) -> void { module_name_ = m; }
#endif

  private:
    std::string name_;
    id_t id_;
    uintptr_t base_;

#ifdef _WIN32
    HANDLE handle_;
    std::string module_name_;
#endif
};






class Memory {
  private:
    Client client;

    void get_pid();

#ifdef _WIN32
    void get_base(DWORD pid, const std::string& module_name = std::string());
#endif

  public:
    Memory(const std::string& name, const std::string& module_name = std::string());
    ~Memory();

    auto get_client() const -> const Client&;

    template <typename T>
    T read(uintptr_t addy);

    template <typename T>
    bool write(uintptr_t addy, T data);
};

#ifdef _WIN32

template <typename T>
T Memory::read(uintptr_t addy) {
    T result{};
    ReadProcessMemory(client.get_handle(), reinterpret_cast<LPCVOID>(addy), &result, sizeof(T), nullptr);
    return result;
}

template <typename T>
bool Memory::write(uintptr_t addy, T data) {
    return WriteProcessMemory(client.get_handle(), reinterpret_cast<LPVOID>(addy), &data, sizeof(T), nullptr);
}
#else

template <typename T>
T Memory::read(uintptr_t addy) {
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

template <typename T>
bool Memory::write(uintptr_t addy, T data) {
    iovec local;
    local.iov_base = &data;
    local.iov_len = sizeof(T);

    iovec remote;
    remote.iov_base = reinterpret_cast<void*>(addy);
    remote.iov_len = sizeof(T);

    ssize_t vwrite = process_vm_writev(client.get_id(), &local, 1, &remote, 1, 0);
    return vwrite == static_cast<ssize_t>(sizeof(T));
}
#endif
