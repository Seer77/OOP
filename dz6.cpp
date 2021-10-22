#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <random>
#include <chrono>
#include <cstdio>
#include <string>
#include <fstream>
#include <array>
#include <cmath>

//dz1
void Number_Input()
{
    int number = 0;
    while (true)
    {
        std::cout << "Enter an integer: ";
        std::cin >> number;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<int>::max(), '\n');
            std::cout << "Incorrect input, Enter a new number:" << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<int>::max(), '\n');
        if (std::cin.gcount() > 1)
        {
            continue;
        }
        if (number <= 0)
        {
            std::cout << "You entered a negative number, please enter a new number:";
            continue;
        }
        break;
    }

    std::cout << "You number:" << number << std::endl << std::endl;
}
//dz2
std::ostream& endll (std::ostream& out)
{
    out << "\n\n" << std::flush;
    return out;
}

enum class CardFace : int
{
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Size
};

enum class CardSuit : int
{
    Hearts,
    Spades,
    Clubs,
    Diamonds,
    Size
};

class Card
{
private:
    bool m_isOpened{ false };
    CardFace m_face{ CardFace::Size };
    CardSuit m_suit{ CardSuit::Size };

public:
    Card() = default;
    Card(CardFace face, CardSuit suit, bool back = false) : m_face(face), m_suit(suit), m_isOpened(back) { }
    int Open() { m_isOpened = true; return getCardValue(); }
    bool IsOpened() { return m_isOpened; }
    CardFace getFace() { return m_face; }
    void Flip() { m_isOpened = !m_isOpened; }

    int getCardValue()
    {
        if (m_face >= CardFace::Ace && m_face <= CardFace::Nine)
            return static_cast<int>(m_face) +1;
        else if (m_face >= CardFace::Ten && m_face <= CardFace::King)
            return 10;
    }

    void Show()
    {
        std::cout << *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};

inline std::ostream& operator<<(std::ostream& os, const Card& card)
{
    const std::array facesChar{ "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
    const std::array suitsChar{ "H", "S", "C", "D" };
    if (card.m_isOpened)
    {
        os << facesChar[static_cast<size_t>(card.m_face)] << suitsChar[static_cast<size_t>(card.m_suit)];
    }
    else
    {
        os << "XX";
    }
    return os;
}

class Hand
{
protected:
    std::vector<Card*> cards{};
public:
    Hand() = default;
    virtual ~Hand() {};

    void Add(Card* deck)
    {
        assert(deck != nullptr);
        cards.emplace_back(deck);
    }

    void Clear()
    {
        for (auto d : cards)
        {
            delete d;
        }
        cards.clear();
    }

    int getTotal() const
    {
        const int Chanch_Ace = 11;
        int Total = 0;
        for (auto d : cards)
        {
            Total += d->getCardValue();
            if (Total <= Chanch_Ace && d->getFace() == CardFace::Ace)
                Total += Chanch_Ace - 1;
        }
        return Total;
    }

    void Open()
    {
        for (auto d : cards) {
            d->Open();
        }
    }

    void Show()
    {
        for (auto d : cards) {
            d->Show();
        }
        std::cout << std::endl;
    }
};

class GenericPlayer : public Hand
{
protected:
    std::string m_namePlayer{ "No name" };
    static const int Point_Win{ 21 };

public:
    GenericPlayer(const std::string& namePlayer) : m_namePlayer(namePlayer) {}
    virtual ~GenericPlayer() {};
    virtual bool IsHitting() const = 0;
    bool isBoosted() { return (getTotal() > Point_Win); }
    void Bust() { std::cout << "The player - " << m_namePlayer << "brute force!" << std::endl; }
    friend std::ostream& operator<<(std::ostream& str, const GenericPlayer& Player);
};
inline std::ostream& operator<<(std::ostream& str, const GenericPlayer& Player)
{
    str << "The player: " << Player.m_namePlayer << " : ";
    if (Player.cards.empty())
    {
        str << "No card.";
    }
    else
    {
        for (const auto n : Player.cards)
        {
            str << *n << "   ";
        }
        str << "Points:  " << Player.getTotal() << std::endl;
    }
    return str;
}
//dz6.3
class House : public GenericPlayer
{
public:
    House(const std::string& m_namePlayer = "Dealer") :GenericPlayer(m_namePlayer) {}
    virtual ~House() {};
    void OpenFirstCard()
    {
        if (!(cards.empty()))
        {
            cards[0]->Open();
        }
        else
        {
            std::cout << "No cards!" << std::endl;
        }
    }
    virtual bool IsHitting()const override
    {
        const int DealerPointSum = 16;
        return (getTotal() <= DealerPointSum);
    };
};

class Player : public GenericPlayer
{
public:
    Player(const std::string& namePlayer) : GenericPlayer(namePlayer) {}
    virtual bool IsHitting() const override
    {
        while (true)
        {
            char addition;
            std::cout << "Want to take a card (Enter y = yes; n = no)? ";
            std::cin >> addition;
            std::cin.ignore(32767, '\n');
            if (addition == 'y' || addition == 'Y')
            {
                return (addition == 'y' || addition == 'Y');
            }
            else if (addition == 'n' || addition == 'N')
            {
                return (addition == 'n' || addition == 'N');
            }
            else
            {
                std::cout << "Incorrect input, enter (y/n): " << std::endl;
            }
        }
    }
    void Win() const { std::cout << m_namePlayer << "Winner!" << std::endl; }
    void lose() const { std::cout << m_namePlayer << "Losing!" << std::endl; }
    void Push() const { std::cout << "Draw!" << std::endl; }
};

int main()
{
    Number_Input();
    std::cout << "Hello" << endll << "World" << endll;

    Player p("Ivan ");
    p.Add(new Card{ CardFace::Eight, CardSuit::Diamonds });
    p.Add(new Card{ CardFace::Queen, CardSuit::Diamonds });
    p.Add(new Card{ CardFace::Two, CardSuit::Diamonds });

    p.Open();
    std::cout << p;

    p.IsHitting();
}