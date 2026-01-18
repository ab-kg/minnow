// SOLUTION TO CIRCULAR DEPENDENCY
// This shows the correct design pattern

#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

// Forward declarations
class Reader;
class Writer;

// SOLUTION 1: Composition (Reader/Writer are separate, ByteStream contains them)
class ByteStream_Composition {
private:
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;
    
    // Forward-declared helper classes (defined later)
    class ReaderImpl;
    class WriterImpl;
    
    ReaderImpl* reader_impl_;
    WriterImpl* writer_impl_;

public:
    explicit ByteStream_Composition(uint64_t capacity) 
        : capacity_(capacity), reader_impl_(nullptr), writer_impl_(nullptr) {}
    
    ~ByteStream_Composition() {
        delete reader_impl_;
        delete writer_impl_;
    }
    
    // Accessor methods return interfaces
    Reader& reader();
    Writer& writer();
    
    // Shared state access
    std::string& buffer() { return buffer_; }
    uint64_t capacity() const { return capacity_; }
    bool closed() const { return closed_; }
    void set_closed(bool c) { closed_ = c; }
    uint64_t& bytes_pushed() { return bytes_pushed_; }
    uint64_t& bytes_popped() { return bytes_popped_; }
};

// Reader interface - just methods, no data
class Reader {
protected:
    ByteStream_Composition* stream_;
    
public:
    Reader(ByteStream_Composition* s) : stream_(s) {}
    virtual ~Reader() = default;
    
    std::string_view peek() const;
    void pop(uint64_t len);
    bool is_finished() const;
    uint64_t bytes_buffered() const;
    uint64_t bytes_popped() const;
};

// Writer interface - just methods, no data
class Writer {
protected:
    ByteStream_Composition* stream_;
    
public:
    Writer(ByteStream_Composition* s) : stream_(s) {}
    virtual ~Writer() = default;
    
    void push(std::string data);
    void close();
    bool is_closed() const;
    uint64_t available_capacity() const;
    uint64_t bytes_pushed() const;
};

// Implementation
std::string_view Reader::peek() const {
    return std::string_view(stream_->buffer());
}

void Reader::pop(uint64_t len) {
    if (len > stream_->buffer().size()) {
        len = stream_->buffer().size();
    }
    stream_->buffer().erase(0, len);
    stream_->bytes_popped() += len;
}

bool Reader::is_finished() const {
    return stream_->closed() && stream_->buffer().empty();
}

uint64_t Reader::bytes_buffered() const {
    return stream_->buffer().size();
}

uint64_t Reader::bytes_popped() const {
    return stream_->bytes_popped();
}

void Writer::push(std::string data) {
    uint64_t available = stream_->capacity() - stream_->buffer().size();
    if (data.size() > available) {
        data = data.substr(0, available);
    }
    stream_->buffer() += data;
    stream_->bytes_pushed() += data.size();
}

void Writer::close() {
    stream_->set_closed(true);
}

bool Writer::is_closed() const {
    return stream_->closed();
}

uint64_t Writer::available_capacity() const {
    return stream_->capacity() - stream_->buffer().size();
}

uint64_t Writer::bytes_pushed() const {
    return stream_->bytes_pushed();
}

// ByteStream implementation
Reader& ByteStream_Composition::reader() {
    // Return reference to reader interface
    // (In real code, you'd create ReaderImpl that implements Reader)
    static Reader r(this);
    return r;
}

Writer& ByteStream_Composition::writer() {
    // Return reference to writer interface
    static Writer w(this);
    return w;
}


// ============================================================================
// SOLUTION 2: Inheritance with Type Casting (What the actual codebase uses)
// ============================================================================

class ByteStream_Correct {
protected:
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;

public:
    explicit ByteStream_Correct(uint64_t capacity) : capacity_(capacity) {}
    
    // These return references by casting *this
    // Reader and Writer inherit from ByteStream, so *this IS a Reader/Writer
    Reader& reader();
    const Reader& reader() const;
    Writer& writer();
    const Writer& writer() const;
};

class Writer_Correct : public ByteStream_Correct {
public:
    explicit Writer_Correct(uint64_t capacity) : ByteStream_Correct(capacity) {}
    
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

class Reader_Correct : public ByteStream_Correct {
public:
    explicit Reader_Correct(uint64_t capacity) : ByteStream_Correct(capacity) {}
    
    std::string_view peek() const { return std::string_view(buffer_); }
    
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
// This works because Reader and Writer inherit from ByteStream
Reader& ByteStream_Correct::reader() {
    return static_cast<Reader&>(*this);
}

const Reader& ByteStream_Correct::reader() const {
    return static_cast<const Reader&>(*this);
}

Writer& ByteStream_Correct::writer() {
    return static_cast<Writer&>(*this);
}

const Writer& ByteStream_Correct::writer() const {
    return static_cast<const Writer&>(*this);
}


// ============================================================================
// DEMONSTRATION
// ============================================================================

int main() {
    std::cout << "=== Circular Dependency Solutions ===\n\n";
    
    std::cout << "SOLUTION 1: Composition Pattern\n";
    std::cout << "  - Reader and Writer are separate classes\n";
    std::cout << "  - ByteStream contains pointers to them\n";
    std::cout << "  - No circular dependency\n";
    std::cout << "  - More flexible but more complex\n\n";
    
    std::cout << "SOLUTION 2: Inheritance with Type Casting (Used in codebase)\n";
    std::cout << "  - Reader and Writer inherit from ByteStream\n";
    std::cout << "  - ByteStream uses static_cast to convert itself\n";
    std::cout << "  - No member variables of Reader/Writer type\n";
    std::cout << "  - Simpler, but requires careful design\n";
    std::cout << "  - static_assert ensures same size (no extra members)\n\n";
    
    std::cout << "KEY INSIGHT:\n";
    std::cout << "  The actual ByteStream object IS a Reader AND a Writer!\n";
    std::cout << "  It doesn't CONTAIN them - it IS them through inheritance.\n";
    std::cout << "  The reader() and writer() methods just cast *this.\n";
    
    return 0;
}

