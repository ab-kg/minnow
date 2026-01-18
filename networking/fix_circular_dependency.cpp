// Fix for Circular Dependency in ByteStream
// This shows what's wrong and how to fix it

#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

// ============================================================================
// ❌ WRONG DESIGN (What you currently have)
// ============================================================================

/*
class ByteStream {
protected:
    std::string buffer;
    Reader myreader;   // ❌ PROBLEM: Needs full Reader definition
    Writer mywriter;   // ❌ PROBLEM: Needs full Writer definition
    uint64_t capacity_;
};

class Writer : public ByteStream { ... };  // Needs ByteStream definition
class Reader : public ByteStream { ... };   // Needs ByteStream definition

CIRCULAR DEPENDENCY:
  ByteStream → needs Reader → needs ByteStream → needs Reader → ...
  ByteStream → needs Writer → needs ByteStream → needs Writer → ...
*/

// ============================================================================
// ✅ CORRECT DESIGN (What it should be)
// ============================================================================

// Forward declarations
class Reader;
class Writer;

class ByteStream {
protected:
    // ✅ All state goes here - NO Reader/Writer member variables!
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;
    bool error_ = false;

public:
    explicit ByteStream(uint64_t capacity) : capacity_(capacity) {}
    
    // ✅ These methods cast *this, they don't return member variables
    Reader& reader();
    const Reader& reader() const;
    Writer& writer();
    const Writer& writer() const;
    
    void set_error() { error_ = true; }
    bool has_error() const { return error_; }
};

// Writer inherits from ByteStream
// A Writer object IS-A ByteStream (not HAS-A ByteStream)
class Writer : public ByteStream {
public:
    explicit Writer(uint64_t capacity) : ByteStream(capacity) {}
    
    void push(std::string data) {
        uint64_t available = capacity_ - buffer_.size();
        if (data.size() > available) {
            data = data.substr(0, available);
        }
        buffer_ += data;
        bytes_pushed_ += data.size();
    }
    
    void close() { closed_ = true; }
    bool is_closed() const { return closed_; }
    uint64_t available_capacity() const { return capacity_ - buffer_.size(); }
    uint64_t bytes_pushed() const { return bytes_pushed_; }
};

// Reader inherits from ByteStream
// A Reader object IS-A ByteStream (not HAS-A ByteStream)
class Reader : public ByteStream {
public:
    explicit Reader(uint64_t capacity) : ByteStream(capacity) {}
    
    std::string_view peek() const {
        return std::string_view(buffer_);
    }
    
    void pop(uint64_t len) {
        if (len > buffer_.size()) len = buffer_.size();
        buffer_.erase(0, len);
        bytes_popped_ += len;
    }
    
    bool is_finished() const { return closed_ && buffer_.empty(); }
    uint64_t bytes_buffered() const { return buffer_.size(); }
    uint64_t bytes_popped() const { return bytes_popped_; }
};

// ✅ The magic: Cast *this to Reader*/Writer*
// This works because Reader/Writer inherit from ByteStream
Reader& ByteStream::reader() {
    // Ensure Reader doesn't add members (would break the cast)
    static_assert(sizeof(Reader) == sizeof(ByteStream),
                  "Reader must not add member variables!");
    return static_cast<Reader&>(*this);
}

const Reader& ByteStream::reader() const {
    static_assert(sizeof(Reader) == sizeof(ByteStream),
                  "Reader must not add member variables!");
    return static_cast<const Reader&>(*this);
}

Writer& ByteStream::writer() {
    static_assert(sizeof(Writer) == sizeof(ByteStream),
                  "Writer must not add member variables!");
    return static_cast<Writer&>(*this);
}

const Writer& ByteStream::writer() const {
    static_assert(sizeof(Writer) == sizeof(ByteStream),
                  "Writer must not add member variables!");
    return static_cast<const Writer&>(*this);
}


// ============================================================================
// DEMONSTRATION
// ============================================================================

int main() {
    std::cout << "=== Fixing Circular Dependency ===\n\n";
    
    std::cout << "THE PROBLEM:\n";
    std::cout << "  ByteStream has: Reader myreader; Writer mywriter;\n";
    std::cout << "  But Reader and Writer inherit from ByteStream\n";
    std::cout << "  → Circular dependency!\n\n";
    
    std::cout << "THE SOLUTION:\n";
    std::cout << "  ❌ Remove: Reader myreader; Writer mywriter;\n";
    std::cout << "  ✅ Use: static_cast<Reader&>(*this)\n";
    std::cout << "  ✅ Use: static_cast<Writer&>(*this)\n\n";
    
    std::cout << "HOW IT WORKS:\n";
    std::cout << "  1. ByteStream object IS-A Reader (through inheritance)\n";
    std::cout << "  2. ByteStream object IS-A Writer (through inheritance)\n";
    std::cout << "  3. Cast *this to Reader& or Writer&\n";
    std::cout << "  4. No separate objects needed!\n\n";
    
    // Create a ByteStream
    ByteStream bs(10);
    
    // Get Writer interface (casts bs to Writer&)
    Writer& w = bs.writer();
    w.push("Hello");
    std::cout << "Pushed 'Hello' via writer()\n";
    std::cout << "Bytes pushed: " << w.bytes_pushed() << "\n\n";
    
    // Get Reader interface (casts bs to Reader&)
    Reader& r = bs.reader();
    std::cout << "Peek via reader(): \"" << r.peek() << "\"\n";
    std::cout << "Bytes buffered: " << r.bytes_buffered() << "\n\n";
    
    std::cout << "KEY INSIGHT:\n";
    std::cout << "  bs, w, and r all refer to the SAME object!\n";
    std::cout << "  They share the same buffer_, capacity_, etc.\n";
    std::cout << "  No circular dependency because no member variables!\n";
    
    return 0;
}

