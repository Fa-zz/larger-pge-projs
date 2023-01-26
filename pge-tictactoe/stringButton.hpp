#pragma once
using namespace std;
#include <string>
/*
There are two types of Button, the stringButton and imageButton

Button
public:
    Button(); // default constructor
private:
    int m_startX; // X coord of button start
    int m_startY; // Y coord of button start
    int m_endX; // X coord of button end
    int m_startY; // Y coord of button end

stringButton
public:
    stringButton(string m_ButtonText ... ); // default constructor
private:
    string m_buttonText; // text of the button
    createButton(); // would construct a button using available information

stringButton:
Firstly, I only want to be able to specify a coords and a text string. The class should fill a rect around
the text by itself. To account for text height, the class should detect usage of '\n' and add an appropriate amount to
the rect height

To account for text width, the class should build 

*/

class stringButton {
public:
    stringButton(int textStartX, int textStartY, std::string buttonText, int buttonStartX, int buttonStartY, int scaleX, int scaleY);

    void setFunc(std::function<void(void)> func);

    void setVisible(bool visible);

    // buttonFunctionality: Helper function that is called by
    void buttonFunctionality();

    bool getVisible();
    int getTextStartX();
    int getTextStartY();
    int getButtonStartX();
    int getButtonStartY();
    int getScaleX();
    int getScaleY();
    std::string getButtonText();


private:
    int m_textStartX, m_textStartY; 
    string m_buttonText;
    int m_buttonStartX, m_buttonStartY;
    int m_scaleX, m_scaleY;
    std::function<void(void)> m_func;
    bool m_visible;
};
