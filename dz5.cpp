#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <random>
#include <chrono>
#include <cstdio>
#include <string>

template < typename T>
class Pair1
{
private:
	T par1{};
	T par2{};
public:
	Pair1(const T& p1, const T& p2) : par1(p1), par2(p2) {}
	void setpar1(const T& p) { par1 = p; }
	void setpar2(const T& p) { par2 - p; }

	T first() const { return par1; }
	T second() const { return par2; }
};

template <typename T1, typename T2>
class Pair
{
private:
	T1 par1;
	T2 par2;
public:
	Pair(const T1& p1, const T2& p2) : par1(p1), par2(p2) {}
	void setpar1(const T1& p) { par1 = p;}
	void setpar2(const T2& p) { par2 = p;}

	T1 first() const { return par1; }
	T2 second() const { return par2; }
};

template <typename T>
class StringValuePair : public Pair<std::string, T>
{
public:
	StringValuePair(const std::string& str, T any ) : Pair<std::string, T> (str, any){}
};

//dz1/2/3/4
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

    const std::string facesChar[static_cast<size_t>(CardFace::Size)]{ "A","2","3","4","5","6","7","8","9","10","J","Q","K" };

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
            return static_cast<int>(m_face) + 1;
        else if (m_face >= CardFace::Ten && m_face <= CardFace::King)
            return 10;

        return 0;
    }

    void Show()
    {
        std::cout << (m_isOpened ? facesChar[static_cast<size_t>(m_face)] : "BACK") << " ";
    }
};

class Hand
{
private:
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

    int getValue() const
    {
        const int Chanch_Ace = 11;
        int sum = 0;
        for (auto d : cards)
        {
            sum += d->getCardValue();
            if (sum <= Chanch_Ace && d->getFace() == CardFace::Ace)
                sum += Chanch_Ace - 1;
        }
        return sum;
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
//dz5.4
class GenericPlayer : public Hand
{
private:
    std::string namePlayer{"No name"};
public:
    GenericPlayer() : Hand() {}
    virtual bool isHitting() = 0;
    bool isBoosted() { return false; }
    void Bust() { std::cout << "The player - " << namePlayer << "brute force!" << std::endl; }
};


int main()
{//Less 5
	{//dz1
		Pair1<int> p1(6, 9);
		std::cout << "Pair: " << p1.first() << ' ' << p1.second() << std::endl;
		const Pair1<double> p2(3.4, 7.8);
		std::cout << "Pair: " << p2.first() << ' ' << p2.second() << std::endl << std::endl;
	}
	{//dz2
		Pair<int, double> p1(6, 7.8);
		std::cout << "Pair: " << p1.first() << ' ' << p1.second() << std::endl;

		const Pair<double, int> p2(3.4, 5);
		std::cout << "Pair: " << p2.first() << ' ' << p2.second() << std::endl << std::endl;
	}
	{//dz3
		StringValuePair<int> svp("Amazing", 7);
		std::cout << "Pair: " << svp.first() << ' ' << svp.second() << std::endl << std::endl;
	}
   
}

