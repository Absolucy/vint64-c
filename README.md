# vint64-c

This is a C port of the [vint64](https://github.com/iqlusioninc/veriform/tree/develop/rust/vint64) algorithm for highly efficient variable-length integers without any loops[^1].

## Licensing

This project was inspired by the original [vint64 code](https://github.com/iqlusioninc/veriform/tree/develop/rust/vint64) by [Tony Arcieri](https://github.com/tony-iqlusion), which is licensed under the [Apache license](https://github.com/iqlusioninc/veriform/blob/develop/LICENSE).

This library is distributed under the terms of both the MIT license and the Apache License (Version 2.0).

See [LICENSE-Apache.md](LICENSE-Apache.md) and [LICENSE-MIT.md](LICENSE-MIT.md) for details.

[^1]: On some lesser-used platforms and compilers, loops may be used when encoding due to a lack of available intrinsics. This should not be a problem on gcc, clang, or msvc with any modern platform, though.
