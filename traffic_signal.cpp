/******************************************************************************

traffic signal design using state design pattern

*******************************************************************************/
#include <iostream>
using namespace std;

class TrafficSignal;  

// State Interface
class TrafficSignalState {
public:
    virtual void handle(TrafficSignal* light) = 0;
    virtual ~TrafficSignalState() = default;
};

// Forward declare states
class RedState;
class GreenState;
class YellowState;

// Context
class TrafficSignal {
private:
    TrafficSignalState* currentState;

public:
    TrafficSignal();

    void setState(TrafficSignalState* state) {
        delete currentState;
        currentState = state;
    }

    void request() {
        currentState->handle(this);
    }

    ~TrafficSignal() {
        delete currentState;
    }
};

// Concrete States (only declaration of handle)

class RedState : public TrafficSignalState {
public:
    void handle(TrafficSignal* light) override;
};

class GreenState : public TrafficSignalState {
public:
    void handle(TrafficSignal* light) override;
};

class YellowState : public TrafficSignalState {
public:
    void handle(TrafficSignal* light) override;
};

// ---- Now define all handle methods AFTER full class definitions ----

void RedState::handle(TrafficSignal* light) {
    cout << "Red Light - Stop 🚫\n";
    light->setState(new GreenState());
}

void GreenState::handle(TrafficSignal* light) {
    cout << "Green Light - Go ✅\n";
    light->setState(new YellowState());
}

void YellowState::handle(TrafficSignal* light) {
    cout << "Yellow Light - Slow Down ⚠️\n";
    light->setState(new RedState());
}

// Constructor
TrafficSignal::TrafficSignal() {
    currentState = new RedState();
}

int main() {
    TrafficSignal signal;

    for (int i = 0; i < 6; i++) {
        signal.request();
    }

    return 0;
}
