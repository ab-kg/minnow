/*
 * INTERACTIVE TUTORIAL: static_cast and static_assert
 * 
 * This tutorial covers:
 * 1. static_cast - Type conversions checked at compile-time
 * 2. static_assert - Compile-time assertions
 * 
 * Compile with: g++ -std=c++17 static_cast_static_assert_tutorial.cpp -o tutorial
 * Run with: ./tutorial
 */

#include <iostream>
#include <type_traits>
#include <cassert>

// ============================================================================
// PART 1: static_cast
// ============================================================================

void demonstrate_static_cast() {
    std::cout << "\n=== PART 1: static_cast ===\n\n";
    
    // ------------------------------------------------------------------------
    // Example 1: Converting between numeric types
    // ------------------------------------------------------------------------
    std::cout << "Example 1: Numeric type conversions\n";
    std::cout << "-----------------------------------\n";
    
    int i = 42;
    double d = 3.14;
    
    // Converting int to double (widening conversion - safe)
    double d_from_int = static_cast<double>(i);
    std::cout << "int " << i << " -> double: " << d_from_int << "\n";
    
    // Converting double to int (narrowing conversion - data loss possible)
    int i_from_double = static_cast<int>(d);
    std::cout << "double " << d << " -> int: " << i_from_double << "\n";
    std::cout << "Note: Fractional part is truncated!\n\n";
    
    // ------------------------------------------------------------------------
    // Example 2: Pointer conversions
    // ------------------------------------------------------------------------
    std::cout << "Example 2: Pointer conversions\n";
    std::cout << "-------------------------------\n";
    
    int value = 100;
    int* int_ptr = &value;
    
    // Converting int* to void* (upcast - safe)
    void* void_ptr = static_cast<void*>(int_ptr);
    std::cout << "int* -> void*: " << void_ptr << "\n";
    
    // Converting void* back to int* (downcast - must be careful!)
    int* int_ptr_again = static_cast<int*>(void_ptr);
    std::cout << "void* -> int*: " << int_ptr_again << " (value: " << *int_ptr_again << ")\n\n";
    
    // ------------------------------------------------------------------------
    // Example 3: Class hierarchy conversions
    // ------------------------------------------------------------------------
    std::cout << "Example 3: Class hierarchy conversions\n";
    std::cout << "---------------------------------------\n";
    
    class Base {
    public:
        virtual ~Base() = default;
        int base_value = 10;
    };
    
    class Derived : public Base {
    public:
        int derived_value = 20;
    };
    
    Derived derived_obj;
    Base* base_ptr = &derived_obj;  // Upcast (automatic)
    
    // Downcast: Base* -> Derived* (must use static_cast or dynamic_cast)
    Derived* derived_ptr = static_cast<Derived*>(base_ptr);
    std::cout << "Base* -> Derived*: base_value=" << derived_ptr->base_value 
              << ", derived_value=" << derived_ptr->derived_value << "\n";
    std::cout << "⚠️  WARNING: static_cast doesn't check if conversion is valid!\n";
    std::cout << "   Use dynamic_cast if you're unsure about the actual type.\n\n";
    
    // ------------------------------------------------------------------------
    // Example 4: Removing const (use with caution!)
    // ------------------------------------------------------------------------
    std::cout << "Example 4: Removing const qualifier\n";
    std::cout << "------------------------------------\n";
    
    const int const_value = 42;
    // int* bad_ptr = &const_value;  // ERROR: can't assign const int* to int*
    
    // Removing const (dangerous - only do this if you know what you're doing!)
    int* non_const_ptr = const_cast<int*>(&const_value);
    std::cout << "const int -> int*: " << *non_const_ptr << "\n";
    std::cout << "⚠️  WARNING: Modifying const data is undefined behavior!\n\n";
    
    // ------------------------------------------------------------------------
    // Example 5: Enum conversions
    // ------------------------------------------------------------------------
    std::cout << "Example 5: Enum conversions\n";
    std::cout << "----------------------------\n";
    
    enum class Color { Red = 1, Green = 2, Blue = 3 };
    enum OldColor { RED = 1, GREEN = 2, BLUE = 3 };
    
    Color color = Color::Red;
    int color_int = static_cast<int>(color);
    std::cout << "enum class Color::Red -> int: " << color_int << "\n";
    
    Color color_from_int = static_cast<Color>(2);
    std::cout << "int 2 -> Color: " << static_cast<int>(color_from_int) << "\n\n";
}

// ============================================================================
// PART 2: static_assert
// ============================================================================

void demonstrate_static_assert() {
    std::cout << "\n=== PART 2: static_assert ===\n\n";
    
    // ------------------------------------------------------------------------
    // Example 1: Basic static_assert
    // ------------------------------------------------------------------------
    std::cout << "Example 1: Basic compile-time assertions\n";
    std::cout << "----------------------------------------\n";
    
    // This will compile successfully
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    std::cout << "✓ Assertion passed: int is at least 4 bytes\n";
    
    // Uncomment the line below to see a compile error:
    // static_assert(sizeof(int) == 8, "int must be exactly 8 bytes");
    std::cout << "💡 Try uncommenting the line above to see a compile error!\n\n";
    
    // ------------------------------------------------------------------------
    // Example 2: Template type checking
    // ------------------------------------------------------------------------
    std::cout << "Example 2: Template type checking\n";
    std::cout << "----------------------------------\n";
    
    template<typename T>
    void process_number(T value) {
        // Ensure T is an arithmetic type (int, float, double, etc.)
        static_assert(std::is_arithmetic_v<T>, 
                     "T must be an arithmetic type");
        std::cout << "Processing number: " << value << "\n";
    }
    
    process_number(42);        // OK: int is arithmetic
    process_number(3.14);      // OK: double is arithmetic
    // process_number("hello"); // ERROR: string is not arithmetic
    
    std::cout << "\n";
    
    // ------------------------------------------------------------------------
    // Example 3: Size requirements
    // ------------------------------------------------------------------------
    std::cout << "Example 3: Size requirements\n";
    std::cout << "-----------------------------\n";
    
    struct Point {
        int x, y;
    };
    
    // Ensure Point has the expected size
    static_assert(sizeof(Point) == sizeof(int) * 2, 
                 "Point struct must contain exactly 2 ints");
    std::cout << "✓ Point struct has correct size: " << sizeof(Point) << " bytes\n\n";
    
    // ------------------------------------------------------------------------
    // Example 4: Platform-specific checks
    // ------------------------------------------------------------------------
    std::cout << "Example 4: Platform-specific checks\n";
    std::cout << "------------------------------------\n";
    
    // Check if we're on a 64-bit system
    static_assert(sizeof(void*) == 8 || sizeof(void*) == 4, 
                 "Unsupported pointer size");
    
    if constexpr (sizeof(void*) == 8) {
        std::cout << "✓ Running on 64-bit system\n";
    } else {
        std::cout << "✓ Running on 32-bit system\n";
    }
    std::cout << "\n";
    
    // ------------------------------------------------------------------------
    // Example 5: Constexpr and static_assert
    // ------------------------------------------------------------------------
    std::cout << "Example 5: Using constexpr with static_assert\n";
    std::cout << "---------------------------------------------\n";
    
    constexpr int array_size = 10;
    static_assert(array_size > 0, "Array size must be positive");
    static_assert(array_size <= 100, "Array size must be <= 100");
    
    int arr[array_size];
    std::cout << "✓ Created array of size " << array_size << "\n";
    std::cout << "  Array has " << sizeof(arr) / sizeof(arr[0]) << " elements\n\n";
    
    // ------------------------------------------------------------------------
    // Example 6: Type traits with static_assert
    // ------------------------------------------------------------------------
    std::cout << "Example 6: Type traits validation\n";
    std::cout << "----------------------------------\n";
    
    template<typename T>
    class SafeContainer {
        static_assert(std::is_copy_constructible_v<T>, 
                     "T must be copy constructible");
        static_assert(std::is_destructible_v<T>, 
                     "T must be destructible");
    public:
        void add(const T& item) {
            // Implementation...
        }
    };
    
    SafeContainer<int> container1;  // OK: int is copy constructible
    // SafeContainer<std::unique_ptr<int>> container2;  // ERROR: not copyable
    
    std::cout << "✓ SafeContainer<int> compiled successfully\n";
    std::cout << "  (uncomment the line above to see compile error)\n\n";
}

// ============================================================================
// PART 3: Common Patterns and Best Practices
// ============================================================================

void demonstrate_best_practices() {
    std::cout << "\n=== PART 3: Best Practices ===\n\n";
    
    // ------------------------------------------------------------------------
    // Best Practice 1: Use static_cast instead of C-style casts
    // ------------------------------------------------------------------------
    std::cout << "Best Practice 1: Prefer static_cast over C-style casts\n";
    std::cout << "------------------------------------------------------\n";
    
    double pi = 3.14159;
    
    // BAD: C-style cast (hard to find, less safe)
    int bad = (int)pi;
    
    // GOOD: static_cast (explicit, easier to find, safer)
    int good = static_cast<int>(pi);
    
    std::cout << "C-style cast: " << bad << "\n";
    std::cout << "static_cast: " << good << "\n";
    std::cout << "✓ static_cast is more explicit and safer\n\n";
    
    // ------------------------------------------------------------------------
    // Best Practice 2: Use static_assert for invariants
    // ------------------------------------------------------------------------
    std::cout << "Best Practice 2: Use static_assert for compile-time invariants\n";
    std::cout << "---------------------------------------------------------------\n";
    
    constexpr int MAX_SIZE = 1000;
    constexpr int MIN_SIZE = 1;
    
    static_assert(MAX_SIZE > MIN_SIZE, "MAX_SIZE must be greater than MIN_SIZE");
    static_assert(MAX_SIZE < 10000, "MAX_SIZE is too large");
    
    std::cout << "✓ Compile-time invariants validated\n\n";
    
    // ------------------------------------------------------------------------
    // Best Practice 3: Combining static_cast and static_assert
    // ------------------------------------------------------------------------
    std::cout << "Best Practice 3: Combining static_cast with static_assert\n";
    std::cout << "----------------------------------------------------------\n";
    
    template<typename From, typename To>
    To safe_cast(From value) {
        // Ensure the types are compatible
        static_assert(std::is_convertible_v<From, To>, 
                     "Types must be convertible");
        return static_cast<To>(value);
    }
    
    int num = 42;
    double result = safe_cast<int, double>(num);
    std::cout << "safe_cast<int, double>(42) = " << result << "\n";
    // safe_cast<int, std::string>(42);  // ERROR: not convertible
    
    std::cout << "\n";
}

// ============================================================================
// INTERACTIVE EXERCISES
// ============================================================================

void interactive_exercises() {
    std::cout << "\n=== INTERACTIVE EXERCISES ===\n\n";
    
    std::cout << "Exercise 1: Fix the type conversion\n";
    std::cout << "------------------------------------\n";
    std::cout << "Convert a float to an int using static_cast:\n";
    float f = 7.8f;

    // TODO: Add your code here
    int i = static_cast<int>(f);
    std::cout << "Result: " << i << "\n\n";
    
    std::cout << "\nExercise 2: Add a static_assert\n";
    std::cout << "-------------------------------\n";
    std::cout << "Add a static_assert to ensure sizeof(char) == 1:\n";
    // TODO: Add your static_assert here
    static_assert(sizeof(char) == 1 , "char must be 1 byte");
    std::cout << "✓ Add the assertion above\n\n";
    
    std::cout << "\nExercise 3: Template with constraints\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Create a template function that only accepts integral types:\n";
    
    // SOLUTION: Use std::is_integral_v<T> to check if T is an integral type
    template<typename T>
    void process_integer(T value) {
        static_assert(std::is_integral_v<T> , "T must be an integral type");
        std::cout << "Processing integer: " << value << "\n";
    }
    
    // Test the function
    process_integer(42);        // OK: int is integral
    process_integer(100L);      // OK: long is integral
    process_integer('A');       // OK: char is integral
    // process_integer(3.14);   // ERROR: double is not integral (uncomment to see error)
    // process_integer("hello"); // ERROR: string is not integral (uncomment to see error)
    
    std::cout << "✓ Template function implemented and tested!\n\n";
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║  INTERACTIVE TUTORIAL: static_cast & static_assert       ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    
    demonstrate_static_cast();
    demonstrate_static_assert();
    demonstrate_best_practices();
    interactive_exercises();
    
    std::cout << "\n=== TUTORIAL COMPLETE ===\n";
    std::cout << "💡 Try modifying the code and experimenting!\n";
    std::cout << "💡 Uncomment the ERROR lines to see compile errors\n";
    std::cout << "💡 Complete the exercises in the interactive_exercises() function\n\n";
    
    return 0;
}

