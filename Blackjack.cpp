#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>

using namespace std;

class Card {
public:
  enum znashen {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
           JACK, QUEEN, KING};
  enum mast {CLUBS, DIAMONDS, HEARTS, SPADES};

  // перегрузка оператора вывода для Task 5
  // если карта перевернута рубашкой вверх (мы ее не видим),
  // вывести ХХ, если мы ее видим, вывести масть и номинал карты
  friend ostream& operator<<(ostream& os, const Card& aCard);

  Card(znashen r = ACE, mast s = SPADES, bool ifUp = true): m_Znashen(r), m_Mast(s), m_IsFaceUp(ifUp)
  {}

  // возвращает значение карты, пока можно считать, что туз = 1
  int GetValue() const
  {
   int value = 0;
   if (m_IsFaceUp)
    {
      value = m_Znashen;
      if (value > 10) value = 10;
    }
   return value;
  };

  // переворачивает карту
  void Flip()
  {
     m_IsFaceUp = !(m_IsFaceUp);
  };

private:
  znashen m_Znashen;
  mast m_Mast;
  bool m_IsFaceUp;
};

class Hand
{

protected:
  vector <Card*> m_Cards;

public:
  Hand()
  {
   m_Cards.reserve(7);
  }
         ;

  // добавляет в коллекцию карт новую карту
  void Add(Card* pCard)
  {
    m_Cards.push_back(pCard);
  }

  // очищает руку от карт
  void Clear()
  {
    //iterate through vector, freeing all memory on the heap
    vector <Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
      delete *iter;
      *iter = 0; //get rid of dangling pointer - null pointer
    }
    m_Cards.clear();
  }

  // возвращает сумму очков карт руки, туз принимает значения 1 или 11
  int GetTotal() const
{

  //если нет карт, возвращает 0
  if (m_Cards.empty()) return 0;

  // если первая карта 0, то карты закрыта, возвращает 0
  if (m_Cards[0]->GetValue() == 0) return 0;

  // сложение значения карт, относимся к каждому тузу как к 1
  int total = 0;
  vector<Card*>::const_iterator iter;
  for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
  total += (*iter)->GetValue();

  // определяем, есть ли в руке туз
  bool containsAce = false;
  for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
  if ((*iter)->GetValue() == Card::ACE)
    containsAce = true;

  // если в руке есть туз, а сумма достаточно низкая, то туз считается 11
  if (containsAce && total <= 11)
  { // добавляем только 10, так как мы уже добавили 1 для туза
   total += 10;
  }

  return total;
}

  ~Hand()
  {
    Clear();
  }
};

//абстрактный класс базового игрока
class GenericPlayer : public Hand
{
  friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

public:
  GenericPlayer(const string& name = "");

  virtual ~GenericPlayer();

  // чисто виртуальная функция, возвращает информацию, нужна ли игроку
  // еще одна карта
  virtual bool IsHitting() const = 0;

  //возвращает bool значение, есть ли у игрока перебор
  bool IsBusted() const;

  //выводит на экран имя игрока и объявляет, что у него перебор
  void Bust() const;

protected:
  string m_Name;
};

GenericPlayer::GenericPlayer(const string& name): m_Name(name) {}
GenericPlayer::~GenericPlayer() {}

bool GenericPlayer::IsBusted() const
{
  return (GetTotal() > 21);
}
void GenericPlayer::Bust() const
{
  cout << m_Name << " перебрал.\n";
}

class Player : public GenericPlayer
{
public:

  Player(const string& name): GenericPlayer(name) {}
  virtual ~Player() {}

  //      Метод спрашивает у пользователя, нужна ли ему еще одна карта и возвращает ответ
  //      пользователя в виде true или false.
  virtual bool IsHitting() const
  {
    cout << m_Name << ", вы хотите еще карту? (Y/N): ";
    char response;
    cin >> response;
    return (response == 'y' || response == 'Y');
  }


  //выводит на экран имя игрока и сообщение, что он выиграл
  void Win() const
  {
    cout << m_Name << " победил.\n";
  }

  //выводит на экран имя игрока и сообщение, что он проиграл
  void Lose() const
  {
    cout << m_Name << " проиграл.\n";
  }

  //выводит на экран имя игрока и сообщение, что он сыграл вничью.
  void Push() const
  {
    cout << m_Name << " сыграл вничью.\n";
  }
};

class House : public GenericPlayer
{
  public:

    House(const string& name = "House"): GenericPlayer( name) {}
    virtual ~House() {};

    //Метод указывает, нужна ли дилеру еще одна карта.
    //Если у дилера не больше 16 очков, то он берет еще одну карту.
    virtual bool IsHitting() const
    {
      return (GetTotal() <= 16);
    }

    //метод переворачивает первую карту дилера
    void FlipFirstCard()
    {
      if (!(m_Cards.empty()))
        m_Cards[0]->Flip();

      else
        cout << "Нет карт для открытия!\n";
    }
};

//    Написать перегрузку оператора вывода для класса Card. Если карта перевернута рубашкой
//    вверх (мы ее не видим), вывести ХХ, если мы ее видим, вывести масть и номинал карты.
//    Также для класса GenericPlayer написать перегрузку оператора вывода, который должен
//    отображать имя игрока и его карты, а также общую сумму очков его карт.

// Перегрузка оператора вывода для класса Card. Если карта перевернута рубашкой
// вверх (мы ее не видим), вывести ХХ, если мы ее видим, вывести масть и номинал карты.
ostream& operator<<(ostream& os, const Card& aCard)
{
  const string RANKS[] = {"0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
                           "10", "J", "Q", "K"};

  const string SUITS[] = {"c", "d", "h", "s"};


  if (aCard.m_IsFaceUp)
      os << RANKS[aCard.m_Znashen] << SUITS[aCard.m_Mast];
  else
      os << "XX";

  return os;
}

// перегрузка оператора вывода, который должен
// отображать имя игрока и его карты, а также общую сумму очков его карт
ostream& operator <<(ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";

    vector <Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();
             pCard != aGenericPlayer.m_Cards.end(); ++pCard)
            os << *(*pCard) << "\t";

      if (aGenericPlayer.GetTotal() != 0)
          cout << "(" << aGenericPlayer.GetTotal() << ")";
    }
    else
      os << "<пусто>";

  return os;
}

// Task 3

class Deck : public Hand
{
  public:
    Deck();
    virtual ~Deck();

    //Создает стандартную колоду из 52 карт, вызывается из конструктора
    void Populate();

    //Метод, который тасует карты, можно использовать функцию из алгоритмов STL random_shuffle
    void Shuffle();

    //Метод, который раздает в руку одну карту
    void Deal(Hand& aHand);

    //Раздает игроку дополнительные карты до тех пор, пока он может и хочет их получать
    void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck()
{
  m_Cards.reserve(52);
  Populate();
}
Deck::~Deck() {}

void Deck::Populate()
{
  Clear();

  //создание колоды
    for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
  {
    for (int r = Card::ACE; r <= Card::KING; ++r)
    {
      Add(new Card(static_cast <Card::znashen>(r),static_cast <Card::mast>(s)));
    }
  }
}

void Deck::Shuffle()
{
  random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand)
{
  if (!m_Cards.empty())
  {
    aHand.Add(m_Cards.back());
    m_Cards.pop_back();
  }

  else
  {
    cout << "Карт нет, раздача невозможна.\n";
  }
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer)
{
  cout << endl;
  // Продолжаем раздавать карту до тех пор, пока обычный игрок не
  // вылетит из игры и не хочет получить еще один удар
  while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting())
  {
    Deal(aGenericPlayer);
    cout << aGenericPlayer << endl;

  if (aGenericPlayer.IsBusted())
     aGenericPlayer.Bust();
  }
}

// Task 4
// В этом методе раздаются каждому игроку по две
// стартовые карты, а первая карта дилера прячется. Далее выводится на экран информация о
// картах каждого игра, в т.ч. и для дилера. Затем раздаются игрокам дополнительные карты.
// Потом показывается первая карта дилера и дилер набирает карты, если ему надо. После этого
// выводится сообщение, кто победил, а кто проиграл. В конце руки всех игроков очищаются
class Game
{
public:
    Game(const vector<string>& names);

    ~Game();

    //Функция обеспечения игры
    void Play();

private:
    Deck m_Deck;
    House m_House;
    vector<Player> m_Players;
};

Game::Game(const vector<string>& names)
{
  //создание массива игроков
  vector<string>::const_iterator pName;
  for (pName = names.begin(); pName != names.end(); ++pName)
        m_Players.push_back(Player(*pName));

  srand(time(0));
  m_Deck.Populate();
  m_Deck.Shuffle();
}

Game::~Game() {}

void Game::Play()
{
  //раздача всем по 2 карты
  vector<Player>::iterator pPlayer;
  for (int i = 0; i < 2; ++i)
  {
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
          m_Deck.Deal(*pPlayer);

    m_Deck.Deal(m_House);
   }

   //Спрятать первую карту House
   m_House.FlipFirstCard();

  //Показать руки игроков
  for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        cout << *pPlayer << endl;

  cout << m_House << endl;

  //раздавать игрокам дополнительные карты
  for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
         m_Deck.AdditionalCards(*pPlayer);

  //Перевернуть первую карту House
  m_House.FlipFirstCard();
  cout << endl << m_House;

  //Раздавать дополнительные карты в House
  m_Deck.AdditionalCards(m_House);

  if (m_House.IsBusted())
  {
    //everyone still playing wins
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
       if (!(pPlayer->IsBusted()))
             pPlayer->Win();
  }
  else
  {
    //Cравнение каждого игрока, продолжающего играть, с House
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end();
          ++pPlayer)
    if (!(pPlayer->IsBusted()))
    {
        if (pPlayer->GetTotal() > m_House.GetTotal())
            pPlayer->Win();

        else if (pPlayer->GetTotal() < m_House.GetTotal())
            pPlayer->Lose();

        else
            pPlayer->Push();
        }
  }
  //Убрать все карты
  for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        pPlayer->Clear();

  m_House.Clear();
}

int  main()
{
    cout << endl << "Task 3,4,5"<< endl << "See Blackjack.cpp"<<endl;
//    Создать класс Deck, который наследует от класса Hand и представляет собой колоду карт.
//    Класс Deck имеет 4 метода:
//     - vold Populate() - Создает стандартную колоду из 52 карт, вызывается из конструктора.
//     - void Shuffle() - Метод, который тасует карты, можно использовать функцию из алгоритмов
//       STL random_shuffle
//     - vold Deal (Hand& aHand) - метод, который раздает в руку одну карту
//     - void AddltionalCards (GenericPlayer& aGenerlcPlayer) - раздает игроку дополнительные
//       карты до тех пор, пока он может и хочет их получать
//    Обратите внимание на применение полиморфизма. В каких методах применяется этот принцип ООП?

//    4. Реализовать класс Game, который представляет собой основной процесс игры. У этого класса
//       будет 3 поля:
//        - колода карт
//        - рука дилера
//        - вектор игроков.
//    Конструктор класса принимает в качестве параметра вектор имен игроков и создает объекты
//    самих игроков. В конструкторе создается колода карт и затем перемешивается.
//    Также класс имеет один метод play(). В этом методе раздаются каждому игроку по две
//    стартовые карты, а первая карта дилера прячется. Далее выводится на экран информация о
//    картах каждого игра, в т.ч. и для дилера. Затем раздаются игрокам дополнительные карты.
//    Потом показывается первая карта дилера и дилер набирает карты, если ему надо. После этого
//    выводится сообщение, кто победил, а кто проиграл. В конце руки всех игроков очищаются.

//    5. Написать функцию main() к игре Блекджек. В этой функции вводятся имена игроков.
//    Создается объект класса Game и запускается игровой процесс. Предусмотреть возможность
//    повторной игры.
    cout << "\t\tДобро пожаловать в Blackjack! \n\n";

    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
      cout << "Сколько игроков в игре? (1 - 7): ";
      cin  >> numPlayers;
    }
    vector <string> names;
    string name;
    for (int i = 0; i < numPlayers; ++i)
    {
      cout << "Введите имя игрока: ";
      cin  >> name;
      names.push_back(name);
    }

    cout << endl;

    //зацикливание игры
    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
      aGame.Play();
      cout << "\nХочете съиграть еще партию? (Y/N): ";
      cin  >> again;
    }

    return 0;
}
