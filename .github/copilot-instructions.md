# Copilot Instructions for OpenApoc

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
cmake -B build -G Ninja          # Configure
cmake --build build              # Build
cd build && ctest                # Run tests
clang-format -i path/to/file.cpp # Format before committing
```

## Directory Structure

```
library/        Core utilities (strings, vectors, sp.h smart pointer aliases)
framework/      Engine core (rendering, sound, filesystem, logging, serialization)
forms/          UI widget system
game/state/     Game logic and state (city, battle, rules)
game/ui/        Game UI screens (city view, battle view, ufopaedia)
tools/          Extractors, editor, launcher, code generators
tests/          CTest unit tests
dependencies/   Git submodules (fmt, glm, lodepng, lua, magic_enum, miniz, physfs, pugixml)
data/           Game data, XML forms, fonts, language files
```

## Code Style

Full guide: `CODE_STYLE.md`. Enforced by `clang-format-18` and `clang-tidy-18`.

### Formatting
- **C++17**, Allman braces (opening brace on its own line), tabs for indentation, 100-column limit
- **Always use braces**, even for single-statement blocks
- **Namespace content is not indented**; closing brace gets `// namespace Name` comment
- **Pointers/references**: Align right (`int *ptr`, `Type &ref`)
- **Line breaks**: Break before operators when wrapping

### Naming
- `CamelCase`: classes, enums, enum class members, namespaces, template parameters
- `camelBack`: methods, member variables, function parameters, local variables
- `SHOUTY_CAPS`: constants, macros
- `lower_case`: labels

### Class Conventions
- `public:`/`private:`/`protected:` at class indent level, not indented into scope
- Always explicitly write `private:`, even though it is the default
- `virtual` only on base class; `override` on derived — never both
- All classes with virtual methods must have a virtual destructor
- Use `= default` instead of empty `{}` constructor/destructor bodies
- Prefer member initializer lists; order must match declaration order
- `struct` for data-only types only; structs must never use access modifiers

### General
- **auto**: Use when the type is obvious from the RHS. Use `auto &` to avoid copies.
- **const**: Apply aggressively to functions, parameters, return types, and locals
- **Early return**: Encouraged
- **Range-for**: `for (const auto &element : container)`
- **No naked `new`** — wrap in smart pointers immediately
- **No C-style casts** — use `static_cast<>`, `reinterpret_cast<>`
- Prefer `{}` brace initialization, `emplace()` over `insert()`, `enum class` over plain `enum`

## Project-Specific Types

- **Strings**: Use `UString` (from `library/strings.h`) for all strings
- **Smart pointers** (`library/sp.h`): `sp<T>`, `up<T>`, `wp<T>`, `mksp<T>()`, `mkup<T>()`. Prefer `up<>` over `sp<>`.
- **Logging** (`framework/logger.h`): `LogInfo("message {0}", arg)` — fmt-style format strings, NOT printf
- **Formatting**: `OpenApoc::format("text {0} {1}", a, b)` from `library/strings_format.h`
- **Translation**: `tr("English text to translate")`

## Include and Header Conventions

- `#pragma once` (no include guards)
- Local headers first (relative to repo root: `"framework/logger.h"`), then system headers. Both blocks alphabetically sorted.
- Prefer forward declarations over `#include` in headers

## Key Review Expectations

1. **const correctness** — if something can be const, it must be const
2. **Readable conditionals** — prefer separate early-exit checks over complex compound conditions
3. **One logical change per PR** — keep changes focused for clean history
4. **Use `auto`** — when the type is already visible on the RHS
5. **Prefer exclusive ownership** — use `up<>` over `sp<>` when shared ownership is not required
6. **Anonymous namespaces** — use instead of `static` for file-local scope

## CI Pipeline

- **GitHub Actions**: Build + test (Ubuntu/GCC 14), `clang-format-18` + `clang-tidy-18` lint
- **AppVeyor**: Windows build with MSVC 2022
