// Circular Dependency Explained - Simple Working Examples

#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

// ============================================================================
// PROBLEM: What happens if you add Reader/Writer as member variables
// ============================================================================

/*
 * THE CIRCULAR DEPENDENCY PROBLEM:
 * 
 * If ByteStream has:
 *   Reader myreader;   // Needs full Reader definition
 *   Writer mywriter;   // Needs full Writer definition
 * 
 * But Reader and Writer inherit from ByteStream:
 *   class Reader : public ByteStream { ... }
 *   class Writer : public ByteStream { ... }
 * 
 * This creates:
 *   ByteStream → needs Reader → needs ByteStream → needs Reader → ...
 *   ByteStream → needs Writer → needs ByteStream → needs Writer → ...
 * 
 * The compiler can't resolve this!
 */

// ============================================================================
// SOLUTION 1: Composition (Reader/Writer are separate objects)
// ============================================================================

class ByteStream_Composition {
private:
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;

public:
    explicit ByteStream_Composition(uint64_t cap) : capacity_(cap) {}
    
    // Access shared state
    std::string& get_buffer() { return buffer_; }
    uint64_t get_capacity() const { return capacity_; }
    bool is_closed() const { return closed_; }
    void set_closed(bool c) { closed_ = c; }
    uint64_t& get_bytes_pushed() { return bytes_pushed_; }
    uint64_t& get_bytes_popped() { return bytes_popped_; }
};

// Reader is a separate class that uses ByteStream
class Reader_Separate {
private:
    ByteStream_Composition* stream_;
    
public:
    Reader_Separate(ByteStream_Composition* s) : stream_(s) {}
    
    std::string_view peek() const {
        return std::string_view(stream_->get_buffer());
    }
    
    void pop(uint64_t len) {
        auto& buf = stream_->get_buffer();
        if (len > buf.size()) len = buf.size();
        buf.erase(0, len);
        stream_->get_bytes_popped() += len;
    }
    
    uint64_t bytes_buffered() const {
        return stream_->get_buffer().size();
    }
};

// Writer is a separate class that uses ByteStream
class Writer_Separate {
private:
    ByteStream_Composition* stream_;
    
public:
    Writer_Separate(ByteStream_Composition* s) : stream_(s) {}
    
    void push(std::string data) {
        uint64_t available = stream_->get_capacity() - stream_->get_buffer().size();
        if (data.size() > available) {
            data = data.substr(0, available);
        }
        stream_->get_buffer() += data;
        stream_->get_bytes_pushed() += data.size();
    }
    
    void close() {
        stream_->set_closed(true);
    }
    
    uint64_t available_capacity() const {
        return stream_->get_capacity() - stream_->get_buffer().size();
    }
};

// ByteStream contains Reader and Writer objects
class ByteStream_WithComposition {
private:
    ByteStream_Composition core_;
    Reader_Separate reader_;
    Writer_Separate writer_;

public:
    explicit ByteStream_WithComposition(uint64_t cap) 
        : core_(cap), reader_(&core_), writer_(&core_) {}
    
    Reader_Separate& reader() { return reader_; }
    Writer_Separate& writer() { return writer_; }
};


// ============================================================================
// SOLUTION 2: Inheritance with Casting (What codebase actually uses)
// ============================================================================

// Forward declarations
class Reader_Cast;
class Writer_Cast;

class ByteStream_Cast {
protected:
    // All state here - shared by Reader and Writer
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;

public:
    explicit ByteStream_Cast(uint64_t cap) : capacity_(cap) {}
    
    // NO member variables of Reader/Writer type!
    // Instead, cast *this to Reader*/Writer*
    Reader_Cast& reader();
    Writer_Cast& writer();
};

// Writer inherits from ByteStream
// A Writer object IS-A ByteStream (not HAS-A)
class Writer_Cast : public ByteStream_Cast {
public:
    explicit Writer_Cast(uint64_t cap) : ByteStream_Cast(cap) {}
    
    void push(std::string data) {
        uint64_t available = capacity_ - buffer_.size();
        if (data.size() > available) {
            data = data.substr(0, available);
        }
        buffer_ += data;
        bytes_pushed_ += data.size();
    }
    
    void close() { closed_ = true; }
    uint64_t available_capacity() const { return capacity_ - buffer_.size(); }
};

// Reader inherits from ByteStream
// A Reader object IS-A ByteStream (not HAS-A)
class Reader_Cast : public ByteStream_Cast {
public:
    explicit Reader_Cast(uint64_t cap) : ByteStream_Cast(cap) {}
    
    std::string_view peek() const {
        return std::string_view(buffer_);
    }
    
    void pop(uint64_t len) {
        if (len > buffer_.size()) len = buffer_.size();
        buffer_.erase(0, len);
        bytes_popped_ += len;
    }
    
    uint64_t bytes_buffered() const {
        return buffer_.size();
    }
};

// The cast: ByteStream* → Reader*/Writer*
// Works because Reader/Writer inherit from ByteStream
Reader_Cast& ByteStream_Cast::reader() {
    // Ensure Reader doesn't add members (would break cast)
    static_assert(sizeof(Reader_Cast) == sizeof(ByteStream_Cast),
                  "Reader must not add member variables!");
    return static_cast<Reader_Cast&>(*this);
}

Writer_Cast& ByteStream_Cast::writer() {
    static_assert(sizeof(Writer_Cast) == sizeof(ByteStream_Cast),
                  "Writer must not add member variables!");
    return static_cast<Writer_Cast&>(*this);
}


// ============================================================================
// DEMONSTRATION
// ============================================================================

int main() {
    std::cout << "=== Circular Dependency Solutions ===\n\n";
    
    std::cout << "SOLUTION 1: Composition Pattern\n";
    std::cout << "  ByteStream CONTAINS Reader and Writer objects\n";
    std::cout << "  - No circular dependency\n";
    std::cout << "  - More memory (3 objects instead of 1)\n";
    std::cout << "  - More flexible\n\n";
    
    ByteStream_WithComposition bs1(10);
    bs1.writer().push("Hello");
    std::cout << "  Pushed via writer: \"" << bs1.reader().peek() << "\"\n";
    std::cout << "  Buffered: " << bs1.reader().bytes_buffered() << "\n\n";
    
    std::cout << "SOLUTION 2: Inheritance with Casting (Codebase design)\n";
    std::cout << "  ByteStream IS-A Reader AND Writer (through inheritance)\n";
    std::cout << "  - No circular dependency\n";
    std::cout << "  - Single object, efficient\n";
    std::cout << "  - Uses static_cast to convert types\n";
    std::cout << "  - Requires Reader/Writer have same size as ByteStream\n\n";
    
    ByteStream_Cast bs2(10);
    bs2.writer().push("World");
    std::cout << "  Pushed via writer: \"" << bs2.reader().peek() << "\"\n";
    std::cout << "  Buffered: " << bs2.reader().bytes_buffered() << "\n\n";
    
    std::cout << "KEY DIFFERENCE:\n";
    std::cout << "  Solution 1: ByteStream HAS Reader and Writer\n";
    std::cout << "  Solution 2: ByteStream IS Reader and Writer\n";
    std::cout << "  (The codebase uses Solution 2)\n\n";
    
    std::cout << "WHY Solution 2 works:\n";
    std::cout << "  - No member variables of Reader/Writer type\n";
    std::cout << "  - Uses inheritance + casting instead\n";
    std::cout << "  - static_assert ensures sizes match\n";
    std::cout << "  - Single object, shared state\n";
    
    return 0;
}

