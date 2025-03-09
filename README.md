# GTK Calculator

A simple calculator application built with C and GTK3.

## Features

- Basic arithmetic operations: addition, subtraction, multiplication, and division
- Decimal point support
- Clear button (C) to reset the calculator
- Backspace button (⌫) to delete the last digit
- Error handling for division by zero

## Requirements

- GCC compiler
- GTK3 development libraries

## Building the Application

To build the calculator, make sure you have the required dependencies installed:

### Ubuntu/Debian
```bash
sudo apt-get install build-essential libgtk-3-dev
```

### Fedora
```bash
sudo dnf install gcc gtk3-devel
```

### Arch Linux
```bash
sudo pacman -S gcc gtk3
```

Then, build the application:
```bash
make
```

## Running the Calculator

After building, run the calculator with:
```bash
./calculator
```

## Usage

- Click the number buttons (0-9) to input numbers
- Click the decimal point (.) button to add a decimal point
- Click operation buttons (+, -, *, /) to perform arithmetic operations
- Click the equals (=) button to calculate the result
- Click the clear (C) button to reset the calculator
- Click the backspace (⌫) button to delete the last digit

## Cleaning Up

To clean the build files:
```bash
make clean
```
