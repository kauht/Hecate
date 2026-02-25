#pragma once

#include <cstring>
#include <sys/types.h>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdint>
#include <unistd.h>

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
    using id_t = DWORD;
#else
    using id_t = pid_t;
#endif

    Client()
        : name_()
        , id_(static_cast<id_t>(0))
#ifdef _WIN32
        , handle_(NULL)
        , module_name_()
#endif
    {}

    Client(const std::string& name, id_t id = static_cast<id_t>(0))
        : name_(name)
        , id_(id)
#ifdef _WIN32
        , handle_(NULL)
        , module_name_()
#endif
    {}

#ifdef _WIN32
    Client(const std::string& name, id_t id, HANDLE handle, const std::string& module_name)
        : name_(name)
        , id_(id)
        , handle_(handle)
        , module_name_(module_name)
    {}
#endif

    auto get_name() const -> const std::string& { return name_; }
    auto get_id() const -> id_t { return id_; }

#ifdef _WIN32
    auto get_handle() const -> HANDLE { return handle_; }
    auto get_module_name() const -> const std::string& { return module_name_; }
#endif

    auto set_name(const std::string& name) -> void { name_ = name; }
    auto set_id(id_t id) -> void { id_ = id; }

#ifdef _WIN32
    auto set_handle(HANDLE h) -> void { handle_ = h; }
    auto set_module_name(const std::string& m) -> void { module_name_ = m; }
#endif

private:
    std::string name_;
    id_t id_;

#ifdef _WIN32
    HANDLE handle_;
    std::string module_name_;
#endif
};
