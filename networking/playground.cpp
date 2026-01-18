// Playground file to experiment with ByteStream concepts
// Compile with: g++ -std=c++17 playground.cpp -o playground
// Or use your IDE's build system

#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

// Simplified version of ByteStream for experimentation
class SimpleByteStream {
private:
    std::string buffer_;
    uint64_t capacity_;
    bool closed_ = false;
    uint64_t bytes_pushed_ = 0;
    uint64_t bytes_popped_ = 0;

public:
    explicit SimpleByteStream(uint64_t capacity) : capacity_(capacity) {}
    
    // Writer methods
    void push(std::string data) {
        if (closed_) return;
        
        // Only push what fits in capacity
        uint64_t available = capacity_ - buffer_.size();
        if (data.size() > available) {
            data = data.substr(0, available);
        }
        
        buffer_ += data;
        bytes_pushed_ += data.size();
    }
    
    void close() {
        closed_ = true;
    }
    
    bool is_closed() const { return closed_; }
    uint64_t available_capacity() const { 
        return capacity_ - buffer_.size(); 
    }
    uint64_t bytes_pushed() const { return bytes_pushed_; }
    
    // Reader methods
    std::string_view peek() const {
        return std::string_view(buffer_);
    }
    
    void pop(uint64_t len) {
        if (len > buffer_.size()) {
            len = buffer_.size();
        }
        buffer_.erase(0, len);
        bytes_popped_ += len;
    }
    
    bool is_finished() const {
        return closed_ && buffer_.empty();
    }
    
    uint64_t bytes_buffered() const {
        return buffer_.size();
    }
    
    uint64_t bytes_popped() const {
        return bytes_popped_;
    }
};

int main() {
    std::cout << "=== ByteStream Playground ===\n\n";

    SimpleByteStream s(100) ; 
    s.push("abhishek");

    std::string_view ss = s.peek();
    std::cout << ss << std::endl; 

    std::cout << s.bytes_buffered() << std::endl; 
    const SimpleByteStream newstr = s ; 
    std::cout << s.bytes_pushed() << std::endl; 


    
    return 0;
}

