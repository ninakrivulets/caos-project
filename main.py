from queue import Queue
from httpx import Client
from pprint import pprint
import config


class ParseRecipe:
    def __init__(self):
        self.url = 'http://recepty-po-ingredientam.ru/api/'
        self.session_queue = Queue()
        self.ingredients_ids = []
        self.previous_message = []
        self.input_items = []

        for i in range(config.session_num):
            session = Client()
            self.session_queue.put(session)
        self.parse_tg_message()

    def parse_tg_message(self):
        while True:
            with open('f.txt', 'r') as f:
                cur_message = f.readlines()
                if cur_message != self.previous_message:
                    self.previous_message = cur_message
                    for line in f:
                        self.input_items.extend(line.strip().split(','))
                    self.find_ingredient_id()

    # Функции, в которой исправляются ошибки с помощью расстояния Левенштейна
    def mistakes_corrector(self, wanted_ingr: str, to_choose: list):
        dist = []
        for ingr in to_choose:
            if abs(len(ingr) - len(wanted_ingr)) <= 2:
                leven = [[0] * (len(wanted_ingr) + 1) for i in range(len(ingr) + 1)]
                for i in range(1, (len(wanted_ingr) + 1)):
                    leven[0][i] = i
                for j in range(1, (len(ingr) + 1)):
                    leven[j][0] = j
                for i in range(1, len(ingr) + 1):
                    for j in range(1, len(wanted_ingr) + 1):
                        leven[i][j] = min(leven[i - 1][j] + 1, leven[i][j - 1] + 1,
                                          leven[i - 1][j - 1] + (wanted_ingr[j - 1] != ingr[i - 1]))
                if leven[-1][-1] == 1:
                    return ingr
                else:
                    dist.append([leven[-1][-1], ingr])
        if dist:
            if min(dist)[0] <= 2:
                return min(dist)[1]
        else:
            return 0

    def find_ingredient_id(self):
        for ingredient in self.input_items:
            session = self.session_queue.get()
            params = {
                'service': 'Parser',
                'method': 'SearchIngredients',
                'data': {
                    'search': ingredient,
                    'token': None,
                    'time': '7a4fd9e0b329284a82b1c4dbc52076c4cf3ade56fb6e5571d37e920d1d1e302b',
                    'signature': 'd30a86367f5f57852327ea3c9ecc7d3702b72298136c79c259be31d3203c4cde',
                },
            }

            response = session.post(self.url, json=params)
            data = response.json()
            to_choose = []
            tmp_mapa = {}
            for x in data['data']['items']:
                tmp_mapa[x['name']] = x['id']
                to_choose.append(x['name'])
            most_similar_ingr = self.mistakes_corrector(ingredient, to_choose)
            if most_similar_ingr == 0:
                continue
            self.ingredients_ids.append(str(tmp_mapa[most_similar_ingr]))
            self.session_queue.put(session)
        self.get_recipes()

    def get_recipes(self):
        params = {
            'service': 'Parser',
            'method': 'Search',
            'data': {
                'ingredients': self.ingredients_ids
            }
        }
        session = self.session_queue.get()
        response = session.post(self.url, json=params)
        data = response.json()

        with open('/Users/syuy/Downloads/caos-project-main/response.txt', 'w') as f2:
            for a in data['data']['items']:
                f2.write(a['name'])
                f2.write('\n')
                f2.write(a['url'])
                f2.write('\n')

                print(a['name'])
                print(a['url'])

        # exit(0)


ParseRecipe()