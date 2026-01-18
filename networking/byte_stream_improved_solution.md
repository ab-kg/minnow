# ByteStream Implementation - Improved Solution

## Issues Found in Current Implementation

### 1. **Header File (byte_stream.hh) Issues**

**Issue 1: Syntax Error**
```cpp
std:::string buffer ;  // ❌ Three colons - should be std::string
```

**Issue 2: Wrong Data Types**
```cpp
int pushedsize ;      // ❌ Should be uint64_t to match return type
int poppedbytes ;     // ❌ Should be uint64_t to match return type
```

**Issue 3: Missing Closed Flag**
- No `closed_` flag to track if stream is closed
- `is_closed()` incorrectly checks buffer size instead

**Issue 4: Missing Initialization**
- Member variables not initialized in constructor

---

### 2. **Writer::push() Issues**

**Issue 1: Wrong Variable Name**
```cpp
uint64_t available = capacity - buffer.size();  // ❌ Should be capacity_
```

**Issue 2: Logic Issue**
- Should check if stream is closed before pushing
- Should handle empty strings gracefully

**Issue 3: Debug Statement**
- Debug statement still present even though code is implemented

---

### 3. **Writer::close() Issues**

**Issue 1: Not Actually Closing**
- Doesn't set a closed flag
- Just checks buffer size (wrong logic)

**Issue 2: Debug Output**
- Uses `std::cout` instead of proper flag

---

### 4. **Writer::is_closed() Issues**

**Issue 1: Wrong Logic**
```cpp
return {buffer.size() == capacity};  // ❌ Wrong - checks buffer size, not closed state
```

---

### 5. **Writer::available_capacity() Issues**

**Issue 1: Wrong Calculation**
```cpp
return {buffer.size()- capacity};  // ❌ Wrong formula and wrong variable name
```

Should be: `capacity_ - buffer.size()`

**Issue 2: Potential Underflow**
- If `buffer.size() > capacity_`, this would underflow (but shouldn't happen)

---

### 6. **Reader::peek() Issues**

**Issue 1: Unnecessary Cast**
```cpp
string_view ss = static_cast<string_view>(buffer);  // ❌ Unnecessary cast
```

Can simply return: `return buffer;` (string_view conversion is automatic)

---

### 7. **Reader::pop() Issues**

**Issue 1: Wrong erase() Usage**
```cpp
buffer.erase(len);  // ❌ Wrong - erases from position len, not first len bytes
```

Should be: `buffer.erase(0, len);` to remove from beginning

**Issue 2: Bounds Check**
- Good that you check `len > buffer.size()`, but should use `min()` for clarity

---

### 8. **Reader::is_finished() Issues**

**Issue 1: Missing Closed Check**
```cpp
return {buffer.size() == 0};  // ❌ Only checks buffer, not closed state
```

Should check: `closed_ && buffer.empty()`

---

### 9. **Reader::bytes_popped() Issues**

**Issue 1: Returns Empty**
```cpp
return {};  // ❌ Should return poppedbytes
```

---

## Improved Solution

### Fixed Header File (byte_stream.hh)

```cpp
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class Reader;
class Writer;

class ByteStream
{
public:
  explicit ByteStream( uint64_t capacity );

  // Helper functions (provided) to access the ByteStream's Reader and Writer interfaces
  Reader& reader();
  const Reader& reader() const;
  Writer& writer();
  const Writer& writer() const;

  void set_error() { error_ = true; };       // Signal that the stream suffered an error.
  bool has_error() const { return error_; }; // Has the stream had an error?

protected:
  // Please add any additional state to the ByteStream here, and not to the Writer and Reader interfaces.
  std::string buffer_;           // Fixed: std::string (not std:::string)
  uint64_t capacity_;
  uint64_t bytes_pushed_;        // Fixed: uint64_t (not int)
  uint64_t bytes_popped_;        // Fixed: uint64_t (not int)
  bool closed_ {};               // Added: track if stream is closed
  bool error_ {};
};
```

### Fixed Implementation (byte_stream.cc)

```cpp
#include "byte_stream.hh"
#include "debug.hh"
#include <algorithm>

using namespace std;

// Initialize all member variables
ByteStream::ByteStream( uint64_t capacity ) 
  : capacity_( capacity )
  , bytes_pushed_( 0 )
  , bytes_popped_( 0 )
  , closed_( false )
  , error_( false )
{}

// Push data to stream, but only as much as available capacity allows.
void Writer::push( string data )
{
  // Don't push if stream is closed
  if ( closed_ ) {
    return;
  }
  
  // Calculate available capacity
  uint64_t available = available_capacity();
  
  // Limit data to available capacity
  if ( data.size() > available ) {
    data = data.substr( 0, available );
  }
  
  // Add data to buffer and update counter
  buffer_ += data;
  bytes_pushed_ += data.size();
}

// Signal that the stream has reached its ending. Nothing more will be written.
void Writer::close()
{
  closed_ = true;
}

// Has the stream been closed?
bool Writer::is_closed() const
{
  return closed_;
}

// How many bytes can be pushed to the stream right now?
uint64_t Writer::available_capacity() const
{
  // Available = total capacity - current buffer size
  // Use min() to prevent underflow (though buffer size should never exceed capacity)
  if ( buffer_.size() >= capacity_ ) {
    return 0;
  }
  return capacity_ - buffer_.size();
}

// Total number of bytes cumulatively pushed to the stream
uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

// Peek at the next bytes in the buffer -- ideally as many as possible.
string_view Reader::peek() const
{
  // string_view automatically converts from string
  return buffer_;
}

// Remove `len` bytes from the buffer.
void Reader::pop( uint64_t len )
{
  // Don't pop more than what's available
  len = min( len, static_cast<uint64_t>( buffer_.size() ) );
  
  // Remove from beginning of buffer
  buffer_.erase( 0, len );
  
  // Update counter
  bytes_popped_ += len;
}

// Is the stream finished (closed and fully popped)?
bool Reader::is_finished() const
{
  // Finished = closed AND buffer is empty
  return closed_ && buffer_.empty();
}

// Number of bytes currently buffered (pushed and not popped)
uint64_t Reader::bytes_buffered() const
{
  return buffer_.size();
}

// Total number of bytes cumulatively popped from stream
uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}
```

---

## Key Improvements Summary

1. ✅ **Fixed syntax error**: `std:::string` → `std::string`
2. ✅ **Fixed data types**: `int` → `uint64_t` for counters
3. ✅ **Added closed flag**: Properly track stream closure state
4. ✅ **Fixed constructor**: Initialize all member variables
5. ✅ **Fixed available_capacity()**: Correct formula `capacity_ - buffer_.size()`
6. ✅ **Fixed is_closed()**: Check `closed_` flag, not buffer size
7. ✅ **Fixed is_finished()**: Check both `closed_` and `buffer_.empty()`
8. ✅ **Fixed pop()**: Use `buffer_.erase(0, len)` to remove from beginning
9. ✅ **Fixed bytes_popped()**: Return `bytes_popped_` counter
10. ✅ **Improved push()**: Check if closed before pushing
11. ✅ **Removed debug statements**: Clean up unnecessary debug calls
12. ✅ **Better naming**: Use `buffer_` with underscore convention

---

## Testing Checklist

After implementing, verify:
- [ ] `is_closed()` returns false initially, true after `close()`
- [ ] `is_finished()` returns true only when closed AND buffer empty
- [ ] `available_capacity()` decreases as buffer fills
- [ ] `bytes_pushed()` increments correctly with each push
- [ ] `bytes_popped()` increments correctly with each pop
- [ ] `pop()` removes from beginning, not end
- [ ] `peek()` returns entire buffer as string_view
- [ ] Pushing after close does nothing
- [ ] Capacity limits are respected

