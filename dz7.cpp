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
#include <array>
#include<string>

class Date
{
public:
    Date() = default;
    Date(const Date&) = default;
    Date& operator=(const Date&) = default;
    Date(const size_t day, const size_t mont, const size_t year) : m_day(day), m_mont(mont), m_year(year) {}
    friend bool operator>(const Date& d1, const Date& d2);
    friend std::ostream& operator<<(std::ostream& str, const Date& d);
private:
    size_t m_day{ 1 };
    static const size_t MinDay{ 1 };
    static const size_t MaxDay{ 31 };
    size_t m_mont{ 1 };
    static const size_t MinMont{ 1 };
    static const size_t MaxMont{ 12 };
    size_t m_year{ 2021 };
};
bool operator>(const Date& d1, const Date& d2)
{
    return d1.m_year > d2.m_year ? true : (d1.m_year > d2.m_year ? false :
        (d1.m_mont > d2.m_mont ? true : (d1.m_mont > d2.m_mont ? false :
            (d1.m_day > d2.m_day))));
}
inline std::ostream& operator<<(std::ostream& str, const Date& d)
{
    if (d.m_day < d.MinDay || d.m_day > d.MaxDay || d.m_mont< d.MinMont || d.m_mont > d.MaxMont)
    {
        str << "Incorrect date entered: " << d.m_day << "." << d.m_mont << "." << d.m_year;
    }
    else
    {
        str << d.m_day << "." << d.m_mont << "." << d.m_year;
    }
    return str;
}

template <class T>
class smart_ptr
{
public:
    smart_ptr(T* ptr = nullptr) : m_ptr(ptr) {}
    ~smart_ptr() { delete m_ptr; }
    smart_ptr(smart_ptr& ptr)
    {
        m_ptr = ptr.m_ptr;
        ptr.m_ptr = nullptr;
    }
    smart_ptr& operator=(smart_ptr& ptr)
    {
        if (&ptr == this)
            return *this;
        delete m_ptr;
        m_ptr = ptr.m_ptr;
        ptr.m_ptr = nullptr;
        return *this;
    }
    T* operator->()const { return m_ptr; }
    T& operator*()const { return *m_ptr; }
    const T* get() const { return m_ptr; }
private:
    T* m_ptr{ nullptr };
};
const smart_ptr<Date>& ComparisonandConclusionOfMore(const smart_ptr<Date>& date1, const smart_ptr<Date>& date2)
{
    return *date1 > *date2 ? date1 : date2;
}
void DateExchange(smart_ptr<Date>& date1, smart_ptr<Date>& date2)
{
    smart_ptr<Date> temp(date1);
    date1 = date2;
    date2 = temp;
}
//dz7.3
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
    Card(CardFace face, CardSuit suit, bool open = false) : m_face(face), m_suit(suit), m_isOpened(open) { }
    int Open() { m_isOpened = true; return getCardValue(); }
    bool IsOpened() { return m_isOpened; }
    CardFace getFace() { return m_face; }
    void Flip() { m_isOpened = !m_isOpened; }

    int getCardValue()
    {
        if (m_face >= CardFace::Ace && m_face <= CardFace::Nine)
            return static_cast<int>(m_face) + 1;
        else if (m_face >= CardFace::Ten && m_face <= CardFace::King)
            return static_cast<int>(CardFace::Ten);
    }

    void Show()
    {
        std::cout << *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};

inline std::ostream& operator<<(std::ostream& os, const Card& card)
{
    const std::array < std::string, static_cast<size_t>(CardFace::Size)> facesChar{ "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
    const std::array < std::string, static_cast<size_t>(CardFace::Size) > suitsChar{ "H", "S", "C", "D" };
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
    void Bust() { std::cout << "The player - " << m_namePlayer << " brute force!" << std::endl; }
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
        for (const auto& n : Player.cards)
        {
            str << *n << "   ";
        }
        str << "Points:  " << Player.getTotal() << std::endl;
    }
    return str;
}

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
    virtual bool IsHitting()const
    {
        const int DealerPointSum = 16;
        return (getTotal() <= DealerPointSum);
    };
};

class Deck : public Hand
{
public:
    Deck() { cards.reserve(DeckCardSize);  HandOut(); }
    void HandOut()
    {
        Clear();
        for (size_t i = 0; i < static_cast<size_t>(CardSuit::Size); ++i)
        {
            for (size_t y = 0; y < static_cast<size_t>(CardFace::Size); ++y)
            {
                Add(new Card(static_cast<CardFace>(y), static_cast<CardSuit>(i)));
            }
        }
    }
    void Shuffle()
    {
        std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(cards.begin(), cards.end(), rnd);
    }
    void Deal(Hand& hand)
    {
        if (!cards.empty())
        {
            hand.Add(cards.back());
            cards.pop_back();
        }
        else
        {
            std::cout << "Deck empty!";
        }
    }
    void AdditionalCards(GenericPlayer& genericPlayer)
    {
        while (!(genericPlayer.isBoosted()) && genericPlayer.IsHitting())
        {
            Deal(genericPlayer);
            std::cout << genericPlayer << std::endl;
            if (genericPlayer.isBoosted())
            {
                genericPlayer.Bust();
            }
        }
    }

private:
    const size_t DeckCardSize{ 52 };
};

template<typename T>
T getUserInput(const std::string& promt, int length = 1)
{
    while (true)
    {
        T val;
        std::cout << promt << ": ";
        std::cin >> val;
        if (std::cin.fail() || (std::to_string(val).length() > length && !std::is_same<T, char>::value))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<T>::max(), '\n');
            std::cout << "Incorrect input, please try again" << std::endl;
        }
        else
        {
            std::cin.ignore(std::numeric_limits<T>::max(), '\n');
            return val;
        }
    }
}

inline std::ostream& endll(std::ostream& stream)
{
    stream << "\n\n" << std::flush;
    return stream;
}

class Player : public GenericPlayer
{
public:
    Player(const std::string& m_namePlayer) : GenericPlayer(m_namePlayer) {}

    virtual bool IsHitting() const override
    {
        char c = getUserInput<char>(m_namePlayer + ", want to take a card?");
        return (c == 'y' || c == 'Y');
    };

    void Win() const { std::cout << m_namePlayer << " Win!" << std::endl; }
    void Lose() const { std::cout << m_namePlayer << " Lose!" << std::endl; }
    void Push() const { std::cout << "Draw!" << std::endl; }
};



class Game
{
public:
    Game(const std::vector<std::string>& names)
    {
        for (const auto& Name : names)
        {
            Players.emplace_back(Player(Name));
        }
        deck.HandOut();
        deck.Shuffle();
    }
    ~Game() {};
    void Play();
private:
    Deck deck;
    House house;
    std::vector<Player>Players;
};
inline void Game::Play()
{
    const size_t MinNumberOfPlayers = 2;
    for (size_t i = 0; i < MinNumberOfPlayers; ++i)
    {
        for (auto& pPlayers : Players)
        {
            deck.Deal(pPlayers);
        }
        deck.Deal(house);
    }
    house.OpenFirstCard();
    for (const auto& pPlayers : Players)
    {
        std::cout << pPlayers << std::endl;
    }
    std::cout << house << std::endl;

    for (auto& pPlayaers : Players)
    {
        deck.AdditionalCards(pPlayaers);
    }
    house.OpenFirstCard();
    std::cout << std::endl << house;
    deck.AdditionalCards(house);

    if (house.isBoosted())
    {
        for (auto& pPlayers : Players)
        {
            if (!(pPlayers.isBoosted()))
            {

                pPlayers.Win();
            }
        }
    }
    else
    {
        for (auto& pPlayers : Players)
        {
            if (!(pPlayers.isBoosted()))
            {
                if (pPlayers.getTotal() > house.getTotal())
                {
                    pPlayers.Win();
                }
                else if (pPlayers.getTotal() < house.getTotal())
                {
                    pPlayers.Lose();
                }
                else
                {
                    pPlayers.Push();
                }
            }
        }
    }
    for (auto& pPlayers : Players)
    {
        pPlayers.Clear();
    }
    house.Clear();
}




int main()
{
    //dz7.1
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* now = std::localtime(&tt);
    smart_ptr<Date> today(new Date(now->tm_mday, now->tm_mon, now->tm_year + 1900));
    smart_ptr<Date> date;
    std::cout << "Today: " << *today << std::endl;
    std::cout << "today: " << (today.get() != nullptr ? today.get() : nullptr) << std::endl;
    std::cout << "date: " << (date.get() != nullptr ? date.get() : nullptr) << std::endl << std::endl;
    date = today;
    std::cout << "Date: " << *date << std::endl;
    std::cout << "today: " << (today.get() != nullptr ? today.get() : nullptr) << std::endl;
    std::cout << "date: " << (date.get() != nullptr ? date.get() : nullptr) << std::endl << std::endl;
    //dz7.2
    smart_ptr<Date> date1(new Date(5, 2, 2021));
    smart_ptr<Date> date2(new Date(15, 6, 2019));
    smart_ptr<Date> date3(new Date(66, 4, 2025));
    smart_ptr<Date> date4(new Date(0, 4, 2025));
    smart_ptr<Date> date5(new Date(6, 15, 2025));
    std::cout << "date 1= " << *date1 << std::endl;
    std::cout << "date 2= " << *date2 << std::endl;
    std::cout << "date 3= " << *date3 << std::endl;
    std::cout << "date 4= " << *date4 << std::endl;
    std::cout << "date 5= " << *date5 << std::endl << std::endl;
    std::cout << "Display the late date (date 1 or date 2): " << *ComparisonandConclusionOfMore(date1, date2) << std::endl;
    std::cout << "Date exchange: " << *date1 << " and " << *date2 << std::endl;
    DateExchange(date1, date2);
    std::cout << "date 1= " << *date1 << std::endl;
    std::cout << "date 2= " << *date2 << std::endl;


    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7) {
        numPlayers = getUserInput<int>("How many players (1 - 7) ? ");
    }

    std::vector<std::string> names;
    std::string name;
    for (int i = 0; i < numPlayers; ++i) {
        std::cout << "Name " << i + 1 << ": ";
        std::cin >> name;
        names.emplace_back(name);
    }
    std::cout << std::endl;

    Game game(names);
    char again = 'y';
    while (again != 'n' && again != 'N') {
        game.Play();
        again = getUserInput<char>("Repeat the game (Y/N) ? ");
    }
}