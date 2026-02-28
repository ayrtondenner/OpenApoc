# CLAUDE.md - OpenApoc AI Agent Instructions

## Project Overview

OpenApoc is an open-source C++17 reimplementation of X-COM: Apocalypse, licensed under GPL3.
CMake (minimum 3.30) with Ninja generator. Targets GCC 8+, Clang 7+, MSVC 2019+.

## Reference Links

- [UFOpaedia Wiki (main)](https://www.ufopaedia.org/index.php) — encyclopaedia for all X-COM games
- [X-COM: Apocalypse (original game)](https://www.ufopaedia.org/index.php?title=Apocalypse) — reference for original game mechanics, units, items, etc.
- [OpenApoc Wiki](https://www.ufopaedia.org/index.php/OpenApoc) — OpenApoc-specific documentation
- [Differences to X-COM (OpenApoc)](https://www.ufopaedia.org/index.php?title=Differences_to_X-COM_(OpenApoc)) — what OpenApoc changes or diverges from the original game

## Build & Test

```bash
# Configure (out-of-source build)
cmake -B build -G Ninja

# Build
cmake --build build

# Run tests
cd build && ctest

# Format all source files (Unix, from build dir)
cmake --build build -t format-sources

# Format specific files
clang-format -i path/to/file.cpp path/to/file.h

# Lint (from build dir)
cmake --build build -t tidy
```

Windows with MSVC:
```cmd
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Always run `clang-format -i` on modified files before committing. The lint CI will reject
unformatted code.

## Directory Structure

```
library/        Core utilities (strings, colours, vectors, sp.h smart pointer aliases)
framework/      Engine core (rendering, sound, filesystem, events, logging, serialization)
forms/          UI widget system (controls, labels, listboxes, buttons)
game/state/     Game logic and state (city, battle, rules, serialization)
game/ui/        Game UI screens (city view, battle view, ufopaedia, base management)
tools/          Extractors, editor, launcher, code generators, lint scripts
tests/          CTest unit tests
dependencies/   Git submodules (fmt, glm, lodepng, lua, magic_enum, miniz, physfs, pugixml, libsmacker)
data/           Game data, XML forms, fonts, language files
```

## Code Style

Full guide: `CODE_STYLE.md`. Enforced by `clang-format-18` and `clang-tidy-18`.

### Formatting
- **Indentation**: Tabs for indenting, spaces for alignment, 100-column limit (tab width = 4)
- **Braces**: Allman style — opening brace on its own line
- **Always use braces**, even for single-statement blocks
- **Namespace content is not indented**; closing brace gets `// namespace Name` comment
- **Pointers/references**: Align right (`int *ptr`, `Type &ref`)
- **Line breaks**: Break before operators when wrapping, indent continuation by one extra tab

### Naming
- `CamelCase`: classes, enums, enum class members, namespaces, template parameters
- `camelBack`: methods, member variables, function parameters, local variables
- `SHOUTY_CAPS`: constants, macros
- `lower_case`: labels

### Class Conventions
- `public:`/`private:`/`protected:` at class indent level (not indented into class scope)
- Always explicitly write `private:`, even though it is the default
- `virtual` only on base class; `override` on derived — never both together
- All classes with virtual methods must have a virtual destructor
- Use `= default` instead of empty `{}` constructor/destructor bodies
- Prefer member initializer lists; initializer order must match declaration order
- Use `struct` only for data-only types; structs must never use access modifiers

### General
- **Use `auto`** liberally, especially when the type is obvious from the RHS. Use `auto &` to avoid copies.
- **Early return** is encouraged
- **const aggressively**: const member functions, const parameters, const return types, const local variables
- **Range-for loops**: `for (auto &element : container)` / `for (const auto &element : container)`
- **No naked `new`** — always wrap in smart pointers immediately
- **No C-style casts** — use `static_cast<>`/`reinterpret_cast<>`
- Prefer `{}` brace initialization
- Prefer `emplace()` over `insert()` in STL containers
- Use `enum class` over plain `enum`

## Project-Specific Types and Patterns

### Strings
Use `UString` (from `library/strings.h`) for all strings. All `char*`/`std::string` values
are assumed UTF-8.

### Smart Pointers (library/sp.h)
```cpp
sp<T>  // std::shared_ptr<T>
up<T>  // std::unique_ptr<T>
wp<T>  // std::weak_ptr<T>
mksp<T>(args...)  // std::make_shared<T>(args...)
mkup<T>(args...)  // std::make_unique<T>(args...)
```
Prefer `up<>` (exclusive ownership) over `sp<>` unless shared ownership is genuinely needed.
Use `std::move()` to transfer `up<>` ownership.

### Logging (framework/logger.h)
Uses **fmt-style format strings** (NOT printf-style):
```cpp
LogInfo("Loaded mod \"{0}\"", modName);
LogWarning("Value {0} exceeds limit {1}", value, limit);
LogError("Failed to load file \"{0}\"", path);
```
- `LogInfo` — general information, use freely
- `LogWarning` — recoverable errors
- `LogError` — fatal errors

### String Formatting (library/strings_format.h)
```cpp
UString result = OpenApoc::format("text {0} more {1}", arg1, arg2);
```
Uses the `fmt` library.

### Translation
```cpp
UString translated = tr("English text to translate");
```

## Include and Header Conventions

- Use `#pragma once` (no include guards)
- **Include order**: local headers first, then system headers. Each block alphabetically sorted.
- **Local headers use paths relative to the OpenApoc root**: `"framework/logger.h"`, not `"../logger.h"` or `"logger.h"`
- **Prefer forward declarations** over `#include` in headers where possible

```cpp
#pragma once

#include "library/sp.h"
#include "library/strings.h"

#include <vector>

namespace OpenApoc
{

class ForwardDeclaredType;

class MyClass
{
private:
	int member = 0;
public:
	void publicFunction();
};

} // namespace OpenApoc
```

## Key Review Expectations

These patterns are consistently enforced during code review:

1. **const correctness** — if something can be const, it must be const
2. **Readable conditionals** — avoid complex conditionals with embedded comments; prefer
   separate early-exit checks (`if (cond) continue;` / `if (cond) return;`)
3. **One logical change per PR** — keep changes focused for clean history and bisectability
4. **Use `auto`** — when the type is already visible on the RHS
5. **Prefer exclusive ownership** — use `up<>` over `sp<>` when shared ownership is not required

## Namespace

All project code lives in `namespace OpenApoc { }`. Namespace content is not indented.

## File-Local Scope

Use anonymous namespaces instead of `static` for file-local functions and classes:
```cpp
namespace
{
void helperFunction()
{
	// ...
}
} // anonymous namespace
```

## CI Pipeline

- **GitHub Actions (cmake.yml)**: Build + test on Ubuntu 24.04 with GCC 14
- **GitHub Actions (lint.yml)**: `clang-format-18` formatting check + `clang-tidy-18` analysis
- **AppVeyor (appveyor.yml)**: Windows build with MSVC 2022
