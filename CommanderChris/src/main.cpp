#include <iostream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

Element TermUI(Component &leftContainer) {
    // Return a horizontal box containing two vertical boxes separated by a separator.
    return hbox({
        // Left vertical box containing the terminal input.
        vbox({
            text(L"Terminal"),
            leftContainer->Render() | border,
        }) | flex,
        separator(),
        // Right vertical box with application details.
        vbox({
            text(L"CommanderChris") | bold,
            text(L"Details and content can be added here."),
        }) | flex,
    });
}

void run_term() {
    // Set the placeholder text for the input field.
    std::string placeholder = "Type commands here...";
    std::string input_content;

    // Create an input component where users can type commands.
    Component input_field = Input({.content = &input_content, .placeholder = &placeholder});

    // Create a vertical container and add the input component to it.
    Component leftContainer = Container::Vertical({
        input_field,
    });
    
    auto screen = ScreenInteractive::TerminalOutput();
    auto renderer = Renderer(leftContainer, [&] { return TermUI(leftContainer); });
    auto component = CatchEvent(renderer, [&](Event event) {
      if (event == Event::Special({10})) {
        std::cout << exec(input_content);
        return true;
      }
      return false;
    });
    // Enter the main loop for the screen with the layout renderer.
    screen.Loop(component);
}

// Main function to run the program.
int main() {
    run_term();

    return 0;
}
