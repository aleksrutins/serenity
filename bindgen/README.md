# Serenity Lua binding generator

`bindgen.py` reads a C++-oriented binding definition and writes a C++ header containing sol2 registration code.

```sh
python3 bindgen/bindgen.py runtime/types/math.idl -o /tmp/math.hpp
```

The runtime build runs the generator separately for each IDL in `runtime/types/` and includes the matching generated header. Edit the IDL rather than the generated file.

## Supported declarations

- `include <header>;` or `include "header";` emits a C++ include in the generated header.
- `using namespace ...;` emits a C++ using directive.
- `namespace name { ... }` creates a Lua table. `Type name(Args...) -> cpp_function;` binds a function with the declared signature; repeated names become a sol2 overload set. `name -> cpp_value;` binds a value or callable expression.
- `global { ... }` binds functions and values directly on the Lua state, also combining repeated function names into overload sets.
- `opaque class Name;` registers a class with no members.
- `class Name [(cpp::Type)] [: Base, ...] { ... }` registers a class, its Lua base classes, and its C++ type. The optional parenthesized type defaults to `Name`.
- `new(Args...);` creates a factory constructor. Multiple constructors are combined into an overload set.
- `ReturnType method(Args...) [-> cpp_method];` binds a method. The C++ method defaults to the declared name. `static name -> cpp_expression;` adds a class-level field after registration.
- `factory name(Args...) -> cpp_factory;` binds a C++ factory method through `sol::factories`.

Argument names are optional because the generator creates lambda parameter names for constructors. Types and C++ expressions are copied into the output, so they must be valid in the including translation unit. The runtime's math, core, graphics, 2D, and input IDL files generate corresponding headers in `runtime/types/` during the Bazel build.
