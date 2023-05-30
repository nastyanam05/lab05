#include <iostream>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAccount : public Account {
public:
    MockAccount(int id, int balance):Account(id, balance){};
    MOCK_METHOD(void, Unlock, ());
    MOCK_METHOD(void, Lock, ());
    MOCK_METHOD(int, id, (), (const));
    MOCK_METHOD(void, ChangeBalance, (int diff), ());
    MOCK_METHOD(int, GetBalance, (), ());
};

class MockTransaction: public Transaction {
public:
    MOCK_METHOD(bool, Make, (Account& from, Account& to, int sum), ());
    MOCK_METHOD(void, set_fee, (int fee), ());
    MOCK_METHOD(int, fee, (), ());
};

TEST(Account, Account_test_1) {
    MockAccount acc(1, 100);
    EXPECT_CALL(acc, GetBalance()).Times(3);
    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, Unlock()).Times(1);
    EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(2);
    EXPECT_CALL(acc, id()).Times(1);
    acc.GetBalance();
    acc.id();
    acc.Unlock();
    acc.ChangeBalance(1000);
    acc.GetBalance();
    acc.ChangeBalance(2);
    acc.GetBalance();
    acc.Lock();
}

TEST(Account, Account_test_2) {
    Account acc(0, 100);
    EXPECT_THROW(acc.ChangeBalance(50), std::runtime_error);
    acc.Lock();
    acc.ChangeBalance(50);
    EXPECT_EQ(acc.GetBalance(), 150);
    EXPECT_THROW(acc.Lock(), std::runtime_error);
    acc.Unlock();
}

TEST(Transaction, Transaction_test) {
    MockTransaction trans;
    MockAccount acc1(1, 100);
    MockAccount acc2(2, 1000);
    MockAccount acc3(3, 10000);
    MockAccount acc4(4, 5000);
    EXPECT_CALL(trans, Make(testing::_, testing::_, testing::_)).Times(3);
    EXPECT_CALL(trans, set_fee(testing::_)).Times(1);
    trans.set_fee(200);
    //EXPECT_THROW(trans.Make(acc3, acc3, 1000), std::logic_error);
    //EXPECT_THROW(trans.Make(acc3, acc4, -1), std::invalid_argument);
    //EXPECT_THROW(trans.Make(acc3, acc4, 1), std::logic_error);
    EXPECT_EQ(trans.Make(acc3, acc4, 50), false);
    EXPECT_EQ(trans.Make(acc2, acc4, 2000), false);
    EXPECT_CALL(trans, fee()).Times(1);
    trans.Make(acc4, acc1, 1000);
    trans.fee();
}
