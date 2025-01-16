#include "TextFieldEntity.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "ecs/callback/EventCallbacks.hpp"
#include "ecs/component/EventHandlerComponent.hpp"
#include "ecs/component/HoverComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/SelectionComponent.hpp"
#include "ecs/component/TextComponent.hpp"

TextFieldEntity::TextFieldEntity(EntityManager &entityManager, sf::RenderWindow &window, sf::Vector2f size,
    sf::Vector2f position, const sf::Font &font, const std::string &title)
    : entity_(entityManager.createEntity()), titleEntity_(entityManager.createEntity()), entityManager_(entityManager),
      window_(window)
{
    auto &eventHandlerComponent =
        entity_.addComponent<EventHandlerComponent>(sf::Event::KeyPressed, [this](const sf::Event &event) {
            EventCallbacks::TextFieldListener(event, window_, *this);
        });
    eventHandlerComponent.setCallback(sf::Event::KeyReleased, [this](const sf::Event &event) {
        EventCallbacks::TextFieldCapsReleaseHandler(event, window_, *this);
    });
    entity_.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    position.y -= size.y;
    titleEntity_.addComponent<TextComponent>(title, font, position, sf::Color(208, 208, 208));
    position.y += size.y;
    entity_.addComponent<TextComponent>("", font, position, sf::Color(228, 228, 228));
    entity_.addComponent<SelectionComponent>();
    entity_.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(100, 100, 100));
    conversion_ = 'a';
}

bool TextFieldEntity::isSelected() const
{
    auto *selectionComponent = entity_.getComponent<SelectionComponent>();
    return selectionComponent->isSelected;
}

void TextFieldEntity::addKey(sf::Keyboard::Key key)
{
    sf::Text &text = entity_.getComponent<TextComponent>()->data;
    std::string buffer = text.getString();
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z) {
        char c = static_cast<char>(conversion_ + key);
        buffer.push_back(c);
    } else {
        std::string toAppend;
        switch (key) {
            case sf::Keyboard::Num0: //!< The 0 key
            case sf::Keyboard::Num1: //!< The 1 key
            case sf::Keyboard::Num2: //!< The 2 key
            case sf::Keyboard::Num3: //!< The 3 key
            case sf::Keyboard::Num4: //!< The 4 key
            case sf::Keyboard::Num5: //!< The 5 key
            case sf::Keyboard::Num6: //!< The 6 key
            case sf::Keyboard::Num7: //!< The 7 key
            case sf::Keyboard::Num8: //!< The 8 key
            case sf::Keyboard::Num9: //!< The 9 key
                toAppend.push_back(static_cast<char>('0' + key - sf::Keyboard::Num0));
                break;
            case sf::Keyboard::Escape: //!< The Escape key
                entity_.getComponent<SelectionComponent>()->isSelected = false;
            case sf::Keyboard::LControl: //!< The left Control key
            case sf::Keyboard::LShift:   //!< The left Shift key
            case sf::Keyboard::LAlt:     //!< The left Alt key
            case sf::Keyboard::LSystem:  //!< The left OS specific key: window (Windows and Linux), apple (macOS), ...
            case sf::Keyboard::RControl: //!< The right Control key
            case sf::Keyboard::RShift:   //!< The right Shift key
            case sf::Keyboard::RAlt:     //!< The right Alt key
            case sf::Keyboard::RSystem:  //!< The right OS specific key: window (Windows and Linux), apple (macOS), ...
            case sf::Keyboard::Menu:     //!< The Menu key
                break;
            case sf::Keyboard::LBracket: //!< The [ key
                toAppend.push_back('[');
                break;
            case sf::Keyboard::RBracket: //!< The ] key
                toAppend.push_back(']');
                break;
            case sf::Keyboard::Semicolon: //!< The ; key
                toAppend.push_back(';');
                break;
            case sf::Keyboard::Comma: //!< The , key
                toAppend.push_back(',');
                break;
            case sf::Keyboard::Period: //!< The . key
                toAppend.push_back('.');
                break;
            case sf::Keyboard::Apostrophe: //!< The ' key
                toAppend.push_back('\'');
                break;
            case sf::Keyboard::Slash: //!< The / key
                toAppend.push_back('/');
                break;
            case sf::Keyboard::Backslash: //!< The \ key
                toAppend.push_back('\\');
                break;
            case sf::Keyboard::Grave: //!< The ` key
                toAppend.push_back('`');
                break;
            case sf::Keyboard::Equal: //!< The = key
                toAppend.push_back('=');
                break;
            case sf::Keyboard::Hyphen: //!< The - key (hyphen)
                toAppend.push_back('-');
                break;
            case sf::Keyboard::Space: //!< The Space key
                toAppend.push_back(' ');
                break;
            case sf::Keyboard::Enter: //!< The Enter/Return keys
                toAppend.push_back('\n');
                break;
            case sf::Keyboard::Backspace: //!< The Backspace key
                if (buffer.size() != 0)
                    buffer.erase(buffer.end() - 1);
                break;
            case sf::Keyboard::Tab: //!< The Tabulation key
                toAppend.push_back('\t');
                break;
            case sf::Keyboard::PageUp:   //!< The Page up key
            case sf::Keyboard::PageDown: //!< The Page down key
            case sf::Keyboard::End:      //!< The End key
            case sf::Keyboard::Home:     //!< The Home key
            case sf::Keyboard::Insert:   //!< The Insert key
            case sf::Keyboard::Delete:   //!< The Delete key
                break;
            case sf::Keyboard::Add: //!< The + key
                toAppend.push_back('+');
                break;
            case sf::Keyboard::Subtract: //!< The - key (minus, usually from numpad)
                toAppend.push_back('-');
                break;
            case sf::Keyboard::Multiply: //!< The * key
                toAppend.push_back('*');
                break;
            case sf::Keyboard::Divide: //!< The / key
                toAppend.push_back('/');
                break;
            case sf::Keyboard::Left:  //!< Left arrow
            case sf::Keyboard::Right: //!< Right arrow
            case sf::Keyboard::Up:    //!< Up arrow
            case sf::Keyboard::Down:  //!< Down arrow
                break;
            case sf::Keyboard::Numpad0: //!< The numpad 0 key
            case sf::Keyboard::Numpad1: //!< The numpad 1 key
            case sf::Keyboard::Numpad2: //!< The numpad 2 key
            case sf::Keyboard::Numpad3: //!< The numpad 3 key
            case sf::Keyboard::Numpad4: //!< The numpad 4 key
            case sf::Keyboard::Numpad5: //!< The numpad 5 key
            case sf::Keyboard::Numpad6: //!< The numpad 6 key
            case sf::Keyboard::Numpad7: //!< The numpad 7 key
            case sf::Keyboard::Numpad8: //!< The numpad 8 key
            case sf::Keyboard::Numpad9: //!< The numpad 9 key
                toAppend.push_back(static_cast<char>('0' + key - sf::Keyboard::Numpad0));
                break;
            case sf::Keyboard::F1:    //!< The F1 key
            case sf::Keyboard::F2:    //!< The F2 key
            case sf::Keyboard::F3:    //!< The F3 key
            case sf::Keyboard::F4:    //!< The F4 key
            case sf::Keyboard::F5:    //!< The F5 key
            case sf::Keyboard::F6:    //!< The F6 key
            case sf::Keyboard::F7:    //!< The F7 key
            case sf::Keyboard::F8:    //!< The F8 key
            case sf::Keyboard::F9:    //!< The F9 key
            case sf::Keyboard::F10:   //!< The F10 key
            case sf::Keyboard::F11:   //!< The F11 key
            case sf::Keyboard::F12:   //!< The F12 key
            case sf::Keyboard::F13:   //!< The F13 key
            case sf::Keyboard::F14:   //!< The F14 key
            case sf::Keyboard::F15:   //!< The F15 key
            case sf::Keyboard::Pause: //!< The Pause key
                break;
            default: break;
        }
        buffer.append(toAppend);
    }
    text.setString(buffer);
}

std::string TextFieldEntity::getFieldValue() const
{
    auto *textComponent = entity_.getComponent<TextComponent>();
    return textComponent->data.getString();
}

void TextFieldEntity::setFieldValue(const std::string &value)
{
    auto *textComponent = entity_.getComponent<TextComponent>();
    textComponent->data.setString(value);
}

void TextFieldEntity::setCaps()
{
    conversion_ = 'A';
}

void TextFieldEntity::unsetCaps()
{
    conversion_ = 'a';
}

Entity &TextFieldEntity::getEntity()
{
    return entity_;
}

Entity &TextFieldEntity::getTitleEntity()
{
    return titleEntity_;
}
