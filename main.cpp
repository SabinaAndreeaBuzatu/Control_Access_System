#include <cstdint>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;


class AccessPoint
{
private:
    bool mUnlocked;
public:
    AccessPoint()
    {
        mUnlocked = false;
    }
    bool isUnlocked()
    {
        return mUnlocked;
    }
    void toggle()
    {
        if (mUnlocked)
        {
            mUnlocked = false;
        }
        else
        {
            mUnlocked = true;
        }
    }
};

class AccessDeniedException : public exception
{
public:
    AccessDeniedException()
    {

    }
};

class User
{
protected:
    string mUserId;
    set<uint64_t> mCards;
    vector<AccessPoint*> mAccessPoints;
    uint32_t mActivity;
    // char type;
public:
    User(const string& ID)
    {
        mUserId = ID;
        mActivity = 0;
    }

    virtual void toggleDoor(AccessPoint* other)
    {
        for (auto a : mAccessPoints)
        {
            if (other == a)
            {
                other->toggle();
                mActivity++;
                return;
            }
        }
        throw *new AccessDeniedException;
    }
    void addAccessPoint(AccessPoint* other)
    {
        for (auto a : mAccessPoints)
        {
            if (other == a)
                return;
        }
        mAccessPoints.push_back(other);
    }
    void addCard(uint64_t ID)
    {
        for (auto a : mCards)
        {
            if (ID == a)
                return;
        }
        mCards.insert(ID);
    }
    bool hasCard(uint64_t ID)
    {
        for (auto a : mCards)
        {
            if (a == ID)
                return true;
        }
        return false;
    }
    uint32_t countCards()
    {
        return mCards.size();
    }
    uint32_t getActivity()
    {
        return mActivity;
    }
    // void setType(char type)
    // {
    //     this->type = type;
    // }
};

class AdminUser : public User
{
public:
    AdminUser(const string& ID)
        :User(ID)
    {

    }
    void toggleDoor(AccessPoint* other) override
    {
        other->toggle();
        mActivity++;
    }
};

class Database
{
private:
    map<string, User*> mUsers;
    unordered_map<uint64_t, AccessPoint> mAccessPoints;
public:
    void addUser(const string& Id, char type)
    {
        if(type == 85)
        {
        User* a = new User(Id);
        mUsers.insert(make_pair(Id, a));
        }
        else
        {
            AdminUser* b = new AdminUser(Id);
            mUsers.insert(make_pair(Id, b));
        }
    }
    void addAccessPoint(uint64_t ID)
    {
        AccessPoint a;
        mAccessPoints.insert(make_pair(ID, a));
    }
    void addCardToUser(const string& name, uint64_t ID)
    {
        mUsers[name]->addCard(ID);
    }
    void addAccessPointToUser(const string& name,uint64_t ID)
    {
        AccessPoint* ptr = &mAccessPoints[ID];
        mUsers[name]->addAccessPoint(ptr);
    }
    void parseEvent(uint64_t ID1, uint64_t ID2)
    {
        for (auto a : mUsers)
        {
            if (a.second->hasCard(ID1) == true)
            {
                AccessPoint* ptr = &mAccessPoints[ID2];
                a.second->toggleDoor(ptr);
                return;
            }
        }
        throw* new AccessDeniedException;
    }
    uint32_t countUnlockedDoors()
    {
        uint32_t count = 0;
        for (auto a : mAccessPoints)
        {
            if(a.second.isUnlocked() == true)
            {
                count++;
            }
        }
        return count;
    }
    uint32_t countActivity()
    {
        uint32_t count = 0;
        for(auto a : mUsers)
        {
            count+=a.second->getActivity();
        }
        return count;
    }
    vector<string> findUsersWithNoCards()
    {
        vector<string> result;
        for(auto a : mUsers)
        {
            if(a.second->countCards() == 0)
            {
                result.push_back(a.first);
            }
        }
        return result;
    }
    uint32_t countCards()
    {
        uint32_t count = 0;
        for(auto a : mUsers)
        {
            count += a.second->countCards();
        }
        return count;
    }

};


int main() {
    uint32_t n, m, k;
    std::cin >> n >> m >> k;
    Database database;
    while (n--) {
        uint64_t accessPointId;
        std::cin >> accessPointId;
        database.addAccessPoint(accessPointId);
    }
    while (m--) {
        std::string userId;
        char type;
        std::cin >> userId >> type;
        database.addUser(userId, type);
        uint32_t c;
        std::cin >> c;
        while (c--) {
            uint64_t cardId;
            std::cin >> cardId;
            database.addCardToUser(userId, cardId);
        }
        uint32_t u;
        std::cin >> u;
        while (u--) {
            uint64_t accessPointId;
            std::cin >> accessPointId;
            database.addAccessPointToUser(userId, accessPointId);
        }
    }
    uint32_t errors = 0;

    while (k--) {
        uint64_t accessPointId, cardId;
        std::cin >> accessPointId >> cardId;
        try {
            database.parseEvent(cardId, accessPointId);
        }
        catch (AccessDeniedException& ex) {
            errors++;
        }

    }
    uint32_t subject;
    std::cin >> subject;
    switch (subject) {
    case 1: {
        std::cout << database.countUnlockedDoors();
        break;
    }
    case 2: {
        std::cout << database.countActivity();
        break;
    }
    case 3: {
        auto users = database.findUsersWithNoCards();
        for (const auto& user : users) std::cout << user << " ";
        break;
    }
    case 4: {
        std::cout << database.countCards();
        break;
    }
    case 5: {
        std::cout << errors;
        break;
    }
    }
    return 0;
}
