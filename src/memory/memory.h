#pragma once

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
    using id_type = DWORD;
#else
    using id_type = pid_t;
#endif
    Client()
        : name_()
        , id_(static_cast<id_type>(0))
#ifdef _WIN32
        , handle_(NULL)
        , module_name_()
#endif
    {}

    explicit Client(const std::string& name, id_type id)
        : name_(name)
        , id_(id)
#ifdef _WIN32
        , handle_(NULL)
        , module_name_()
#endif
    {}

#ifdef _WIN32
    Client(const std::string& name, id_type id, HANDLE handle, const std::string& module_name)
        : name_(name)
        , id_(id)
        , handle_(handle)
        , module_name_(module_name)
    {}
#endif

    const std::string& get_name() const { return name_; }
    id_type get_id() const { return id_; }

#ifdef _WIN32
    HANDLE get_handle() const { return handle_; }
    const std::string& get_module_name() const { return module_name_; }
#endif

    void set_name(const std::string& name) { name_ = name; }
    void set_id(id_type id) { id_ = id; }

#ifdef _WIN32
    void set_handle(HANDLE h) { handle_ = h; }
    void set_module_name(const std::string& m) { module_name_ = m; }
#endif

private:
    std::string name_;
    id_type id_;

#ifdef _WIN32
    HANDLE handle_;
    std::string module_name_;
#endif
};
