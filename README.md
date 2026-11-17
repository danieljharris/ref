# `Ref`

A small C++23 header-only **shared handle** around `std::shared_ptr<T>` with **no nullable handle** in the public API. Every `Ref<T>` that exists was either constructed with valid constructor arguments or copied from another `Ref`.

**Header:** [`include/ref/ref.hpp`](include/ref/ref.hpp) — `#include <ref/ref.hpp>`

## What it is

- **Create:** `Ref<int> r(42);` — heap-allocates `T` via `make_shared` and forwards constructor arguments.
- **Share:** copy or upcast (`Ref<Base> b = derived_ref;`) — same allocation, refcounted.
- **Use:** `*r`, `r.get()`, `r->` — like a non-null `shared_ptr` wrapper.
- **Not** a `unique_ptr` alternative: move is deleted; copying always shares.

## States the type cannot represent

| State | Representable? |
|-------|----------------|
| “Empty” / no-target `Ref` | **No** — default constructor deleted; no `nullptr` adoption; `operator bool` deleted |
| Construct from `nullptr` | **No** — `nullptr_t` rejected in `Ref(Args...)` |
| Pointer as element type (`Ref<int*>`, `Ref<void*>`, …) | **No** — `static_assert(!std::is_pointer_v<T>)` |
| Construct `T` from another `Ref` via forwarding ctor (ambiguous with share) | **No** — `Ref` arguments must use copy/upcast constructors |
| `Ref<void>`, `Ref<int&>` | **No** — `static_assert` on `T` |

**If it compiles, you have a live handle to a heap `T` object** (the `shared_ptr` control block is non-null after successful construction).

## What compile-time checks actually prove

**Proved (within these rules):**

- You cannot spell a null **handle** with the public API.
- You cannot pass **`nullptr`** into the **forwarding constructor** at compile time.
- You cannot use **pointer types** as `T` (no `Ref<int*>`).
- Sharing vs creating from `Ref` args is separated by overload resolution (`arg_is_ref` gate + copy ctor).

**Not proved:**

- **`T` is “fully owning”** or free of dangling interior pointers (`struct { int* p; }` is still your problem).
- **Deep copy vs share** — `Ref<int> b(a)` shares; use `Ref<int>(*a)` if you want a new `int`.
- **Ref cycles** — same as `shared_ptr`; use `weak_ptr` patterns if needed.

## Requirements

- C++23 (concepts, `std::constructible_from`, etc.)
- Standard library: `<memory>`, `<concepts>`, `<type_traits>`, `<utility>`

## CMake

```cmake
find_package(ref CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE ref::ref)
```

```cpp
#include <ref/ref.hpp>
```

Installed layout: `include/ref/ref.hpp`, `share/cmake/ref/refConfig.cmake`.

## vcpkg

**Overlay port (ships in this repo):**

```bash
git clone https://github.com/danieljharris/ref.git
vcpkg install ref --overlay-ports=ref/ports
```

**Consumer `vcpkg.json`:**

```json
{
  "dependencies": [
    "ref"
  ]
}
```

(Set `VCPKG_OVERLAY_PORTS` to this repo’s `ports` directory, or add it in your toolchain chain file.)

**CMake after install:**

```cmake
find_package(ref CONFIG REQUIRED)
target_link_libraries(app PRIVATE ref::ref)
```

Release tags (`v0.1.0`, …) match `ports/ref/vcpkg.json` version; the portfile downloads the matching tag from GitHub.

## Example

```cpp
#include <ref/ref.hpp>

Ref<int> a(1);
Ref<int> b = a;          // share
Ref<int> c(*a);          // new heap int, value 1

// Ref<int>(nullptr);     // error
// Ref<int*> p(q);        // error — T must not be a pointer
// Ref<int> empty;        // error — no default ctor
```

## License

MIT — see [LICENSE](LICENSE).