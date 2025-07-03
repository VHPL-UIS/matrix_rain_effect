# Matrix Digital Rain Effect 🌧️

A stunning C++ terminal application that recreates the iconic "digital rain" effect from The Matrix movie. This implementation features dynamic terminal sizing, enhanced visual effects, and cross-platform compatibility.

![Matrix Rain Demo](https://img.shields.io/badge/Terminal-Matrix%20Rain-green?style=for-the-badge)
![C++](https://img.shields.io/badge/C%2B%2B-11%2B-blue?style=for-the-badge&logo=cplusplus)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=for-the-badge)

## ✨ Features

### 🎬 Authentic Matrix Experience

- **Green digital rain** with varying character intensities
- **Dynamic character generation** using Matrix-style symbols
- **Trailing effects** with smooth intensity gradients
- **Random bold characters** for added visual depth
- **Flickering effects** for organic digital appearance

### 📱 Dynamic & Responsive

- **Auto-detects terminal size** for true full-screen coverage
- **Live resize support** - adapts when you resize your terminal
- **Scalable trail lengths** based on terminal dimensions
- **Cross-platform compatibility** (Linux, macOS, Windows)

### ⚡ Performance Optimized

- **Efficient rendering** with minimal screen updates
- **Smart memory management** using dynamic vectors
- **Configurable frame rates** for smooth animation
- **Resource-conscious** implementation

## 🚀 Quick Start

### Prerequisites

- C++ compiler with C++11 support (g++, clang++, or MSVC)
- Terminal with ANSI escape sequence support

### Compilation

**Linux/macOS:**

```bash
g++ -std=c++11 -O2 matrix_rain.cpp -o matrix_rain
./matrix_rain
```

**Windows (MinGW):**

```cmd
g++ -std=c++11 -O2 matrix_rain.cpp -o matrix_rain.exe
matrix_rain.exe
```

**Windows (Visual Studio):**

```cmd
cl /std:c++11 matrix_rain.cpp
matrix_rain.exe
```

## 🎮 Usage

Simply run the compiled executable:

```bash
./matrix_rain
```

The program will:

1. Detect your terminal size automatically
2. Adapt if you resize your terminal window
3. Clean up and exit gracefully

**Pro Tips:**

- Maximize your terminal for the best experience
- Try resizing your terminal while the effect is running
- Use a dark terminal theme for authentic Matrix vibes

## 🔧 Technical Details

### Visual Effects System

- **5 Intensity Levels**: From invisible to bold bright green
- **Dynamic Trail System**: Head → bright → normal → dim → fading
- **Random Bold Characters**: 33% at head, decreasing through trail
- **Flickering Effect**: 5% chance per frame for organic feel

### Performance Specifications

- **Frame Rate**: ~12.5 FPS (80ms per frame)
- **Memory Usage**: Scales with terminal size (typical: <1MB)
- **CPU Usage**: Minimal - optimized for efficiency
- **Terminal Support**: Any ANSI-compatible terminal

### Cross-Platform Implementation

```cpp
#ifdef _WIN32
    // Windows-specific terminal handling
    #include <windows.h>
    #include <conio.h>
#else
    // Unix/Linux terminal handling
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif
```

## 📚 Tutorial Series

This project was designed as a step-by-step tutorial for YouTube/educational content. Each step builds upon the previous one:

1. **Step 1**: Basic green text output
2. **Step 2**: Screen control and cursor manipulation
3. **Step 3**: Single falling column animation
4. **Step 4**: Trail effects with multiple characters
5. **Step 5**: Random character generation
6. **Step 6**: Multiple columns with independent movement
7. **Step 7**: Variable speeds and random timing
8. **Step 8**: Full screen coverage with enhanced effects

Each step is a complete, working program that demonstrates key concepts in terminal graphics programming.

## 🎨 Customization

### Modify Character Set

```cpp
const char *matrixChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
```

### Adjust Trail Length

```cpp
void calculateTrailLength() {
    if (height <= 24) {
        trailLength = 12;      // Adjust for small terminals
    } else if (height <= 40) {
        trailLength = 18;      // Medium terminals
    }
    // ... customize as needed
}
```

### Change Animation Speed

```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(80));  // Adjust frame delay
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

_"Wake up, Neo... The Matrix has you."_
