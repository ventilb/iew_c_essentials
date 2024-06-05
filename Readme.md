# IEW C Essentials

Some utilities I use in my vulkan projects. This is my first C project 
using the really great [JetBrains CLion IDE][2].

## macros.h

Macros used to create typed vectors and vector apis.

## string.h

UTF-8 String utilities. Uses [sheredom/utf8.h][1] under the hood.
Some code from [sheredom/utf8.h][1] was copied and adapted to our
needs.

The intention of the string utilities is to work as natively with
UTF-8 as possible. For example indices passed to `str_substring` 
expect codepoint indices and not byte indices.

## hash.h
Adds FNV32a and FNV64a utilities. Uses [catb0t/fnv-hash][3] library. That 
library is compiled and statically linked into *IEW C Essentials*.

The functions are designed to work on Little-Endian machines.

According to [Wikipedia: Fowler–Noll–Vo hash function][4] and [FNV Hash][5]
the FNV algorithm was put under [CC0 - Public Domain][6]. 

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>


[1]: https://github.com/sheredom/utf8.h
[2]: https://www.jetbrains.com/clion/
[3]: https://github.com/catb0t/fnv-hash
[4]: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#Overview
[5]: http://www.isthe.com/chongo/tech/comp/fnv/index.html#public_domain
[6]: http://www.isthe.com/chongo/tech/comp/fnv/index.html#public_domain