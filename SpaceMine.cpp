#include <string>
#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#ifdef _WIN32
#include <locale.h>
#include <vector>
#include <Windows.h>

std::string utf8_to_cp1251(std::string const& utf8)
{// конвертация строки из utf8 в cp1251
    if (!utf8.empty())
    {
        int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
        if (wchlen > 0 && wchlen != 0xFFFD)
        {
            std::vector<wchar_t> wbuf(wchlen);
            int result_u = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
            if (!result_u) {
                throw std::runtime_error("utf8_to_cp1251 cannot convert MultiByteToWideChar!");
            }
            std::vector<char> buf(wchlen);
            int result_c = WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);
            if (!result_c) {
                throw std::runtime_error("utf8_to_cp1251 cannot convert WideCharToMultiByte!");
            }

            return std::string(&buf[0], wchlen);
        }
    }
    return std::string();
}

std::string cp1251_to_utf8(const std::string& cp1251)
{// конвертация строки из cp1251 в utf8
    std::string res;
    int result_u, result_c;
    enum { CP1251 = 1251 };
    result_u = MultiByteToWideChar(CP1251, 0, cp1251.c_str(), -1, 0, 0);
    if (!result_u) {
        throw std::runtime_error("cp1251_to_utf8 cannot convert MultiByteToWideChar!");
    }
    wchar_t* ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(CP1251, 0, cp1251.c_str(), -1, ures, result_u)) {
        delete[] ures;
        throw std::runtime_error("cp1251_to_utf8 cannot convert MultiByteToWideChar 2!");
    }
    result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c) {
        delete[] ures;
        throw std::runtime_error("cp1251_to_utf8 cannot convert WideCharToMultiByte!");
    }
    char* cres = new char[result_c];
    if (!WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0)) {
        delete[] cres;
        throw std::runtime_error("cp1251_to_utf8 cannot convert WideCharToMultiByte 2!");
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

#endif


const std::string prompt(">");

void writeOutput(const std::string& something)
{// строка пользователю на вывод
    std::string output = something;
#ifdef _WIN32
    output = utf8_to_cp1251(output);
#endif
    std::cout << output;
}

std::string readUserInput()
{// строка пользователю на ввод
    std::string input;
    std::getline(std::cin, input);
    std::transform(input.begin(), input.end(), input.begin(), tolower);
#ifdef _WIN32
    input = cp1251_to_utf8(input);
#endif
    return input;
}

void help(int help_num)
{// помощь
    switch (help_num)
    {
    case 0:
        writeOutput("1. Начать новую игру - вы начинаете новую игру, со всеми огрехами и полноценным вступлением.\n");
        writeOutput("   Режим рекомендован тем, кто только начинает знакомиться с игрой.\n");
        writeOutput("2. Начать игру на счет - новая игра без вступления со всеми возможными усложнителями и без возможности сохранения.\n");
        writeOutput("3. Загрузить игру - загружает ранее сохраненную игру.\n");
        writeOutput("4. Таблица лидеров - отображает имя игрока и счет, с которым игрок полностью закончил игру, не важно успешно или нет.\n");
        writeOutput("5. Описание - небольшое художественное описание игры, которое помимо прочего содержит и некоторые намеки на происходящее в ней.\n");
        writeOutput("6. Помощь - список команд и их краткое описание. Данная команда доступна в любой момент,\n");
        writeOutput("   когда игра не просит указать количество или число и содержит в себе полный список команд доступный вам на данном этапе игры.\n");
        writeOutput("7. Выход - выход из игры.\n\n");
        break;
    case 1:
        writeOutput("1.  Продать - позволяет продать шахту, минералы или продовольствие, за цену, регламентированную рынком.\n");
        writeOutput("2.  Купить - позволяет купить шахту, минералы или продовольствие, за цену, регламентированную рынком.\n");
        writeOutput("3.  Cтатистика - выводит полноценную статистику, полностью аналогичную получаемой в начале года.\n");
        writeOutput("4.  Добывать - через эту команду вы можете направить определенное количество колонистов в шахты, добывать минералы.\n");
        writeOutput("    Внимание! минералы добывают только колонисты, которых вы направили в шахты! Каждый год этот показатель обнуляется!\n");
        writeOutput("    Количество шахтеров не может превышать количество колонистов!\n");
        writeOutput("5.  !год - выводит на экран текущий год.\n");
        writeOutput("6.  !население - выводит на экран популяцию колонии.\n");
        writeOutput("7.  !шахтеры - выводит на экран количество колонистов, направленных в шахты в этом году.\n");
        writeOutput("8.  !шахты - выводит на экран количество шахт, имеющееся в вашем распоряжении.\n");
        writeOutput("9.  !деньги - выводит на экран количество денежных средств, которыми вы владеете.\n");
        writeOutput("10. !цены - выводит на экран текущие цены на покупку/продажу шахт, минералов, продовольствия.\n");
        writeOutput("11. !счастье - выводит на экран текущий уровень счастья колонии.\n");
        writeOutput("12. !производство - выводит на экран количество минералов, добываемых в год колонистом.\n");
        writeOutput("13. !минералы - выводит на экран количество минералов, хранящихся на вашем складе.\n");
        writeOutput("14. !продовольствие - выводит на экран количество продовольствия, хранящегося на вашем складе.\n");
        writeOutput("15. !погода - выводит на экран текущие погодные условия.\n");
        writeOutput("16. Отправить - отправляет указанное количество минералов Империи.\n");
        writeOutput("17. Помощь - список команд и их краткое описание. Данная команда доступна в любой момент,\n");
        writeOutput("    когда игра не просит указать количество или число и содержит в себе полный список команд доступный вам на данном этапе игры.\n");
        writeOutput("18. Сохранить - команда выполняет сохранение вашего текущего прогресса.\n");
        writeOutput("19. Завершить год - производит завершение текущего года и переход в следующий.\n\n");
        break;
    case 2:
        writeOutput("1. Шахту - команда для продажи/покупки шахты.\n");
        writeOutput("2. Минералы - команда для продажи/покупки минералов.\n");
        writeOutput("3. Продовольствие - команда для продажи/покупки продовольствия.\n");
        writeOutput("4. Помощь - список команд и их краткое описание. Данная команда доступна в любой момент,\n");
        writeOutput("   когда игра не просит указать количество или число и содержит в себе полный список команд доступный вам на данном этапе игры.\n");
        writeOutput("5. Выход - отменить покупку/продажу.\n\n");
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    default:
        writeOutput("Идентификатор помощи отсутствует, возможно ее не существует!\n\n");
    }
}

int GetRandomNumber(int min, int max)
{// получение случайного целого числа в указанном диапазоне
    srand(time(NULL));
    int num = min + rand() % (max - min + 1);
    return num;
}

void RandomFactors(int& food, int& mine, int& ore, int& production)
{// Генерация случайных значений
    food = GetRandomNumber(40, 80);
    mine = GetRandomNumber(2000, 4000);
    ore = GetRandomNumber(7, 14);
    production = GetRandomNumber(4, 8);
}

void statistics(int year, int population, int mines, int money, int satisfaction, int production, int ore, int* food, int workers, int ImpOre, int Delivered)
{// Статистика
    writeOutput("Год " + std::to_string(year) + " \n");
    writeOutput("Население вашей колонии составляет: " + std::to_string(population) + " человек. Из них шахтеров: " + std::to_string(workers) + ".\n");
    writeOutput("Шахт в вашем распоряжении: " + std::to_string(mines) + ".\n");
    writeOutput("Доступных вам средств: " + std::to_string(money) + " $.\n");
    writeOutput("Уровень счастья: " + std::to_string(satisfaction) + " %\n\n");

    writeOutput("Вы добываете: " + std::to_string(production) + " тон минералов за шахтера.\n");
    writeOutput("На вашем складе: " + std::to_string(ore) + " тон минералов и " + std::to_string(food[0] + food[1] + food[2]) + " единиц продовольствия.\n\n");

    writeOutput("Империи требуется " + std::to_string(ImpOre) + " тон минералов\n");
    writeOutput("Доставлено: " + std::to_string(Delivered) + " тон минералов\n\n");
}

void sell_buy(int& money, int& mines, int& ore, int* food, int mprice, int oprice, int fprice, std::string operation)
{// Покупка/продажа
    int k = 1;
    std::string operation2 = "продажи";
    if (operation == "купить")
    {
        k = -1;
        operation2 = "покупки";
    }
    writeOutput("В вашем распоряжении " + std::to_string(money) + " $, " + std::to_string(mines) + " шахт и " + std::to_string(ore) + " тонн минералов.\n");
    writeOutput("Так же на вашем складе хранится " + std::to_string(food[0] + food[1] + food[2]) + " единиц продовольствия.\n");
    writeOutput("Цена " + operation2 + " шахты составляет: " + std::to_string(mprice) + " $\n");
    if (operation == "купить") writeOutput("Вы можете позволить себе купить: " + std::to_string(money / mprice) + " шахт.\n");
    writeOutput("Цена " + operation2 + " одной тонны минералов составляет: " + std::to_string(oprice) + " $\n");
    if (operation == "купить") writeOutput("Вы можете позволить себе купить: " + std::to_string(money / oprice) + " тон минералов.\n");
    writeOutput("Цена " + operation2 + " одной единицы продовольствия составляет: " + std::to_string(fprice) + " $\n");
    if (operation == "купить") writeOutput("Вы можете позволить себе купить: " + std::to_string(money / fprice) + " единиц продовольствия.\n");
    std::cout << std::endl;
    while (true)
    {
        writeOutput("Что вы желаете " + operation + "?\n");
        writeOutput(prompt);
        std::string choice = readUserInput();
        if ((choice == "шахту") || (choice == "1"))
        {
            for (;;)
            {
                int num = 0;
                if (operation == "купить") writeOutput("Вы можете купить " + std::to_string(money / mprice) + " шахт.\n");
                else writeOutput("В вашем распоряжении " + std::to_string(mines) + " шахт.\n");
                writeOutput("Укажите количество шахт для " + operation2 + ":\n");
                num = std::stoi(readUserInput());
                if (((k == 1) && (num <= mines)) || ((k == -1) && (money >= num * mprice)) && (num >= 0))
                {
                    money += (k * num * mprice);
                    mines -= (k * num);
                    k = 0;
                    std::cout << std::endl;
                    break;
                }
                else if (k == 1) writeOutput("У вас недостаточно шахт, чтобы продать введенное вами количество!\n");
                else if (k == -1) writeOutput("У вас недостаточно денег, чтобы купить желаемое количество шахт!\n");
                else if (num < 0) writeOutput("Долги в вашей колонии не привествуются, фигурируйте натуральными числами!\n");
            }
        }
        else if ((choice == "минералы") || (choice == "2"))
        {
            for (;;)
            {
                int num = 0;
                if (operation == "купить") writeOutput("Вы можете купить " + std::to_string(money / oprice) + " тон минералов.\n");
                else writeOutput("В вашем распоряжении " + std::to_string(ore) + " тон минералов.\n");
                writeOutput("Укажите количество минералов для " + operation2 + ":\n");
                num = std::stoi(readUserInput());
                if (((k == 1) && (num <= ore)) || ((k == -1) && (money >= num * oprice)) && (num >= 0))
                {
                    money += (k * num * oprice);
                    ore -= (k * num);
                    k = 0;
                    std::cout << std::endl;
                    break;
                }
                else if (k == 1) writeOutput("У вас недостаточно минералов, чтобы продать введенное вами количество!\n");
                else if (k == -1) writeOutput("У вас недостаточно денег, чтобы купить желаемое количество минералов!\n");
                else if (num < 0) writeOutput("Долги в вашей колонии не привествуются, фигурируйте натуральными числами!\n");
            }
        }
        else if ((choice == "продовольствие") || (choice == "3"))
        {
            for (;;)
            {
                int num = 0;
                if (operation == "купить") writeOutput("Вы можете купить " + std::to_string(money / fprice) + " единиц продовольствия.\n");
                else writeOutput("В вашем распоряжении " + std::to_string(food[0] + food[1] + food[2]) + " единиц продовольствия.\n");
                writeOutput("Укажите количество продовольствия для " + operation2 + ":\n");
                num = std::stoi(readUserInput());
                if (((k == 1) && (num <= food[0] + food[1] + food[2])) || ((k == -1) && (money >= num * fprice)) && (num >= 0))
                {
                    money += (k * num * fprice);
                    std::cout << std::endl;
                    if (k == -1) food[2] -= (k * num);
                    if (k == 1)
                    {
                        food[0] -= num;
                        if (food[0] < 0)
                        {
                            food[1] -= food[0];
                            if (food[1] < 0) food[2] -= food[1];
                        }
                    }
                    k = 0;
                    break;
                }
                else if (k == 1) writeOutput("У вас недостаточно продовольствия, чтобы продать введенное вами количество!\n");
                else if (k == -1) writeOutput("У вас недостаточно денег, чтобы купить желаемое количество продовольствия!\n");
                else if (num < 0) writeOutput("Долги в вашей колонии не привествуются, фигурируйте натуральными числами!\n");
            }
        }
        else if ((choice == "помощь") || (choice == "4")) help(2);
        else if ((choice == "выход") || (choice == "5")) k = 0;
        else writeOutput("Введена неизвестная команда! Для получения списка всех доступных команд введите помощь или 4!\n");
        if (k == 0) break;
    }
}

bool failure(int population, int satisfaction, int mines, int year)
{// Провал игры
    bool fail = false;
    if (population < 10)
    {
        writeOutput("Ваше население перетрудилось!\n");
        fail = true;
    }
    else if (satisfaction == 0)
    {
        writeOutput("Бунт! Ваша голова теперь украшает чью то стену!\n");
        fail = true;
    }
    return fail;
}

int Hazard(int year, bool mode, int& ore, int& production, int population)
{// Сезоны
    int death_chance = 0;
    bool neg_weather = false;
    if ((year % 2 == 0) && (year > 5))
    {
        neg_weather = true;
        writeOutput("Этот год будет холодным! Для поддержания тепла в помещениях колонии необходимо небольшое количество минералов!\n");
        if (mode == true)
        {
            if (ore >= population / 2)
            {
                ore -= population / 2;
                writeOutput("Колонисты воспользовались запасами минералов для обогрева\n");
            }
            if (ore < population / 2)
            {
                writeOutput("У вас не было достаточно минералов для обогрева колонии, возможно до конца года кто то не доживет!\n");
                death_chance += 5;
            }
        }
    }
    if ((year % 3 == 0) && (year > 5))
    {
        neg_weather = true;
        writeOutput("Неизвестные химикаты покрыли поверхность планеты! Ваши шахтеры стали вдвое производительнее!\n");
        writeOutput("Но имеется небольшой шанс последующей смерти!\n");
        if (mode == true)
        {
            death_chance += 10;
            production *= 2;
        }
    }
    if ((year % 4 == 0) && (year > 5))
    {
        neg_weather = true;
        writeOutput("Рабочим лучше оставаться дома! Кислотный дождь противопоказан их благоприятным жизненным показателям.\n");
        if (mode == true) death_chance += 20;
    }
    if ((year % 5 == 0) && (year > 5))
    {
        neg_weather = true;
        writeOutput("Повышенная гравитация, рабочие будут тратить больше сил на добычу минералов! Их добыча уменьшена.\n");
        if (mode == true) production /= 2;
    }
    if (neg_weather == false) writeOutput("Погода спокойна, сопутствующих эффектов нет.\n\n");
    return death_chance;
}

void happiness(int* food, int population, int& satisfaction)
{// Счастье
    if ((food[0] + food[1] + food[2]) / population > 50) satisfaction += 5;
    if ((food[0] + food[1] + food[2]) / population < 10) satisfaction -= 5;
    if (satisfaction > 100) satisfaction = 100;
}

void save(int ore, int mines, int population, int satisfaction, int* food, int money, int year, int ImpOre, int Delivered, std::string player_name)
{// Сохранение игры
    std::string save = player_name + " " + std::to_string(year) + " " + std::to_string(money) + " " + std::to_string(population) + " " + std::to_string(satisfaction) + " " + std::to_string(food[0]) + " " + std::to_string(food[1]) + " " + std::to_string(food[2]) + " " + std::to_string(mines) + " " + std::to_string(ore) + " " + std::to_string(ImpOre) + " " + std::to_string(Delivered);
    std::ofstream save_file("svf.txt", std::ios_base::app);
    if (!save_file.is_open()) std::ofstream save_file("svf.txt", std::ios_base::trunc);
    else save_file << std::endl;
    save_file << save;
    save_file.close();
}

void high_score(int ore, int mines, int population, int satisfaction, int* food, int money, int year, std::string player_name)
{// Сохранение счета в таблица лидеров
    std::string high_score = player_name + " " + std::to_string(year * (ore * mines + (food[0] + food[1] + food[2]) * satisfaction + money * population));
    std::ofstream score_board("hs.txt", std::ios_base::app);
    if (!score_board.is_open()) std::ofstream score_board("hs.txt", std::ios_base::trunc);
    else score_board << std::endl;
    score_board << high_score;
    score_board.close();
}

std::string story(int year)
{
    std::string story_output;
    switch (year)
    {
    case 1:
        writeOutput("Введите имя игрока: ");
        story_output = readUserInput();
        break;
    case 2:
 //       writeOutput("Вам пришло новое сообщение!\n");
 //       writeOutput("Произвожу воспроизведение сообщения...\n");
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
    return story_output;
}

void rotting_food(int* food)
{// Гниение еды +
    int rotten = food[0];
    food[0] = food[1];
    food[1] = food[2];
    food[2] = 0;
    if (rotten > 0) writeOutput("У вас сгнило " + std::to_string(rotten) + " единиц продовольствия!\n");
}

void runGameStep(bool no_text, std::string game_basics)
{// Игровая база
    std::string player_name;

    int mines = GetRandomNumber(3, 6);
    int population = GetRandomNumber(40, 60);
    int money = GetRandomNumber(10, 50) * population;
    int year = 1;
    int satisfaction = 50;
    int food[3] = { 0, 0, 0 };
    int ore = 0;
    int ImpOre = ((3000 * mines + money) / 10) + 1000;
    int Delivered = 0;

    if (!(game_basics == ""))
    {
        int space1 = game_basics.find(" ");
        player_name = game_basics.substr(0, space1);
        int space2 = game_basics.find(" ", space1 + 1);
        year = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        money = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        population = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        satisfaction = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        food[0] = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        food[1] = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        food[2] = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        mines = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        ore = stoi(game_basics.substr(space1 + 1, space2 - 1));
        space1 = space2;
        space2 = game_basics.find(" ", space1 + 1);
        ImpOre = stoi(game_basics.substr(space1 + 1, space2 - 1));
        Delivered = stoi(game_basics.substr(space2 + 1));
    }

    if ((year == 1) && (no_text)) player_name = story(year);

    bool futile = false;
    //   bool enough = false;
    while (true)
    {
        std::system("cls");
        int foodprice, mineprice, oreprice, production;
        RandomFactors(foodprice, mineprice, oreprice, production);
        rotting_food(food);
        int workers = 0;
        statistics(year, population, mines, money, satisfaction, production, ore, food, workers, ImpOre, Delivered);
        int death_chance = Hazard(year, true, ore, production, population);
        while (true)
        {
            writeOutput("Введите команду:\n");
            std::string choice = readUserInput();
            if ((choice == "продать") || (choice == "1")) sell_buy(money, mines, ore, food, mineprice, oreprice, foodprice, "продать");
            else if ((choice == "купить") || (choice == "2")) sell_buy(money, mines, ore, food, mineprice, oreprice, foodprice, "купить");
            else if ((choice == "статистика") || (choice == "3")) statistics(year, population, mines, money, satisfaction, production, ore, food, workers, ImpOre, Delivered);
            else if ((choice == "добывать") || (choice == "4"))
            {
                while (true)
                {
                    writeOutput("Количество шахтеров не может превышать вашу популяцию колонистов. Каждая шахта может вместить до 10 рабочих.\n");
                    writeOutput("Сколько колонистов вы желаете отправить на добычу минералов?\n");
                    int count = stoi(readUserInput());
                    if (workers + count > population) writeOutput("Вы ввели недопустимое число, суммарное количество шахтеров не может превышать количество колонистов!\n");
                    else if (workers + count > mines * 10) writeOutput("Переизбыток рабочих на шахтах, каждая шахта может вмещать до 10 шахтеров!\n");
                    else
                    {
                        workers += count;
                        writeOutput("Ваше текущее количество шахтеров: " + std::to_string(workers) + " человек.\n\n");
                        break;
                    }
                }
            }
            else if ((choice == "!год") || (choice == "5")) writeOutput("Текущий год: " + std::to_string(year) + ".\n");
            else if ((choice == "!население") || (choice == "6")) writeOutput("Население: " + std::to_string(population) + ".\n");
            else if ((choice == "!шахтеры") || (choice == "7")) writeOutput("На данный момент в шахты направлено " + std::to_string(workers) + " колонистов.\n");
            else if ((choice == "!шахты") || (choice == "8")) writeOutput("Количество ваших шахт: " + std::to_string(mines) + ".\n");
            else if ((choice == "!деньги") || (choice == "9")) writeOutput("На вашем счету: " + std::to_string(money) + " $.\n");
            else if ((choice == "!цены") || (choice == "10"))
            {
                writeOutput("Стоимость купли/продажи шахты: " + std::to_string(mineprice) + " $.\n");
                writeOutput("Вы можете позволить себе купить: " + std::to_string(money / mineprice) + " шахт.\n");
                writeOutput("Стоимость купли/продажи минералов: " + std::to_string(oreprice) + " $.\n");
                writeOutput("Вы можете позволить себе купить: " + std::to_string(money / oreprice) + " тон минералов.\n");
                writeOutput("Стоимость купли/продажи продовольствия: " + std::to_string(foodprice) + " $.\n");
                writeOutput("Вы можете позволить себе купить: " + std::to_string(money / foodprice) + " единиц продовольствия.\n\n");
            }
            else if ((choice == "!счастье") || (choice == "11")) writeOutput("Ваше население счастливо на " + std::to_string(satisfaction) + " %.\n");
            else if ((choice == "!производство") || (choice == "12")) writeOutput("Каждый колонист, отправленный в шахту, добывает " + std::to_string(production) + " минералов.\n");
            else if ((choice == "!минералы") || (choice == "13")) writeOutput("На вашем складе хранится " + std::to_string(ore) + " тонн минералов.\n");
            else if ((choice == "!продовольствие") || (choice == "14")) writeOutput("На вашем складе хранится " + std::to_string(food[0] + food[1] + food[2]) + " единиц продовольствия.\n");
            else if ((choice == "!погода") || (choice == "15")) Hazard(year, false, ore, production, population);
            else if ((choice == "отправить") || (choice == "16"))
            {
                if (ore > 0)
                {
                    while (true)
                    {
                        writeOutput("На вашем складе " + std::to_string(ore) + " тон минералов.\n");
                        writeOutput("Какое количество минералов вы желаете отправить?\n");
                        int count = stoi(readUserInput());
                        if (count > ore) writeOutput("Вы не можете отправить больше минералов, чем находится на вашем складе.\n");
                        else
                        {
                            ore -= count;
                            Delivered += count;
                            break;
                        }
                    }
                }
                else writeOutput("В наличии нет минералов для отправки!\n\n");
            }
            else if ((choice == "помощь") || (choice == "17")) help(1);
            else if ((choice == "сохранить") || (choice == "18")) save(ore, mines, population, satisfaction, food, money, year, ImpOre, Delivered, player_name);
            else if ((choice == "завершить год") || (choice == "19")) break;
            else writeOutput("Введена неизвестная команда! Для получения списка всех доступных команд введите помощь или 16!\n");
        }
        if (futile = failure(population, satisfaction, mines, year)) break;
        if (ImpOre-1 < Delivered) break;
        happiness(food, population, satisfaction);
        if (satisfaction >= 70) population += GetRandomNumber(1, 10);
        if (satisfaction < 30) population -= GetRandomNumber(1, 10);
        ore += workers * production;
        if (death_chance > 0)
        {
            int sorrow = 0;
            writeOutput("Текущий год был не самым благоприятным!\n");
            for (int i = 0; i < population; i++)
            {
                if (death_chance >= GetRandomNumber(0, 100))
                {
                    population--;
                    sorrow++;
                }
            }
            if (sorrow > 0)
            {
                writeOutput("Нас покинуло " + std::to_string(sorrow) + " человек!\n");
                writeOutput("Ваша колония горюет об их утрате!\n\n");
                satisfaction -= sorrow;
            }
        }
        year++;
    }
    if (futile == true) writeOutput("Конец игры!");
    else writeOutput("Вы снабдили империю необходимым количеством минералов! Бунт подавлен!\n\n");
    if (no_text == false) high_score(ore, mines, population, satisfaction, food, money, year, player_name);
}

void high_score_output()
{//Вывод таблицы лидеров +
    std::ifstream fin("hs.txt");
    if (fin.is_open())
    {
        while (!fin.eof())
        {
            std::string score;
            std::getline(fin, score);
            writeOutput(score);
            std::cout << std::endl;
        }
        fin.close();
        std::cout << std::endl;
    }
    else writeOutput("Нет зарегистрированных лидеров! Попробуйте сыграть в игру.\n");
}

std::string load()
{// Загрузка игры +
    std::string load_save = "";
    std::string player_name;
    std::ifstream fin("svf.txt");
    if (fin.is_open())
    {
        int max_saves = 0;
        while (!fin.eof())
        {
            max_saves++;
            fin >> player_name;
            writeOutput(std::to_string(max_saves) + " ");
            writeOutput(player_name);
            std::cout << std::endl;
            getline(fin, player_name);
        }
        fin.close();
        std::cout << std::endl;
        if (!(max_saves == 0))
        {
            std::cout << std::endl;
            int choice = 0;
            for (; (choice < 1) || (choice > max_saves);)
            {
                writeOutput("Введите номер сохранения, который желаете загрузить.\n");
                choice = stoi(readUserInput());
                if ((choice < 1) || (choice > max_saves)) writeOutput("Сохранения с введенным номером не существует!\n");
            }
            fin.open("svf.txt", std::ios_base::in);
            for (int i = 1; i < choice; i++) getline(fin, load_save);
            getline(fin, load_save);
            fin.close();
        }
        else writeOutput("Нет созданных сохранений!\n");
    }
    else writeOutput("Нет созданных сохранений!\n");
    return load_save;
}

int main(int argc, char* argv[])
{//+
#ifdef _WIN32
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");
#endif

    writeOutput("Добро пожаловать! Мы искренне рады, что вы решили уделить свое внимание этой игре,\n");
    writeOutput("искренне надеемся что она доставит вам удовольствие!\n\n");
    for (;;)
    {
        writeOutput("Для продолжения наберите одну из предложенных ниже команд:\n\n");
        writeOutput("1. Начать новую игру\n");
        writeOutput("2. Начать игру на счет\n");
        writeOutput("3. Загрузить игру\n");
        writeOutput("4. Таблица лидеров\n");
        writeOutput("5. Описание\n");
        writeOutput("6. Помощь\n");
        writeOutput("7. Выход\n\n");
        std::string choice = readUserInput();
        std::string base_stats = { "" };
        std::cout << std::endl;
        if ((choice == "начать новую игру") || (choice == "1"))
        {
            runGameStep(true, base_stats);
        }
        else if ((choice == "начать игру на счет") || (choice == "2"))
        {
            runGameStep(false, base_stats);
        }
        else if ((choice == "загрузить игру") || (choice == "3"))
        {
            base_stats = load();
            if (!(base_stats == "")) runGameStep(true, base_stats);
        }
        else if ((choice == "таблица лидеров") || (choice == "4"))
        {
            high_score_output();
        }
        else if ((choice == "описание") || (choice == "5"))
        {
            writeOutput("Добро пожаловать, управляющий, я - ВИ3И0Н модель 15367830, интерфейс для взаимодействия с окружением.\n");
            writeOutput("Через меня вы будете взаимодействовать с окружением и влиять на обстановку, вашей колонии.\n");
            writeOutput("Вы - управляющий космической минералодобывающей колонии, расположенной на спутнике Астрон планеты Протон\n");
            writeOutput("Вас назначили на данную должность в связи с упразднением предыдущего управляющего из за его взглядов.\n");
            writeOutput("На данный момент империю человечества разрывает гражданская война и ваша роль в ней - высочайшая.\n");
            writeOutput("Колония, которой вас назначили управлять, является ключевым объектом по добыче минералов.\n");
            writeOutput("Минералы - широко используются в военном производстве и в обилии могут добываться на спутнике.\n");
            writeOutput("К сожалению, в связи с малой востребованностью до войны ваша колония была почти забыта,\n");
            writeOutput("а ее население было оставлено на произвол судьбы без внешней поддержки\n");
            writeOutput("В ваши задачи входит:\n");
            writeOutput("1. Развить колонию для увеличения добычи минералов в масштабах, удовлетворяющих производственные потребности\n");
            writeOutput("2. Снабжать империю минералами до тех пор, пока она не одержит верх в войне.\n");
            writeOutput("Достигать этих целей вы будете посредством взаимодействия с интерфейсом ввода.\n");
            writeOutput("Каждый год вам будет предоставляться подробная статистическая информация о состоянии колонии.\n");
            writeOutput("Ваши колонисты - лучшие колонисты во всей империи, а значит продовольствие они не потребляют!\n");
            writeOutput("Но продовольствие необходимо для регулирования их уровня счастья, к сожалению, оно хранится 3 дня.\n");
            writeOutput("Для повышения уровня счастья необходимо более 50 единиц продовольствия на колониста,\n");
            writeOutput("Но если единиц продовольствия менее 10 на колониста, то уровень счастья будет уменьшаться.\n");
            writeOutput("При поддержании счастья на высоком уровне к вам начнут прибывать новые колонисты.\n");
            writeOutput("Если же он будет слишком низок, колонисты начнут покидать вас.\n\n");
        }
        else if ((choice == "помощь") || (choice == "6"))
        {
            help(0);
        }
        else if ((choice == "выход") || (choice == "7"))
        {
            break;
        }
        else writeOutput("Введенная вами команда не существует, введите помощь или 6, для получения списка всех команд.\n");
    }
    return 0;
}