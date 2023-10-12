#include <iostream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

// Define a class for the user interface named CommanderUI.
class CommanderUI {
public:
    // Constructor for the CommanderUI class.
    CommanderUI() {
        // Set the placeholder text for the input field.
        placeholder = "Type commands here...";

        // Create an input component where users can type commands.
        input_field = Input(&input_content, {.placeholder = &placeholder});

        // Create a vertical container and add the input component to it.
        leftContainer = Container::Vertical({
            input_field,
        });
    }

    // Function to run the UI.
    void Run() {
        // Create a terminal output screen.
        auto screen = ScreenInteractive::TerminalOutput();

        // Enter the main loop for the screen with the layout renderer.
        screen.Loop(Renderer(leftContainer, [this] { return RenderLayout(); }));
    }

private:
    // Private member variables.
    std::string input_content;  // Holds the content of the input field.
    std::string placeholder;    // Placeholder text for the input field.
    Component input_field;      // The input component.
    Component leftContainer;    // Container holding the input component.

    // Function to render the layout of the UI.
    Element RenderLayout() {
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
};

// Main function to run the program.
int main() {
    // Instantiate the CommanderUI class.
    CommanderUI ui;

    // Run the UI.
    ui.Run();

    return 0;
}
