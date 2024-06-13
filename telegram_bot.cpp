#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread> // Для std::this_thread::sleep_for
#include <chrono> // Для std::chrono::milliseconds

using namespace TgBot;
using namespace std;

vector<string> bot_commands = {"start", "find_recipe", "get_to_know_a_joke"};

enum BotState {
    IDLE, // Состояние, когда бот ожидает команды
    AWAIT_INGREDIENTS // Состояние, когда бот ожидает ввода ингредиентов
};

BotState currentState = IDLE; // Начальное состояние - IDLE

int main() {
    TgBot::Bot bot("6645521404:AAFvUe2P3QtXSIltaBmm0p5hLj-BjFepuXg");
    // Определение команд
    std::vector<BotCommand::Ptr> commands;
    for (const auto& cmd : bot_commands) {
        BotCommand::Ptr command(new BotCommand);
        command->command = cmd;
        command->description = "Описание команды " + cmd; // Замените описание на подходящее
        commands.push_back(command);
    }
    bot.getApi().setMyCommands(commands);


    InlineKeyboardMarkup::Ptr keyboard1(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> buttons1;
    InlineKeyboardButton::Ptr get_receipe(new InlineKeyboardButton);
    get_receipe->text = "узнать рецепт";
    get_receipe->callbackData = "узнать рецепт";
    buttons1.push_back(get_receipe);
    keyboard1->inlineKeyboard.push_back(buttons1);


    bot.getEvents().onCommand("start", [&bot, &keyboard1](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "Привет, " + message->chat->firstName + "! \nНаш бот поможет тебе найти рецепты актуальных блюд из имеющихся дома инредиентов", false, 0, keyboard1); //вторым аргументом онкоманд передается лямбда функция
    });

    bot.getEvents().onCallbackQuery([&bot](const TgBot::CallbackQuery::Ptr& query) {
        if (query->data == "узнать рецепт") {
            std::string response = "Введите до 10 ингредиентов";
            bot.getApi().sendMessage(query->message->chat->id, response);
            currentState = AWAIT_INGREDIENTS;
        }
    });


    bot.getEvents().onCommand("find_recipe", [&bot](const TgBot::Message::Ptr& message) {
        std::string response = "Введите до 10 ингредиентов";
        bot.getApi().sendMessage(message->chat->id, response);
        currentState = AWAIT_INGREDIENTS;
    });

    std::vector<std::string> jokes = { // Анекдоты
            "— Доктор, у меня плохое настроение!\n— Позвольте мне выписать вам счёт.",
            "— Почему курица перешла дорогу?\n— Чтобы показать всем, что у неё есть яйца.",
            "— Зачем пьют валерьянку?\n— Чтобы отпраздновать свадьбу котов.",
            "— Почему математик боится отрицательных чисел?\n— Потому что они всегда такие отрицательные.",
            "— Какая рыба нападает на людей?\n— Хулиган.",
            "— Что делает фокусник в отпуске?\n— Исчезает.",
            "— Почему нельзя брать два раза одну и ту же шутку?\n— Потому что юмор должен быть свежим!",
            "— Чем заканчивается день?\n— Буквой \"ь\".",
            "— Что будет, если скрестить ежа и змею?\n— 1 метр колючей проволоки.",
            "— Почему компьютеры не едят пиццу?\n— Потому что они не могут есть, у них нет рта!",
            "— Какая самая сложная часть работы электрика?\n— Найти подходящую лампочку.",
            "— Какой элемент таблицы Менделеева не любит сидеть на месте?\n— Уран.",
            "— Почему у программистов нет выходных?\n— Потому что они всегда кодят.",
            "— Какая мышь может летать?\n— Компьютерная, если её сильно бросить.",
            "— Почему холодильник не улыбается?\n— Потому что у него всегда полная холодильная камера.",
            "— Что будет, если бросить сковородку в воду?\n— Жаркая схватка!",
            "— Почему рыбы не могут работать программистами?\n— Потому что у них нет рук, чтобы нажимать на клавиши.",
            "— Какой компьютер не может считать?\n— Тот, который выключен.",
            "— Почему огурец не ходит в школу?\n— Потому что он зелёный.",
            "— Почему стол не может петь?\n— Потому что у него нет голосовых связок."
    };

    bot.getEvents().onCommand("get_to_know_a_joke", [&bot, &jokes, &keyboard1](const TgBot::Message::Ptr& message) {
        std::string response = jokes[std::rand() % jokes.size()];
        bot.getApi().sendMessage(message->chat->id, response, false, 0);
    });
//todo

    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr& message) {
        if (currentState == AWAIT_INGREDIENTS) {

            std::string filepath = "/Users/syuy/Downloads/caos-project-main/CaosParseRecipe/f.txt";
            std::ofstream file(filepath);
            file << message->text << std::endl;
            file.close();
            bot.getApi().sendMessage(message->chat->id, "Готово! Вот рецепты :)");
            std::this_thread::sleep_for(std::chrono::seconds(4));

            std::string filepath_response = "/Users/syuy/Downloads/caos-project-main/response.txt";
            std::string content;

            std::ifstream file_response(filepath_response);
            if (file_response.is_open()) {
                std::stringstream buffer;
                buffer << file_response.rdbuf();
                content = buffer.str();
                file_response.close();
            } else {
                std::cerr << "Не удалось открыть файл: " << filepath_response << std::endl;
            }

            std::cout << "here1";
            bot.getApi().sendMessage(message->chat->id, content);
            std::cout << "here2";

            currentState = IDLE;
            return;

        } else {
            for (const auto &command: bot_commands) {
                if ("/" + command == message->text) {
                    return;
                }
            }
            bot.getApi().sendMessage(message->chat->id, "Извините, я не знаю такой команды :(");
        }
    });


    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException &e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}