#include "calc.hpp"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    CalculatorWindow calc;

    return app->run(calc);
}
