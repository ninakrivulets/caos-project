#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread> // Для std::this_thread::sleep_for
#include <chrono> // Для std::chrono::milliseconds

using namespace TgBot;
using namespace std;

vector<string> bot_commands = {"start", "find_recipe"};

enum BotState {
    IDLE, // Состояние, когда бот ожидает команды
    AWAIT_INGREDIENTS // Состояние, когда бот ожидает ввода ингредиентов
};

BotState currentState = IDLE; // Начальное состояние - IDLE


int main() {
    TgBot::Bot bot("6645521404:AAFvUe2P3QtXSIltaBmm0p5hLj-BjFepuXg");

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
            std::string response = "Введите до 10 ингредиентов в формате: яблоко, капуста, ...";
            bot.getApi().sendMessage(query->message->chat->id, response);
            currentState = AWAIT_INGREDIENTS;
        }
    });


    bot.getEvents().onCommand("find_recipe", [&bot](const TgBot::Message::Ptr& message) {
        std::string response = "Введите до 10 ингредиентов в формате: яблоко, капуста, ...";
        bot.getApi().sendMessage(message->chat->id, response);
        currentState = AWAIT_INGREDIENTS;
    });


    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr& message) {
        if (currentState == AWAIT_INGREDIENTS) {

            std::string filepath = "/Users/syuy/Downloads/caos-project-main/CaosParseRecipe/f.txt";
            std::ofstream file(filepath);
            file << message->text << std::endl;
            file.close();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            bot.getApi().sendMessage(message->chat->id, "Готово! Вот рецепты :)");

            std::ifstream fil("response.txt");
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();
            file.close();

            bot.getApi().sendMessage(message->chat->id, content);

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