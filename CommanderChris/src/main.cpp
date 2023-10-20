#include <iostream>
#include <vector>
#include <sstream>  
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "ftxui/component/component_options.hpp"
#include "ftxui/screen/color.hpp"
#include "scroller.hpp"

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

Element TermUI(std::vector<Element> term_lines, Component &input_box) {
    // Return a horizontal box containing two vertical boxes separated by a separator.
    return hbox({
        // Left vertical box containing the terminal input.
        vbox({
            text(L"Terminal"),
            vbox({
                vbox(std::move(term_lines)) | focusPositionRelative(0, 1) | yframe | flex,
                separator(),
                input_box->Render(),
            }) | border,
        }) | flex,
        separator(),
        // Right vertical box with application details.
        vbox({
            text(L"CommanderChris") | bold,
            text(L"Details and content can be added here."),
        }),
    });
}

void run_term() {
    // Set the placeholder text for the input field.
    std::string placeholder = "Type commands here...";
    std::string input_content;
    std::vector<Element> term_lines;

    // Create an input component where users can type commands.
    InputOption opt = InputOption::Default();
    opt.content = &input_content;
    opt.placeholder = &placeholder;
    opt.on_enter = [&]{
        term_lines.emplace_back(color(Color::Grey0, text(input_content)));
        std::string out = exec(input_content);
        std::stringstream out_stream(out);
        std::string line;
        while(getline(out_stream, line, '\n')) term_lines.emplace_back(text(line));
        input_content = "";
    };
    Component input_box = Input(opt);
    
    auto screen = ScreenInteractive::Fullscreen();
    auto renderer = Renderer(input_box, [&] { return TermUI(term_lines, input_box); });
    // Enter the main loop for the screen with the layout renderer.
    screen.Loop(renderer);
}

// Main function to run the program.
int main() {
    run_term();

    return 0;
}
