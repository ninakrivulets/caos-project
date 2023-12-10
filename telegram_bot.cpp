#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>

using namespace TgBot;
using namespace std;

vector<string> bot_commands = {"start", "find_recipe"};

//string get_receipe() {} потенциальный парсер


int main() {
    TgBot::Bot bot("6645521404:AAFvUe2P3QtXSIltaBmm0p5hLj-BjFepuXg");

    InlineKeyboardMarkup::Ptr keyboard1(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> buttons1;
    InlineKeyboardButton::Ptr get_receipe(new InlineKeyboardButton);
    get_receipe->text = "узнать рецепт";
    get_receipe->callbackData = "узнать рецепт";
    buttons1.push_back(get_receipe);
    keyboard1->inlineKeyboard.push_back(buttons1);



    InlineKeyboardMarkup::Ptr keyboard2(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> buttons2;
    InlineKeyboardButton::Ptr first_dish(new InlineKeyboardButton), second_dish(new InlineKeyboardButton);
    first_dish->text = " первое";
    second_dish->text = "второе";
    first_dish->callbackData = "первое";
    second_dish->callbackData = "второе";
    buttons2.push_back(first_dish);
    buttons2.push_back(second_dish);
    keyboard2->inlineKeyboard.push_back(buttons2);




    bot.getEvents().onCommand("start", [&bot, &keyboard1](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "Привет, " + message->chat->firstName + "! \nНаш бот поможет тебе найти рецепты актуальных блюд из имеющихся дома инредиентов", false, 0, keyboard1); //вторым аргументом онкоманд передается лямбда функция
    });

    bot.getEvents().onCommand("find_recipe", [&bot, &keyboard2](const TgBot::Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id, "Какой тип блюда вас интересует?", false, 0, keyboard2);
//        bot.getApi().sendMessage(message->chat->id, "Перечислите до 10-ти ингредиентов в формате: Яблоко, Капуста, ..."); //вторым аргументом онкоманд передается лямбда функция
    });

    bot.getEvents().onCallbackQuery([&bot, &keyboard1, &keyboard2](const CallbackQuery::Ptr& query) {
        bot.getApi().sendMessage(query->message->chat->id, "Какой тип блюда вас интересует?", false, 0, keyboard2);

    });



    bot.getEvents().onCallbackQuery([&bot, &keyboard2](const CallbackQuery::Ptr& query){
        // сюда надо добавить запрос "введите ингредиенты"
        if (query->data, "первое"){
            bot.getApi().sendMessage(query->message->chat->id, "функция в разработке :) появится чуть-чуть попозже");
        } else {
            bot.getApi().sendMessage(query->message->chat->id, "функция также в разработке :) появится чуть-чуть попозже");
        }
    });

    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr& message) {
        for (const auto &command: bot_commands) {
            if ("/" + command == message->text) {
                return;
            }
        }
        bot.getApi().sendMessage(message->chat->id, "Извините, я не знаю такой команды :(");
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