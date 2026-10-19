
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;


//  PART 1: DATA  (arrays)
const int STOCKS = 5;     
const int DAYS = 120;     

string stockName[STOCKS]  = {"RELIANCE", "TCS", "INFY", "HDFCBANK", "ITC"};
double startPrice[STOCKS] = {2400, 3400, 1500, 1650, 430};
double drift[STOCKS]      = {0.0006, 0.0004, 0.0002, 0.0005, 0.0008};  
double swing[STOCKS]      = {0.015, 0.012, 0.017, 0.013, 0.011};       
double allPrices[STOCKS][DAYS];    
void makeMarketData()
{
    for (int s = 0; s < STOCKS; s++)
    {
        srand(500 + s);                      
        allPrices[s][0] = startPrice[s];
        for (int d = 1; d < DAYS; d++)
        {
            double noise = ((rand() % 2001) - 1000) / 1000.0;      
            double change = drift[s] + noise * swing[s] * 1.7;     
            allPrices[s][d] = allPrices[s][d - 1] * (1 + change);
        }
    }
    srand(time(0));                           
}
// ----------------------------------------------------------------------------
//  PART 2: SMALL HELPER FUNCTIONS
// ----------------------------------------------------------------------------

string ask(string question)
{
    cout << question;
    string text;
    if (!getline(cin, text))                 
    {
        cout << "\nGoodbye.\n";
        exit(0);
    }
    if (text.length() > 0 && text[text.length() - 1] == '\r')   
        text = text.substr(0, text.length() - 1);
    return text;
}
void printPercent(ostream &out, double p)
{
    if (p > 0)
        out << "+";
    out << p << "%";
}
string lowerCase(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}
void title(string text)
{
    cout << "\n" << text << "\n";
    cout << "------------------------------------------------------------\n";
}

// ----------------------------------------------------------------------------
//  PART 3: OPERATOR OVERLOADING  -  class Money
// ----------------------------------------------------------------------------
class Money
{
    double value;
public:
    Money(double v = 0)
    {
        value = v;
    }
    Money operator+(Money other)              
    {
        return Money(value + other.value);
    }
    Money operator-(Money other)              
    {
        return Money(value - other.value);
    }
    bool operator>(Money other)               
    {
        return value > other.value;
    }
    void show()                               
    {
        long rounded = (long)floor(value + 0.5);
        cout << "Rs " << rounded;
    }
};

// ----------------------------------------------------------------------------
//  PART 4: INHERITANCE
//
//      
// ----------------------------------------------------------------------------


class Stock
{
protected:                                    
    string name;
    double price[DAYS];
    int days;
public:
    Stock(int id)
    {
        name = stockName[id];
        days = DAYS;
        for (int d = 0; d < days; d++)
            price[d] = allPrices[id][d];
    }
    string getName()
    {
        return name;
    }
    double last()                             
    {
        return price[days - 1];
    }
    double dayChange()                        
    {
        return (price[days - 1] - price[days - 2]) / price[days - 2] * 100;
    }
    double change(int daysBack)               
    {
        double old = price[days - 1 - daysBack];
        return (price[days - 1] - old) / old * 100;
    }
    double average(int count)                 
    {
        double sum = 0;
        for (int d = days - count; d < days; d++)
            sum = sum + price[d];
        return sum / count;
    }
    double highest(int count)                 
    {
        double best = price[days - 1];
        for (int d = days - count; d < days; d++)
            if (price[d] > best)
                best = price[d];
        return best;
    }
    double lowest(int count)                  
    {
        double low = price[days - 1];
        for (int d = days - count; d < days; d++)
            if (price[d] < low)
                low = price[d];
        return low;
    }
};


class PriceAnalysis : public Stock
{
public:
    PriceAnalysis(int id) : Stock(id)         
    {
    }
    void showPrice()
    {
        title("PRICE ANALYSIS - " + name);

        // A simple bar chart of the last 15 days
        cout << "Last 15 days (a longer bar means a higher price)\n";
        double lo = lowest(15);
        double hi = highest(15);
        for (int d = days - 15; d < days; d++)
        {
            int bar = 1;
            if (hi > lo)
                bar = 1 + (int)((price[d] - lo) / (hi - lo) * 40);
            cout << "Day " << setw(3) << d + 1 << " " << setw(9) << price[d] << "  ";
            for (int i = 0; i < bar; i++)
                cout << "#";
            cout << "\n";
        }

        cout << "\nLast price      : Rs " << last() << "\n";
        cout << "Today's change  : ";
        printPercent(cout, dayChange());
        cout << "\n1 week change   : ";
        printPercent(cout, change(5));
        cout << "\n1 month change  : ";
        printPercent(cout, change(20));
        cout << "\n3 month change  : ";
        printPercent(cout, change(60));
        cout << "\n\nHighest (all data): Rs " << highest(days) << "\n";
        cout << "Lowest  (all data): Rs " << lowest(days) << "\n";
        cout << "Average (all data): Rs " << average(days) << "\n";
    }
};


class TrendAnalysis : public PriceAnalysis
{
    int score;                                
public:
    TrendAnalysis(int id) : PriceAnalysis(id)
    {
        
        score = 0;
        if (last() > average(20))             
            score++;
        else
            score--;
        if (average(5) > average(20))         
            score++;
        else
            score--;
        if (change(20) > 0)                   
            score++;
        else
            score--;
    }
    int getScore()
    {
        return score;
    }
    string verdict()
    {
        if (score == 3)
            return "Strong uptrend";
        if (score == 1)
            return "Uptrend";
        if (score == -1)
            return "Downtrend";
        return "Strong downtrend";
    }
    void showTrend()
    {
        title("TREND ANALYSIS - " + name);
        cout << "Last price           : Rs " << last() << "\n";
        cout << "5-day average        : Rs " << average(5) << "\n";
        cout << "20-day average       : Rs " << average(20) << "\n";
        cout << "60-day average       : Rs " << average(60) << "\n";
        cout << "Support (20-day low) : Rs " << lowest(20) << "\n";
        cout << "Resistance (20-day high): Rs " << highest(20) << "\n\n";

        cout << "Check 1: price vs 20-day average     -> ";
        if (last() > average(20)) cout << "above (bullish)\n"; else cout << "below (bearish)\n";
        cout << "Check 2: 5-day vs 20-day average     -> ";
        if (average(5) > average(20)) cout << "above (bullish)\n"; else cout << "below (bearish)\n";
        cout << "Check 3: price vs 20 days ago        -> ";
        if (change(20) > 0) cout << "higher (bullish)\n"; else cout << "lower (bearish)\n";

        cout << "\nScore  : " << score << "  (from -3 to +3)\n";
        cout << "Verdict: " << verdict() << "\n";
    }
};

class VolatilityAnalysis : public Stock
{
    double ret[DAYS];                          
    int count;                                
    double dailyStd;                          
    double yearlyStd;                         
    double maxDrawdown;                       
    double bestDay, worstDay;
public:
    VolatilityAnalysis(int id) : Stock(id)
    {
        
        count = days - 1;
        for (int d = 1; d < days; d++)
            ret[d - 1] = (price[d] - price[d - 1]) / price[d - 1] * 100;

        
        double sum = 0;
        bestDay = ret[0];
        worstDay = ret[0];
        for (int i = 0; i < count; i++)
        {
            sum = sum + ret[i];
            if (ret[i] > bestDay)
                bestDay = ret[i];
            if (ret[i] < worstDay)
                worstDay = ret[i];
        }
        double mean = sum / count;

        
        double squares = 0;
        for (int i = 0; i < count; i++)
            squares = squares + (ret[i] - mean) * (ret[i] - mean);
        dailyStd = sqrt(squares / (count - 1));
        yearlyStd = dailyStd * sqrt(252.0);   

        
        double peak = price[0];
        maxDrawdown = 0;
        for (int d = 0; d < days; d++)
        {
            if (price[d] > peak)
                peak = price[d];
            double drop = (price[d] - peak) / peak * 100;
            if (drop < maxDrawdown)
                maxDrawdown = drop;
        }
    }
    double getYearly()
    {
        return yearlyStd;
    }
    string riskLevel()
    {
        if (yearlyStd < 15)
            return "Low";
        if (yearlyStd < 25)
            return "Medium";
        return "High";
    }
    void showRisk()
    {
        title("VOLATILITY ANALYSIS - " + name);
        cout << "Typical daily move   : " << dailyStd << "%\n";
        cout << "Yearly volatility    : " << yearlyStd << "%\n";
        cout << "Risk level           : " << riskLevel() << "   (below 15% low, below 25% medium, else high)\n";
        cout << "Best day             : ";
        printPercent(cout, bestDay);
        cout << "\nWorst day            : ";
        printPercent(cout, worstDay);
        cout << "\nMax drawdown         : " << maxDrawdown << "%   (biggest fall from a peak)\n";


        double badDayLoss = 1.65 * dailyStd / 100 * 100000;
        cout << "\nOn Rs 100000 invested, a bad day (1 in 20) could lose about Rs "
             << (long)badDayLoss << ".\n";
    }
};

// ----------------------------------------------------------------------------
//  PART 5: NEWS SENTIMENT  (strings)

// ----------------------------------------------------------------------------
const int WORDS = 10;
string goodWords[WORDS] = {"profit", "growth", "strong", "gain", "record",
                           "upgrade", "wins", "beats", "dividend", "expansion"};
string badWords[WORDS]  = {"loss", "weak", "fall", "cut", "downgrade",
                           "probe", "delay", "warns", "drop", "decline"};


string sampleNews[STOCKS][3] = {
    {"Reliance posts record profit as retail growth stays strong",
     "Analysts upgrade Reliance after expansion plan",
     "Reliance warns of delay in new energy project"},
    {"TCS wins large deal, shares gain",
     "TCS faces weak demand in Europe",
     "TCS announces dividend"},
    {"Infosys cuts revenue guidance",
     "Infosys faces probe over a contract",
     "Infosys hires more freshers this year"},
    {"HDFC Bank reports strong loan growth",
     "HDFC Bank profit beats estimates",
     "HDFC Bank shares drop on market fall"},
    {"ITC dividend surprises investors",
     "ITC FMCG business shows growth",
     "ITC hotels demerger update expected"}};

const int MAXNEWS = 10;

class NewsSentiment
{
    string headline[MAXNEWS];
    int score[MAXNEWS];
    int count;
    string stock;
public:
    void init(int id)                         
    {
        stock = stockName[id];
        count = 3;
        for (int i = 0; i < 3; i++)
            headline[i] = sampleNews[id][i];
        scoreAll();
    }
    int scoreText(string text)                
    {
        text = lowerCase(text);
        int result = 0;
        for (int i = 0; i < WORDS; i++)
        {
            if (text.find(goodWords[i]) != string::npos)
                result++;
            if (text.find(badWords[i]) != string::npos)
                result--;
        }
        return result;
    }
    void scoreAll()
    {
        for (int i = 0; i < count; i++)
            score[i] = scoreText(headline[i]);
    }
    int total()                               
    {
        int sum = 0;
        for (int i = 0; i < count; i++)
            sum = sum + score[i];
        return sum;
    }
    string label()
    {
        if (total() > 0)
            return "Positive";
        if (total() < 0)
            return "Negative";
        return "Neutral";
    }
    void clearAll()
    {
        count = 0;
    }
    bool addHeadline(string text)
    {
        if (count >= MAXNEWS)
            return false;
        headline[count] = text;
        score[count] = scoreText(text);
        count++;
        return true;
    }
    void show()
    {
        title("NEWS SENTIMENT - " + stock);
        for (int i = 0; i < count; i++)
            cout << setw(3) << score[i] << "  " << headline[i] << "\n";
        cout << "\nTotal score: " << total() << "  ->  " << label() << "\n";
        cout << "(Sample headlines are made up. Type your own to test real news.)\n";
    }
};

NewsSentiment news[STOCKS];                   

// ----------------------------------------------------------------------------
//  PART 6: PORTFOLIO SIMULATOR
// ----------------------------------------------------------------------------
class Portfolio
{
    double weight[STOCKS];                    
    double amount;                            
    double dailyReturn[DAYS];                 
public:
    Portfolio()
    {
        amount = 100000;
        for (int s = 0; s < STOCKS; s++)
            weight[s] = 20;                   
    }
    void setup()
    {
        string text = ask("Money to invest in Rs [" + string("100000") + " if you press Enter]: ");
        if (text.length() > 0)
            amount = atof(text.c_str());
        if (amount < 1000)
            amount = 1000;

        cout << "Enter the percent of money for each stock (press Enter for 20):\n";
        for (int s = 0; s < STOCKS; s++)
        {
            text = ask("  " + stockName[s] + ": ");
            if (text.length() > 0)
                weight[s] = atof(text.c_str());
            else
                weight[s] = 20;
            if (weight[s] < 0)
                weight[s] = 0;
        }
    }
    bool calculate()                          
    {
        double total = 0;
        for (int s = 0; s < STOCKS; s++)
            total = total + weight[s];
        if (total <= 0)
            return false;

        for (int d = 1; d < DAYS; d++)
        {
            double r = 0;
            for (int s = 0; s < STOCKS; s++)
            {
                double stockReturn = allPrices[s][d] / allPrices[s][d - 1] - 1;
                r = r + weight[s] / total * stockReturn;
            }
            dailyReturn[d - 1] = r;
        }
        return true;
    }
    void backtest()                           
    {
        cout << "\nPAST PERFORMANCE (using the " << DAYS << " days of data)\n";
        Money start(amount);
        double value = amount;
        for (int d = 0; d < DAYS - 1; d++)
        {
            value = value * (1 + dailyReturn[d]);
            if ((d + 1) % 30 == 0)
                cout << "  After " << setw(3) << d + 1 << " days: Rs " << (long)value << "\n";
        }
        Money end(value);
        Money profit = end - start;           
        cout << "  Start: ";
        start.show();
        cout << "\n  End  : ";
        end.show();
        if (end > start)                      
        {
            cout << "\n  Gain : ";
            profit.show();
        }
        else
        {
            cout << "\n  Loss : ";
            (start - end).show();
        }
        cout << "\n";
    }
    void simulate()                           
    {
        int trials = 500;                     
        int daysAhead = 60;                   
        double best = 0, worst = 1e18, sum = 0;
        int losses = 0;

        for (int t = 0; t < trials; t++)
        {
            double value = amount;
            for (int d = 0; d < daysAhead; d++)
            {
                int pick = rand() % (DAYS - 1);        
                value = value * (1 + dailyReturn[pick]);
            }
            if (value > best)
                best = value;
            if (value < worst)
                worst = value;
            sum = sum + value;
            if (value < amount)
                losses++;
        }

        cout << "\nSIMULATION: " << trials << " random futures of " << daysAhead << " days\n";
        cout << "  Best case    : Rs " << (long)best << "\n";
        cout << "  Worst case   : Rs " << (long)worst << "\n";
        cout << "  Average      : Rs " << (long)(sum / trials) << "\n";
        cout << "  Chance of loss: " << (double)losses / trials * 100 << "%\n";
        cout << "\n(The simulation re-uses past daily changes in random order.\n";
        cout << " It shows possible risk. It does not predict the future.)\n";
    }
};

// ----------------------------------------------------------------------------
//  PART 7: FULL REPORT  (file handling)

// ----------------------------------------------------------------------------
void writeReport(int id, string user)
{
    TrendAnalysis t(id);
    VolatilityAnalysis v(id);
    string fileName = "report_" + stockName[id] + ".txt";

    ofstream out(fileName.c_str());          
    if (!out)
    {
        cout << "Could not create " << fileName << "\n";
        return;
    }
    out << fixed << setprecision(2);
    out << "STOCK ANALYSIS REPORT: " << stockName[id] << "\n";
    out << "Prepared for: " << user << "\n";
    out << "============================================================\n\n";

    out << "PRICE\n";
    out << "  Last price      : Rs " << t.last() << "\n";
    out << "  Today           : ";
    printPercent(out, t.dayChange());
    out << "\n  1 month         : ";
    printPercent(out, t.change(20));
    out << "\n  3 months        : ";
    printPercent(out, t.change(60));
    out << "\n  Highest / Lowest: Rs " << t.highest(DAYS) << " / Rs " << t.lowest(DAYS) << "\n\n";

    out << "TREND\n";
    out << "  Verdict         : " << t.verdict() << " (score " << t.getScore() << ")\n";
    out << "  5-day average   : Rs " << t.average(5) << "\n";
    out << "  20-day average  : Rs " << t.average(20) << "\n";
    out << "  Support         : Rs " << t.lowest(20) << "\n";
    out << "  Resistance      : Rs " << t.highest(20) << "\n\n";

    out << "VOLATILITY\n";
    out << "  Yearly volatility: " << v.getYearly() << "%\n";
    out << "  Risk level       : " << v.riskLevel() << "\n\n";

    out << "NEWS SENTIMENT\n";
    out << "  Overall          : " << news[id].label() << " (score " << news[id].total() << ")\n\n";

    out << "SUMMARY\n";
    out << "  " << stockName[id] << " is in a " << t.verdict() << ", with "
        << v.riskLevel() << " risk and " << news[id].label() << " news.\n\n";
    out << "Prices and headlines are simulated sample data.\n";
    out << "This report is for learning and is not investment advice.\n";
    out.close();                              

    
    title("FULL REPORT");
    ifstream in(fileName.c_str());            
    string line;
    while (getline(in, line))
        cout << line << "\n";
    in.close();
    cout << "\nSaved as " << fileName << " in the current folder.\n";
}

// ----------------------------------------------------------------------------
//  PART 8: LOGIN SYSTEM  (file handling)

// ----------------------------------------------------------------------------
long makeCode(string password)
{
    long code = 7;
    for (int i = 0; i < (int)password.length(); i++)
        code = (code * 31 + password[i]) % 1000000007;
    return code;
}

bool userExists(string name)
{
    ifstream in("users.txt");
    string u;
    long code;
    while (in >> u >> code)                   
    {
        if (u == name)
            return true;
    }
    return false;
}

bool passwordCorrect(string name, string password)
{
    ifstream in("users.txt");
    string u;
    long code;
    while (in >> u >> code)
    {
        if (u == name && code == makeCode(password))
            return true;
    }
    return false;
}

string currentUser = "";

bool createAccount()
{
    title("CREATE ACCOUNT");
    string name = lowerCase(ask("Choose a username (3+ letters, no spaces): "));
    if (name.length() < 3 || name.find(' ') != string::npos)
    {
        cout << "That username is not allowed.\n";
        return false;
    }
    if (userExists(name))
    {
        cout << "That username is already taken.\n";
        return false;
    }
    string pass1 = ask("Choose a password (6+ characters): ");
    if (pass1.length() < 6)
    {
        cout << "The password is too short.\n";
        return false;
    }
    string pass2 = ask("Type the password again: ");
    if (pass1 != pass2)
    {
        cout << "The passwords do not match.\n";
        return false;
    }
    ofstream out("users.txt", ios::app);      
    out << name << " " << makeCode(pass1) << "\n";
    out.close();
    currentUser = name;
    cout << "Account created. Welcome, " << name << "!\n";
    return true;
}

bool signIn()
{
    title("SIGN IN");
    for (int attempt = 1; attempt <= 3; attempt++)     
    {
        string name = lowerCase(ask("Username: "));
        string pass = ask("Password: ");
        if (passwordCorrect(name, pass))
        {
            currentUser = name;
            cout << "Signed in. Welcome back, " << name << "!\n";
            return true;
        }
        cout << "Wrong username or password.\n";
    }
    cout << "Too many wrong tries.\n";
    return false;
}

void useDemoAccount()
{
    if (!userExists("demo"))
    {
        ofstream out("users.txt", ios::app);
        out << "demo " << makeCode("demo123") << "\n";
        out.close();
    }
    currentUser = "demo";
    cout << "Signed in with the demo account.\n";
}

// ----------------------------------------------------------------------------
//  PART 9: SCREENS AND MENUS
// ----------------------------------------------------------------------------
int current = 0;                              
Portfolio portfolio;

void showDashboard()
{
    title("DASHBOARD");
    cout << left << setw(12) << "Stock" << right << setw(10) << "Price"
         << setw(10) << "Today" << setw(10) << "1 Month"
         << "  " << left << setw(18) << "Trend" << setw(8) << "Risk" << "News\n";
    for (int s = 0; s < STOCKS; s++)
    {
        TrendAnalysis t(s);
        VolatilityAnalysis v(s);
        cout << left << setw(12) << stockName[s] << right << setw(10) << t.last();
        cout << setw(9) << t.dayChange() << "%";
        cout << setw(9) << t.change(20) << "%";
        cout << "  " << left << setw(18) << t.verdict() << setw(8) << v.riskLevel()
             << news[s].label() << "\n";
    }
    cout << right;
    cout << "\nSelected stock: " << stockName[current] << "\n";
}

void chooseStock()
{
    title("CHOOSE A STOCK");
    for (int s = 0; s < STOCKS; s++)
        cout << "  " << s + 1 << ") " << stockName[s] << "\n";
    int pick = atoi(ask("Number: ").c_str());
    if (pick >= 1 && pick <= STOCKS)
    {
        current = pick - 1;
        cout << "Selected " << stockName[current] << ".\n";
    }
    else
    {
        cout << "Not a valid number.\n";
    }
}

void newsScreen()
{
    news[current].show();
    string answer = lowerCase(ask("\nType your own headlines? (y/n): "));
    if (answer == "y")
    {
        news[current].clearAll();
        cout << "Type one headline per line. Press Enter on an empty line to finish.\n";
        while (true)
        {
            string text = ask("> ");
            if (text.length() == 0)
                break;
            if (!news[current].addHeadline(text))
            {
                cout << "That is the maximum number of headlines.\n";
                break;
            }
        }
        news[current].show();
    }
}

void portfolioScreen()
{
    title("PORTFOLIO SIMULATOR");
    portfolio.setup();
    if (!portfolio.calculate())
    {
        cout << "Give at least one stock a weight above 0.\n";
        return;
    }
    portfolio.backtest();
    portfolio.simulate();
}

void waitForEnter()
{
    ask("\nPress Enter to continue...");
}

void runApp()
{
    int choice = -1;
    while (choice != 0)
    {
        cout << "\n===== STOCK PRICE ANALYZER =====  user: " << currentUser
             << "  stock: " << stockName[current] << "\n";
        cout << " 1) Dashboard           5) News sentiment\n";
        cout << " 2) Price analysis      6) Portfolio simulator\n";
        cout << " 3) Trend analysis      7) Full report\n";
        cout << " 4) Volatility analysis 8) Change stock\n";
        cout << " 0) Sign out\n";
        choice = atoi(ask("Choose: ").c_str());

        switch (choice)
        {
        case 1:
            showDashboard();
            waitForEnter();
            break;
        case 2:
        {
            PriceAnalysis p(current);
            p.showPrice();
            waitForEnter();
            break;
        }
        case 3:
        {
            TrendAnalysis t(current);
            t.showTrend();
            waitForEnter();
            break;
        }
        case 4:
        {
            VolatilityAnalysis v(current);
            v.showRisk();
            waitForEnter();
            break;
        }
        case 5:
            newsScreen();
            break;
        case 6:
            portfolioScreen();
            waitForEnter();
            break;
        case 7:
            writeReport(current, currentUser);
            waitForEnter();
            break;
        case 8:
            chooseStock();
            break;
        case 0:
            cout << "Signed out.\n";
            break;
        default:
            cout << "Please pick a number from the menu.\n";
        }
    }
}

// ----------------------------------------------------------------------------
//  PART 10: MAIN  -  start here
// ----------------------------------------------------------------------------
int main()
{
    makeMarketData();                         
    for (int s = 0; s < STOCKS; s++)
        news[s].init(s);                      
    cout << fixed << setprecision(2);         

    cout << "\nSTOCK PRICE ANALYZER\n";
    cout << "Prices are simulated.\n";

    int choice = -1;
    while (choice != 0)
    {
        cout << "\n 1) Sign in\n 2) Create account\n 3) Try the demo account\n 0) Exit\n";
        choice = atoi(ask("Choose: ").c_str());

        if (choice == 1)
        {
            if (signIn())
                runApp();
        }
        else if (choice == 2)
        {
            if (createAccount())
                runApp();
        }
        else if (choice == 3)
        {
            useDemoAccount();
            runApp();
        }
        else if (choice != 0)
        {
            cout << "Please pick 0, 1, 2 or 3.\n";
        }
    }
    cout << "\nGoodbye.\n";
    return 0;
}