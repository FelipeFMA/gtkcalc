#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
#include <math.h>  // Added for additional math functions

// Define a calculator struct to encapsulate state
typedef struct {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *display;
    GtkWidget *buttons[20];
    GtkCssProvider *css_provider;
    
    char display_text[256];
    double first_operand;
    double second_operand;
    char operation;
    gboolean new_input;
    gboolean has_decimal;
    gboolean error_state;  // Added to track error state
} Calculator;

// Button labels
const char *button_labels[20] = {
    "7", "8", "9", "/",
    "4", "5", "6", "*",
    "1", "2", "3", "-",
    "0", ".", "=", "+",
    "C", "⌫", NULL, NULL
};

// Function prototypes
static void update_display(Calculator *calc);
static void clear_calculator(Calculator *calc);
static void button_clicked(GtkWidget *widget, gpointer data);
static void calculate_result(Calculator *calc);
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);
static void on_window_destroy(GtkWidget *widget, gpointer data);
static void highlight_button(Calculator *calc, const char *label);
static gboolean remove_highlight(GtkWidget *button);

// Global calculator instance
Calculator calculator;

// Update the display with the current text
static void update_display(Calculator *calc) {
    // If there's an active operation and we're waiting for the second operand,
    // show the operator in the display
    if (calc->operation != '\0' && calc->new_input) {
        // Create a new buffer with enough space for the text, a space, the operator, and null terminator
        char display_with_op[256];
        
        // Copy the display text, ensuring we leave room for " X\0" (3 characters)
        size_t max_len = sizeof(display_with_op) - 3;
        strncpy(display_with_op, calc->display_text, max_len);
        display_with_op[max_len] = '\0';  // Ensure null termination
        
        // Get the actual length after potential truncation
        size_t len = strlen(display_with_op);
        
        // Append the space and operator
        display_with_op[len] = ' ';
        display_with_op[len + 1] = calc->operation;
        display_with_op[len + 2] = '\0';
        
        gtk_label_set_text(GTK_LABEL(calc->display), display_with_op);
    } else {
        gtk_label_set_text(GTK_LABEL(calc->display), calc->display_text);
    }
}

// Clear calculator state
static void clear_calculator(Calculator *calc) {
    strcpy(calc->display_text, "0");
    calc->first_operand = 0.0;
    calc->second_operand = 0.0;
    calc->operation = '\0';
    calc->new_input = TRUE;
    calc->has_decimal = FALSE;
    calc->error_state = FALSE;  // Reset error state
    update_display(calc);
}

// Calculate the result based on the operation
static void calculate_result(Calculator *calc) {
    // If we're in an error state, don't try to calculate
    if (calc->error_state) {
        return;
    }
    
    calc->second_operand = atof(calc->display_text);
    double result = 0.0;
    
    switch (calc->operation) {
        case '+':
            result = calc->first_operand + calc->second_operand;
            break;
        case '-':
            result = calc->first_operand - calc->second_operand;
            break;
        case '*':
            result = calc->first_operand * calc->second_operand;
            break;
        case '/':
            if (fabs(calc->second_operand) > 1e-10) {  // Better check for zero
                result = calc->first_operand / calc->second_operand;
            } else {
                strcpy(calc->display_text, "Error: Div by 0");
                calc->error_state = TRUE;
                update_display(calc);
                return;
            }
            break;
    }
    
    // Check for overflow or invalid result
    if (isinf(result) || isnan(result)) {
        strcpy(calc->display_text, "Error: Invalid result");
        calc->error_state = TRUE;
        update_display(calc);
        return;
    }
    
    // Convert result to string with appropriate precision
    snprintf(calc->display_text, sizeof(calc->display_text), "%.10g", result);
    
    // Reset calculator state
    calc->first_operand = result;
    calc->operation = '\0';
    calc->new_input = TRUE;
    calc->has_decimal = (strchr(calc->display_text, '.') != NULL);
    
    update_display(calc);
}

// Button click handler
static void button_clicked(GtkWidget *widget, gpointer data) {
    Calculator *calc = &calculator;  // Get our calculator instance
    const char *button_text = (const char *)data;
    
    // If this was triggered by a mouse click (widget is not NULL), highlight the button
    if (widget != NULL) {
        // Apply the "active" CSS class to the button
        gtk_style_context_add_class(gtk_widget_get_style_context(widget), "active-button");
        
        // Schedule the removal of the highlight after a short delay
        g_timeout_add(150, (GSourceFunc)remove_highlight, widget);
    }
    
    // Handle clear button first to reset error state
    if (strcmp(button_text, "C") == 0) {
        clear_calculator(calc);
        return;
    }
    
    // If we're in an error state, only allow clear operation
    if (calc->error_state) {
        return;
    }
    
    // Handle numeric buttons (0-9)
    if ((button_text[0] >= '0' && button_text[0] <= '9') || (button_text[0] == '.' && !calc->has_decimal)) {
        // Limit the number of digits to prevent window resizing issues
        const size_t MAX_DIGITS = 12;
        
        if (calc->new_input) {
            if (button_text[0] == '.') {
                strcpy(calc->display_text, "0.");
                calc->has_decimal = TRUE;
            } else {
                strcpy(calc->display_text, button_text);
            }
            calc->new_input = FALSE;
        } else {
            // Don't allow multiple decimal points
            if (button_text[0] == '.') {
                calc->has_decimal = TRUE;
            }
            
            // Only append digit if we haven't reached the maximum length
            size_t current_len = strlen(calc->display_text);
            if (current_len < MAX_DIGITS) {
                strcat(calc->display_text, button_text);
            }
        }
        update_display(calc);
        return;
    }
    
    // Handle operation buttons (+, -, *, /)
    if (button_text[0] == '+' || button_text[0] == '-' || 
        button_text[0] == '*' || button_text[0] == '/') {
        
        // If we already have an operation pending, calculate it first
        if (calc->operation != '\0' && !calc->new_input) {
            calculate_result(calc);
            // If calculation resulted in an error, return
            if (calc->error_state) {
                return;
            }
        } else {
            calc->first_operand = atof(calc->display_text);
        }
        
        calc->operation = button_text[0];
        calc->new_input = TRUE;
        calc->has_decimal = FALSE;
        update_display(calc);  // Update display to show the operator
        return;
    }
    
    // Handle equals button
    if (button_text[0] == '=') {
        if (calc->operation != '\0') {
            calculate_result(calc);
        }
        return;
    }
    
    // Handle backspace button
    if (strcmp(button_text, "⌫") == 0) {
        int len = strlen(calc->display_text);
        if (len > 0) {
            // Check if we're removing a decimal point
            if (calc->display_text[len - 1] == '.') {
                calc->has_decimal = FALSE;
            }
            
            // Remove the last character
            calc->display_text[len - 1] = '\0';
            
            // If we've deleted everything, show 0
            if (len == 1 || strlen(calc->display_text) == 0) {
                strcpy(calc->display_text, "0");
                calc->new_input = TRUE;
            }
            
            update_display(calc);
        }
        return;
    }
}

// Highlight a button briefly to provide visual feedback
static void highlight_button(Calculator *calc, const char *label) {
    // Find the button with the matching label
    for (int i = 0; i < 18; i++) {
        if (label && strcmp(label, "") != 0 && strcmp(label, " ") != 0 && 
            button_labels[i] && strcmp(button_labels[i], label) == 0) {
            // Apply the "active" CSS class to the button
            gtk_style_context_add_class(gtk_widget_get_style_context(calc->buttons[i]), "active-button");
            
            // Schedule the removal of the highlight after a short delay
            g_timeout_add(150, (GSourceFunc)remove_highlight, calc->buttons[i]);
            break;
        }
    }
}

// Remove the highlight from a button (called by timeout)
static gboolean remove_highlight(GtkWidget *button) {
    gtk_style_context_remove_class(gtk_widget_get_style_context(button), "active-button");
    return G_SOURCE_REMOVE; // Remove the timeout source
}

// Handle key press events
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    (void)widget; // Silence unused parameter warning
    (void)data;   // Silence unused parameter warning
    Calculator *calc = &calculator;
    
    // Debug output to see what keys are being pressed
    g_print("Key pressed: keyval=%u, keyname=%s\n", event->keyval, gdk_keyval_name(event->keyval));
    
    // Handle numeric keys (0-9)
    if ((event->keyval >= GDK_KEY_0 && event->keyval <= GDK_KEY_9) || 
        (event->keyval >= GDK_KEY_KP_0 && event->keyval <= GDK_KEY_KP_9)) {
        
        char digit[2] = {0};
        if (event->keyval >= GDK_KEY_0 && event->keyval <= GDK_KEY_9) {
            digit[0] = '0' + (event->keyval - GDK_KEY_0);
        } else {
            digit[0] = '0' + (event->keyval - GDK_KEY_KP_0);
        }
        
        highlight_button(calc, digit);
        button_clicked(NULL, (gpointer)digit);
        return TRUE;
    }
    
    // Handle decimal point
    if (event->keyval == GDK_KEY_period || event->keyval == GDK_KEY_KP_Decimal) {
        if (!calc->has_decimal) {
            highlight_button(calc, ".");
            button_clicked(NULL, (gpointer)".");
        }
        return TRUE;
    }
    
    // Handle operators - use direct keyval comparison for common operators
    switch (event->keyval) {
        // Addition
        case GDK_KEY_plus:
        case GDK_KEY_KP_Add:
            highlight_button(calc, "+");
            button_clicked(NULL, (gpointer)"+");
            return TRUE;
            
        // Subtraction
        case GDK_KEY_minus:
        case GDK_KEY_KP_Subtract:
            highlight_button(calc, "-");
            button_clicked(NULL, (gpointer)"-");
            return TRUE;
            
        // Multiplication - handle both * and x
        case GDK_KEY_asterisk:
        case GDK_KEY_KP_Multiply:
        case GDK_KEY_x:
        case GDK_KEY_X:
            highlight_button(calc, "*");
            button_clicked(NULL, (gpointer)"*");
            return TRUE;
            
        // Division
        case GDK_KEY_slash:
        case GDK_KEY_KP_Divide:
            highlight_button(calc, "/");
            button_clicked(NULL, (gpointer)"/");
            return TRUE;
            
        // Equals
        case GDK_KEY_equal:
        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:
            highlight_button(calc, "=");
            button_clicked(NULL, (gpointer)"=");
            return TRUE;
            
        // Backspace
        case GDK_KEY_BackSpace:
            highlight_button(calc, "⌫");
            button_clicked(NULL, (gpointer)"⌫");
            return TRUE;
            
        // Clear
        case GDK_KEY_Escape:
        case GDK_KEY_Delete:
        case GDK_KEY_c:
        case GDK_KEY_C:
            highlight_button(calc, "C");
            button_clicked(NULL, (gpointer)"C");
            return TRUE;
    }
    
    return FALSE; // Let other handlers process the event
}

// Window close handler
static void on_window_destroy(GtkWidget *widget, gpointer data) {
    (void)widget; // Silence unused parameter warning
    (void)data;   // Silence unused parameter warning
    
    // Clean up resources
    g_object_unref(calculator.css_provider);
    
    gtk_main_quit();
}

/**
 * Initialize the calculator UI and state
 * 
 * @param calc Pointer to the calculator struct to initialize
 */
static void init_calculator(Calculator *calc) {
    // Initialize calculator state
    strcpy(calc->display_text, "0");
    calc->first_operand = 0.0;
    calc->second_operand = 0.0;
    calc->operation = '\0';
    calc->new_input = TRUE;
    calc->has_decimal = FALSE;
    calc->error_state = FALSE;
    
    // Create CSS provider for styling
    calc->css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(calc->css_provider,
        ".active-button {\n"
        "    background-color: rgba(100, 200, 255, 0.5);\n"
        "    transition: background-color 150ms ease-in-out;\n"
        "}\n"
        "button {\n"
        "    transition: background-color 150ms ease-in-out;\n"
        "    font-size: 16px;\n"
        "    padding: 8px;\n"
        "}\n"
        "button:focus {\n"
        "    outline: none;\n"
        "    box-shadow: none;\n"
        "}\n"
        ".display {\n"
        "    font-size: 24px;\n"
        "    font-weight: bold;\n"
        "    padding: 10px;\n"
        "    background-color: #f0f0f0;\n"
        "    border: 1px solid #ccc;\n"
        "    border-radius: 4px;\n"
        "    color: #222222;\n"
        "    min-width: 260px;\n"
        "    max-width: 260px;\n"
        "}\n", -1, NULL);
    
    // Apply CSS to default screen
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(calc->css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    // Create the main window
    calc->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(calc->window), "GTK Calculator");
    gtk_window_set_default_size(GTK_WINDOW(calc->window), 300, 350);
    gtk_window_set_position(GTK_WINDOW(calc->window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(calc->window), 10);
    gtk_window_set_resizable(GTK_WINDOW(calc->window), FALSE);
    g_signal_connect(calc->window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect(calc->window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    
    // Create a grid layout
    calc->grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(calc->grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(calc->grid), 8);
    gtk_container_add(GTK_CONTAINER(calc->window), calc->grid);
    
    // Create the display
    calc->display = gtk_label_new("0");
    gtk_widget_set_hexpand(calc->display, TRUE);
    gtk_label_set_xalign(GTK_LABEL(calc->display), 1.0);  // Right-align text
    gtk_widget_set_margin_bottom(calc->display, 15);
    
    // Set ellipsization mode to truncate text at the start if it's too long
    gtk_label_set_ellipsize(GTK_LABEL(calc->display), PANGO_ELLIPSIZE_START);
    gtk_label_set_width_chars(GTK_LABEL(calc->display), 12);  // Set a reasonable width
    gtk_label_set_max_width_chars(GTK_LABEL(calc->display), 12);  // Limit maximum width
    
    // Add CSS class to display
    gtk_style_context_add_class(gtk_widget_get_style_context(calc->display), "display");
    
    // Add the display to the grid (spanning all 4 columns)
    gtk_grid_attach(GTK_GRID(calc->grid), calc->display, 0, 0, 4, 1);
    
    // Create the calculator buttons
    int button_index = 0;
    
    for (int row = 1; row <= 5; row++) {
        for (int col = 0; col < 4; col++) {
            // Skip the last two positions in the grid
            if (row == 5 && col > 1) {
                continue;
            }
            
            calc->buttons[button_index] = gtk_button_new_with_label(button_labels[button_index]);
            gtk_widget_set_hexpand(calc->buttons[button_index], TRUE);
            gtk_widget_set_vexpand(calc->buttons[button_index], TRUE);
            
            // Make the C and backspace buttons span 2 columns
            if (row == 5) {
                gtk_grid_attach(GTK_GRID(calc->grid), calc->buttons[button_index], col * 2, row, 2, 1);
            } else {
                gtk_grid_attach(GTK_GRID(calc->grid), calc->buttons[button_index], col, row, 1, 1);
            }
            
            g_signal_connect(calc->buttons[button_index], "clicked", 
                           G_CALLBACK(button_clicked), 
                           (gpointer)button_labels[button_index]);
            
            button_index++;
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Initialize calculator
    init_calculator(&calculator);
    
    // Show all widgets
    gtk_widget_show_all(calculator.window);
    
    // Start the GTK main loop
    gtk_main();
    
    return 0;
}
