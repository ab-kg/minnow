// Experiment with const vs non-const reader methods
// This demonstrates why ByteStream has both const and non-const reader()

#include <iostream>
#include <string>
#include <string_view>

// Simplified Reader class
class Reader {
private:
    std::string buffer_;
    
public:
    void add_data(const std::string& data) { buffer_ += data; }
    
    // Const method - can be called on const Reader
    std::string_view peek() const {
        return std::string_view(buffer_);
    }
    
    // Non-const method - modifies the Reader
    void pop(size_t len) {
        if (len > buffer_.size()) len = buffer_.size();
        buffer_.erase(0, len);
    }
    
    // Const method
    size_t size() const { return buffer_.size(); }
};


// Simplified ByteStream with const/non-const reader methods
class ByteStream {
private:
    Reader reader_;
    
public:
    void add_data(const std::string& data) { reader_.add_data(data); }
    
    // Non-const version - returns non-const reference
    Reader& reader() {
        return reader_;
    }
    
    // Const version - returns const reference
    const Reader& reader() const {
        return reader_;
    }
};


int main() {
    
    

    return 0;
}

