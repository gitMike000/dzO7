#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

const int dayDefault = 1;
const int mounthDefault = 1;
const int yearDefault = 2020;

class Date {
public:
    Date() : day(dayDefault), mounth(mounthDefault), year(yearDefault) { ++number; }
    Date( int Day, int Mounth, int Year) : day(Day), mounth(Mounth), year(Year) { ++number; }
    ~Date() { --number; }

    friend ostream& operator<<(ostream& os, const Date& date);
    friend bool operator== (const Date& date1, const Date& date2);
//    void changeDay(int arg);
    int get_day() const;
    void set_day(int m_day);
    int get_mounth() const;
    void set_mounth(int m_mounth);
    int get_year() const;
    void set_year(int m_year);
    int get_number() const;

    //std::shared_ptr<Date>& verify(std::shared_ptr<Date>& date1, std::shared_ptr<Date>& date2);
    void verify(std::shared_ptr<Date>& date1, std::shared_ptr<Date>& date2);
    void change(std::shared_ptr<Date>& date1,std::shared_ptr<Date>& date2);

private:
    int day;
    int mounth;
    int year;
    static int number;
};

int Date::number = 0;

//void Date::changeDay(int arg) {
//    day = arg;
//}

int Date::get_day() const {
    return day;
}

void Date::set_day(int m_day) {day=m_day;}

int Date::get_mounth() const {
    return mounth;
}

void Date::set_mounth(int m_mounth) {mounth=m_mounth;}

int Date::get_year() const {
    return year;
}

void Date::set_year(int m_year) {year=m_year;}

int Date::get_number() const {
    return number;   
}

ostream& operator << (ostream& os, const Date& date)
{
    os << date.get_day() << '/' << date.get_mounth() << '/' << date.get_year();
    return os;
}

// -- Task 2

bool operator==(const Date& date1, const Date& date2)
{
    return (date1.day==date2.day &&
             date1.mounth==date2.day &&
              date1.year==date2.year);
}

void Date::verify(std::shared_ptr<Date>& date1, std::shared_ptr<Date>& date2)
{
   std::shared_ptr<Date> temp;
   if (date1->get_year() > date2->get_year()) temp=date1;
    else if (date1->get_mounth() > date2->get_mounth()) temp=date1;
          else if (date1->get_day() > date2->get_day()) temp=date1;
               else temp=date2;
   this->set_day(temp->get_day());
   this->set_mounth(temp->get_mounth());
   this->set_year(temp->get_year());
}

void Date::change(std::shared_ptr<Date>& date1,std::shared_ptr<Date>& date2)
{
  std::swap(date1, date2);
}

int main()
{
    cout << "Task 1" << endl;
//  Создайте класс Date с полями день, месяц, год и методами доступа к этим полям.
//  Перегрузите оператор вывода для данного класса. Создайте два "умных" указателя today
//  и date. Первому присвойте значение сегодняшней даты. Для него вызовите по отдельности
//  методы доступа к полям класса Date, а также выведите на экран данные всего объекта с
//  помощью перегруженного оператора вывода. Затем переместите ресурс, которым владеет
//  указатель today в указатель date. Проверьте, являются ли нулевыми указатели today и
//  date и выведите соответствующую информацию об этом в консоль.

    Date d1;
    cout << d1 << endl;
    //std::shared_ptr<Date> today=std::make_shared<Date>();
    std::unique_ptr<Date> today(new Date());
    today->set_day(10);
    today->set_mounth(7);
    today->set_year(2021);
    cout << *today << endl;


    //std::shared_ptr<Date> data = today;
    std::unique_ptr<Date> data(new Date());
    data=std::move(today);
    cout << *data << " data=" << data.get() << endl;

    assert(today==nullptr);
    if (today!=nullptr)
        std::cout << "Указатель today не нулевой" << endl;
    else
        std::cout << "Указатель today нулевой" << endl;

    assert(data!=nullptr);
    if (data!=nullptr)
        std::cout << "Указатель data не нулевой" << endl;
    else
        std::cout << "Указатель data нулевой" << endl;

    cout << "Число объектов Date=" << d1.get_number() << endl;

    cout << endl << "Task 2" << endl;
//  По условию предыдущей задачи создайте два умных указателя date1 и date2.
//  - Создайте функцию, которая принимает в качестве параметра два умных указателя
//  типа Date и сравнивает их между собой (сравнение происходит по датам).
//  Функция должна вернуть более позднюю дату.
//  - Создайте функцию, которая обменивает ресурсами (датами) два умных указателя,
//  переданных в функцию в качестве параметров.
//  Примечание: обратите внимание, что первая функция не должна уничтожать объекты,
//  переданные ей в качестве параметров.
    std::shared_ptr<Date> date1=std::make_shared<Date>(1,2,2020);
    std::shared_ptr<Date> date2=std::make_shared<Date>(5,7,2021);
    std::shared_ptr<Date> date3=std::make_shared<Date>();

    cout<<"Date 1:" << *date1<< " " << date1 << endl;
    cout<<"Date 2:" << *date2<< " " << date2 << endl;

    date3->verify( date1, date2);
    cout<<"Более поздняя дата:" << *date3<< " " << date3 << endl<<endl;

    Date().change(date1, date2);
    cout<<"Date 1:" << *date1<< " " << date1 << endl;
    cout<<"Date 2:" << *date2<< " " << date2 << endl;

    date3->verify( date1, date2);
    cout<<"Более поздняя дата:" << *date3<< " " << date3 << endl;

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

    return 0;
}
