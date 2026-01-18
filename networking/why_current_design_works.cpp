// Why the Current Design Works (and why adding Reader/Writer members breaks it)

#include <iostream>
#include <string>
#include <cstdint>
#include <cassert>

// Forward declarations
class Reader;
class Writer;

// CORRECT DESIGN (what the codebase should use)
class ByteStream {
protected:
    // All state goes here - Reader and Writer share this
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;

public:
    explicit ByteStream(uint64_t capacity) : capacity_(capacity) {}
    
    // These methods use static_cast - no member variables needed!
    Reader& reader();
    const Reader& reader() const;
    Writer& writer();
    const Writer& writer() const;
};

// Writer inherits from ByteStream
// A Writer object IS-A ByteStream
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
// A Reader object IS-A ByteStream
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

// The magic: static_cast converts ByteStream* to Reader*/Writer*
// This ONLY works if Reader and Writer have the same size as ByteStream
// (i.e., they don't add any member variables)
Reader& ByteStream::reader() {
    // Assert that sizes match (prevents adding members to Reader)
    static_assert(sizeof(Reader) == sizeof(ByteStream),
                  "Reader must not add member variables!");
    
    // Cast this ByteStream* to Reader*
    // This is safe because Reader inherits from ByteStream
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
// DEMONSTRATION: How it works
// ============================================================================

int main() {
    std::cout << "=== Understanding the Current Design ===\n\n";
    
    std::cout << "1. Creating a ByteStream:\n";
    ByteStream bs(10);
    std::cout << "   ByteStream created with capacity 10\n\n";
    
    std::cout << "2. Getting Writer interface:\n";
    Writer& w = bs.writer();
    w.push("Hello");
    std::cout << "   Pushed 'Hello' via writer()\n";
    std::cout << "   Bytes pushed: " << w.bytes_pushed() << "\n\n";
    
    std::cout << "3. Getting Reader interface:\n";
    Reader& r = bs.reader();
    std::cout << "   Peek: \"" << r.peek() << "\"\n";
    std::cout << "   Bytes buffered: " << r.bytes_buffered() << "\n\n";
    
    std::cout << "4. The Key Insight:\n";
    std::cout << "   - bs, w, and r all refer to the SAME object!\n";
    std::cout << "   - writer() and reader() just cast the same ByteStream\n";
    std::cout << "   - No separate Reader/Writer objects exist\n";
    std::cout << "   - All share the same buffer_, capacity_, etc.\n\n";
    
    std::cout << "5. Why this works:\n";
    std::cout << "   - Reader and Writer inherit from ByteStream\n";
    std::cout << "   - They don't add any member variables\n";
    std::cout << "   - static_cast safely converts ByteStream* to Reader*/Writer*\n";
    std::cout << "   - static_assert ensures sizes match\n\n";
    
    std::cout << "6. What BREAKS this design:\n";
    std::cout << "   ❌ Adding Reader myreader; as member variable\n";
    std::cout << "   ❌ Adding Writer mywriter; as member variable\n";
    std::cout << "   ❌ Adding member variables to Reader or Writer\n";
    std::cout << "   These create circular dependencies or break the cast!\n";
    
    return 0;
}

