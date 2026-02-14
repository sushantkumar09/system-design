/******************************************************************************
City
    -> Movie
        ->Theatre
            ->screen
                ->Show (date+time) =>movie
                    ->seat
                        ->booking
                            ->payment
       
seat-> seatID,seatCategory
screen ->screenId and list<seat>, map<datetime,list<show>>
Theatre 
*******************************************************************************/

#include <iostream>
#include <bits/stdc++.h>
#include <mutex>
using namespace std;

class Show;
class Theatre;
class Screen;
class Booking;
class Movie;



enum class SeatCategory {
    NORMAL,
    PREMIUM
};

enum class CITY {
    BENGALURU,
    PUNE
};

enum class SeatStatus {
    AVAILABLE,
    LOCKED,
    BOOKED
};

enum class PaymentStatus {
    SUCCESS,
    FAILED
};



class Seat {
    int seatId;
    SeatCategory category;

public:
    Seat(int id, SeatCategory cat) : seatId(id), category(cat) {}

    int getSeatId() const { return seatId; }
    SeatCategory getCategory() const { return category; }
};


class Movie {
    string name;

public:
    Movie(string name) : name(name) {}

    string getName() const { return name; }
};


class Screen {
    int screenId;
    vector<Seat> seats;
    unordered_map<string, vector<Show*>> showsByDate;

public:
    Screen(int id, vector<Seat> seats) : screenId(id), seats(seats) {}

    vector<Seat>& getSeats() { return seats; }

    void addShow(Show* show) {
        showsByDate[show->getDate()].push_back(show);
    }

    vector<Show*> getShowsByDate(const string& date) {
        return showsByDate[date];
    }
};



class Theatre {
    string name;
    CITY city;
    vector<Screen*> screens;

public:
    Theatre(string name, CITY city, vector<Screen*> screens)
        : name(name), city(city), screens(screens) {}

    string getName() const { return name; }
    CITY getCity() const { return city; }
    vector<Screen*>& getScreens() { return screens; }
};



class TheatreService {
    unordered_map<CITY, vector<Theatre*>> cityTheatres;

public:
    void addTheatre(Theatre* theatre) {
        cityTheatres[theatre->getCity()].push_back(theatre);
    }

    unordered_set<string> getMovies(CITY city, const string& date) {
        unordered_set<string> movies;

        for (Theatre* theatre : cityTheatres[city]) {
            for (Screen* screen : theatre->getScreens()) {
                for (Show* show : screen->getShowsByDate(date)) {
                    movies.insert(show->getMovie()->getName());
                }
            }
        }
        return movies;
    }

    vector<Theatre*> getTheatres(CITY city, const string& movieName, const string& date) {
        vector<Theatre*> result;

        for (Theatre* theatre : cityTheatres[city]) {
            bool found = false;

            for (Screen* screen : theatre->getScreens()) {
                for (Show* show : screen->getShowsByDate(date)) {
                    if (show->getMovie()->getName() == movieName) {
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }

            if (found) result.push_back(theatre);
        }

        return result;
    }

    vector<Show*> getShows(Theatre* theatre, const string& movieName, const string& date) {
        vector<Show*> result;

        for (Screen* screen : theatre->getScreens()) {
            for (Show* show : screen->getShowsByDate(date)) {
                if (show->getMovie()->getName() == movieName) {
                    result.push_back(show);
                }
            }
        }
        return result;
    }
};



enum class Status{
    AVAILABLE,
    UNAVAILABLE,
    LOCKED,
};

class Movie {
    string name;

public:
    Movie(string name) : name(name) {}

    string getName() const {
        return name;
    }
};

class Show {
    Movie* movie;
    string showDate;
    string showTime;

    unordered_map<int, SeatStatus> seatStatus;
    unordered_map<int, mutex> seatLocks;

public:
    Show(Movie* movie, string date, string time, const vector<Seat>& seats)
        : movie(movie), showDate(date), showTime(time) {

        for (const Seat& seat : seats) {
            seatStatus[seat.getSeatId()] = SeatStatus::AVAILABLE;
        }
    }

    string getDate() const { return showDate; }
    string getTime() const { return showTime; }
    Movie* getMovie() const { return movie; }

    bool lockSeats(vector<int> seatIds) {
        sort(seatIds.begin(), seatIds.end());
        vector<unique_lock<mutex>> locks;

        for (int seatId : seatIds) {
            locks.emplace_back(seatLocks[seatId]);
        }

        for (int seatId : seatIds) {
            if (seatStatus[seatId] != SeatStatus::AVAILABLE)
                return false;
        }

        for (int seatId : seatIds) {
            seatStatus[seatId] = SeatStatus::LOCKED;
        }

        return true;
    }

    void confirmSeats(const vector<int>& seatIds) {
        for (int seatId : seatIds) {
            seatStatus[seatId] = SeatStatus::BOOKED;
        }
    }

    void releaseSeats(const vector<int>& seatIds) {
        for (int seatId : seatIds) {
            seatStatus[seatId] = SeatStatus::AVAILABLE;
        }
    }
};



class TheatreController {
    TheatreService theatreService;

public:
    void addTheatre(Theatre* theatre) {
        theatreService.addTheatre(theatre);
    }

    unordered_set<string> getMovies(CITY city, const string& date) {
        return theatreService.getMovies(city, date);
    }

    vector<Theatre*> getTheatres(CITY city, const string& movie, const string& date) {
        return theatreService.getTheatres(city, movie, date);
    }

    vector<Show*> getShows(Theatre* theatre, const string& movie, const string& date) {
        return theatreService.getShows(theatre, movie, date);
    }
};


class User {
    string userId;
    string name;

public:
    User(string id, string name) : userId(id), name(name) {}
};

class Payment {
    PaymentStatus status;

public:
    Payment(PaymentStatus status) : status(status) {}
    PaymentStatus getStatus() const { return status; }
};

class Booking {
    string bookingId;
    User* user;
    Show* show;
    vector<int> seats;
    Payment payment;

public:
    Booking(User* user, Show* show, vector<int> seats, Payment payment)
        : user(user), show(show), seats(seats), payment(payment) {
        bookingId = to_string(rand());
    }

    string getBookingId() const { return bookingId; }
    User* getUser() const { return user; }
};


class BookingService {
    unordered_map<string, Booking*> bookings;

public:
    Booking* book(User* user, Show* show, vector<int> seats) {

        if (!show->lockSeats(seats)) {
            throw runtime_error("Seats unavailable");
        }

        Payment payment(PaymentStatus::SUCCESS);

        if (payment.getStatus() == PaymentStatus::SUCCESS) {
            show->confirmSeats(seats);

            Booking* booking = new Booking(user, show, seats, payment);
            bookings[booking->getBookingId()] = booking;
            return booking;
        } else {
            show->releaseSeats(seats);
            throw runtime_error("Payment failed");
        }
    }

    Booking* getBooking(const string& bookingId) {
        return bookings.count(bookingId) ? bookings[bookingId] : nullptr;
    }

    vector<Booking*> getBookingsForUser(User* user) {
        vector<Booking*> result;
        for (auto& it : bookings) {
            if (it.second->getUser() == user)
                result.push_back(it.second);
        }
        return result;
    }
};

class BookingController {
    BookingService* bookingService;

public:
    BookingController(BookingService* service) : bookingService(service) {}

    Booking* createBooking(User* user, Show* show, const vector<int>& seats) {
        return bookingService->book(user, show, seats);
    }

    Booking* getBooking(const string& bookingId) {
        return bookingService->getBooking(bookingId);
    }

    vector<Booking*> getBookingsForUser(User* user) {
        return bookingService->getBookingsForUser(user);
    }
};


int main() {

    // ---------- 1️⃣ Create Movies ----------
    Movie avengers("Avengers");
    Movie inception("Inception");

    // ---------- 2️⃣ Create Seats ----------
    vector<Seat> seats;
    for (int i = 1; i <= 10; i++) {
        seats.emplace_back(i, SeatCategory::NORMAL);
    }

    // ---------- 3️⃣ Create Screen ----------
    Screen* screen1 = new Screen(1, seats);

    // ---------- 4️⃣ Create Shows ----------
    Show* morningShow = new Show(&avengers, "2026-02-10", "10:00", seats);
    Show* eveningShow = new Show(&inception, "2026-02-10", "18:00", seats);

    screen1->addShow(morningShow);
    screen1->addShow(eveningShow);

    // ---------- 5️⃣ Create Theatre ----------
    Theatre* pvrBangalore = new Theatre(
        "PVR Orion Mall",
        CITY::BENGALURU,
        {screen1}
    );

    // ---------- 6️⃣ Setup Theatre Controller ----------
    TheatreController theatreController;
    theatreController.addTheatre(pvrBangalore);

    // ---------- 7️⃣ User enters system ----------
    User user("U1", "Sushant");

    cout << "User logged in\n";

    // ---------- 8️⃣ User selects city & date ----------
    CITY selectedCity = CITY::BENGALURU;
    string selectedDate = "2026-02-10";

    cout << "Selected City: BENGALURU\n";
    cout << "Selected Date: " << selectedDate << "\n";

    // ---------- 9️⃣ Show movies running ----------
    auto movies = theatreController.getMovies(selectedCity, selectedDate);
    cout << "\nMovies running:\n";
    for (auto& m : movies) {
        cout << "- " << m << "\n";
    }

    // ---------- 🔟 User selects movie ----------
    string selectedMovie = "Avengers";
    cout << "\nSelected Movie: " << selectedMovie << "\n";

    // ---------- 1️⃣1️⃣ Show theatres ----------
    auto theatres = theatreController.getTheatres(
        selectedCity, selectedMovie, selectedDate
    );

    cout << "\nTheatres available:\n";
    for (auto* t : theatres) {
        cout << "- " << t->getName() << "\n";
    }

    // ---------- 1️⃣2️⃣ User selects theatre ----------
    Theatre* selectedTheatre = theatres[0];
    cout << "\nSelected Theatre: " << selectedTheatre->getName() << "\n";

    // ---------- 1️⃣3️⃣ Show available shows ----------
    auto shows = theatreController.getShows(
        selectedTheatre, selectedMovie, selectedDate
    );

    cout << "\nAvailable Shows:\n";
    for (auto* s : shows) {
        cout << "- " << s->getTime() << "\n";
    }

    // ---------- 1️⃣4️⃣ User selects show ----------
    Show* selectedShow = shows[0];
    cout << "\nSelected Show Time: " << selectedShow->getTime() << "\n";

    // ---------- 1️⃣5️⃣ User selects seats ----------
    vector<int> selectedSeats = {1, 2, 3};
    cout << "Selected Seats: ";
    for (int s : selectedSeats) cout << s << " ";
    cout << "\n";

    // ---------- 1️⃣6️⃣ Booking ----------
    BookingService bookingService;
    BookingController bookingController(&bookingService);

    try {
        Booking* booking = bookingController.createBooking(
            &user, selectedShow, selectedSeats
        );

        cout << "\n🎉 BOOKING SUCCESSFUL 🎉\n";
        cout << "Booking ID: " << booking->getBookingId() << "\n";
    }
    catch (exception& e) {
        cout << "\n❌ Booking Failed: " << e.what() << "\n";
    }

    return 0;
}
