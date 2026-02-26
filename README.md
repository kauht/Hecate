<div align="center">
  <h1>Hecate</h1>
  <h3>Cross-platform process manipulation framework</h3>
  <br>
  <p>
    <a href="#"><img src="https://img.shields.io/badge/license-MIT-blue" alt="License"></a>
    <a href="#"><img src="https://img.shields.io/badge/platform-x64-lightgrey" alt="Platform"></a>
  </p>
  <p><em>At the crossroads of worlds.</em></p>
</div>

<hr>

<div align="center">
  <p>
    <b><a href="#overview">Overview</a></b> •
    <b><a href="#features">Features</a></b> •
    <b><a href="#roadmap">Roadmap</a></b> •
    <b><a href="#research">Research</a></b> •
    <b><a href="#license">License</a></b>
  </p>
</div>

---

# Overview

Hecate is a cross-platform process manipulation framework designed for personal research.

It provides structured abstractions for:

- Remote memory interaction
- Injection techniques
- Pattern scanning
- Hooking mechanisms
- Platform abstraction (Windows / POSIX)

---

# Features

## Memory

- Typed read/write utilities
- Raw region access
- Pointer chain resolution
- String extraction
- Module enumeration

## Scanner

- AOB pattern scanning
- Wildcard signatures
- Section-aware scanning
- ASLR-safe resolution

## Injection

- Remote allocation utilities
- Shellcode execution
- LoadLibrary injection (Windows)
- dlopen injection (Linux)
- Manual mapping loader

## Hooking

- Inline trampolines
- VTable swapping
- IAT patching
- Disassembler integration (Capstone / Zydis)

---

# Roadmap

## Foundation
- [ ] Clean Windows / Linux backend abstraction
- [ ] Static + shared builds (.lib/.a/.dll/.so)
- [ ] Optional header-only distribution
- [ ] Documented public API
- [ ] Stable error handling model

## Memory Layer
- [ ] Safe typed read/write
- [ ] Region querying
- [ ] Pointer chain utilities

## Scanner
- [ ] Wildcard AOB scanning
- [ ] Efficient region scanning
- [ ] Module-relative resolution

## Injection
- [ ] LoadLibrary injection
- [ ] dlopen injection
- [ ] Reflective/manual mapping

## Hooking
- [ ] Inline trampolines
- [ ] Disassembly-assisted relocation
- [ ] VTable swapping

## Advanced
- [ ] Unified internal/external API
- [ ] Optional kernel backend
- [ ] Research documentation

---

# Research

See `RESEARCH.md` for the structured learning roadmap for each subsystem of Hecate, feel free to check it out for yourself and learn along with me!

---

# License

Hecate is licensed under the MIT License.

See the `LICENSE` file for full details.
