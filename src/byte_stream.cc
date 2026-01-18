#include "byte_stream.hh"
#include "debug.hh"
#include <algorithm>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) 
  : buffer_()
  , capacity_( capacity )
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
// It's not required to return a string_view of the *whole* buffer, but
// if the peeked string_view is only one byte at a time, it will probably force
// the caller to do a lot of extra work.
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
