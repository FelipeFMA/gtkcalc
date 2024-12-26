#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

// Incluímos as bibliotecas necessárias
// gtkmm.h nos dá acesso a todos os widgets do GTK
// string é necessário para manipular texto
#include <gtkmm.h>
#include <string>

// Nossa classe principal que herda de Gtk::Window
// Herdar de Window significa que nossa classe será uma janela GTK
class CalculatorWindow : public Gtk::Window {
public:
    // Construtor padrão - vai configurar toda nossa interface
    CalculatorWindow();
    
    // Destrutor virtual - necessário quando temos herança em C++
    virtual ~CalculatorWindow();

protected:
    // Métodos que serão chamados quando os botões forem clicados
    // Estes são nossos "event handlers" ou manipuladores de eventos
    void on_button_clicked(const Glib::ustring& digit);    // Para botões numéricos
    void on_operator_clicked(const Glib::ustring& op);     // Para operadores (+,-,*,/)
    void on_clear_clicked();                               // Para o botão C (limpar)
    void on_equals_clicked();                              // Para o botão =

    // Widget que mostra os números digitados e o resultado
    Gtk::Entry display;
    
    // Container que organiza nossos botões em uma grade
    Gtk::Grid grid;
    
    // Declaração de todos os botões da calculadora
    // Cada botão é um objeto da classe Gtk::Button
    Gtk::Button button0, button1, button2, button3, button4;
    Gtk::Button button5, button6, button7, button8, button9;
    Gtk::Button buttonDot;        // Botão do ponto decimal (.)
    Gtk::Button buttonEquals;     // Botão de igual (=)
    Gtk::Button buttonPlus;       // Botão de adição (+)
    Gtk::Button buttonMinus;      // Botão de subtração (-)
    Gtk::Button buttonMultiply;   // Botão de multiplicação (*)
    Gtk::Button buttonDivide;     // Botão de divisão (/)
    Gtk::Button buttonClear;      // Botão de limpar (C)

private:
    // Variáveis para controlar o estado da calculadora
    double firstNumber;           // Armazena o primeiro número da operação
    std::string currentOperator;  // Armazena a operação atual (+, -, *, /)
    bool newNumber;               // Indica se devemos começar um número novo
};

#endif // CALCULATOR_HPP
