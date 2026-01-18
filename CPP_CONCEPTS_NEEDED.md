# C++ Concepts You Need to Know for This Codebase

This document lists all C++ concepts used in the Minnow codebase, organized by priority and complexity.

## 🔴 ESSENTIAL - Must Know Before Starting

### 1. Basic C++ Syntax
- **Comments**: `//` and `/* */`
- **Semicolons**: End of statements
- **Braces**: `{}` for blocks
- **Preprocessor directives**: `#include`, `#pragma once`

### 2. Data Types
- **Primitive types**: `int`, `bool`, `char`
- **Fixed-width integers**: `uint64_t`, `size_t` (from `<cstdint>`)
- **Standard library types**: `std::string`, `std::vector`
- **Type aliases**: Understanding what `uint64_t` means (unsigned 64-bit integer)

### 3. Variables and Constants
- **Variable declaration**: `int x;` or `int x = 5;`
- **Const variables**: `const int x = 10;`
- **Initialization**: `bool error_ {};` (brace initialization, sets to false)

### 4. Functions
- **Function declaration vs definition**
- **Return types**: `void`, `bool`, `uint64_t`, `std::string_view`
- **Parameters**: Pass by value `void push(std::string data)`
- **Function calls**: `object.method()` or `function(arg)`

### 5. Classes and Objects
- **Class definition**: `class ByteStream { ... };`
- **Public vs Private vs Protected**: Access modifiers
- **Member variables**: `uint64_t capacity_;`
- **Member functions**: `void push(std::string data);`
- **Object creation**: `ByteStream bs(15);` (constructor call)
- **Member access**: `bs.writer().push("hello");`

### 6. Constructors
- **Constructor syntax**: `ByteStream(uint64_t capacity) : capacity_(capacity) {}`
- **Initializer lists**: `: capacity_(capacity)` (initialize member variables)
- **Explicit constructors**: `explicit ByteStream(...)` (prevents implicit conversion)

### 7. Const Correctness
- **Const member functions**: `bool is_closed() const;` (doesn't modify object)
- **Const objects**: `const Reader& reader() const;`
- **Const parameters**: `void insert(uint64_t first_index, std::string data, bool is_last_substring);`

### 8. References
- **Reference parameters**: `Reader& reader()` (returns a reference, not a copy)
- **Const references**: `const Reader& reader() const;` (read-only reference)
- **Why references**: Avoid copying, allow modification

### 9. Standard Library Containers
- **`std::string`**: Mutable string type
  - Methods: `.size()`, `.empty()`, `.substr()`, etc.
- **`std::vector<T>`**: Dynamic array
  - Methods: `.push_back()`, `.size()`, `.empty()`, `.clear()`, `.reserve()`
  - Access: `vector[i]`, `vector.at(i)`
- **`std::optional<T>`**: May or may not contain a value (C++17)

### 10. Standard Library Utilities
- **`std::string_view`**: Lightweight read-only string reference (C++17)
  - Doesn't own memory, just points to it
  - Methods: `.size()`, `.empty()`, `.data()`
- **`std::move()`**: Transfer ownership (move semantics)
- **`std::to_string()`**: Convert number to string

### 11. Header Files and Includes
- **Header guards**: `#pragma once` (prevents multiple includes)
- **Include syntax**: `#include "local.hh"` vs `#include <system>`
- **Forward declarations**: `class Reader;` (declare without defining)

### 12. Namespaces
- **`std::` prefix**: `std::string`, `std::vector`
- **`using namespace std;`**: Avoid typing `std::` everywhere
- **Namespace scope**: Understanding where names come from

---

## 🟡 IMPORTANT - Learn Early

### 13. Inheritance
- **Base class**: `class ByteStream { ... };`
- **Derived class**: `class Writer : public ByteStream { ... };`
- **Public inheritance**: `: public ByteStream`
- **Accessing base members**: Inherited members are accessible
- **Polymorphism**: Understanding that `Writer` IS-A `ByteStream`

### 14. Member Initialization
- **In-class initialization**: `bool error_ {};` (initializes to false/zero)
- **Constructor initializer list**: `: capacity_(capacity), error_(false) {}`
- **Default initialization**: `{}` means zero-initialize

### 15. Function Overloading
- **Same name, different parameters**: 
  ```cpp
  Reader& reader();
  const Reader& reader() const;  // const version
  ```

### 16. Default Parameters
- **Optional parameters**: `void listen(int backlog = 16);`
- **Calling**: `listen()` uses 16, `listen(32)` uses 32

### 17. Return Types
- **Return by value**: `std::string peek() const;` (returns a copy)
- **Return by reference**: `Reader& reader();` (returns reference, no copy)
- **Return by const reference**: `const Reader& reader() const;`

### 18. Standard Library Algorithms (Basic)
- **Range-based for loops**: `for (const auto& buf : buffers) { ... }`
- **Iterators**: Understanding that containers can be iterated

### 19. Error Handling Basics
- **Boolean returns**: `bool is_closed() const;` (returns true/false)
- **Error flags**: `bool error_ {};` (track error state)

### 20. Memory Management Basics
- **Stack allocation**: `ByteStream bs(15);` (automatic cleanup)
- **No manual `new`/`delete`**: Modern C++ avoids raw pointers
- **RAII**: Resources are cleaned up automatically

---

## 🟢 INTERMEDIATE - Learn as You Go

### 21. Move Semantics
- **R-value references**: `ByteStream&& output` (temporary object)
- **`std::move()`**: `std::move(output)` (transfer ownership)
- **Move constructor**: `output_(std::move(output))`
- **Why it matters**: Efficient transfer without copying

### 22. Templates (Basic Understanding)
- **Template syntax**: `template<typename option_type>`
- **Template functions**: 
  ```cpp
  template<typename option_type>
  void setsockopt(int level, int option, const option_type& option_value);
  ```
- **Type deduction**: Compiler figures out the type

### 23. Concepts (C++20)
- **Concept definition**: 
  ```cpp
  template<typename R>
  concept StringViewRange = std::ranges::sized_range<R> && ...;
  ```
- **Concept usage**: `const StringViewRange auto&& payloads`
- **What it does**: Constrains template parameters to specific types

### 24. Lambda Functions (If Used)
- **Basic syntax**: `[capture](params) { body }`
- **Used with**: `std::function`, algorithms

### 25. Smart Pointers
- **`std::shared_ptr<T>`**: Reference-counted pointer
  - Example: `std::shared_ptr<FDWrapper> internal_fd_;`
  - Automatically deletes when last reference is gone
- **Why used**: Automatic memory management, no leaks

### 26. Static Members
- **Static variables**: `static constexpr size_t kReadBufferSize = 16384;`
- **Static functions**: Belong to class, not instance
- **`constexpr`**: Computed at compile time

### 27. Deleted Functions
- **Delete syntax**: `FileDescriptor(const FileDescriptor& other) = delete;`
- **Purpose**: Prevent copying/moving when not desired

### 28. Defaulted Functions
- **Default syntax**: `FileDescriptor(FileDescriptor&& other) = default;`
- **Purpose**: Use compiler-generated version

### 29. Static Assertions
- **`static_assert`**: 
  ```cpp
  static_assert(sizeof(Reader) == sizeof(ByteStream), "message");
  ```
- **Purpose**: Compile-time checks

### 30. Exception Handling (Basic)
- **`throw`**: `throw std::runtime_error("message");`
- **When used**: Error conditions that can't be ignored

---

## 🔵 ADVANCED - Reference Only (You'll See But Not Write)

### 31. Advanced Templates
- **Template specialization**: Custom behavior for specific types
- **SFINAE**: Substitution Failure Is Not An Error

### 32. Perfect Forwarding
- **`auto&&`**: Universal reference
- **Forwarding references**: `const StringViewRange auto&& payloads`

### 33. Thread-Local Storage
- **`thread_local`**: 
  ```cpp
  static thread_local std::vector<iovec> iovecs;
  ```
- **Purpose**: One copy per thread

### 34. Ranges (C++20)
- **`std::ranges::sized_range`**: Concept for ranges with known size
- **Range-based operations**: Modern iteration

### 35. Type Traits
- **`std::is_constructible_v`**: Check if type can be constructed
- **`std::ranges::range_value_t`**: Get element type of range

---

## 📚 Learning Resources by Topic

### For Beginners
1. **C++ Basics**: Variables, functions, classes
   - [Learn C++](https://www.learncpp.com/) - Free online tutorial
   - Focus on chapters 1-10

2. **Standard Library**: `std::string`, `std::vector`
   - [cppreference.com](https://cppreference.com/) - Complete reference
   - Look up specific classes as needed

3. **Object-Oriented Programming**: Classes, inheritance
   - Learn C++ chapters 11-15

### For Intermediate Topics
4. **Move Semantics**: 
   - [C++ Move Semantics Explained](https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners)
   - Focus on understanding `std::move()` and r-value references

5. **Templates**: 
   - Learn C++ chapters 18-19
   - You mainly need to *read* templates, not write them

6. **Smart Pointers**:
   - Learn C++ chapter 22
   - Focus on `std::shared_ptr`

### Quick Reference
- **cppreference.com**: Official C++ reference
- **cplusplus.com**: Alternative reference with examples
- **Stack Overflow**: For specific questions

---

## 🎯 Priority Learning Path

### Week 1: Get Started
1. ✅ Basic syntax (variables, functions, classes)
2. ✅ `std::string` and `std::vector`
3. ✅ References (`&`) and const
4. ✅ Constructors and member initialization

### Week 2: Core Concepts
5. ✅ Inheritance (`: public Base`)
6. ✅ `std::string_view` (read-only string reference)
7. ✅ Function overloading
8. ✅ Return types (value vs reference)

### Week 3: Advanced Basics
9. ✅ Move semantics (`std::move`, `&&`)
10. ✅ Smart pointers (`std::shared_ptr`) - reading only
11. ✅ Templates - reading only
12. ✅ Concepts - reading only

### As Needed
- Everything else is reference material
- Look it up when you encounter it

---

## 💡 Key Takeaways

1. **You don't need to master everything** - Focus on the 🔴 Essential topics first
2. **Learn by doing** - Start coding, look up concepts as you encounter them
3. **The codebase uses modern C++** - C++17/C++20 features, but you can learn incrementally
4. **Most concepts are about efficiency** - Move semantics, references, etc. avoid unnecessary copying
5. **Tests are your friend** - They show you how things are used

---

## 🔍 How to Use This List

1. **Before starting**: Review all 🔴 Essential concepts
2. **While coding**: Reference 🟡 Important concepts as needed
3. **When confused**: Look up specific concepts in 🟢 Intermediate
4. **Advanced features**: Don't worry about 🔵 Advanced - you'll see them but not write them

Remember: **You can learn C++ while working on this project!** Start with the basics, implement simple code, and gradually learn more advanced concepts as you encounter them.

