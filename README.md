# Bastion

A lightweight, file-agnostic encryption and decryption service. Bastion doesn't care what kind of file you throw at it — text, binary, images, archives, whatever — it reads bytes in, transforms them, and writes bytes out. It is not a wrapper for any specific use case (no built-in "encrypt my photos" or "secure my chat" logic); it's a general-purpose cryptographic engine meant to be built on top of.

## What it does

- Encrypts arbitrary files into ciphertext output
- Decrypts ciphertext back into the original bytes
- Operates purely on byte streams, so it has no knowledge of (or opinion about) file type, extension, or structure
- Exposes simple entry points that other tools/scripts can call into

## What it deliberately does *not* do

- No file-type-specific handling (no "if it's a .jpg, do X")
- No GUI, no cloud sync, no key-sharing service — this is the core crypto engine only
- No assumptions about how you store or transmit the resulting ciphertext

## Design philosophy

Bastion is written in a **procedural, non-object-oriented style** on purpose. Cryptographic operations are naturally a pipeline of discrete, stateless steps (read input → pad/chunk → transform → write output), and procedural code keeps that pipeline easy to trace, test, and audit line-by-line. There's no hidden state tucked inside objects, no inheritance chains to trace through to figure out what a function actually does. Every function does one thing, takes explicit inputs, and returns explicit outputs.

Some concrete conventions that fall out of this — including in a language like C++ that would otherwise nudge you toward classes and inheritance:

- **No classes with attached behavior.** Where C++ features are used (namespaces, `struct`, RAII for resource cleanup), they're used for organization and safety — not to model behavior as objects. Structs hold data only; they don't have member functions beyond maybe a constructor/destructor for resource management.
- **No inheritance or polymorphism.** No virtual functions, no class hierarchies. If two operations share logic, that logic gets pulled into its own free function, not inherited from a base type.
- **Free functions over methods.** Functions live in their own translation unit grouped by responsibility, take their inputs explicitly, and operate on data passed in — not on `this`.
- **Explicit state.** Nothing is a hidden instance variable. If a function needs something, it's passed in as a parameter.
- **Pure-ish functions where possible.** Given the same input and key material, a function should produce the same output — makes unit testing straightforward and makes side effects easy to spot when they do exist (e.g. writing to disk).
- **RAII is fine, "objects" are not.** It's fine to use a destructor to zero out sensitive memory or close a file handle — that's resource safety, not object modeling. The line is: does this type have *behavior*, or does it just clean up after itself?

## Project structure — one microprocess per file

Rather than one large monolithic source file, each discrete step in the encryption/decryption pipeline lives in its own translation unit. This keeps each unit small enough to reason about (and test) in isolation, and makes it obvious where to look when something in the pipeline breaks.

```
/src
  io.cpp / io.hpp                 — reading input files and writing output files as raw byte buffers
  keygen.cpp / keygen.hpp         — key generation and derivation
  padding.cpp / padding.hpp       — block padding / unpadding
  chunking.cpp / chunking.hpp     — splitting input into fixed-size blocks for block-based ciphers
  cipher_core.cpp / .hpp          — the actual encrypt/decrypt transform logic
  mac.cpp / mac.hpp               — message authentication / integrity checking
  errors.cpp / errors.hpp         — centralized error codes and error handling
  main.cpp                        — glues the above together into a single CLI entry point
/tests
  test_io.cpp
  test_padding.cpp
  test_chunking.cpp
  test_cipher_core.cpp
  test_mac.cpp
CMakeLists.txt
```

Guidelines for adding new functionality:

- If you're adding a new step to the pipeline (e.g. a new padding scheme, a new key derivation function), give it its own `.cpp`/`.hpp` pair rather than appending to an existing one.
- Each file should expose a small, well-documented set of free functions via its header — internal helpers stay in an anonymous namespace and don't leak into the public interface.
- Files should not reach into each other's internals. If `cipher_core` needs padded data, it calls into `padding`'s public functions — it doesn't duplicate padding logic itself.
- Keep `main` thin. It should read like a table of contents for the pipeline (read → derive key → pad → transform → write), not contain actual cryptographic logic.

## Building

```bash
cmake -B build
cmake --build build
```

## Running

```bash
./bastion encrypt --in plaintext.bin --out ciphertext.bin --key <keyfile>
./bastion decrypt --in ciphertext.bin --out plaintext.bin --key <keyfile>
```

## Contributing

- Match the existing procedural style — no new classes/OOP patterns, even in languages that support them.
- New pipeline steps get new files, not additions to existing ones (see Project Structure above).
- Add a corresponding test file under `/tests` for any new module.
- Keep functions small and single-purpose; if a function is doing more than one conceptual step, split it.

## License

GPL 2.0
