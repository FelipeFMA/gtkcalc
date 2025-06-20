# GTK Calculator

A simple calculator application built with C and GTK3.

![image](https://github.com/user-attachments/assets/eebed299-db4b-4e00-8f2f-813a473fb4e2)


## Features

- Basic arithmetic operations: addition, subtraction, multiplication, and division
- Decimal point support
- Clear button (C) to reset the calculator
- Backspace button (⌫) to delete the last digit
- Improved error handling for division by zero and invalid operations
- Better visual feedback with button highlighting
- Responsive UI with improved styling

## Requirements

- GCC compiler
- GTK3 development libraries
- Math library (libm)

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

### Keyboard Shortcuts

The calculator also supports keyboard input:
- Number keys (0-9) for entering digits
- Period (.) for decimal point
- Operation keys (+, -, *, /) for arithmetic operations
- Enter or = for calculating the result
- Escape or 'c' key for clearing the calculator
- Backspace for deleting the last digit

## Code Structure

The calculator has been refactored to use a more modular approach:
- Calculator state is encapsulated in a struct
- Improved error handling for mathematical operations
- Better visual feedback with CSS styling
- Cleaner code organization with proper documentation

## Cleaning Up

To clean the build files:
```bash
make clean
```

---

## Please consider donating
Bitcoin:
`bc1qnkq7hf6r53fg73jh3awfsn6ydeh87u5cf8hs3g`

![bitcoin](https://github.com/user-attachments/assets/9aaf40c6-6bdb-4480-8bdd-05b9023613d9)

Ko-fi:
https://ko-fi.com/felipefma

Paypal:
felipefmavelar@gmail.com

Brazilian Pix:
felipefmavelar@gmail.com

---
