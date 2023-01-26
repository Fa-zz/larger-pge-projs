#include "stringButton.hpp"

stringButton::stringButton(int textStartX, int textStartY, string buttonText, int buttonStartX, int buttonStartY, int scaleX, int scaleY) {
    m_textStartX = textStartX;
    m_textStartY = textStartY;
    m_buttonText = buttonText;
    m_buttonStartX = buttonStartX;
    m_buttonStartY = buttonStartY;
    m_scaleX = scaleX;
    m_scaleY = scaleY;
}

void stringButton::setFunc(std::function<void(void)> func) {
    m_func = func;
}

void stringButton::setVisible(bool visible) {
    m_visible = visible;
}

// buttonFunctionality: Helper function that is called by
void stringButton::buttonFunctionality() {
    m_func;
}

bool stringButton::getVisible() {return m_visible;}
int stringButton::getTextStartX() {return m_textStartX;}
int stringButton::getTextStartY() {return m_textStartY;}
int stringButton::getButtonStartX() {return m_buttonStartX;}
int stringButton::getButtonStartY() {return m_buttonStartY;}
int stringButton::getScaleX() {return m_scaleX;}
int stringButton::getScaleY() {return m_scaleY;}
string stringButton::getButtonText() {return m_buttonText;}
