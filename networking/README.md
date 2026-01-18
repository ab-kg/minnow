# Networking Playground

This folder is for experimenting with and understanding the codebase concepts.

## Files

- `playground.cpp` - Simple ByteStream implementation to experiment with
- `test_const.cpp` - Demonstrates const vs non-const methods

## How to Compile and Run

### Using g++ (if you have it):
```bash
g++ -std=c++17 playground.cpp -o playground
./playground

g++ -std=c++17 test_const.cpp -o test_const
./test_const
```

### Using Visual Studio (Windows):
- Create a new C++ project
- Add these files
- Set C++ standard to C++17 or later
- Build and run

### Using your IDE:
- Most IDEs can compile single .cpp files
- Make sure C++17 or later is enabled

## What to Try

1. **Modify playground.cpp**:
   - Change the capacity
   - Try different push/pop sequences
   - See what happens when you exceed capacity
   - Experiment with closing and finishing

2. **Modify test_const.cpp**:
   - Uncomment the `r2.pop(2)` line to see the compiler error
   - Try creating const and non-const versions
   - Understand when each version is called

3. **Copy code from the real project**:
   - Copy `src/byte_stream.hh` here
   - Try implementing your own version
   - Compare with the real implementation

## Learning Tips

- **Break things**: Try invalid operations to see what happens
- **Add print statements**: See what's happening inside
- **Simplify**: Remove features to understand core concepts
- **Compare**: Look at the real code and see how it differs