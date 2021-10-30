#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cmath>
#include <exception>
#include <cassert>
#include <iomanip>
#include <list>
#include<string>
#include<sstream>


//dz1
class DivisionByZero
{
public:
    DivisionByZero(const std::string& message = "") : m_message(message)       
    {
        m_message = "Exception: " + m_message;
    }
    virtual const std::string& GetMessage() const
    {
        return m_message;
    }
protected:
    std::string m_message{};
};

template <typename T>
double Div(T a, T b)
{
    if (b == 0)
        throw DivisionByZero("cannot be divided by 0\n");

    return a / b;
}
//dz2
class Ex
{
public:
    Ex(double X) : x(X) {};
    double x;
};

class Bar
{
public:
    Bar(double Y = 0.0) : y(Y) {}

    void Set(double a)
    {
        if ((y + a) > 100)
            throw Ex(a * y);
        else
            y = a;
    }

private:
    double y;
};

template<typename T>
T GetUserInput(const std::string& prompt)
{
    while (true) {
        T value;

        std::cout << prompt << ": ";
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<T>::max(), '\n');
            std::cout << "Incorrect input, enter again." << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<T>::max(), '\n');
            return value;
        }
    }
}

const int CELL_SIZE = 10;

class RobotException
{
public:
    RobotException(int currentX, int currentY, int newX, int newY) :
        m_currentPositionX(currentX),
        m_currentPositionY(currentY),
        m_newPositionX(newX),
        m_newPositionY(newY),
        m_message("")
    {}

    const std::string& GetInfo() const {return m_message; }
    const std::string& GetCoordinates()
    {
        m_coords = "from (" + std::to_string(m_currentPositionX) + ", " + std::to_string(m_currentPositionY) + ") v (" +
        std::to_string(m_newPositionX) + ", " + std::to_string(m_newPositionY) + ")";
        return m_coords;
    }

protected:
    int m_currentPositionX{ 0 };
    int m_currentPositionY{ 0 };
    int m_newPositionX{ 0 };
    int m_newPositionY{ 0 };
    std::string m_message{};
    std::string m_coords{};
};

class OffTheField : public RobotException
{
public:
    OffTheField(int currentX, int currentY, int newX, int newY) :
        RobotException(currentX, currentY, newX, newY)
    {
        if (m_newPositionX > CELL_SIZE || m_newPositionX < 1) {
            m_message = "Error, out of bounds on the x-axis" + GetCoordinates();
        }

        if (m_newPositionY > CELL_SIZE || m_newPositionY < 1) {
            m_message = "Error, out of bounds on the y-axis" + GetCoordinates();
        }
    }
};

class IllegalCommand : public RobotException
{
public:
    IllegalCommand(int currentX, int currentY, int newX, int newY) :
        RobotException(currentX, currentY, newX, newY)
    {
        if (std::abs(m_currentPositionX - m_newPositionX) > 1) {
            m_message = "Too large a step of movement along the x-axis" + GetCoordinates();
        }

        if (std::abs(m_currentPositionY - m_newPositionY) > 1) {
            m_message = "Too large a step of movement along the y-axis" + GetCoordinates();
        }

        if (m_currentPositionX == m_newPositionX && m_currentPositionY == m_newPositionY) {
            m_message = "The move did not happen (we stand still) " + GetCoordinates();
        }
    }
};

class Robot
{
public:
    Robot(int x = CELL_SIZE / 2, int y = CELL_SIZE / 2) : m_positionX(x), m_positionY(y)
    {
        std::cout << "Starting position " << m_positionX << ", " << m_positionY << std::endl;
    }

    void Move(int x, int y)
    {
        if (abs(m_positionX - x) > 1 || abs(m_positionY - y) > 1 || (m_positionX == x && m_positionY == y))
            throw IllegalCommand(m_positionX, m_positionY, x, y);
        if (x > CELL_SIZE || x < 1 || y > CELL_SIZE || y < 1)
            throw OffTheField(m_positionX, m_positionY, x, y);
        std::cout << "Moving from (" << m_positionX << ", " << m_positionY << ") v (" << x << ", " << y << ")" << std::endl << std::endl;
        m_positionX = x;
        m_positionY = y;
    }

private:
    int m_positionX{};
    int m_positionY{};
};

int main()
{
 //dz1
    try
    {
        double result = Div(100, 0);
        std::cout << result << std::endl;
    
    }
    catch (const DivisionByZero& e)
    {
        std::cout << e.GetMessage();
    }
    std::cout << std::endl;
    
//dz2
    Bar bar;
    int b;

    try {
        while (true) {
            b = GetUserInput<int>("Enter integer number (to exit enter 0)");
            if (b == 0)
                break;
            bar.Set(b);
        }
    }
    catch (const Ex& e)
    {
        std::cout << "Exception: " << e.x << std::endl;
    }
    std::cout << std::endl;

    //dz3
    int x = 0, y = 0;
    Robot robot;

    while (true) {
        std::cout << "Enter 0 for x and 0 y for exit." << std::endl;
        x = GetUserInput<int>("Enter the position for the x-axis (1 to 10)");
        y = GetUserInput<int>("Enter the position for the y-axis (1 to 10)");

        if (x == 0 && y == 0)
            break;

        try {
            robot.Move(x, y);
        }
        catch (const IllegalCommand& e) {
            std::cout << e.GetInfo() << std::endl;
        }
        catch (const OffTheField& e) {
            std::cout << e.GetInfo() << std::endl;
        }
    }
}
