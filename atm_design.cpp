/******************************************************************************

ATM machine design pattern using state design pattern and chain of responsibility design pattern


*******************************************************************************/
#include <iostream>
#include <algorithm>
using namespace std;

/* ================================
   ENUM
================================ */
enum class OperationType {
    WITHDRAW,
    DEPOSIT,
    CHECK_BALANCE,
    PIN_CHANGE,
};

/* ================================
   DOMAIN
================================ */

class UserBankAccount {
private:
    int balance;

public:
    UserBankAccount(int bal) : balance(bal) {}

    int getBalance() { return balance; }

    void deductBalance(int amount) { balance -= amount; }
};

class Card {
private:
    int pin;
    UserBankAccount* bankAccount;

public:
    Card(int p, UserBankAccount* acc)
        : pin(p), bankAccount(acc) {}

    bool isCorrectPINEntered(int enteredPin) {
        return pin == enteredPin;
    }

    int getBankBalance() {
        return bankAccount->getBalance();
    }

    void deductBankBalance(int amount) {
        bankAccount->deductBalance(amount);
    }
};

/* ================================
   FORWARD DECLARATIONS
================================ */

class ATM;
class IdleState;
class HasCardState;
class SelectOperationState;
class CashWithdrawalState;
class CheckBalanceState;

/* ================================
   STATE BASE
================================ */

class ATMState {
public:
    virtual ~ATMState() = default;

    virtual void insertCard(ATM*, Card*) {}
    virtual void authenticatePin(ATM*, Card*, int) {}
    virtual void selectOperation(ATM*, Card*, OperationType) {}
    virtual void cashWithdrawal(ATM*, Card*, int) {}
    virtual void displayBalance(ATM*, Card*) {}
    virtual void exit(ATM*) {}
    virtual void returnCard() {}
};

/* ================================
   ATM CLASS
================================ */

class ATM {

private:
    ATMState* currentATMState;

    int atmBalance;
    int noOfTwoThousandNotes;
    int noOfFiveHundredNotes;
    int noOfOneHundredNotes;

public:
    ATM(int balance, int twoK, int fiveH, int oneH);

    void setCurrentATMState(ATMState* state) {
        currentATMState = state;
    }

    ATMState* getCurrentATMState() {
        return currentATMState;
    }

    // Getters
    int getAtmBalance() { return atmBalance; }
    int getNoOfTwoThousandNotes() { return noOfTwoThousandNotes; }
    int getNoOfFiveHundredNotes() { return noOfFiveHundredNotes; }
    int getNoOfOneHundredNotes() { return noOfOneHundredNotes; }

    // Deduction
    void deductATMBalance(int amount) {
        atmBalance -= amount;
    }

    void deductTwoThousandNotes(int number) {
        noOfTwoThousandNotes -= number;
    }

    void deductFiveHundredNotes(int number) {
        noOfFiveHundredNotes -= number;
    }

    void deductOneHundredNotes(int number) {
        noOfOneHundredNotes -= number;
    }

    void printCurrentATMStatus() {
        cout << "ATM Balance: " << atmBalance << endl;
        cout << "2k Notes: " << noOfTwoThousandNotes << endl;
        cout << "500 Notes: " << noOfFiveHundredNotes << endl;
        cout << "100 Notes: " << noOfOneHundredNotes << endl;
        cout << "---------------------------\n";
    }
};

/* ================================
   CHAIN OF RESPONSIBILITY
================================ */

class CashWithdrawProcessor {
protected:
    CashWithdrawProcessor* next;

public:
    CashWithdrawProcessor(CashWithdrawProcessor* nxt)
        : next(nxt) {}

    virtual void withdraw(ATM* atm, int amount) {
        if (next)
            next->withdraw(atm, amount);
    }
};

class TwoThousandWithdrawProcessor : public CashWithdrawProcessor {
public:
    TwoThousandWithdrawProcessor(CashWithdrawProcessor* nxt)
        : CashWithdrawProcessor(nxt) {}

    void withdraw(ATM* atm, int amount) override {

        int required = amount / 2000;
        int balance = amount % 2000;

        int available = atm->getNoOfTwoThousandNotes();
        int toDeduct = min(required, available);

        atm->deductTwoThousandNotes(toDeduct);
        atm->deductATMBalance(toDeduct * 2000);

        balance += (required - toDeduct) * 2000;

        if (balance > 0)
            CashWithdrawProcessor::withdraw(atm, balance);
    }
};

class FiveHundredWithdrawProcessor : public CashWithdrawProcessor {
public:
    FiveHundredWithdrawProcessor(CashWithdrawProcessor* nxt)
        : CashWithdrawProcessor(nxt) {}

    void withdraw(ATM* atm, int amount) override {

        int required = amount / 500;
        int balance = amount % 500;

        int available = atm->getNoOfFiveHundredNotes();
        int toDeduct = min(required, available);

        atm->deductFiveHundredNotes(toDeduct);
        atm->deductATMBalance(toDeduct * 500);

        balance += (required - toDeduct) * 500;

        if (balance > 0)
            CashWithdrawProcessor::withdraw(atm, balance);
    }
};

class OneHundredWithdrawProcessor : public CashWithdrawProcessor {
public:
    OneHundredWithdrawProcessor(CashWithdrawProcessor* nxt)
        : CashWithdrawProcessor(nxt) {}

    void withdraw(ATM* atm, int amount) override {

        int required = amount / 100;
        int balance = amount % 100;

        if (required <= atm->getNoOfOneHundredNotes()) {
            atm->deductOneHundredNotes(required);
            atm->deductATMBalance(required * 100);
        } else {
            cout << "Cannot dispense exact amount\n";
            return;
        }

        if (balance != 0)
            cout << "Cannot dispense exact amount\n";
    }
};

/* ================================
   STATE DECLARATIONS
================================ */

class IdleState : public ATMState {
public:
    void insertCard(ATM*, Card*) override;
};

class HasCardState : public ATMState {
public:
    HasCardState();
    void authenticatePin(ATM*, Card*, int) override;
    void exit(ATM*) override;
    void returnCard() override;
};

class SelectOperationState : public ATMState {
public:
    void selectOperation(ATM*, Card*, OperationType) override;
};

class CashWithdrawalState : public ATMState {
public:
    CashWithdrawalState();
    void cashWithdrawal(ATM*, Card*, int) override;
    void exit(ATM*) override;
    void returnCard() override;
};

class CheckBalanceState : public ATMState {
public:
    void displayBalance(ATM*, Card*) override;
    void exit(ATM*) override;
    void returnCard() override;
};

/* ================================
   ATM IMPLEMENTATION
================================ */

ATM::ATM(int balance, int twoK, int fiveH, int oneH)
    : atmBalance(balance),
      noOfTwoThousandNotes(twoK),
      noOfFiveHundredNotes(fiveH),
      noOfOneHundredNotes(oneH) {

    currentATMState = new IdleState();
}


/* ================================
   STATE IMPLEMENTATIONS
================================ */

void IdleState::insertCard(ATM* atm, Card* card) {
    cout << "Card Inserted\n";
    atm->setCurrentATMState(new HasCardState());
}

HasCardState::HasCardState() {
    cout << "Enter PIN\n";
}

void HasCardState::authenticatePin(ATM* atm, Card* card, int pin) {
    if (card->isCorrectPINEntered(pin)) {
        atm->setCurrentATMState(new SelectOperationState());
    } else {
        cout << "Invalid PIN\n";
        exit(atm);
    }
}

void HasCardState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

void HasCardState::returnCard() {
    cout << "Please collect your card\n";
}

void SelectOperationState::selectOperation(ATM* atm, Card* card, OperationType type) {
    if (type == OperationType::WITHDRAW)
        atm->setCurrentATMState(new CashWithdrawalState());
    else
        atm->setCurrentATMState(new CheckBalanceState());
}

CashWithdrawalState::CashWithdrawalState() {
    cout << "Enter Withdrawal Amount\n";
}

void CashWithdrawalState::cashWithdrawal(ATM* atm, Card* card, int amount) {

    if (atm->getAtmBalance() < amount) {
        cout << "ATM Insufficient Funds\n";
        exit(atm);
        return;
    }

    if (card->getBankBalance() < amount) {
        cout << "Bank Insufficient Funds\n";
        exit(atm);
        return;
    }

    card->deductBankBalance(amount);

    CashWithdrawProcessor* processor =
        new TwoThousandWithdrawProcessor(
        new FiveHundredWithdrawProcessor(
        new OneHundredWithdrawProcessor(nullptr)));

    processor->withdraw(atm, amount);

    cout << "Withdrawal Successful\n";
    exit(atm);
}

void CashWithdrawalState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

void CashWithdrawalState::returnCard() {
    cout << "Please collect your card\n";
}

void CheckBalanceState::displayBalance(ATM* atm, Card* card) {
    cout << "Your Balance: " << card->getBankBalance() << endl;
    exit(atm);
}

void CheckBalanceState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

void CheckBalanceState::returnCard() {
    cout << "Please collect your card\n";
}

/* ================================
   MAIN
================================ */

int main() {

    ATM atm(3500, 1, 2, 5);

    UserBankAccount account(3000);
    Card card(112211, &account);

    atm.printCurrentATMStatus();

    atm.getCurrentATMState()->insertCard(&atm, &card);
    atm.getCurrentATMState()->authenticatePin(&atm, &card, 112211);
    atm.getCurrentATMState()->selectOperation(&atm, &card, OperationType::WITHDRAW);
    atm.getCurrentATMState()->cashWithdrawal(&atm, &card, 2700);

    atm.printCurrentATMStatus();

    return 0;
}
