#include "calc.hpp"
#include <cstdlib>
#include <iostream>

// O construtor configura toda nossa interface gráfica
// Aqui inicializamos todos os botões com seus respectivos rótulos
CalculatorWindow::CalculatorWindow()
    : button0("0"), button1("1"), button2("2"), button3("3"), button4("4"),
      button5("5"), button6("6"), button7("7"), button8("8"), button9("9"),
      buttonDot("."), buttonEquals("="),
      buttonPlus("+"), buttonMinus("-"), buttonMultiply("×"), buttonDivide("÷"),
      buttonClear("C"),
      firstNumber(0),
      newNumber(true)
{
    // Configura o título e a borda da janela
    set_title("Calculadora GTK");
    set_border_width(10);

    // Adiciona a grid como o widget principal da janela
    add(grid);

    // Configura o display onde aparecerão os números
    display.set_text("0");                    // Começa mostrando zero
    display.set_alignment(Gtk::ALIGN_END);    // Alinha os números à direita
    display.set_editable(false);              // Impede edição direta pelo usuário

    // Organiza todos os widgets na grid
    // O método attach recebe: widget, coluna, linha, largura, altura
    grid.attach(display, 0, 0, 4, 1);         // Display ocupa 4 colunas
    grid.attach(buttonClear, 0, 1, 2, 1);     // Botão C ocupa 2 colunas
    grid.attach(buttonDivide, 2, 1, 1, 1);
    grid.attach(buttonMultiply, 3, 1, 1, 1);
    
    // Segunda linha de botões (7, 8, 9, -)
    grid.attach(button7, 0, 2, 1, 1);
    grid.attach(button8, 1, 2, 1, 1);
    grid.attach(button9, 2, 2, 1, 1);
    grid.attach(buttonMinus, 3, 2, 1, 1);
    
    // Terceira linha (4, 5, 6, +)
    grid.attach(button4, 0, 3, 1, 1);
    grid.attach(button5, 1, 3, 1, 1);
    grid.attach(button6, 2, 3, 1, 1);
    grid.attach(buttonPlus, 3, 3, 1, 1);
    
    // Quarta linha (1, 2, 3, =)
    grid.attach(button1, 0, 4, 1, 1);
    grid.attach(button2, 1, 4, 1, 1);
    grid.attach(button3, 2, 4, 1, 1);
    grid.attach(buttonEquals, 3, 4, 1, 2);    // = ocupa 2 linhas
    
    // Quinta linha (0, .)
    grid.attach(button0, 0, 5, 2, 1);         // 0 ocupa 2 colunas
    grid.attach(buttonDot, 2, 5, 1, 1);

    // Conecta os sinais (eventos) dos botões às funções correspondentes
    // Cada botão numérico chama on_button_clicked com seu próprio número
    button0.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "0"));
    button1.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "1"));
    button2.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "2"));
    button3.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "3"));
    button4.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "4"));
    button5.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "5"));
    button6.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "6"));
    button7.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "7"));
    button8.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "8"));
    button9.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "9"));
    buttonDot.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_button_clicked), "."));

    // Conecta os sinais dos operadores
    buttonPlus.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_operator_clicked), "+"));
    buttonMinus.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_operator_clicked), "-"));
    buttonMultiply.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_operator_clicked), "*"));
    buttonDivide.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &CalculatorWindow::on_operator_clicked), "/"));

    // Conecta os sinais dos botões especiais
    buttonEquals.signal_clicked().connect(sigc::mem_fun(*this, &CalculatorWindow::on_equals_clicked));
    buttonClear.signal_clicked().connect(sigc::mem_fun(*this, &CalculatorWindow::on_clear_clicked));

    // Mostra todos os widgets
    show_all_children();
}

// Destrutor - não precisa fazer nada especial
CalculatorWindow::~CalculatorWindow() {
}

// Função chamada quando um botão numérico é clicado
void CalculatorWindow::on_button_clicked(const Glib::ustring& digit) {
    Glib::ustring current_text = display.get_text();
    
    if (newNumber) {
        display.set_text(digit);
        newNumber = false;
    } else {
        // Evita múltiplos pontos decimais
        if (digit == "." && current_text.find('.') != Glib::ustring::npos) {
            return;
        }
        display.set_text(current_text + digit);
    }
}

// Função chamada quando um operador (+, -, *, /) é clicado
void CalculatorWindow::on_operator_clicked(const Glib::ustring& op) {
    firstNumber = std::stod(display.get_text());  // Converte o texto para número
    currentOperator = op;
    newNumber = true;
}

// Função chamada quando o botão C (clear) é clicado
void CalculatorWindow::on_clear_clicked() {
    display.set_text("0");
    firstNumber = 0;
    currentOperator.clear();
    newNumber = true;
}

// Função chamada quando o botão = é clicado
void CalculatorWindow::on_equals_clicked() {
    double secondNumber = std::stod(display.get_text());
    double result = 0;

    // Realiza a operação correspondente
    if (currentOperator == "+") {
        result = firstNumber + secondNumber;
    } else if (currentOperator == "-") {
        result = firstNumber - secondNumber;
    } else if (currentOperator == "*") {
        result = firstNumber * secondNumber;
    } else if (currentOperator == "/") {
        if (secondNumber != 0) {
            result = firstNumber / secondNumber;
        } else {
            display.set_text("Erro");
            return;
        }
    }

    // Mostra o resultado e prepara para nova operação
    display.set_text(Glib::ustring::format(result));
    newNumber = true;
}
