# RESEARCH.md

Hecate — Research & Roadmap

---

# 1 — Process & Memory Fundamentals

## Docs

https://learn.microsoft.com/en-us/sysinternals/resources/windows-internals  
https://man7.org/linux/man-pages/  
https://man7.org/linux/man-pages/man2/ptrace.2.html  
https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/  
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/  

## And

https://www.unknowncheats.me/forum/general-programming-and-reversing/  
https://www.unknowncheats.me/forum/c-and-c-/  
https://www.unknowncheats.me/forum/anti-cheat-bypass/  

(“ReadProcessMemory explanation”, “ptrace example”, “remote memory reading explanation”)

## Topics

- 64-bit virtual memory layout
- ASLR mechanics
- Page protections (R/W/X)
- Process handles & permissions
- Remote memory primitives
- Thread creation primitives
- Differences: Windows vs Linux model

## Completion

You must be able to:

- Draw a 64-bit process memory layout.
- Explain ASLR entropy and base randomization.
- Explain how ReadProcessMemory works internally.
- Compare ptrace vs OpenProcess conceptually.

## Test Project

Build a standalone tool that:

- Enumerates processes
- Retrieves module base address
- Reads a known variable from target
- Survives restarts (ASLR-safe)

---

# 2 — Binary Format Mastery (PE / ELF)

## Docs

https://learn.microsoft.com/en-us/windows/win32/debug/pe-format  
https://0xrick.github.io/win-internals/pe1/  
https://wiki.osdev.org/ELF  
https://refspecs.linuxfoundation.org/elf/elf.pdf  

## And

Search UC for:
- “Manual map explanation”
- “PE relocation explanation”
- “IAT fixing guide”

## Topics

- DOS header
- NT headers
- Optional header
- Section tables
- Import Address Table
- Export tables
- Base relocations
- ELF program headers
- Dynamic linking

## Completion

You must:

- Parse PE manually.
- Print entrypoint.
- Resolve exports without LoadLibrary.
- Explain relocation patching logic.

## Test Project

Write a PE parser that:

- Prints sections
- Prints entrypoint
- Lists imported modules
- Lists exported functions

No third-party libraries.

---

# 3 — Injection (Usermode)

## Docs

https://github.com/stephenfewer/ReflectiveDLLInjection  
https://www.ired.team/offensive-security/code-injection-process-injection  
https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/  
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/  

## And

Search UC for:
- “Manual map source breakdown”
- “Reflective loader explanation”
- “CreateRemoteThread internals”
- “dlopen injection ptrace”

## Topics

- Remote allocation
- Remote thread creation
- Manual mapping algorithm
- Relocation application
- IAT resolution
- TLS callback execution
- Linux dlopen injection

## Completion

You must explain:

- Manual mapping steps sequentially.
- Why relocations are mandatory.
- How IAT resolution works.
- Why TLS callbacks must be executed.

## Test Project

Implement:

1. LoadLibrary injection.
2. Manual mapping injection without LoadLibrary.

---

# 4 — Pattern Scanning

## Research

https://www.codereversing.com/archives/593  
https://www.codereversing.com/archives/592  

## And

Search UC for:
- “AOB scanner implementation”
- “Wildcard pattern scan”
- “Signature scanning tutorial”

## Topics

- Byte pattern scanning
- Wildcard matching
- Region enumeration
- Memory protection filtering
- Efficient search algorithms

## Completion

You must explain:

- Why offsets break after updates.
- Why scanning must respect protection flags.
- Wildcard comparison logic.

## Test Project

Implement a remote scanner that:

- Scans only executable regions
- Finds signature across restarts
- Works module-relative

---

# 5 — Hooking & Trampolines

## Core Docs

https://github.com/microsoft/Detours  
https://capstone-engine.org/  
https://github.com/zyantific/zydis  

## And

Search UC for:
- “Trampoline hook explanation”
- “RIP relative relocation problem”
- “Inline hook x64 tutorial”
- “VTable swap example”

## Topics

- x86-64 instruction encoding
- Relative jumps
- RIP-relative addressing
- Prologue patching
- Stolen instruction relocation
- Trampoline construction
- VTable modification

## Completion

You must explain:

- Why instruction boundaries matter.
- Why 5-byte jumps aren’t always safe.
- How RIP-relative displacement breaks naive hooks.
- How stolen instructions are relocated.

## Test Project

Implement:

- Inline hook
- Safe trampoline
- Automatic patch-size calculation via disassembler

No third-party hooking libraries.

---

# 6 — Architecture & Framework Design

## Topics

- Backend abstraction pattern
- OS separation boundaries
- ABI stability
- Error model design
- Public API discipline

## Completion

You must:

- Separate Windows/Linux backends.
- Ensure no platform headers leak into public API.
- Provide static + shared builds.
- Maintain stable API surface.

## Test Project

Refactor Hecate so:

- Backends are swappable
- API remains unchanged
- Public headers are clean

---

# 7 — Kernel Mode

## Docs

https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/  
https://sysprog21.github.io/lkmpg/  
https://www.kernel.org/doc/html/latest/driver-api/ioctl.html  

## Topics

- IOCTL handling
- IRP dispatch
- MmCopyVirtualMemory
- copy_to_user / copy_from_user
- Driver signing model

## Completion

You must explain:

- User ↔ kernel communication path.
- Why kernel memory access bypasses user restrictions.
- IRP lifecycle.

## Test Project

Implement minimal driver that:

- Exposes device
- Handles IOCTL
- Reads target process memory safely

---

End of Research Plan.
