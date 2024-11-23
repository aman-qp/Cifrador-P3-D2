#include <SFML/Graphics.hpp>
#include "client.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "ErrorDetection.h"
#include "Security.h"
#include "Caesar.h"
#include "Vigenere.h"
#include "Sustitucion.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Caesar.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Vigenere.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Sustitucion.h"

class ClientGUI
{
public:
    ClientGUI(const char* serverIp, int port) :
        window(sf::VideoMode(850, 600), "Sistema de Cifrado"),
        client(new Client(serverIp, port)),
        currentState(State::MAIN_MENU),
        selectedAlgorithm(-1)
    {
        initializeGUI();
        window.setFramerateLimit(60);
    }

    ~ClientGUI() {
        delete client;
    }

    void run() {

        try {
            client->initializeConnection();
            while (window.isOpen()) {
                handleEvents();
                update();
                render();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }




private:
    sf::RenderWindow window;
    sf::Font font;
    Client* client;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    enum class State {
        MAIN_MENU,
        HISTORY
    } currentState;

    Caesar caesar;
    Vigenere vigenere;
    Sustitucion sustitucion;

    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        bool isHovered;
    };

    struct TextInput {
        sf::RectangleShape background;
        sf::Text text;
        sf::Text label;
        std::string content;
        bool isSelected;
        bool isMultiline;
        float maxWidth;
    };

    std::vector<Button> algorithmButtons;
    TextInput messageInput;
    TextInput keyInput;
    Button sendButton;
    Button historyButton;
    Button backButton;
    sf::Text serverResponse;
    sf::Text titleText;

    TextInput decryptedOutput;

    int selectedAlgorithm;
    std::vector<std::string> historyContent;

    void initializeGUI() {
        // Cargar fondo
        if (!backgroundTexture.loadFromFile("/home/amanda/CLionProjects/Cifrador-P3-D2/recs/D.jpg")) {
            throw std::runtime_error("No se pudo cargar la imagen de fondo");
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Ajustar la escala del fondo al tamaño de la ventana
        float scaleX = 850.0f / backgroundTexture.getSize().x;
        float scaleY = 600.0f / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);

        if (!font.loadFromFile("/home/amanda/CLionProjects/Cifrador-P3-D2/recs/Oxlaide.otf")) {
            throw std::runtime_error("No se pudo cargar la fuente");
        }

        // Inicializar título
        titleText.setFont(font);
        titleText.setString("Sistema de Cifrado");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(
            (850 - titleText.getLocalBounds().width) / 2.f,
            10.f
        );

        // Inicializar botones de algoritmos
        std::vector<std::string> algorithms = {"Cesar","Vigenere","Sustitucion"};
        float buttonY = 100.f;
        for (const auto& algo : algorithms) {
            Button btn;
            btn.shape.setSize(sf::Vector2f(200.f, 40.f));
            btn.shape.setPosition(50.f, buttonY);
            btn.shape.setFillColor(sf::Color(255, 255, 255, 200));
            btn.shape.setOutlineThickness(2.f);
            btn.shape.setOutlineColor(sf::Color::Black);

            btn.text.setFont(font);
            btn.text.setString(algo);
            btn.text.setCharacterSize(20);
            btn.text.setFillColor(sf::Color::Black);
            btn.text.setPosition(
                btn.shape.getPosition().x + (btn.shape.getSize().x - btn.text.getLocalBounds().width) / 2.f,
                btn.shape.getPosition().y + (btn.shape.getSize().y - btn.text.getLocalBounds().height) / 2.f
            );

            algorithmButtons.push_back(btn);
            buttonY += 60.f;
        }

        // Modificar messageInput para mejor manejo de texto
        messageInput.background.setSize(sf::Vector2f(500.f, 100.f));
        messageInput.background.setPosition(300.f, 100.f);
        messageInput.background.setFillColor(sf::Color(255, 255, 255, 230));
        messageInput.background.setOutlineThickness(2.f);
        messageInput.background.setOutlineColor(sf::Color::Black);

        messageInput.label.setFont(font);
        messageInput.label.setString("Mensaje:");
        messageInput.label.setCharacterSize(18);
        messageInput.label.setFillColor(sf::Color::White);
        messageInput.label.setPosition(300.f, 75.f);

        messageInput.text.setFont(font);
        messageInput.text.setCharacterSize(20);
        messageInput.text.setFillColor(sf::Color::Black);
        messageInput.text.setPosition(310.f, 110.f);
        messageInput.isSelected = false;
        messageInput.isMultiline = true;
        messageInput.maxWidth = 480.f;  // Leave some padding

        // Modificar keyInput
        keyInput.background.setSize(sf::Vector2f(500.f, 40.f));
        keyInput.background.setPosition(300.f, 240.f);
        keyInput.background.setFillColor(sf::Color(255, 255, 255, 230));
        keyInput.background.setOutlineThickness(2.f);
        keyInput.background.setOutlineColor(sf::Color::Black);

        keyInput.label.setFont(font);
        keyInput.label.setString("Clave:");
        keyInput.label.setCharacterSize(18);
        keyInput.label.setFillColor(sf::Color::White);
        keyInput.label.setPosition(300.f, 215.f);

        keyInput.text.setFont(font);
        keyInput.text.setCharacterSize(20);
        keyInput.text.setFillColor(sf::Color::Black);
        keyInput.text.setPosition(310.f, 250.f);
        keyInput.isSelected = false;
        keyInput.isMultiline = false;
        keyInput.maxWidth = 480.f;

        // Añadir campo para mensaje descifrado
        decryptedOutput.background.setSize(sf::Vector2f(500.f, 100.f));
        decryptedOutput.background.setPosition(300.f, 400.f);
        decryptedOutput.background.setFillColor(sf::Color(240, 240, 240, 230));
        decryptedOutput.background.setOutlineThickness(2.f);
        decryptedOutput.background.setOutlineColor(sf::Color::Black);

        decryptedOutput.label.setFont(font);
        decryptedOutput.label.setString("Mensaje Descifrado:");
        decryptedOutput.label.setCharacterSize(18);
        decryptedOutput.label.setFillColor(sf::Color::White);
        decryptedOutput.label.setPosition(300.f, 375.f);

        decryptedOutput.text.setFont(font);
        decryptedOutput.text.setCharacterSize(20);
        decryptedOutput.text.setFillColor(sf::Color::Black);
        decryptedOutput.text.setPosition(310.f, 410.f);
        decryptedOutput.isSelected = false;
        decryptedOutput.isMultiline = true;
        decryptedOutput.maxWidth = 480.f;

        // Botón de envío
        sendButton.shape.setSize(sf::Vector2f(150.f, 40.f));
        sendButton.shape.setPosition(300.f, 300.f);
        sendButton.shape.setFillColor(sf::Color(0, 0, 150, 230));
        sendButton.text.setFont(font);
        sendButton.text.setString("Enviar");
        sendButton.text.setCharacterSize(20);
        sendButton.text.setFillColor(sf::Color::White);
        sendButton.text.setPosition(
            sendButton.shape.getPosition().x + 45.f,
            sendButton.shape.getPosition().y + 10.f
        );

        // Botón de historial
        historyButton.shape.setSize(sf::Vector2f(150.f, 40.f));
        historyButton.shape.setPosition(50.f, 300.f);
        historyButton.shape.setFillColor(sf::Color(0, 0, 150, 230));
        historyButton.text.setFont(font);
        historyButton.text.setString("Historial");
        historyButton.text.setCharacterSize(20);
        historyButton.text.setFillColor(sf::Color::White);
        historyButton.text.setPosition(
            historyButton.shape.getPosition().x + 35.f,
            historyButton.shape.getPosition().y + 10.f
        );

        // Botón de volver (para el historial)
        backButton.shape.setSize(sf::Vector2f(150.f, 40.f));
        backButton.shape.setPosition(50.f, 520.f);
        backButton.shape.setFillColor(sf::Color(150, 0, 0, 230));
        backButton.text.setFont(font);
        backButton.text.setString("Volver");
        backButton.text.setCharacterSize(20);
        backButton.text.setFillColor(sf::Color::White);
        backButton.text.setPosition(
            backButton.shape.getPosition().x + 45.f,
            backButton.shape.getPosition().y + 10.f
        );

        // Texto de respuesta del servidor
        serverResponse.setFont(font);
        serverResponse.setCharacterSize(18);
        serverResponse.setFillColor(sf::Color::White);
        serverResponse.setPosition(300.f, 360.f);


    }





private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejar scroll con la rueda del mouse
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (currentState == State::HISTORY) {
                    scrollOffset += event.mouseWheelScroll.delta * SCROLL_SPEED;

                    // Limitar el scroll
                    if (scrollOffset > 0) scrollOffset = 0;
                    if (scrollOffset < -maxScrollOffset) scrollOffset = -maxScrollOffset;
                }
            }

            handleMouseEvents(event);
            handleKeyboardEvents(event);
        }
    }

    void updateButtonStates() {
        for (size_t i = 0; i < algorithmButtons.size(); i++) {
            algorithmButtons[i].shape.setFillColor(
                (i + 1 == selectedAlgorithm) ? sf::Color(200, 200, 200) : sf::Color::White
            );
        }
    }


   void handleMouseEvents(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (currentState == State::MAIN_MENU) {
                // Manejar clicks en botones de algoritmos
                for (size_t i = 0; i < algorithmButtons.size(); i++) {
                    if (algorithmButtons[i].shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selectedAlgorithm = i + 1;
                        updateButtonStates();
                    }
                }

                // Manejar clicks en campos de texto
                messageInput.isSelected = messageInput.background.getGlobalBounds().contains(mousePos.x, mousePos.y);
                keyInput.isSelected = keyInput.background.getGlobalBounds().contains(mousePos.x, mousePos.y);

                // Actualizar visuales de selección
                messageInput.background.setOutlineColor(messageInput.isSelected ? sf::Color::Blue : sf::Color::Black);
                keyInput.background.setOutlineColor(keyInput.isSelected ? sf::Color::Blue : sf::Color::Black);

                // Manejar click en botón de envío
                if (sendButton.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    sendMessage();
                }

                // Manejar click en botón de historial
                if (historyButton.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    loadHistory();
                    currentState = State::HISTORY;
                }
            }
            else if (currentState == State::HISTORY) {
                // Manejar click en botón de volver
                if (backButton.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentState = State::MAIN_MENU;
                }
            }
        }
    }

    void initializeTextInputs() {
        // Configuración del campo de mensaje
        messageInput.isMultiline = true;
        messageInput.maxWidth = 480.f;
        messageInput.background.setOutlineThickness(2.f);
        messageInput.background.setFillColor(sf::Color(255, 255, 255, 230));
        messageInput.text.setCharacterSize(20);
        messageInput.text.setFillColor(sf::Color::Black);

        // Configuración del campo de clave
        keyInput.isMultiline = false;
        keyInput.maxWidth = 480.f;
        keyInput.background.setOutlineThickness(2.f);
        keyInput.background.setFillColor(sf::Color(255, 255, 255, 230));
        keyInput.text.setCharacterSize(20);
        keyInput.text.setFillColor(sf::Color::Black);

        // Inicializar las posiciones y tamaños
        messageInput.background.setPosition(300.f, 100.f);
        messageInput.background.setSize(sf::Vector2f(500.f, 100.f));
        messageInput.text.setPosition(310.f, 110.f);

        keyInput.background.setPosition(300.f, 240.f);
        keyInput.background.setSize(sf::Vector2f(500.f, 40.f));
        keyInput.text.setPosition(310.f, 250.f);
    }

    void handleKeyboardEvents(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if (messageInput.isSelected) {
                handleTextInput(messageInput, event.text.unicode);
            }
            else if (keyInput.isSelected) {
                handleTextInput(keyInput, event.text.unicode);
            }
        }
    }

    void handleTextInput(TextInput& input, uint32_t unicode)
    {
        // Manejar backspace
        if (unicode == 8) { // Backspace
            if (!input.content.empty()) {
                input.content.pop_back();
            }
        }
        // Manejar enter para campos multilinea
        else if (unicode == 13 && input.isMultiline) { // Enter key
            input.content += '\n';
        }
        // Manejar caracteres imprimibles
        else if (unicode >= 32 && unicode < 128) {
            // Para campos no multilinea, verificar que no exceda el ancho máximo
            if (!input.isMultiline) {
                sf::Text tempText(input.content + static_cast<char>(unicode), font, input.text.getCharacterSize());
                if (tempText.getLocalBounds().width <= input.maxWidth) {
                    input.content += static_cast<char>(unicode);
                }
            } else {
                input.content += static_cast<char>(unicode);
            }
        }

        // Actualizar el texto visible
        if (input.isMultiline) {
            // Para campos multilinea, manejar el wrapping de texto
            std::string displayText;
            std::string currentLine;
            std::istringstream stream(input.content);
            std::string line;

            while (std::getline(stream, line, '\n')) {
                std::string word;
                std::istringstream lineStream(line);
                std::string currentLineTemp;

                while (lineStream >> word) {
                    std::string tempLine = currentLineTemp;
                    if (!tempLine.empty()) {
                        tempLine += ' ';
                    }
                    tempLine += word;

                    sf::Text tempText(tempLine, font, input.text.getCharacterSize());
                    if (tempText.getLocalBounds().width > input.maxWidth) {
                        if (!currentLineTemp.empty()) {
                            displayText += currentLineTemp + '\n';
                            currentLineTemp = word;
                        } else {
                            displayText += word + '\n';
                        }
                    } else {
                        currentLineTemp = tempLine;
                    }
                }
                displayText += currentLineTemp + '\n';
            }

            // Eliminar el último salto de línea si existe
            if (!displayText.empty() && displayText.back() == '\n') {
                displayText.pop_back();
            }

            input.text.setString(displayText);
        } else {
            // Para campos de una sola línea, simplemente actualizar el texto
            input.text.setString(input.content);
        }

        // Centrar verticalmente el texto en el campo
        float textHeight = input.text.getLocalBounds().height;
        float backgroundHeight = input.background.getSize().y;
        float newY = input.background.getPosition().y + (backgroundHeight - textHeight) / 2.0f;
        input.text.setPosition(input.text.getPosition().x, newY);
    }



   void sendMessage() {
        if (selectedAlgorithm == -1) {
            serverResponse.setString("Por favor seleccione un algoritmo de cifrado");
            return;
        }

        if (messageInput.content.empty()) {
            serverResponse.setString("Por favor ingrese un mensaje");
            return;
        }

        if (keyInput.content.empty()) {
            serverResponse.setString("Por favor ingrese una clave");
            return;
        }

        try {
            std::string message = messageInput.content;
            std::string key = keyInput.content;
            std::string encryptedMessage;
            std::string cipherInfo;

            bool isKeyValid = false;
            switch(selectedAlgorithm) {
                case 1: // César
                    isKeyValid = caesar.validateKey(key);
                    if (isKeyValid) {
                        encryptedMessage = caesar.encrypt(message, key);
                        cipherInfo = "CAESAR:" + key + ":" + encryptedMessage;
                    }
                    break;
                case 2: // Vigenère
                    isKeyValid = vigenere.validateKey(key);
                    if (isKeyValid) {
                        encryptedMessage = vigenere.encrypt(message, key);
                        cipherInfo = "VIGENERE:" + key + ":" + encryptedMessage;
                    }
                    break;
                case 3: // Sustitución
                    isKeyValid = sustitucion.validateKey(key);
                    if (isKeyValid) {
                        encryptedMessage = sustitucion.encrypt(message, key);
                        cipherInfo = "SUSTITUCION:" + key + ":" + encryptedMessage;
                    }
                    break;
                default:
                    serverResponse.setString("Algoritmo no válido");
                    return;
            }

            if (!isKeyValid) {
                serverResponse.setString("Clave inválida para el algoritmo seleccionado");
                serverResponse.setFillColor(sf::Color::Red);
                return;
            }

            // Generar checksum del mensaje cifrado
            std::string checksum = ErrorDetection::generateChecksum(encryptedMessage);

            // Añadir el checksum al mensaje
            cipherInfo += ":" + checksum + ":";

            // Enviar el mensaje cifrado al servidor
            if (send(client->getSocket(), cipherInfo.c_str(), cipherInfo.length(), 0) < 0) {
                serverResponse.setString("Error al enviar el mensaje");
                serverResponse.setFillColor(sf::Color::Red);
                return;
            }

            // Recibir la respuesta del servidor
            char buffer[1024] = {0};
            ssize_t bytesRead = recv(client->getSocket(), buffer, 1024, 0);

            if (bytesRead > 0) {
                std::string response(buffer);

                if (response.find("ERROR:") != std::string::npos) {
                    serverResponse.setString(response);
                    serverResponse.setFillColor(sf::Color::Red);
                } else {
                    size_t pos = response.find(": ");
                    if (pos != std::string::npos) {
                        std::string decryptedMsg = response.substr(pos + 2);

                        // Obtener el tipo de cifrado como string
                        std::string cipherType =
                            selectedAlgorithm == 1 ? std::string("CAESAR") :
                            selectedAlgorithm == 2 ? std::string("VIGENERE") :
                            std::string("SUSTITUCION");

                        // Crear el mensaje formateado usando std::string
                        std::string formattedMessage =
                            std::string("Tipo de cifrado: ") + cipherType +
                            std::string("\nMensaje cifrado: ") + encryptedMessage +
                            std::string("\nMensaje descifrado: ") + decryptedMsg;

                        // Actualizar el contenido y texto del campo de salida
                        decryptedOutput.content = formattedMessage;
                        decryptedOutput.text.setString(formattedMessage);

                        // Actualizar el mensaje de estado
                        serverResponse.setString("Mensaje procesado con exito");
                        serverResponse.setFillColor(sf::Color::Red);
                    }
                }
            } else {
                serverResponse.setString("Error al recibir respuesta del servidor");
                serverResponse.setFillColor(sf::Color::Red);
            }
        }
        catch (const std::exception& e) {
            serverResponse.setString("Error: " + std::string(e.what()));
            serverResponse.setFillColor(sf::Color::Red);
        }
    }

    void loadHistory() {
        historyContent.clear();
        std::ifstream file("cipher_history.json");
        if (file.is_open()) {
            try {
                nlohmann::json data = nlohmann::json::parse(file);
                for (const auto& record : data) {
                    historyContent.push_back(record.dump());
                }
            } catch (const nlohmann::json::exception& e) {

            }
            file.close();
        }
    }

    void update() {
        // Actualizar estados de hover de botones
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        for (auto& btn : algorithmButtons) {
            btn.isHovered = btn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
            if (btn.isHovered) {
                btn.shape.setFillColor(sf::Color(230, 230, 230));
            }
        }

        sendButton.isHovered = sendButton.shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
        if (sendButton.isHovered) {
            sendButton.shape.setFillColor(sf::Color(0, 180, 0));
        } else {
            sendButton.shape.setFillColor(sf::Color::Green);
        }
    }

    void render() {
        window.clear(sf::Color(240, 240, 240));

        switch (currentState) {
        case State::MAIN_MENU:
            renderMainMenu();
            break;
        case State::HISTORY:
            renderHistory();
            break;
        default:
            break;
        }

        window.display();
    }

    void renderMainMenu() {
        window.draw(backgroundSprite);
        window.draw(titleText);

        // Renderizar botones de algoritmos
        for (const auto& btn : algorithmButtons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }

        // Renderizar campos de texto con etiquetas
        window.draw(messageInput.label);
        window.draw(messageInput.background);
        window.draw(messageInput.text);

        window.draw(keyInput.label);
        window.draw(keyInput.background);
        window.draw(keyInput.text);

        // Renderizar campo de mensaje descifrado
        window.draw(decryptedOutput.label);
        window.draw(decryptedOutput.background);
        window.draw(decryptedOutput.text);

        // Renderizar botones
        window.draw(sendButton.shape);
        window.draw(sendButton.text);
        window.draw(historyButton.shape);
        window.draw(historyButton.text);

        // Renderizar respuesta del servidor
        window.draw(serverResponse);
    }

    float scrollOffset = 0.0f;
    float maxScrollOffset = 0.0f;
    const float SCROLL_SPEED = 20.0f;

    void renderHistory() {
    window.draw(backgroundSprite);

    // Título del historial
    sf::Text historyTitle;
    historyTitle.setFont(font);
    historyTitle.setString("Historial de Mensajes");
    historyTitle.setCharacterSize(24);
    historyTitle.setFillColor(sf::Color::White);
    historyTitle.setPosition(50.f, 20.f);
    window.draw(historyTitle);

    // Crear un área visible para el scroll
    sf::RectangleShape scrollArea;
    scrollArea.setPosition(50.f, 70.f);
    scrollArea.setSize(sf::Vector2f(750.f, 430.f)); // Ajusta según necesites
    scrollArea.setFillColor(sf::Color(0, 0, 0, 40));
    window.draw(scrollArea);

    // Crear una vista para el área de scroll
    sf::View scrollView;
    scrollView.reset(sf::FloatRect(0.f, 0.f, 750.f, 430.f));
    scrollView.setViewport(sf::FloatRect(
        scrollArea.getPosition().x / window.getSize().x,
        scrollArea.getPosition().y / window.getSize().y,
        scrollArea.getSize().x / window.getSize().x,
        scrollArea.getSize().y / window.getSize().y
    ));

    // Guardar la vista actual
    sf::View defaultView = window.getView();

    // Configurar la vista de scroll
    window.setView(scrollView);

    // Renderizar el contenido del historial
    float yPos = scrollOffset;
    for (const auto& historyItem : historyContent) {
        // Parsear el JSON para extraer los datos
        try {
            nlohmann::json record = nlohmann::json::parse(historyItem);

            // Crear un contenedor visual para cada entrada
            sf::RectangleShape entryBg;
            entryBg.setPosition(10.f, yPos);
            entryBg.setSize(sf::Vector2f(730.f, 140.f));
            entryBg.setFillColor(sf::Color(255, 255, 255, 200));
            window.draw(entryBg);

            // Renderizar la información formateada
            sf::Text entryText;
            entryText.setFont(font);
            entryText.setCharacterSize(16);
            entryText.setFillColor(sf::Color::Black);

            // Formatear el texto de manera más amigable
            std::string formattedText =
                "Fecha: " + record["timestamp"].get<std::string>() + "\n" +
                "Tipo de cifrado: " + record["type"].get<std::string>() + "\n" +
                "Mensaje cifrado: " + record["encrypted"].get<std::string>() + "\n" +
                "Mensaje descifrado: " + record["decrypted"].get<std::string>();

            entryText.setString(formattedText);
            entryText.setPosition(20.f, yPos + 10.f);
            window.draw(entryText);

            yPos += 160.f; // Espacio entre entradas
        } catch (const nlohmann::json::exception& e) {
            continue;
        }
    }

    // Actualizar maxScrollOffset
    maxScrollOffset = std::max(0.f, yPos - 430.f);

    // Restaurar la vista original
    window.setView(defaultView);

    // Renderizar botón de volver
    window.draw(backButton.shape);
    window.draw(backButton.text);
    }

};