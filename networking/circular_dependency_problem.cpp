// CIRCULAR DEPENDENCY PROBLEM
// This demonstrates the design issue you identified

#include <iostream>
#include <string>
#include <string_view>

// Forward declarations
class Reader;
class Writer;

// PROBLEM DESIGN - Creates circular dependency
class ByteStream_BAD {
public:
    explicit ByteStream_BAD(uint64_t capacity) : capacity_(capacity) {}
    
    // ByteStream needs full definition of Reader/Writer here
    // because it has them as member variables
    Reader myreader;   // ❌ ERROR: Incomplete type!
    Writer mywriter;   // ❌ ERROR: Incomplete type!
    
    uint64_t capacity_;
};

// But Reader needs ByteStream's full definition to inherit
class Reader_BAD : public ByteStream_BAD {
public:
    std::string_view peek() const { return {}; }
    void pop(uint64_t len) {}
};

// And Writer needs ByteStream's full definition to inherit  
class Writer_BAD : public ByteStream_BAD {
public:
    void push(std::string data) {}
    void close() {}
};

/*
 * THE PROBLEM:
 * 
 * 1. ByteStream_BAD needs Reader and Writer to be fully defined
 *    (because it has them as member variables)
 * 
 * 2. But Reader_BAD and Writer_BAD need ByteStream_BAD to be fully defined
 *    (because they inherit from it)
 * 
 * 3. This creates a CIRCULAR DEPENDENCY:
 *    ByteStream → needs Reader/Writer → need ByteStream → needs Reader/Writer → ...
 * 
 * This won't compile! The compiler can't resolve this circular dependency.
 */

int main() {
    std::cout << "This code demonstrates the circular dependency problem.\n";
    std::cout << "If you try to compile this, you'll get errors like:\n";
    std::cout << "  - 'Reader' was not declared in this scope\n";
    std::cout << "  - incomplete type 'Reader' used in nested name specifier\n";
    std::cout << "  - field 'myreader' has incomplete type\n";
    return 0;
}

